#include "acutest.h"
#include "my_memmove.h"
#include "utils.h"
#include <stdlib.h>
#include <time.h>

void test_memmove_numbers() {
    uint8_t a[] = {1, 2, 3, 4, 5};
    uint8_t* b = a + 1;
    
    /*
     * Block 1: Forward overlap (dst > src)
     * Array state: [ 1,  2,  3,  4,  5 ]
     * Pointers:      ^   ^
     *                a   b
     */
    my_memmove(b, a, 3);
    uint8_t expected[] = {1, 1, 2, 3, 5};
    TEST_CHECK(is_same_content_memory(a, expected, 5) == true);
    
    /*
     * Block 2: Backward overlap (dst < src)
     * Array state: [ 1,  1,  2,  3,  5 ]
     * Pointers:      ^   ^
     *                b   c
     */
    uint8_t* c = a + 1;
    b = a;
    my_memmove(b, c, 4);
    uint8_t expected_2[] = {1, 2, 3, 5, 5};
    TEST_CHECK(is_same_content_memory(a, expected_2, 5) == true);
    
    /*
     * Block 3: Non-overlapping / Separate bounds
     * Array state: [ 1,  2,  3,  5,  5 ]
     * Pointers:      ^           ^
     *                b           c
     */
    c = a + 3;
    my_memmove(b, c, 2);
    uint8_t expected_3[] = {5, 5, 3, 5, 5};
    TEST_CHECK(is_same_content_memory(a, expected_3, 5) == true);
    
    /*
     * Block 4: Identical pointers (dst == src)
     * Array state: [ 5,  5,  3,  5,  5 ]
     * Pointers:      ^
     *                a
     */
    my_memmove(a, a, 5);
    TEST_CHECK(is_same_content_memory(a, expected_3, 5) == true);
}

void test_memmove_overlap_at_boundary() {
    uint8_t a[] = {1, 2, 3, 4, 5};
    uint8_t* b = a + 2;
    my_memmove(b, a, 3);
    uint8_t exp_1[] = {1, 2, 1, 2, 3};
    TEST_CHECK(is_same_content_memory(a, exp_1, 5));
}

void test_memmove_string() {
    const char* s = "Hello World";
    const uint32_t len = strlen(s);
    char* a = malloc(len + 1);
    memcpy(a, s, len + 1);
    
    char* b = a + 1;
    my_memmove(b, a, 1);
    TEST_CHECK(strcmp(a, "HHllo World") == 0);
    
    b = a + 6;
    my_memmove(b, a, 5);
    TEST_CHECK(strcmp(a, "HHllo HHllo") == 0);
    
    b = a + 2;
    my_memmove(a, b, 6);
    TEST_CHECK(strcmp(a, "llo HHHHllo") == 0);
    
    free(a);
}

TEST_LIST = {
    {"test_memmove_numbers", test_memmove_numbers},
    {"test_memmove_string", test_memmove_string},
    {"test_memmove_overlap_at_boundary", test_memmove_overlap_at_boundary},

    { NULL, NULL }
};