#include "acutest.h"
#include "endianess_detection.h"
#include <stdbool.h>

extern bool IS_RUNNING_ENV_LITTLE_ENDIAN;

void test_endianess_detection(void) {
    TEST_CHECK(is_little_endian() == IS_RUNNING_ENV_LITTLE_ENDIAN);
}

TEST_LIST = {
    { "test_endianess_detection", test_endianess_detection },
    { NULL, NULL }
};