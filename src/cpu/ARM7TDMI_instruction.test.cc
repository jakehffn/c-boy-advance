#include <gtest/gtest.h>

extern "C" {
  #include "ARM7TDMI_instructions.h"
}

// Decode Tests
TEST(cpu_decode_group, BRANCH) {
  EXPECT_EQ(ARM7TDMI_decode_group((InstructionWord){0xea00002e}), BRANCH); // b #0xc0 
  EXPECT_EQ(ARM7TDMI_decode_group((InstructionWord){0x3a00397e}), BRANCH); // bcc #0xE600
  EXPECT_EQ(ARM7TDMI_decode_group((InstructionWord){0xeaffc67e}), BRANCH); // b #-0xE600
}

TEST(cpu_decode_group, DATA_PROC_IMM) {
  EXPECT_EQ(ARM7TDMI_decode_group((InstructionWord){0xe2800042}), DATA_PROC_IMM); // add r0, #0x42 
  EXPECT_EQ(ARM7TDMI_decode_group((InstructionWord){0xe3a070ff}), DATA_PROC_IMM); // mov r7, #0xff
  EXPECT_EQ(ARM7TDMI_decode_group((InstructionWord){0xe3370007}), DATA_PROC_IMM); // teq r7, #0x7
}