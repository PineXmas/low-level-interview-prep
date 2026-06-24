#include "acutest.h"
#include "my_memcpy.h"
#include "utils.h"
#include <stdlib.h>
#include <time.h>

void test_memcpy_numbers() {
    uint8_t a[] = {1, 2, 3, 4, 5};
    uint8_t b[5];
    my_memcpy(b, a, 5);
    TEST_CHECK(is_same_content_memory(a, b, 5) == true);
    
    uint8_t c[] = {9, 9, 9, 9, 9};
    my_memcpy(c, b, 4);
    TEST_CHECK(is_same_content_memory(c, b, 4) == true);
    TEST_CHECK(c[4] == 9);
}

void test_memcpy_string() {
    char* a = "Hello There!";
    uint32_t n_bytes = strlen(a);
    char* b = malloc(n_bytes);
    
    my_memcpy(b, a, n_bytes);
    b[n_bytes] = 0;
    TEST_CHECK(strcmp(a, b) == 0);
    
    free(b);
}

void test_memcpy_random() {
    srand(time(NULL));
    uint32_t n = rand() % 50 + 1;
    uint32_t n_bytes = n * sizeof(uint32_t);
    uint32_t* a = malloc(n_bytes);
    uint32_t* b = malloc(n_bytes);
    
    for (uint32_t i=0; i<n; i++) {
        a[i] = (uint32_t)rand();
    }
    
    my_memcpy(b, a, n_bytes);
    TEST_CHECK(is_same_content_memory(a, b, n_bytes));
    
    free(b);
    free(a);
}

TEST_LIST = {
    {"test_memcpy_numbers", test_memcpy_numbers},
    {"test_memcpy_string", test_memcpy_string},
    {"test_memcpy_random", test_memcpy_random},

    { NULL, NULL }
};