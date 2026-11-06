#ifndef MY_MEMCPY_H
#define MY_MEMCPY_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void my_memcpy(void* dst, void* src, uint32_t n_bytes);

#ifdef __cplusplus
}
#endif

#endif