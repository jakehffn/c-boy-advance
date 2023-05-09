#include "ARM7TDMI_instruction.h"
#include "ARM7TDMI_instruction_debug.h"

void (*instruction_group_func[])(ARM7TDMI* cpu, InstructionWord inst) = {
    [INSTRUCTION_GROUP_DATA_PROC_IMM_SHIFT] = ARM7TDMI_execute_data_proc_imm_shift_instruction,
    [INSTRUCTION_GROUP_MISC_1] = ARM7TDMI_execute_misc_1_instruction,
    [INSTRUCTION_GROUP_DATA_PROC_REG_SHIFT] = ARM7TDMI_execute_data_proc_reg_shift_instruction,
    [INSTRUCTION_GROUP_MISC_2] = ARM7TDMI_execute_misc_2_instruction,
    [INSTRUCTION_GROUP_MULTIPLIES_OR_EX_LOAD_STORE] = ARM7TDMI_execute_multiplies_ex_load_store_instruction,
    [INSTRUCTION_GROUP_DATA_PROC_IMM] = ARM7TDMI_execute_data_proc_imm_instruction,
    [INSTRUCTION_GROUP_UNDEFINED] = ARM7TDMI_execute_undefined_instruction,
    [INSTRUCTION_GROUP_MOVE_IMM_TO_STAT] = ARM7TDMI_execute_move_imm_to_stat_instruction,
    [INSTRUCTION_GROUP_LOAD_STORE_IMM_OFFSET] = ARM7TDMI_execute_load_store_imm_offset_instruction,
    [INSTRUCTION_GROUP_LOAD_STORE_REG_OFFSET] = ARM7TDMI_execute_load_store_reg_offset_instruction,
    [INSTRUCTION_GROUP_MEDIA] = ARM7TDMI_execute_media_instruction,
    [INSTRUCTION_GROUP_ARCH_UNDEFINED] = ARM7TDMI_execute_arch_undefined_instruction,
    [INSTRUCTION_GROUP_LOAD_STORE_MULTIPLE] = ARM7TDMI_execute_load_store_mult_instruction,
    [INSTRUCTION_GROUP_BRANCH] = ARM7TDMI_execute_branch_instruction,
    [INSTRUCTION_GROUP_COPROC_LOAD_STORE_AND_DOUBLE_REG_TRANS] = ARM7TDMI_execute_coproc_load_store_and_double_reg_trans_instruction,
    [INSTRUCTION_GROUP_COPROC_DATA_PROC] = ARM7TDMI_execute_coproc_data_proc_instruction,
    [INSTRUCTION_GROUP_COPROC_REG_TRANS] = ARM7TDMI_execute_coproc_reg_trans_instruction,
    [INSTRUCTION_GROUP_SOFTWARE_INT] = ARM7TDMI_execute_software_int_instruction,
    [INSTRUCTION_GROUP_UNCONDITIONAL] = ARM7TDMI_execute_unconditional_instruction
};

void (*data_proc_func[])(ARM7TDMI* cpu, REGISTER dest, REGISTER lint, uint32_t rint, bool S) = {
    [DATA_PROC_OPCODE_ADC] = ARM7TDMI_data_proc_and,
    [DATA_PROC_OPCODE_EOR] = ARM7TDMI_data_proc_eor,
    [DATA_PROC_OPCODE_SUB] = ARM7TDMI_data_proc_sub,
    [DATA_PROC_OPCODE_RSB] = ARM7TDMI_data_proc_rsb,
    [DATA_PROC_OPCODE_ADD] = ARM7TDMI_data_proc_add,
    [DATA_PROC_OPCODE_ADC] = ARM7TDMI_data_proc_adc,
    [DATA_PROC_OPCODE_SBC] = ARM7TDMI_data_proc_sbc,
    [DATA_PROC_OPCODE_RSC] = ARM7TDMI_data_proc_rsc,
    [DATA_PROC_OPCODE_TST] = ARM7TDMI_data_proc_tst,
    [DATA_PROC_OPCODE_TEQ] = ARM7TDMI_data_proc_teq,
    [DATA_PROC_OPCODE_CMP] = ARM7TDMI_data_proc_cmp,
    [DATA_PROC_OPCODE_CMN] = ARM7TDMI_data_proc_cmn,
    [DATA_PROC_OPCODE_ORR] = ARM7TDMI_data_proc_orr,
    [DATA_PROC_OPCODE_MOV] = ARM7TDMI_data_proc_mov,
    [DATA_PROC_OPCODE_BIC] = ARM7TDMI_data_proc_bic,
    [DATA_PROC_OPCODE_MVN] = ARM7TDMI_data_proc_mvn
};

