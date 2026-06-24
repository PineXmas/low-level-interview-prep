#include "acutest.h"
#include "swap_2_nums.h"

void test_swap_no_extremes() {
    uint32_t a = 100;
    uint32_t b = 200;
    swap(&a, &b);
    TEST_CHECK(a == 200);
    TEST_CHECK(b == 100);
}

void test_swap_zero() {
    uint32_t a = 0;
    uint32_t b = 200;
    swap(&a, &b);
    TEST_CHECK(a == 200);
    TEST_CHECK(b == 0);
}

void test_swap_max_val() {
    uint32_t a = 100;
    uint32_t b = 0xFFFFFFFF;
    swap(&a, &b);
    TEST_CHECK(a == 0xFFFFFFFF);
    TEST_CHECK(b == 100);
}

void test_swap_zero_and_max_val() {
    uint32_t a = 0;
    uint32_t b = 0xFFFFFFFF;
    swap(&a, &b);
    TEST_CHECK(a == 0xFFFFFFFF);
    TEST_CHECK(b == 0);
}

TEST_LIST = {
    {"test_swap_no_extremes", test_swap_no_extremes},
    {"test_swap_zero", test_swap_zero},
    {"test_swap_max_val", test_swap_max_val},
    {"test_swap_zero_and_max_val", test_swap_zero_and_max_val},
    
    { NULL, NULL } // This MUST be at the end to signal end of list
};