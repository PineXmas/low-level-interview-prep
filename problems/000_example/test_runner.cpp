#include "sum.h"

#include <gtest/gtest.h>

TEST(Example, HelloWorld) {
    EXPECT_EQ(1, 1);
}

TEST(Example, SumEq) {
    EXPECT_EQ(sum(2, 3), 5);
}

TEST(Example, SumNotEq) {
    EXPECT_FALSE(sum(2, 3) == 4);
}
