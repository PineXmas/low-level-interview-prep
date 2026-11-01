#include "swap_2_nums.h"

#include <gtest/gtest.h>

TEST(Swap, NoExtremes) {
    uint32_t a = 100;
    uint32_t b = 200;
    swap(&a, &b);
    EXPECT_EQ(a, 200);
    EXPECT_EQ(b, 100);
}

TEST(Swap, Zero) {
    uint32_t a = 0;
    uint32_t b = 200;
    swap(&a, &b);
    EXPECT_EQ(a, 200);
    EXPECT_EQ(b, 0);
}

TEST(Swap, MaxValue) {
    uint32_t a = 100;
    uint32_t b = 0xFFFFFFFF;
    swap(&a, &b);
    EXPECT_EQ(a, 0xFFFFFFFF);
    EXPECT_EQ(b, 100);
}

TEST(Swap, ZeroAndMaxValue) {
    uint32_t a = 0;
    uint32_t b = 0xFFFFFFFF;
    swap(&a, &b);
    EXPECT_EQ(a, 0xFFFFFFFF);
    EXPECT_EQ(b, 0);
}
