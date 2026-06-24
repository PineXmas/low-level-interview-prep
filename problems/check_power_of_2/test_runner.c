#include "acutest.h"
#include "check_power_of_2.h"

void test_true_32(void) {
    TEST_CHECK(is_power_of_2((uint32_t)32) == true);
}

void test_false_31(void) {
    TEST_CHECK(is_power_of_2((uint32_t)31) == false);
}

void test_false_10(void) {
    TEST_CHECK(is_power_of_2((uint32_t)10) == false);
}

void test_true_0x4(void) {
    TEST_CHECK(is_power_of_2((uint32_t)0x4) == true);
}

void test_true_0x1(void) {
    TEST_CHECK(is_power_of_2((uint32_t)0x1) == true);
}

void test_true_max_pow2(void) {
    TEST_CHECK(is_power_of_2((uint32_t)0x80000000) == true);
}

void test_false_mixed_bits(void) {
    TEST_CHECK(is_power_of_2((uint32_t)0x800F0200) == false);
}

void test_false_0(void) {
    TEST_CHECK(is_power_of_2((uint32_t)0) == false);
}

TEST_LIST = {
    { "test_true_32",          test_true_32 },
    { "test_false_31",         test_false_31 },
    { "test_false_10",         test_false_10 },
    { "test_true_0x4",         test_true_0x4 },
    { "test_true_0x1",         test_true_0x1 },
    { "test_true_max_pow2",    test_true_max_pow2 },
    { "test_false_mixed_bits", test_false_mixed_bits },
    { "test_false_0",          test_false_0 },
    { NULL, NULL }
};