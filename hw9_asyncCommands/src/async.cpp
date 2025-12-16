#include "async.h"

#include <unordered_map>
#include <mutex>
#include <memory>
#include <string>
#include <cstring>
#include <iostream>

#include "CommandCollector.h"
#include "Dispatcher.h"

// Context
struct AsyncContext {
    AsyncContext(std::size_t bulk_size, Dispatcher& dispatcher)
        : collector(static_cast<int>(bulk_size), dispatcher) {}

    CommandCollector collector;
    std::string leftover; // input (no '\n')
};

// Global manager
namespace {
    std::mutex g_mutex;
    std::unordered_map<async_handle_t, std::shared_ptr<AsyncContext>> g_contexts;
    Dispatcher* g_dispatcher = nullptr;
    std::unique_ptr<Dispatcher> g_dispatcher_owner;

    // dispatcher started
    void start_dispatcher() {
        if (!g_dispatcher) {
            // create dispatcher and start it
            g_dispatcher_owner = std::make_unique<Dispatcher>();
            g_dispatcher = g_dispatcher_owner.get();
            g_dispatcher->start();
        }
    }

    // stop dispatcher
    void stop_dispatcher() {
        if (g_dispatcher && g_contexts.empty()) {
            g_dispatcher->stop();
            g_dispatcher_owner.reset();
            g_dispatcher = nullptr;
        }
    }
}

// create a new context and return handle
async_handle_t connect(std::size_t bulk_size) {
    try {
        std::lock_guard<std::mutex> lg(g_mutex);
        start_dispatcher();
        auto ctx = std::make_shared<AsyncContext>(bulk_size, *g_dispatcher);
        // use pointer value as handle
        async_handle_t handle = reinterpret_cast<async_handle_t>(ctx.get());
        // store shared_ptr in map - alive
        g_contexts[handle] = ctx;
        return handle;
    } catch (const std::exception& ex) {
        std::cerr << "connect_async exception: " << ex.what() << std::endl;
        return nullptr;
    }
}

// process buffer: accumulate leftover, cut lines
void receive(async_handle_t handle, const char* data, std::size_t size) {
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
void disconnect(async_handle_t handle) {
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

    // call finish
    ctx->collector.finish();

    // stop if no contexts remain
    std::lock_guard<std::mutex> lg(g_mutex);
    stop_dispatcher();
}
