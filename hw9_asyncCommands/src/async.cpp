#include "async.h"

#include <unordered_map>
#include <mutex>
#include <memory>
#include <string>
#include <cstring>
#include <iostream>

#include "CommandCollector.h"
#include "Dispatcher.h"

// Context struct holds collector and leftover partial data
struct AsyncContext {
    AsyncContext(std::size_t bulk_size, Dispatcher& dispatcher)
        : collector(static_cast<int>(bulk_size), dispatcher) {}

    CommandCollector collector;
    std::string leftover; // partial input (no '\n')
};

// Global manager
namespace {
    std::mutex g_mutex;
    std::unordered_map<async_handle_t, std::shared_ptr<AsyncContext>> g_contexts;
    Dispatcher* g_dispatcher = nullptr;
    std::unique_ptr<Dispatcher> g_dispatcher_owner; // owns the dispatcher if created here

    // helper to ensure dispatcher started
    void ensure_dispatcher_started() {
        if (!g_dispatcher) {
            // create dispatcher and start it
            g_dispatcher_owner = std::make_unique<Dispatcher>();
            g_dispatcher = g_dispatcher_owner.get();
            g_dispatcher->start();
        }
    }

    // helper to maybe stop dispatcher when no contexts remain
    void maybe_stop_dispatcher() {
        if (g_dispatcher && g_contexts.empty()) {
            g_dispatcher->stop();
            g_dispatcher_owner.reset();
            g_dispatcher = nullptr;
        }
    }
}

// create a new context and return opaque handle
async_handle_t connect_async(std::size_t bulk_size) {
    try {
        std::lock_guard<std::mutex> lg(g_mutex);
        ensure_dispatcher_started();
        auto ctx = std::make_shared<AsyncContext>(bulk_size, *g_dispatcher);
        // use pointer value as handle
        async_handle_t handle = reinterpret_cast<async_handle_t>(ctx.get());
        // store shared_ptr in map so it remains alive
        g_contexts[handle] = ctx;
        return handle;
    } catch (const std::exception& ex) {
        std::cerr << "connect_async exception: " << ex.what() << std::endl;
        return nullptr;
    }
}

// process buffer: accumulate leftover, cut lines
void receive_async(async_handle_t handle, const char* data, std::size_t size) {
    if (!handle || !data || size == 0) return;

    std::shared_ptr<AsyncContext> ctx;
    {
        std::lock_guard<std::mutex> lg(g_mutex);
        auto it = g_contexts.find(handle);
        if (it == g_contexts.end()) {
            // invalid handle
            return;
        }
        ctx = it->second;
    }

    // Now operate on ctx WITHOUT holding global mutex.
    // According to spec, calls with same context are from same thread,
    // but we'll be safe: protect ctx->leftover with a small mutex if needed.
    // For simplicity and performance, assume same-thread calls for same handle.

    // append incoming data
    ctx->leftover.append(data, data + size);

    // extract full lines
    std::size_t pos = 0;
    while (true) {
        std::size_t nl = ctx->leftover.find('\n', pos);
        if (nl == std::string::npos) break;
        // get line (pos .. nl-1)
        std::string line = ctx->leftover.substr(pos, nl - pos);
        // trim possible '\r'
        if (!line.empty() && line.back() == '\r') line.pop_back();

        // process special tokens
        if (line == "{") {
            ctx->collector.openBrace();
        } else if (line == "}") {
            ctx->collector.closeBrace();
        } else if (!line.empty()) {
            ctx->collector.addCommand(line);
            ctx->collector.tryFlushBySize();
        } else {
            // empty line — ignore
        }

        pos = nl + 1;
    }

    // keep remaining tail
    if (pos == 0) {
        // no full line consumed
        return;
    }
    ctx->leftover.erase(0, pos);
}

// finish and destroy context
void disconnect_async(async_handle_t handle) {
    if (!handle) return;

    std::shared_ptr<AsyncContext> ctx;
    {
        std::lock_guard<std::mutex> lg(g_mutex);
        auto it = g_contexts.find(handle);
        if (it == g_contexts.end()) {
            return;
        }
        ctx = it->second;
        g_contexts.erase(it);
    }

    // call finish (treat as EOF)
    ctx->collector.finish();

    // allow dispatcher to stop if no contexts remain
    std::lock_guard<std::mutex> lg(g_mutex);
    maybe_stop_dispatcher();
}
