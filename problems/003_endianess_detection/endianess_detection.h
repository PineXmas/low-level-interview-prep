#ifndef ENDIANESS_DETECTION_H
#define ENDIANESS_DETECTION_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

bool is_little_endian(void);

#ifdef __cplusplus
}
#endif

#endif // ENDIANESS_DETECTION_H