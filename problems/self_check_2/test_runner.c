#include "../../shared/acutest.h"
#include <stdio.h>

void test_hello_world_2(void) {
    printf("\nHello World 2!\n");
    TEST_CHECK(1 == 1);
}

void test_passed(void) {
    TEST_CHECK(1 == 2-1);
}

TEST_LIST = {
    { "Verify Hello World Self Check 2", test_hello_world_2 },
    { "Verify Passed Test", test_passed },
    { NULL, NULL }
};