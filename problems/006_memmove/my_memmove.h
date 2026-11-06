#ifndef MY_MEMMOVE_H
#define MY_MEMMOVE_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void my_memmove(void* dst, void* src, uint32_t n_bytes);

#ifdef __cplusplus
}
#endif

#endif