INSTRUCTION_GROUP ARM7TDMI_decode_group(InstructionWord inst) {

    if (inst.decode_bits.cond == CONDITION_CODE_UN) {
        return INSTRUCTION_GROUP_UNCONDITIONAL;
    }

    switch (inst.decode_bits.b_27_25) {
    case 0b000:

        if (inst.decode_bits.b_7 == 0b1 && inst.decode_bits.b_4 == 0b1 ) {
            return INSTRUCTION_GROUP_MULTIPLIES_OR_EX_LOAD_STORE;
        }

        if (inst.decode_bits.b_4 == 0b0) {
            if (inst.decode_bits.b_20 == 0b0 && inst.decode_bits.b_24_23 == 0b10) {
                return INSTRUCTION_GROUP_MISC_1;
            } else {
                return INSTRUCTION_GROUP_DATA_PROC_IMM_SHIFT;
            }
        } else {
            if (inst.decode_bits.b_20 == 0b0 && 
                inst.decode_bits.b_24_23 == 0b10 &&
                inst.decode_bits.b_7 == 0b0) {
                    return INSTRUCTION_GROUP_MISC_2;
            } else {
                return INSTRUCTION_GROUP_DATA_PROC_REG_SHIFT;
            }
        }

    case 0b001:
        if (inst.decode_bits.b_24_23 == 0b10) {
            if (inst.decode_bits.b_20 == 0b0) {
                if (inst.decode_bits.b_22_21 & 0b01) {
                    return INSTRUCTION_GROUP_MOVE_IMM_TO_STAT;
                } else {
                    return INSTRUCTION_GROUP_UNDEFINED;
                }
            } 
        }
        return INSTRUCTION_GROUP_DATA_PROC_IMM;

    case 0b010:
        return INSTRUCTION_GROUP_LOAD_STORE_IMM_OFFSET;

    case 0b011:
        if (inst.decode_bits.b_4 == 0b0) {
            return INSTRUCTION_GROUP_LOAD_STORE_REG_OFFSET;
        } else {
            if (inst.decode_bits.b_24_23 == 0b11 &&
                inst.decode_bits.b_22_21 == 0b11 &&
                inst.decode_bits.b_20 == 0b1 &&
                inst.decode_bits.b_7 == 0b1 &&
                inst.decode_bits.b_6_5 == 0b11 && 
                inst.decode_bits.b_4 == 0b1) {
                    return INSTRUCTION_GROUP_ARCH_UNDEFINED;
            } else {
                return INSTRUCTION_GROUP_MEDIA;
            }
        }

    case 0b100:
        return INSTRUCTION_GROUP_LOAD_STORE_MULTIPLE;

    case 0b101:
        return INSTRUCTION_GROUP_BRANCH;

    case 0b110:
        return INSTRUCTION_GROUP_COPROC_LOAD_STORE_AND_DOUBLE_REG_TRANS;

    case 0b111:
        if (inst.decode_bits.b_24_23 & 0b10) {
            return INSTRUCTION_GROUP_SOFTWARE_INT;
        } else {
            if (inst.decode_bits.b_4 == 0b0) {
                return INSTRUCTION_GROUP_COPROC_DATA_PROC;
            } else {
                return INSTRUCTION_GROUP_COPROC_REG_TRANS;
            }
        }
    
    default:
        return INSTRUCTION_GROUP_UNKNOWN;
    }
} 

