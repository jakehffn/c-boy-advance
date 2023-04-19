#include "ARM7TDMI_instructions.h"
#include <stdio.h>

uint32_t registers[18];

INSTRUCTION_GROUP ARM7TDMI_decode_group(InstructionWord inst) {

    if (inst.decode_bits.cond == UN) {
        return UNCONDITIONAL;
    }

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

    case 0b001:
        if (inst.decode_bits.b_24_23 == 0b10) {
            if (inst.decode_bits.b_20 == 0b0) {
                if (inst.decode_bits.b_22_21 & 0b01) {
                    return MOVE_IMM_TO_STAT;
                } else {
                    return UNDEFINED;
                }
            } 
        }
        return DATA_PROC_IMM;

    case 0b010:
        return LOAD_STORE_IMM_OFFSET;

    case 0b011:
        if (inst.decode_bits.b_4 == 0b0) {
            return LOAD_STORE_REG_OFFSET;
        } else {
            if (inst.decode_bits.b_24_23 == 0b11 &&
                inst.decode_bits.b_22_21 == 0b11 &&
                inst.decode_bits.b_20 == 0b1 &&
                inst.decode_bits.b_7 == 0b1 &&
                inst.decode_bits.b_6_5 == 0b11 && 
                inst.decode_bits.b_4 == 0b1) {
                    return ARCH_UNDEFINED;
            } else {
                return MEDIA;
            }
        }

    case 0b100:
        return LOAD_STORE_MULTIPLE;

    case 0b101:
        return BRANCH;

    case 0b110:
        return COPROC_LOAD_STORE_AND_DOUBLE_REG_TRANS;

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
    
    default:
        return UNKNOWN;
    }
}