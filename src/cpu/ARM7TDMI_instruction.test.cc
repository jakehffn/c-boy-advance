#include <gtest/gtest.h>

extern "C" {
  #include "ARM7TDMI_instructions.h"
}

// DATA_PROC_IMM_SHIFT instructions
InstructionWord mov_r7_r5{0xe1a07005}; // mov r7, r5
InstructionWord sub_r1_r0{0xe0411000}; // sub r1, r0 
InstructionWord add_r4_r5_r5_lsl_4{0xe0854205}; // add r4, r5, r5, lsl #4

// DATA_PROC_REG_SHIFT instructions
InstructionWord add_r4_r5_r5_lsl_r1{0xe0854115}; // add r4, r5, r5, lsl r1

// DATA_PROC_IMM instructions
InstructionWord add_r0_0x42{0xe2800042}; // add r0, #0x42 
InstructionWord mov_r7_0xff{0xe3a070ff}; // mov r7, #0xff
InstructionWord teq_r7_0x7{0xe3370007}; // teq r7, #0x7

// BRANCH instructions
InstructionWord b_0xc0{0xea00002e}; // b #0xc0 
InstructionWord bcc_0xe600{0x3a00397e}; // bcc #0xe600
InstructionWord b_n0xe600{0xeaffc67e}; // b #-0xe600

TEST(cpu_decode_group, DATA_PROC_IMM_SHIFT) {
  EXPECT_EQ(ARM7TDMI_decode_group(mov_r7_r5), INSTRUCTION_GROUP_DATA_PROC_IMM_SHIFT);
  EXPECT_EQ(ARM7TDMI_decode_group(sub_r1_r0), INSTRUCTION_GROUP_DATA_PROC_IMM_SHIFT); 
  EXPECT_EQ(ARM7TDMI_decode_group(add_r4_r5_r5_lsl_4), INSTRUCTION_GROUP_DATA_PROC_IMM_SHIFT); 
}

TEST(cpu_decode_group, DATA_PROC_REG_SHIFT) {
  EXPECT_EQ(ARM7TDMI_decode_group(add_r4_r5_r5_lsl_r1), INSTRUCTION_GROUP_DATA_PROC_REG_SHIFT);
}

TEST(cpu_decode_group, DATA_PROC_IMM) {
  EXPECT_EQ(ARM7TDMI_decode_group(add_r0_0x42), INSTRUCTION_GROUP_DATA_PROC_IMM); 
  EXPECT_EQ(ARM7TDMI_decode_group(mov_r7_0xff), INSTRUCTION_GROUP_DATA_PROC_IMM); 
  EXPECT_EQ(ARM7TDMI_decode_group(teq_r7_0x7), INSTRUCTION_GROUP_DATA_PROC_IMM); 
}

TEST(cpu_decode_group, BRANCH) {
  EXPECT_EQ(ARM7TDMI_decode_group(b_0xc0), INSTRUCTION_GROUP_BRANCH); 
  EXPECT_EQ(ARM7TDMI_decode_group(bcc_0xe600), INSTRUCTION_GROUP_BRANCH);
  EXPECT_EQ(ARM7TDMI_decode_group(b_n0xe600), INSTRUCTION_GROUP_BRANCH); 
}