#pragma once
#include <cstddef>

#ifdef __cplusplus
extern "C" {
#endif

typedef void* async_handle_t;

async_handle_t connect_async(std::size_t bulk_size);

void receive_async(async_handle_t handle, const char* data, std::size_t size);

void disconnect_async(async_handle_t handle);

#ifdef __cplusplus
}
#endif
