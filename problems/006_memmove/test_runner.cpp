#include "my_memmove.h"
#include "utils.h"
#include <gtest/gtest.h>
#include <cstring>
#include <cstdlib>

TEST(Memmove, Numbers) {
    uint8_t a[] = {1, 2, 3, 4, 5};
    uint8_t* b = a + 1;

    my_memmove(b, a, 3);
    uint8_t expected[] = {1, 1, 2, 3, 5};
    EXPECT_TRUE(is_same_content_memory(a, expected, 5));

    uint8_t* c = a + 1;
    b = a;
    my_memmove(b, c, 4);
    uint8_t expected_2[] = {1, 2, 3, 5, 5};
    EXPECT_TRUE(is_same_content_memory(a, expected_2, 5));

    c = a + 3;
    my_memmove(b, c, 2);
    uint8_t expected_3[] = {5, 5, 3, 5, 5};
    EXPECT_TRUE(is_same_content_memory(a, expected_3, 5));

    my_memmove(a, a, 5);
    EXPECT_TRUE(is_same_content_memory(a, expected_3, 5));
}

TEST(Memmove, OverlapAtBoundary) {
    uint8_t a[] = {1, 2, 3, 4, 5};
    uint8_t* b = a + 2;
    my_memmove(b, a, 3);
    uint8_t expected[] = {1, 2, 1, 2, 3};
    EXPECT_TRUE(is_same_content_memory(a, expected, 5));
}

TEST(Memmove, String) {
    const char* s = "Hello World";
    const uint32_t len = std::strlen(s);
    char* a = static_cast<char*>(std::malloc(len + 1));
    std::memcpy(a, s, len + 1);

    char* b = a + 1;
    my_memmove(b, a, 1);
    EXPECT_STREQ(a, "HHllo World");

    b = a + 6;
    my_memmove(b, a, 5);
    EXPECT_STREQ(a, "HHllo HHllo");

    b = a + 2;
    my_memmove(a, b, 6);
    EXPECT_STREQ(a, "llo HHHHllo");

    std::free(a);
}
