#include "my_memcpy.h"
#include "utils.h"

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <ctime>

#include <gtest/gtest.h>

TEST(Memcpy, Numbers) {
    uint8_t a[] = {1, 2, 3, 4, 5};
    uint8_t b[5];
    my_memcpy(b, a, 5);
    EXPECT_TRUE(is_same_content_memory(a, b, 5));

    uint8_t c[] = {9, 9, 9, 9, 9};
    my_memcpy(c, b, 4);
    EXPECT_TRUE(is_same_content_memory(c, b, 4));
    EXPECT_EQ(c[4], 9);
}

TEST(Memcpy, String) {
    char a[] = "Hello There!";
    uint32_t n_bytes = std::strlen(a);
    char* b = static_cast<char*>(std::malloc(n_bytes + 1));
    ASSERT_NE(b, nullptr);

    my_memcpy(b, a, n_bytes);
    b[n_bytes] = 0;
    EXPECT_STREQ(a, b);

    std::free(b);
}

TEST(Memcpy, Random) {
    std::srand(std::time(nullptr));
    uint32_t n = std::rand() % 50 + 1;
    uint32_t n_bytes = n * sizeof(uint32_t);
    uint32_t* a = static_cast<uint32_t*>(std::malloc(n_bytes));
    uint32_t* b = static_cast<uint32_t*>(std::malloc(n_bytes));
    ASSERT_NE(a, nullptr);
    ASSERT_NE(b, nullptr);

    for (uint32_t i = 0; i < n; i++) {
        a[i] = static_cast<uint32_t>(std::rand());
    }

    my_memcpy(b, a, n_bytes);
    EXPECT_TRUE(is_same_content_memory(a, b, n_bytes));

    std::free(b);
    std::free(a);
}
