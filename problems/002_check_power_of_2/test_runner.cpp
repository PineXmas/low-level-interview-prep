#include "check_power_of_2.h"

#include <gtest/gtest.h>

TEST(CheckPowerOf2, True32) {
    EXPECT_EQ(is_power_of_2(static_cast<uint32_t>(32)), true);
}

TEST(CheckPowerOf2, False31) {
    EXPECT_EQ(is_power_of_2(static_cast<uint32_t>(31)), false);
}

TEST(CheckPowerOf2, False10) {
    EXPECT_EQ(is_power_of_2(static_cast<uint32_t>(10)), false);
}

TEST(CheckPowerOf2, True0x4) {
    EXPECT_EQ(is_power_of_2(static_cast<uint32_t>(0x4)), true);
}

TEST(CheckPowerOf2, True0x1) {
    EXPECT_EQ(is_power_of_2(static_cast<uint32_t>(0x1)), true);
}

TEST(CheckPowerOf2, TrueMaxPow2) {
    EXPECT_EQ(is_power_of_2(static_cast<uint32_t>(0x80000000)), true);
}

TEST(CheckPowerOf2, FalseMixedBits) {
    EXPECT_EQ(is_power_of_2(static_cast<uint32_t>(0x800F0200)), false);
}

TEST(CheckPowerOf2, False0) {
    EXPECT_EQ(is_power_of_2(static_cast<uint32_t>(0)), false);
}
