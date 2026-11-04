#include "bit_manip.h"

#include <gtest/gtest.h>

TEST(BitManip, GetOneBit) {
    uint32_t reg = 0x5;
    EXPECT_EQ(get_bit(reg, 0), 1);
    EXPECT_EQ(get_bit(reg, 1), 0);
    EXPECT_EQ(get_bit(reg, 2), 1);

    reg = 0x80000001;
    EXPECT_EQ(get_bit(reg, 0), 1);
    EXPECT_EQ(get_bit(reg, 31), 1);
}

TEST(BitManip, GetBitRange) {
    uint32_t reg = 0xAB;
    EXPECT_EQ(get_bits(reg, 0, 3), 0xB);
    EXPECT_EQ(get_bits(reg, 4, 7), 0xA);
    EXPECT_EQ(get_bits(reg, 1, 5), 0x15);
    EXPECT_EQ(get_bits(reg, 0, 7), 0xAB);
}

TEST(BitManip, SetOneBit) {
    uint32_t reg = 0x0;
    set_bit(&reg, 0);
    EXPECT_EQ(reg, 0x1);
    set_bit(&reg, 2);
    EXPECT_EQ(reg, 0x5);
    set_bit(&reg, 31);
    EXPECT_EQ(reg, 0x80000005);
}

TEST(BitManip, SetBitRange) {
    uint32_t reg = 0x8;
    set_bits(&reg, 0, 2);
    EXPECT_EQ(reg, 0xF);
    set_bits(&reg, 5, 6);
    EXPECT_EQ(reg, 0x6F);
    set_bits(&reg, 29, 31);
    EXPECT_EQ(reg, 0xE000006F);
    set_bits(&reg, 0, 31);
    EXPECT_EQ(reg, 0xFFFFFFFF);
}

TEST(BitManip, ClearOneBit) {
    uint32_t reg = 0xC;
    clear_bit(&reg, 3);
    EXPECT_EQ(reg, 0x4);
    clear_bit(&reg, 2);
    EXPECT_EQ(reg, 0x0);
    reg = 0;
    clear_bit(&reg, 22);
    EXPECT_EQ(reg, 0x0);
    reg = 0xF0000001;
    clear_bit(&reg, 31);
    EXPECT_EQ(reg, 0x70000001);
}

TEST(BitManip, ClearBitRange) {
    uint32_t reg = 0xBD;
    clear_bits(&reg, 0, 2);
    EXPECT_EQ(reg, 0xB8);
    clear_bits(&reg, 4, 5);
    EXPECT_EQ(reg, 0x88);
    clear_bits(&reg, 1, 6);
    EXPECT_EQ(reg, 0x80);
    clear_bits(&reg, 0, 7);
    EXPECT_EQ(reg, 0x0);
}

TEST(BitManip, ToggleOneBit) {
    uint32_t reg = 0x3A;
    toggle_bit(&reg, 6);
    EXPECT_EQ(reg, 0x7A);
    toggle_bit(&reg, 3);
    EXPECT_EQ(reg, 0x72);
    toggle_bit(&reg, 5);
    EXPECT_EQ(reg, 0x52);
    toggle_bit(&reg, 31);
    EXPECT_EQ(reg, 0x80000052);
}

TEST(BitManip, ToggleBitRange) {
    uint32_t reg = 0x57;
    toggle_bits(&reg, 2, 3);
    EXPECT_EQ(reg, 0x5B);
    toggle_bits(&reg, 3, 5);
    EXPECT_EQ(reg, 0x63);
    toggle_bits(&reg, 2, 4);
    EXPECT_EQ(reg, 0x7F);
    toggle_bits(&reg, 0, 31);
    EXPECT_EQ(reg, 0xFFFFFF80);
}
