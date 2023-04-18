#include "ARM7TDMI_instructions.h"
#include <stdio.h>

INSTRUCTION_GROUP ARM7TDMI_decode_group(InstructionWord inst) {

    printf("Decoding %x\n", inst.word);

    switch (inst.decode_bits.b_27_25) {
    case 0b000:

        if (inst.decode_bits.b_7 == 0b1 && inst.decode_bits.b_4 == 0b1 ) {
            return MULTIPLIES_OR_EX_LOAD_STORE;
        }

        if (inst.decode_bits.b_4 == 0b0) {
            if (inst.decode_bits.b_20 == 0b0 && inst.decode_bits.b_24_23 == 0b10) {
                return MISC_1;
            } else {
                return DATA_PROC_IMM_SHIFT;
            }
        } else {
            if (inst.decode_bits.b_20 == 0b0 && 
                inst.decode_bits.b_24_23 == 0b10 &&
                inst.decode_bits.b_7 == 0b0) {
                    return MISC_2;
            } else {
                return DATA_PROC_REG_SHIFT;
            }
        }

        break;

    case 0b001:
        break;

    case 0b010:
        break;

    case 0b011:
        break;

    case 0b100:
        return LOAD_STORE_MULTIPLE;
        break;

    case 0b101:
        return BRANCH;
        break;

    case 0b110:
        return COPROC_LOAD_STORE_AND_DOUBLE_REG_TRANS;
        break;

    case 0b111:
        if (inst.decode_bits.b_24_23 & 0b10) {
            return SOFTWARE_INT;
        } else {
            if (inst.decode_bits.b_4 == 0b0) {
                return COPROC_DATA_PROC;
            } else {
                return COPROC_REG_TRANS;
            }
        }
        break;
    
    default:
        break;
    }
}