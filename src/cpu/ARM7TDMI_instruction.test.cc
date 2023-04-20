#include <gtest/gtest.h>

extern "C" {
  #include "ARM7TDMI_instructions.h"
}

TEST(cpu_decode_group, DATA_PROC_IMM_SHIFT) {
  EXPECT_EQ(ARM7TDMI_decode_group((InstructionWord){0xe1a07005}), DATA_PROC_IMM_SHIFT); // mov r7, r5 
  EXPECT_EQ(ARM7TDMI_decode_group((InstructionWord){0xe0411000}), DATA_PROC_IMM_SHIFT); // sub r1, r0 
  EXPECT_EQ(ARM7TDMI_decode_group((InstructionWord){0xe0854205}), DATA_PROC_IMM_SHIFT); // add r4, r5, r5, lsl #4
}

TEST(cpu_decode_group, DATA_PROC_REG_SHIFT) {
  EXPECT_EQ(ARM7TDMI_decode_group((InstructionWord){0xe0854115}), DATA_PROC_REG_SHIFT); // add r4, r5, r5, lsl r1
}

TEST(cpu_decode_group, DATA_PROC_IMM) {
  EXPECT_EQ(ARM7TDMI_decode_group((InstructionWord){0xe2800042}), DATA_PROC_IMM); // add r0, #0x42 
  EXPECT_EQ(ARM7TDMI_decode_group((InstructionWord){0xe3a070ff}), DATA_PROC_IMM); // mov r7, #0xff
  EXPECT_EQ(ARM7TDMI_decode_group((InstructionWord){0xe3370007}), DATA_PROC_IMM); // teq r7, #0x7
}

TEST(cpu_decode_group, BRANCH) {
  EXPECT_EQ(ARM7TDMI_decode_group((InstructionWord){0xea00002e}), BRANCH); // b #0xc0 
  EXPECT_EQ(ARM7TDMI_decode_group((InstructionWord){0x3a00397e}), BRANCH); // bcc #0xE600
  EXPECT_EQ(ARM7TDMI_decode_group((InstructionWord){0xeaffc67e}), BRANCH); // b #-0xE600
}