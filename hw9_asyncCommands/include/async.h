#pragma once
#include <cstddef>

#ifdef __cplusplus
extern "C" {
#endif

typedef void* async_handle_t;

async_handle_t connect(std::size_t bulk_size);

void receive(async_handle_t handle, const char* data, std::size_t size);

void disconnect(async_handle_t handle);

#ifdef __cplusplus
}
#endif
