#include <gtest/gtest.h>

extern "C" {
  #include "ARM7TDMI_instructions.h"
}

// Decode Tests
TEST(cpu_decode_group, BRANCH) {
  EXPECT_EQ(ARM7TDMI_decode_group((InstructionWord){0xea00002e}), BRANCH); // b #0xc0 
  EXPECT_EQ(ARM7TDMI_decode_group((InstructionWord){0xea00397e}), BRANCH); // b #0xE600
  EXPECT_EQ(ARM7TDMI_decode_group((InstructionWord){0xeaffc67e}), BRANCH); // b #-0xE600
}