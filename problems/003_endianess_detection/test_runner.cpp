#include "endianess_detection.h"

#include <gtest/gtest.h>

extern bool IS_RUNNING_ENV_LITTLE_ENDIAN;

TEST(EndianessDetection, DetectsHostEndianness) {
    EXPECT_EQ(is_little_endian(), IS_RUNNING_ENV_LITTLE_ENDIAN);
}
