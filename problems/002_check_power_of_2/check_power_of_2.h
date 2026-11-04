#ifndef CHECK_POWER_OF_2_H
#define CHECK_POWER_OF_2_H

#include <stdint.h>
#include <stdbool.h>

/*
Check whether the given number is power of 2 using bit manipulation.
*/
#ifdef __cplusplus
extern "C" {
#endif

bool is_power_of_2(uint32_t a);

#ifdef __cplusplus
}
#endif

#endif