bool ARM7DMI_check_condition(ARM7TDMI* cpu, InstructionWord inst) {
    
    switch(inst.decode_bits.cond) {
    case CONDITION_CODE_EQ:
        return (cpu->flags & CPSR_FLAG_Z);
    case CONDITION_CODE_NE:
        return (!(cpu->flags & CPSR_FLAG_Z));
    case CONDITION_CODE_CS_HS:
        return (cpu->flags & CPSR_FLAG_C);
    case CONDITION_CODE_CC_LO:
        return (!(cpu->flags & CPSR_FLAG_C));
    case CONDITION_CODE_MI:
        return (cpu->flags & CPSR_FLAG_N);
    case CONDITION_CODE_PL:
        return (!(cpu->flags & CPSR_FLAG_N));
    case CONDITION_CODE_VS:
        return (cpu->flags & CPSR_FLAG_V);
    case CONDITION_CODE_VC:
        return (!(cpu->flags & CPSR_FLAG_V));
    case CONDITION_CODE_HI:
        return ((cpu->flags & CPSR_FLAG_C) && !(cpu->flags & CPSR_FLAG_Z));
    case CONDITION_CODE_LS:
        return (!(cpu->flags & CPSR_FLAG_C) || (cpu->flags & CPSR_FLAG_Z));
    case CONDITION_CODE_GE:
        return ((cpu->flags & CPSR_FLAG_N) == (cpu->flags & CPSR_FLAG_V));
    case CONDITION_CODE_LT:
        return ((cpu->flags & CPSR_FLAG_N) != (cpu->flags & CPSR_FLAG_V));
    case CONDITION_CODE_GT:
        return (!(cpu->flags & CPSR_FLAG_Z) && ((cpu->flags & CPSR_FLAG_N) == (cpu->flags & CPSR_FLAG_V)));
    case CONDITION_CODE_LE:
        return ((cpu->flags & CPSR_FLAG_Z) || ((cpu->flags & CPSR_FLAG_N) != (cpu->flags & CPSR_FLAG_V)));
    case CONDITION_CODE_AL:
        return true;
    default:
        return true;
    }
}

void ARM7DMI_execute(ARM7TDMI* cpu, InstructionWord inst) {

    if (ARM7DMI_check_condition(cpu, inst)) {
        (*instruction_group_func[ARM7TDMI_decode_group(inst)])(cpu, inst);
    }
}

void ARM7TDMI_execute_data_proc_imm_shift_instruction(ARM7TDMI* cpu, InstructionWord inst) {

}

void ARM7TDMI_execute_misc_1_instruction(ARM7TDMI* cpu, InstructionWord inst) {

}

void ARM7TDMI_execute_data_proc_reg_shift_instruction(ARM7TDMI* cpu, InstructionWord inst) {

}

void ARM7TDMI_execute_misc_2_instruction(ARM7TDMI* cpu, InstructionWord inst) {

}

void ARM7TDMI_execute_multiplies_ex_load_store_instruction(ARM7TDMI* cpu, InstructionWord inst) {

}

void ARM7TDMI_execute_data_proc_imm_instruction(ARM7TDMI* cpu, InstructionWord inst) {

    uint32_t rint = inst.data_proc_imm_instruction.immediate;
    unsigned int rotate = inst.data_proc_imm_instruction.rotate * 2;
    uint32_t carry;

    bool isArithmetic = inst.data_proc_imm_instruction.opcode < 0b0010 || 
        inst.data_proc_imm_instruction.opcode > 0b0111;

    // rotate right
    if (rotate != 0) {

        rint = bit_manip_ROR(rint, rotate*2);

        // If not an arithmetic operand, use the shift carry out
        if (!isArithmetic) {
            cpu->flags = bit_manip_set_mask(cpu->flags, CPSR_FLAG_C, rint >> 31);
        }
    }

    (*data_proc_func[inst.data_proc_imm_instruction.opcode])(
        cpu, 
        inst.data_proc_imm_instruction.Rd, 
        inst.data_proc_imm_instruction.Rn,
        rint,
        inst.data_proc_imm_instruction.S
    );
}

void ARM7TDMI_execute_undefined_instruction(ARM7TDMI* cpu, InstructionWord inst) {

}

void ARM7TDMI_execute_move_imm_to_stat_instruction(ARM7TDMI* cpu, InstructionWord inst) {

}

