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

PSR (*data_proc_func[])(ARM7TDMI* cpu, REGISTER Rd, uint32_t Rn, uint32_t shifter_operand) = {
    [DATA_PROC_OPCODE_AND] = ARM7TDMI_data_proc_and,
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

bool ARM7TDMI_check_condition(ARM7TDMI* cpu, InstructionWord inst) {
    
    switch(inst.decode_bits.cond) {
    case CONDITION_CODE_EQ:
        return (cpu->ps_registers[PS_REGISTER_CPSR].Z);
    case CONDITION_CODE_NE:
        return (!(cpu->ps_registers[PS_REGISTER_CPSR].Z));
    case CONDITION_CODE_CS_HS:
        return (cpu->ps_registers[PS_REGISTER_CPSR].C);
    case CONDITION_CODE_CC_LO:
        return (!(cpu->ps_registers[PS_REGISTER_CPSR].C));
    case CONDITION_CODE_MI:
        return (cpu->ps_registers[PS_REGISTER_CPSR].N);
    case CONDITION_CODE_PL:
        return (!(cpu->ps_registers[PS_REGISTER_CPSR].N));
    case CONDITION_CODE_VS:
        return (cpu->ps_registers[PS_REGISTER_CPSR].V);
    case CONDITION_CODE_VC:
        return (!(cpu->ps_registers[PS_REGISTER_CPSR].V));
    case CONDITION_CODE_HI:
        return ((cpu->ps_registers[PS_REGISTER_CPSR].C) 
            && !(cpu->ps_registers[PS_REGISTER_CPSR].Z));
    case CONDITION_CODE_LS:
        return (!(cpu->ps_registers[PS_REGISTER_CPSR].C) 
            || (cpu->ps_registers[PS_REGISTER_CPSR].Z));
    case CONDITION_CODE_GE:
        return ((cpu->ps_registers[PS_REGISTER_CPSR].N) 
            == (cpu->ps_registers[PS_REGISTER_CPSR].V));
    case CONDITION_CODE_LT:
        return ((cpu->ps_registers[PS_REGISTER_CPSR].N) 
            != (cpu->ps_registers[PS_REGISTER_CPSR].V));
    case CONDITION_CODE_GT:
        return (!(cpu->ps_registers[PS_REGISTER_CPSR].Z) 
            && ((cpu->ps_registers[PS_REGISTER_CPSR].N) == (cpu->ps_registers[PS_REGISTER_CPSR].V)));
    case CONDITION_CODE_LE:
        return ((cpu->ps_registers[PS_REGISTER_CPSR].Z) 
            || ((cpu->ps_registers[PS_REGISTER_CPSR].N) 
            != (cpu->ps_registers[PS_REGISTER_CPSR].V)));
    case CONDITION_CODE_AL:
        return true;
    default:
        return true;
    }
}
 
// Calculates the shifter operand and returns the carry flag
PSR ARM7TDMI_calculate_shifter_operand(uint32_t* shifter_operand, PSR old_flags, DATA_PROC_SHIFT shift, uint32_t shift_amount, bool Imm) {

    PSR shifter_carry_out;

    switch (shift) {
    case DATA_PROC_SHIFT_LSL:
        if (shift_amount == 0) {
            shifter_carry_out.C = old_flags.C;
            // Shifter Operand remains unchanged
        } else if (shift_amount < 32) {
            shifter_carry_out.C = *shifter_operand >> (32 - shift_amount);
            *shifter_operand <<= shift_amount;
        } else if (shift_amount == 32) {
            shifter_carry_out.C = *shifter_operand;
            *shifter_operand = 0;
        } else {
            shifter_carry_out.C = 0;
            *shifter_operand = 0;
        }
        break;
    case DATA_PROC_SHIFT_LSR:
        if (shift_amount == 0) {
            shifter_carry_out.C = old_flags.C;
            // Shifter Operand remains unchanged
        } else if (shift_amount < 32) {
            shifter_carry_out.C = *shifter_operand >> (shift_amount - 1);
            *shifter_operand >>= shift_amount;
        } else if (shift_amount == 32) {
            shifter_carry_out.C = *shifter_operand >> 31;
            *shifter_operand = 0;
        } else {
            shifter_carry_out.C = 0;
            *shifter_operand = 0;
        }
        break;
    case DATA_PROC_SHIFT_ASR:
        if (shift_amount == 0) {
            if (Imm) {
                if ((*shifter_operand >> 31) == 0) {
                    shifter_carry_out.C = 0;
                    *shifter_operand = 0;
                } else {
                    shifter_carry_out.C = 1;
                    *shifter_operand = 0xffffffff;
                }
            } else {
                shifter_carry_out.C = old_flags.C;
                // Shifter Operand remains unchanged
            }
        } else if (shift_amount < 32) {
            shifter_carry_out.C = *shifter_operand >> (shift_amount - 1);
            *shifter_operand = bit_manip_ASR(*shifter_operand, shift_amount);
        } else {
            if ((*shifter_operand >> 31) == 0) {
                shifter_carry_out.C = 0;
                *shifter_operand = 0;
            } else {
                shifter_carry_out.C = 1;
                *shifter_operand = 0xffffffff;
            }
        }
        break;
    case DATA_PROC_SHIFT_ROR:
        if (shift_amount == 0) {
            if (Imm) {
                shifter_carry_out.C = *shifter_operand;
                *shifter_operand = (old_flags.C << 31) | (*shifter_operand >> 1);
            } else {
                shifter_carry_out.C = old_flags.C;
                // Shifter Operand remains unchanged
            }
        } else {
            if (Imm) {
                shifter_carry_out.C = *shifter_operand >> (shift_amount - 1);
                *shifter_operand = bit_manip_ASR(*shifter_operand, shift_amount);
            } else {
                if ((shift_amount & 0b11111) == 0) {
                    shifter_carry_out.C = *shifter_operand >> 31;
                    // Shifter Operand remains unchanged
                } else {
                    shifter_carry_out.C = *shifter_operand >> ((shift_amount & 0b11111) - 1);
                    *shifter_operand = bit_manip_ASR(*shifter_operand, (shift_amount & 0b11111));
                }
            }
        }
        break;
    default:
        break;
    }

    return shifter_carry_out;
}

void ARM7TDMI_execute(ARM7TDMI* cpu, InstructionWord inst) {

    if (ARM7TDMI_check_condition(cpu, inst)) {
        (*instruction_group_func[ARM7TDMI_decode_group(inst)])(cpu, inst);
    }
}

void ARM7TDMI_execute_data_proc_imm_shift_instruction(ARM7TDMI* cpu, InstructionWord inst) {

    REGISTER Rm = inst.data_proc_imm_shift_instruction.Rm;

    uint32_t shifter_operand = cpu->registers[Rm] + (Rm == REGISTER_PC) ? 8 : 0;
    PSR shifter_carry_out = ARM7TDMI_calculate_shifter_operand(
        &shifter_operand, 
        cpu->ps_registers[PS_REGISTER_CPSR],
        inst.data_proc_imm_shift_instruction.shift, 
        inst.data_proc_imm_shift_instruction.shift_amount,
        true
    );

    ARM7TDMI_execute_data_proc_instruction(
        cpu, 
        inst.data_proc_imm_shift_instruction.opcode,
        inst.data_proc_imm_shift_instruction.Rd,
        inst.data_proc_imm_shift_instruction.Rn,
        shifter_operand,
        shifter_carry_out,
        inst.data_proc_imm_shift_instruction.S
    );
}

void ARM7TDMI_execute_misc_1_instruction(ARM7TDMI* cpu, InstructionWord inst) {

}

void ARM7TDMI_execute_data_proc_reg_shift_instruction(ARM7TDMI* cpu, InstructionWord inst) {
    
    REGISTER Rm = inst.data_proc_reg_shift_instruction.Rm;

    uint32_t shifter_operand = cpu->registers[Rm] + (Rm == REGISTER_PC) ? 8 : 0;
    PSR shifter_carry_out = ARM7TDMI_calculate_shifter_operand(
        &shifter_operand, 
        cpu->ps_registers[PS_REGISTER_CPSR],
        inst.data_proc_reg_shift_instruction.shift, 
        inst.data_proc_reg_shift_instruction.Rs & 0xff,
        false
    );

    ARM7TDMI_execute_data_proc_instruction(
        cpu, 
        inst.data_proc_reg_shift_instruction.opcode,
        inst.data_proc_reg_shift_instruction.Rd,
        inst.data_proc_reg_shift_instruction.Rn,
        shifter_operand,
        shifter_carry_out,
        inst.data_proc_reg_shift_instruction.S
    );
}

void ARM7TDMI_execute_misc_2_instruction(ARM7TDMI* cpu, InstructionWord inst) {

}

void ARM7TDMI_execute_multiplies_ex_load_store_instruction(ARM7TDMI* cpu, InstructionWord inst) {

}

void ARM7TDMI_execute_data_proc_imm_instruction(ARM7TDMI* cpu, InstructionWord inst) {

    uint32_t shifter_operand = inst.data_proc_imm_instruction.immediate;
    unsigned int rotate = inst.data_proc_imm_instruction.rotate * 2;

    PSR shifter_carry_out = {.C = 0};

    if (rotate != 0) {
        shifter_operand = bit_manip_ROR(shifter_operand, rotate*2);
        shifter_carry_out.C = shifter_operand >> 31;
    }

    ARM7TDMI_execute_data_proc_instruction(
        cpu, 
        inst.data_proc_imm_instruction.opcode,
        inst.data_proc_imm_instruction.Rd,
        inst.data_proc_imm_shift_instruction.Rn,
        shifter_operand,
        shifter_carry_out,
        inst.data_proc_imm_shift_instruction.S
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

void ARM7TDMI_execute_data_proc_instruction(ARM7TDMI* cpu, DATA_PROC_OPCODE opcode, REGISTER Rd, uint32_t Rn, uint32_t shifter_operand, PSR shifter_carry_out, bool S) {

    PSR new_flags = (*data_proc_func[opcode])(
        cpu, 
        Rd, 
        cpu->registers[Rn] + (Rn == REGISTER_PC) ? 8 : 0,
        shifter_operand
    );

    if (S) {

        if (Rd == REGISTER_R15) {
            // !TODO
            // CPSR = SPSR for current mode
        } else {
            
            // If the instruction is not an arithmetic instuction, the carry flag is set to the carry from the shifter
            if (opcode > 0b0111 || opcode < 0b0010) {
                new_flags.C = shifter_carry_out.C;
            }

            cpu->ps_registers[PS_REGISTER_CPSR] = new_flags;
        }
    }
}

PSR ARM7TDMI_data_proc_and(ARM7TDMI* cpu, REGISTER Rd, uint32_t Rn, uint32_t shifter_operand) {
    LOG("AND r%d, r%d, #%d\n", Rd, Rn, shifter_operand);
    cpu->registers[Rd] = Rn & shifter_operand;

    PSR new_flags = cpu->ps_registers[PS_REGISTER_CPSR];
    new_flags.N = cpu->registers[Rd] >> 31;
    new_flags.Z = cpu->registers[Rd] == 0;
    return new_flags;
}

PSR ARM7TDMI_data_proc_eor(ARM7TDMI* cpu, REGISTER Rd, uint32_t Rn, uint32_t shifter_operand) {
    LOG("EOR r%d, r%d, #%d\n", Rd, Rn, shifter_operand);
    cpu->registers[Rd] = Rn ^ shifter_operand;

    PSR new_flags = cpu->ps_registers[PS_REGISTER_CPSR];
    new_flags.N = cpu->registers[Rd] >> 31;
    new_flags.Z = cpu->registers[Rd] == 0;
    return new_flags;
}

PSR ARM7TDMI_data_proc_sub(ARM7TDMI* cpu, REGISTER Rd, uint32_t Rn, uint32_t shifter_operand) {
    LOG("SUB r%d, r%d, #%d\n", Rd, Rn, shifter_operand);
    cpu->registers[Rd] = Rn - shifter_operand;

    PSR new_flags = cpu->ps_registers[PS_REGISTER_CPSR];
    new_flags.N = cpu->registers[Rd] >> 31;
    new_flags.Z = cpu->registers[Rd] == 0;
    new_flags.C = cpu->registers[Rd] > shifter_operand;
    new_flags.V = ((Rn ^ shifter_operand) & ~(cpu->registers[Rd] ^ shifter_operand)) >> 31;
    return new_flags;
}

PSR ARM7TDMI_data_proc_rsb(ARM7TDMI* cpu, REGISTER Rd, uint32_t Rn, uint32_t shifter_operand) {
    LOG("RSB r%d, r%d, #%d\n", Rd, Rn, shifter_operand);
    cpu->registers[Rd] = shifter_operand - Rn;

    PSR new_flags = cpu->ps_registers[PS_REGISTER_CPSR];
    new_flags.N = cpu->registers[Rd] >> 31;
    new_flags.Z = cpu->registers[Rd] == 0;
    new_flags.C = cpu->registers[Rd] > shifter_operand;
    new_flags.V = ((Rn ^ shifter_operand) & ~(shifter_operand ^ cpu->registers[Rd])) >> 31;
    return new_flags;
}

PSR ARM7TDMI_data_proc_add(ARM7TDMI* cpu, REGISTER Rd, uint32_t Rn, uint32_t shifter_operand) {
    LOG("ADD r%d, r%d, #%d\n", Rd, Rn, shifter_operand);
    cpu->registers[Rd] = Rn + shifter_operand;

    PSR new_flags = cpu->ps_registers[PS_REGISTER_CPSR];
    new_flags.N = cpu->registers[Rd] >> 31;
    new_flags.Z = cpu->registers[Rd] == 0;
    new_flags.C = cpu->registers[Rd] < shifter_operand;
    new_flags.V = ((cpu->registers[Rd] ^ Rn) & (cpu->registers[Rd] ^ shifter_operand)) >> 31;
    return new_flags;
}

PSR ARM7TDMI_data_proc_adc(ARM7TDMI* cpu, REGISTER Rd, uint32_t Rn, uint32_t shifter_operand) {
    LOG("ADC r%d, r%d, #%d\n", Rd, Rn, shifter_operand);
    cpu->registers[Rd] = Rn + shifter_operand + cpu->ps_registers[PS_REGISTER_CPSR].C;

    PSR new_flags = cpu->ps_registers[PS_REGISTER_CPSR];
    new_flags.N = cpu->registers[Rd] >> 31;
    new_flags.Z = cpu->registers[Rd] == 0;
    new_flags.C = cpu->registers[Rd] < shifter_operand;
    new_flags.V = ((cpu->registers[Rd] ^ Rn) & (cpu->registers[Rd] ^ shifter_operand)) >> 31;
    return new_flags;
}

PSR ARM7TDMI_data_proc_sbc(ARM7TDMI* cpu, REGISTER Rd, uint32_t Rn, uint32_t shifter_operand) {
    LOG("SBC r%d, r%d, #%d\n", Rd, Rn, shifter_operand);
    cpu->registers[Rd] = Rn - shifter_operand - ~cpu->ps_registers[PS_REGISTER_CPSR].C;

    PSR new_flags = cpu->ps_registers[PS_REGISTER_CPSR];
    new_flags.N = cpu->registers[Rd] >> 31;
    new_flags.Z = cpu->registers[Rd] == 0;
    new_flags.C = cpu->registers[Rd] > shifter_operand;
    new_flags.V = ((Rn ^ shifter_operand) & ~(cpu->registers[Rd] ^ shifter_operand)) >> 31;
    return new_flags;
}

PSR ARM7TDMI_data_proc_rsc(ARM7TDMI* cpu, REGISTER Rd, uint32_t Rn, uint32_t shifter_operand) {
    LOG("RSC r%d, r%d, #%d\n", Rd, Rn, shifter_operand);
    cpu->registers[Rd] = shifter_operand - Rn - ~cpu->ps_registers[PS_REGISTER_CPSR].C;

    PSR new_flags = cpu->ps_registers[PS_REGISTER_CPSR];
    new_flags.N = cpu->registers[Rd] >> 31;
    new_flags.Z = cpu->registers[Rd] == 0;
    new_flags.C = cpu->registers[Rd] > shifter_operand;
    new_flags.V = ((Rn ^ shifter_operand) & ~(shifter_operand ^ cpu->registers[Rd])) >> 31;
    return new_flags;
}

PSR ARM7TDMI_data_proc_tst(ARM7TDMI* cpu, REGISTER Rd, uint32_t Rn, uint32_t shifter_operand) {
    LOG("TST r%d, r%d, #%d\n", Rd, Rn, shifter_operand);
    uint32_t result = Rn & shifter_operand;

    PSR new_flags = cpu->ps_registers[PS_REGISTER_CPSR];
    new_flags.N = result >> 31;
    new_flags.Z = result == 0;
    return new_flags;
}

PSR ARM7TDMI_data_proc_teq(ARM7TDMI* cpu, REGISTER Rd, uint32_t Rn, uint32_t shifter_operand) {
    LOG("TEQ r%d, r%d, #%d\n", Rd, Rn, shifter_operand);
    uint32_t result = Rn ^ shifter_operand;

    PSR new_flags = cpu->ps_registers[PS_REGISTER_CPSR];
    new_flags.N = result >> 31;
    new_flags.Z = result == 0;
    return new_flags;
}

PSR ARM7TDMI_data_proc_cmp(ARM7TDMI* cpu, REGISTER Rd, uint32_t Rn, uint32_t shifter_operand) {
    LOG("CMP r%d, r%d, #%d\n", Rd, Rn, shifter_operand);
    uint32_t result = Rn - shifter_operand;

    PSR new_flags = cpu->ps_registers[PS_REGISTER_CPSR];
    new_flags.N = result >> 31;
    new_flags.Z = result == 0;
    new_flags.C = result > shifter_operand;
    new_flags.V = ((Rn ^ shifter_operand) & ~(result ^ shifter_operand)) >> 31;
    return new_flags;
}

PSR ARM7TDMI_data_proc_cmn(ARM7TDMI* cpu, REGISTER Rd, uint32_t Rn, uint32_t shifter_operand) {
    LOG("CMN r%d, r%d, #%d\n", Rd, Rn, shifter_operand);
    uint32_t result = Rn + shifter_operand;

    PSR new_flags = cpu->ps_registers[PS_REGISTER_CPSR];
    new_flags.N = result >> 31;
    new_flags.Z = result == 0;
    new_flags.C = result < shifter_operand;
    new_flags.V = ((result ^ Rn) & (result ^ shifter_operand)) >> 31;
    return new_flags;
}

PSR ARM7TDMI_data_proc_orr(ARM7TDMI* cpu, REGISTER Rd, uint32_t Rn, uint32_t shifter_operand) {
    LOG("ORR r%d, r%d, #%d\n", Rd, Rn, shifter_operand);
    cpu->registers[Rd] = Rn | shifter_operand;

    PSR new_flags = cpu->ps_registers[PS_REGISTER_CPSR];
    new_flags.N = cpu->registers[Rd] >> 31;
    new_flags.Z = cpu->registers[Rd] == 0;
    return new_flags;
}

PSR ARM7TDMI_data_proc_mov(ARM7TDMI* cpu, REGISTER Rd, uint32_t Rn, uint32_t shifter_operand) {
    LOG("MOV r%d, r%d, #%d\n", Rd, Rn, shifter_operand);
    cpu->registers[Rd] = shifter_operand;
    
    PSR new_flags = cpu->ps_registers[PS_REGISTER_CPSR];
    new_flags.N = cpu->registers[Rd] >> 31;
    new_flags.Z = cpu->registers[Rd] == 0;
    return new_flags;
}

PSR ARM7TDMI_data_proc_bic(ARM7TDMI* cpu, REGISTER Rd, uint32_t Rn, uint32_t shifter_operand) {
    LOG("BIC r%d, r%d, #%d\n", Rd, Rn, shifter_operand);
    cpu->registers[Rd] = Rn & ~shifter_operand;

    PSR new_flags = cpu->ps_registers[PS_REGISTER_CPSR];
    new_flags.N = cpu->registers[Rd] >> 31;
    new_flags.Z = cpu->registers[Rd] == 0;
    return new_flags;
}

PSR ARM7TDMI_data_proc_mvn(ARM7TDMI* cpu, REGISTER Rd, uint32_t Rn, uint32_t shifter_operand) {
    LOG("MVN r%d, r%d, #%d\n", Rd, Rn, shifter_operand);
    cpu->registers[Rd] = ~shifter_operand;

    PSR new_flags = cpu->ps_registers[PS_REGISTER_CPSR];
    new_flags.N = cpu->registers[Rd] >> 31;
    new_flags.Z = cpu->registers[Rd] == 0;
    return new_flags;
}
