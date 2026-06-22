#include "../../shared/acutest.h"
#include <stdio.h>

void test_hello_world(void) {
    printf("\nHello World from the Test UI Panel!\n");
    TEST_CHECK(1 == 1);
}

void test_failed(void) {
    printf("\nThis test is designed to fail.\n");
    TEST_CHECK(1 == 0);
}

void test_passed(void) {
    TEST_CHECK(1 == 1);
}

TEST_LIST = {
    { "Verify Hello World UI Execution", test_hello_world },
    { "Verify Failed Test Execution", test_failed },
    { "Verify Passed Test Execution", test_passed },
    { NULL, NULL }
};