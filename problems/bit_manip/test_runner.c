#include "acutest.h"
#include "test_bit_manip.h"

TEST_LIST = {
    {"Get 1 Bit", test_get_bit},
    {"Get Bit Range", test_get_bits},
    {"Set 1 Bit", test_set_bit},
    {"Set Bit Range", test_set_bits},
    {"Clear 1 Bit", test_clear_bit},
    {"Clear Bit Range", test_clear_bits},
    {"Toggle 1 Bit", test_toggle_bit},
    {"Toggle Bit Range", test_toggle_bits},
    
    { NULL, NULL } // This MUST be at the end to signal end of list
};