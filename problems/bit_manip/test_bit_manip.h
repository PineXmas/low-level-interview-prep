#ifndef TEST_BIT_MANIP_H
#define TEST_BIT_MANIP_H

#include "acutest.h"
#include "bit_manip.h"

void test_get_bit() {
    uint32_t reg = 0x5;
    TEST_CHECK(get_bit(reg, 0) == 1);
    TEST_CHECK(get_bit(reg, 1) == 0);
    TEST_CHECK(get_bit(reg, 2) == 1);
    
    reg = 0x80000001;
    TEST_CHECK(get_bit(reg, 0) == 1);
    TEST_CHECK(get_bit(reg, 31) == 1);
}

void test_get_bits() {
    uint32_t reg = 0xAB;    // binary: 1010 1011
    
    // 1011
    TEST_CHECK(get_bits(reg, 0, 3) == 0xB);
    
    // 1010
    TEST_CHECK(get_bits(reg, 4, 7) == 0xA);
    
    // 1 0101
    TEST_CHECK(get_bits(reg, 1, 5) == 0x15);
    
    // all
    TEST_CHECK(get_bits(reg, 0, 7) == 0xAB);
}

void test_set_bit() {
    uint32_t reg = 0x0;
    
    // 0001
    set_bit(&reg, 0);
    TEST_CHECK(reg == 0x1);
    
    // 0101
    set_bit(&reg, 2);
    TEST_CHECK(reg == 0x5);
    
    // 1000 ... 0101
    set_bit(&reg, 31);
    TEST_CHECK(reg == 0x80000005);
}

void test_set_bits() {
    uint32_t reg = 0x8;     // binary: 1000
    
    // 1111
    set_bits(&reg, 0, 2);
    TEST_CHECK(reg == 0xF);
    
    // 0110 1111
    set_bits(&reg, 5, 6);
    TEST_CHECK(reg == 0x6F);
    
    // 1110 ... 0110 1111
    set_bits(&reg, 29, 31);
    TEST_CHECK(reg == 0xE000006F);
    
    // all
    set_bits(&reg, 0, 31);
    TEST_CHECK(reg == 0xFFFFFFFF);
}

void test_clear_bit() {
    uint32_t reg = 0xC;     // binary: 1100
    
    // 0100
    clear_bit(&reg, 3);
    TEST_CHECK(reg == 0x4);
    
    // 0000
    clear_bit(&reg, 2);
    TEST_CHECK(reg == 0x0);
    
    reg = 0;
    clear_bit(&reg, 22);
    TEST_CHECK(reg == 0x0);
    
    reg = 0xF0000001;
    clear_bit(&reg, 31);
    TEST_CHECK(reg == 0x70000001);
}

void test_clear_bits() {
    uint32_t reg = 0xBD;    // binary: 1011 1101
    
    // 1011 1000
    clear_bits(&reg, 0, 2);
    TEST_CHECK(reg == 0xB8);
    
    // 1000 1000
    clear_bits(&reg, 4, 5);
    TEST_CHECK(reg == 0x88);
    
    // 1000 0000
    clear_bits(&reg, 1, 6);
    TEST_CHECK(reg == 0x80);
    
    // 0
    clear_bits(&reg, 0, 7);
    TEST_CHECK(reg == 0x0);
}

void test_toggle_bit() {
    uint32_t reg = 0x3A;    // binary: 0011 1010
    
    // 0111 1010
    toggle_bit(&reg, 6);
    TEST_CHECK(reg == 0x7A);
    
    // 0111 0010
    toggle_bit(&reg, 3);
    TEST_CHECK(reg == 0x72);
    
    // 0101 0010
    toggle_bit(&reg, 5);
    TEST_CHECK(reg == 0x52);
    
    // 1000 ... 0101 0010
    toggle_bit(&reg, 31);
    TEST_CHECK(reg == 0x80000052);
}

void test_toggle_bits() {
    uint32_t reg = 0x57;    // binary: 0101 0111
    
    // 0101 1011
    toggle_bits(&reg, 2, 3);
    TEST_CHECK(reg == 0x5B);
    
    // 0110 0011
    toggle_bits(&reg, 3, 5);
    TEST_CHECK(reg == 0x63);
    
    // 0111 1111
    toggle_bits(&reg, 2, 4);
    TEST_CHECK(reg == 0x7F);
    
    // 1111 1111 ... 1000 0000
    toggle_bits(&reg, 0, 31);
    TEST_CHECK(reg == 0xFFFFFF80);
}

#endif  // #ifndef TEST_BIT_MANIP_H