#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

bool is_same_content_memory(void* a, void* b, uint32_t n_bytes);

#ifdef __cplusplus
}
#endif

#endif