void ARM7TDMI_execute_load_store_imm_offset_instruction(ARM7TDMI* cpu, InstructionWord inst) {

}

void ARM7TDMI_execute_load_store_reg_offset_instruction(ARM7TDMI* cpu, InstructionWord inst) {

}

void ARM7TDMI_execute_media_instruction(ARM7TDMI* cpu, InstructionWord inst) {

}

void ARM7TDMI_execute_arch_undefined_instruction(ARM7TDMI* cpu, InstructionWord inst) {

}

void ARM7TDMI_execute_load_store_mult_instruction(ARM7TDMI* cpu, InstructionWord inst) {

}

void ARM7TDMI_execute_branch_instruction(ARM7TDMI* cpu, InstructionWord inst) {

}

void ARM7TDMI_execute_coproc_load_store_and_double_reg_trans_instruction(ARM7TDMI* cpu, InstructionWord inst) {

}

void ARM7TDMI_execute_coproc_data_proc_instruction(ARM7TDMI* cpu, InstructionWord inst) {

}

void ARM7TDMI_execute_coproc_reg_trans_instruction(ARM7TDMI* cpu, InstructionWord inst) {

}

void ARM7TDMI_execute_software_int_instruction(ARM7TDMI* cpu, InstructionWord inst) {

}

void ARM7TDMI_execute_unconditional_instruction(ARM7TDMI* cpu, InstructionWord inst) {

}

void ARM7TDMI_data_proc_and(ARM7TDMI* cpu, REGISTER dest, REGISTER lint, uint32_t rint, bool S) {
    LOG("AND%s r%d, r%d, #%d\n", (S) ? "S" : "", dest, lint, rint);
}

void ARM7TDMI_data_proc_eor(ARM7TDMI* cpu, REGISTER dest, REGISTER lint, uint32_t rint, bool S) {

}

void ARM7TDMI_data_proc_sub(ARM7TDMI* cpu, REGISTER dest, REGISTER lint, uint32_t rint, bool S) {

}

void ARM7TDMI_data_proc_rsb(ARM7TDMI* cpu, REGISTER dest, REGISTER lint, uint32_t rint, bool S) {

}

void ARM7TDMI_data_proc_add(ARM7TDMI* cpu, REGISTER dest, REGISTER lint, uint32_t rint, bool S) {
    LOG("ADD%s r%d, r%d, #0x%x\n", (S) ? "S" : "", dest, lint, rint);
}

void ARM7TDMI_data_proc_adc(ARM7TDMI* cpu, REGISTER dest, REGISTER lint, uint32_t rint, bool S) {

}

void ARM7TDMI_data_proc_sbc(ARM7TDMI* cpu, REGISTER dest, REGISTER lint, uint32_t rint, bool S) {

}

void ARM7TDMI_data_proc_rsc(ARM7TDMI* cpu, REGISTER dest, REGISTER lint, uint32_t rint, bool S) {

}

void ARM7TDMI_data_proc_tst(ARM7TDMI* cpu, REGISTER dest, REGISTER lint, uint32_t rint, bool S) {

}

void ARM7TDMI_data_proc_teq(ARM7TDMI* cpu, REGISTER dest, REGISTER lint, uint32_t rint, bool S) {

}

void ARM7TDMI_data_proc_cmp(ARM7TDMI* cpu, REGISTER dest, REGISTER lint, uint32_t rint, bool S) {

}

void ARM7TDMI_data_proc_cmn(ARM7TDMI* cpu, REGISTER dest, REGISTER lint, uint32_t rint, bool S) {

}

void ARM7TDMI_data_proc_orr(ARM7TDMI* cpu, REGISTER dest, REGISTER lint, uint32_t rint, bool S) {

}

void ARM7TDMI_data_proc_mov(ARM7TDMI* cpu, REGISTER dest, REGISTER lint, uint32_t rint, bool S) {

}

void ARM7TDMI_data_proc_bic(ARM7TDMI* cpu, REGISTER dest, REGISTER lint, uint32_t rint, bool S) {

}

void ARM7TDMI_data_proc_mvn(ARM7TDMI* cpu, REGISTER dest, REGISTER lint, uint32_t rint, bool S) {

}
