#include "ARM7TDMI_instructions.h"
#include <stdio.h>

int main() {

    // Branch always forward 0xC0
    InstructionWord b_al_0xC0 = { 0b11101010000000000000000000101110 };

    ARM7TDMI_decode(b_al_0xC0);
}