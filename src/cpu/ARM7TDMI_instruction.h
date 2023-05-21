#pragma once

#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "bit_manip.h"

// Program Status Register
typedef struct {
    union {
        struct {
            unsigned int M : 5; // Mode
            unsigned int T : 1; // Thumb mode status
            unsigned int F : 1; 
            unsigned int I : 1;
            unsigned int A : 1;
            unsigned int E : 1;
            unsigned int : 14;
            unsigned int J : 1;
            unsigned int : 2;
            unsigned int Q : 1;
            unsigned int V : 1;
            unsigned int C : 1;
            unsigned int Z : 1;
            unsigned int N : 1;
        };

        uint32_t data;
    };
} PSR;

typedef struct {
    uint32_t registers[16];
    PSR ps_registers[6];
    uint32_t fiq_registers[7];
    uint32_t svc_registers[2];
    uint32_t abt_registers[2];
    uint32_t irq_registers[2];
    uint32_t und_registers[2];

    uint8_t* memory;

} ARM7TDMI;

typedef enum {
    REGISTER_R0,
    REGISTER_R1,
    REGISTER_R2,
    REGISTER_R3,
    REGISTER_R4,
    REGISTER_R5,
    REGISTER_R6,
    REGISTER_R7,  // Banked registers:
    REGISTER_R8,  //   R8_fiq,
    REGISTER_R9,  //   R9_fiq,
    REGISTER_R10, //   R10_fiq,
    REGISTER_R11, //   R11_fiq,
    REGISTER_R12, //   R12_fiq,
    REGISTER_R13, //   R13_fiq, R13_svc, R13_abt, R13_irq, R13_und,
    REGISTER_R14, //   R14_fiq, R14_svc, R14_abt, R14_irq, R14_und,
    REGISTER_R15,

    REGISTER_SP = 13,
    REGISTER_LR = 14,
    REGISTER_PC = 15
} REGISTER;

typedef enum {
    PS_REGISTER_CPSR,  // Current Program Status Register

    // fiq - FIQ
    // svc - Supervisor
    // abt - Abort
    // irq - IRQ
    // und - Undefined
    PS_REGISTER_SPSR_fiq, // Saved Program Status Registers
    PS_REGISTER_SPSR_svc,
    PS_REGISTER_SPSR_abt,
    PS_REGISTER_SPSR_irq,
    PS_REGISTER_SPSR_und,
} PS_REGISTER;

typedef enum {
    CONDITION_CODE_EQ, // Equal - Z set
    CONDITION_CODE_NE, // Not Equal - Z clear
    CONDITION_CODE_CS_HS, // Carry set/unsigned higher or same - C set
    CONDITION_CODE_CC_LO, // Carry clear/ unsigned lower - C clear
    CONDITION_CODE_MI, // Minus/negative - N set
    CONDITION_CODE_PL, // Plus/positive or zero - N clear
    CONDITION_CODE_VS, // Overflow - V set
    CONDITION_CODE_VC, // No overflow - V clear
    CONDITION_CODE_HI, // Unsinged higher - C set and Z clear
    CONDITION_CODE_LS, // Unsigned lower or same - C clear or Z set
    CONDITION_CODE_GE, // Signed greater than or equal - N == V
    CONDITION_CODE_LT, // Signed less than - N != V
    CONDITION_CODE_GT, // Signed greater than - Z == 0 && N == V
    CONDITION_CODE_LE, // Signed less than or equal - Z == 1 && N != V
    CONDITION_CODE_AL, // Always
    CONDITION_CODE_UN // Unconditional
} CONDITION_CODE;

// Data Processing Instruction OpCodes
typedef enum {
    DATA_PROC_OPCODE_AND, // Logical AND
    DATA_PROC_OPCODE_EOR, // Logical Exclusive OR
    DATA_PROC_OPCODE_SUB, // Subtract
    DATA_PROC_OPCODE_RSB, // Reverse Subtract
    DATA_PROC_OPCODE_ADD, // Add
    DATA_PROC_OPCODE_ADC, // Add with Carry
    DATA_PROC_OPCODE_SBC, // Subtract with Carry
    DATA_PROC_OPCODE_RSC, // Reverse Subtract with Carry
    DATA_PROC_OPCODE_TST, // Test
    DATA_PROC_OPCODE_TEQ, // Test Equivalence
    DATA_PROC_OPCODE_CMP, // Compare
    DATA_PROC_OPCODE_CMN, // Compare Negated
    DATA_PROC_OPCODE_ORR, // Logical (inclusive) OR
    DATA_PROC_OPCODE_MOV, // Move
    DATA_PROC_OPCODE_BIC, // Bit Clear
    DATA_PROC_OPCODE_MVN // Move Not
} DATA_PROC_OPCODE;

typedef enum {
    SHIFT_LSL, // Logical Shift Left
    SHIFT_LSR, // logical Shift Right
    SHIFT_ASR, // Arithmetic  Shift Right
    SHIFT_ROR // Rotate Right
} SHIFT;

typedef enum {
    INSTRUCTION_GROUP_DATA_PROC_IMM_SHIFT, // Data Processing (Register) Immediate Shift
    INSTRUCTION_GROUP_MISC_1, // Miscellaneous group 1
    INSTRUCTION_GROUP_DATA_PROC_REG_SHIFT, // Data Processing (Register) Register Shift
    INSTRUCTION_GROUP_MISC_2, // Miscellaneous group 2
    INSTRUCTION_GROUP_MULTIPLIES_OR_EX_LOAD_STORE, // Multiplies or Extra Loads/Stores
    INSTRUCTION_GROUP_DATA_PROC_IMM, // Data Processing Immediate
    INSTRUCTION_GROUP_UNDEFINED,
    INSTRUCTION_GROUP_MOVE_IMM_TO_STAT, // Move Immediate to Status Register
    INSTRUCTION_GROUP_LOAD_STORE_IMM_OFFSET, // Load/Store Immediate offset
    INSTRUCTION_GROUP_LOAD_STORE_REG_OFFSET, // Load/Store Register offset
    INSTRUCTION_GROUP_MEDIA, // Media Instructions
    INSTRUCTION_GROUP_ARCH_UNDEFINED, // Architecturally Undefined
    INSTRUCTION_GROUP_LOAD_STORE_MULTIPLE, // Load/Store Multiple registers
    INSTRUCTION_GROUP_BRANCH, // Branch and Branch with Link
    INSTRUCTION_GROUP_COPROC_LOAD_STORE_AND_DOUBLE_REG_TRANS, // Coprocessor Load/Store or Double Register Transfers
    INSTRUCTION_GROUP_COPROC_DATA_PROC, // Coprocessor Data Processsing  
    INSTRUCTION_GROUP_COPROC_REG_TRANS, // Coprocessor Data Transfers
    INSTRUCTION_GROUP_SOFTWARE_INT, // Software Interrupt
    INSTRUCTION_GROUP_UNCONDITIONAL, // Unconditional Instructions

    INSTRUCTION_GROUP_UNKNOWN // This case should never be found
} INSTRUCTION_GROUP;

// Data Processing (Register) Immediate Shift Instruction format
typedef struct {
    REGISTER Rm : 4;
    unsigned int : 1; // Must be 0b0
    SHIFT shift : 2;
    unsigned int shift_amount : 5;
    REGISTER Rd : 4; // Destination register
    REGISTER Rn : 4; // First source operand register
    unsigned int S : 1; // Signifies condition codes updates
    DATA_PROC_OPCODE opcode : 4;
    unsigned int : 3; // Must be 0b000
    CONDITION_CODE cond : 4;
} DataProcImmShiftInstruction;

// NEEDS INFO
typedef struct {
    unsigned int : 4;
    unsigned int : 1; // Must be 0b0
    unsigned int : 15;
    unsigned int : 1; // Must be 0b0
    unsigned int : 4; // 0b10xx
    unsigned int : 3; // Must be 0b000
    CONDITION_CODE cond : 4;
} Misc1Instruction;

// Data Processing (Register) Register Shift Instruction format
typedef struct {
    REGISTER Rm : 4;
    unsigned int : 1; // Must be 0b1;
    SHIFT shift : 2;
    unsigned int : 1; // Must be 0b0
    REGISTER Rs : 4;
    REGISTER Rd : 4; // Destination register
    REGISTER Rn : 4; // First source operand register
    unsigned int S : 1; // Signifies condition codes updates
    DATA_PROC_OPCODE opcode : 4;
    unsigned int : 3; // Must be 0b000
    CONDITION_CODE cond : 4;
} DataProcRegShiftInstruction;

// NEEDS INFO
typedef struct {
    unsigned int : 4;
    unsigned int : 1; // Must be 0b1
    unsigned int : 2;
    unsigned int : 1; // Must be 0b0
    unsigned int : 12;
    unsigned int : 1; // Must be 0b0
    unsigned int : 4; // 0b10xx
    unsigned int : 3; // Must be 0b000
    CONDITION_CODE cond : 4;
} Misc2Instruction;

// NEEDS INFO
typedef struct {
    unsigned int : 4;
    unsigned int : 1; // Must be 0b1
    unsigned int : 2;
    unsigned int : 1; // Must be 0b1
    unsigned int : 17;
    unsigned int : 3; // Must be 0b000
    CONDITION_CODE cond : 4;
} MultipliesInstruction;

// Extra Load and Store Instruction format
// Load and Store Unsigned Half-Word and Sign-Extend Half-Word or Byte
typedef struct {
    unsigned int addr_mode_2 : 4;
    unsigned int : 1; // Must be 0b1
    unsigned int H : 1; // Load type specifier
    unsigned int S : 1; // Load type specifier
    unsigned int : 1; // Must be 0b1
    unsigned int addr_mode_1 : 4;
    REGISTER Rd : 4; // Source
    REGISTER Rn : 4; // Base register
    unsigned int L : 1; // Load type specifier
    unsigned int W : 1; 
    unsigned int I : 1;
    unsigned int U : 1;
    unsigned int P : 1;
    unsigned int : 3; // Must be 0b000
    CONDITION_CODE cond : 4;
} ExLoadStoreInstruction;

// Data Processing Immediate Instruction format
typedef struct {
    unsigned int immediate : 8;
    unsigned int rotate : 4;
    REGISTER Rd : 4; // Destination register
    REGISTER Rn : 4; // First source operand register
    unsigned int S : 1; // Signifies condition codes updates
    DATA_PROC_OPCODE opcode : 4;
    unsigned int : 3; // Must be 0b001
    CONDITION_CODE cond : 4;
} DataProcImmInstruction;

// NEEDS INFO
typedef struct {
    unsigned int : 20;
    unsigned int : 2; // Must be 0b00
    unsigned int : 1;
    unsigned int : 2; // Must be 0b10
    unsigned int : 3; // Must be 0b001
    CONDITION_CODE cond : 4;
} UndefinedInstruction;

// Move Immediate to Status Register Instruction format
// NEEDS INFO
typedef struct {
    unsigned int immediate : 8;
    unsigned int rotate : 4;
    unsigned int SBO : 4; 
    unsigned int mask : 4; 
    unsigned int : 2; // Must be 0b10
    unsigned int R : 1;
    unsigned int : 2; // Must be 0b10
    unsigned int : 3; // Must be 0b001
    CONDITION_CODE cond : 4;
} MoveImmToStatInstruction;

// Load and Store Word or Unsigned Byte Instruction format
typedef struct {
    unsigned int immediate : 12; // Immediate offset
    REGISTER Rd : 4; // Source
    REGISTER Rn : 4; // Base register
    unsigned int L : 1; // Distinguishes between a Load (L == 1) and a Store instruction (L == 0)
    unsigned int W : 1; 
    unsigned int B : 1; // Distinguishes unsigned unsigned byte (B == 1) and word (B == 0) access
    unsigned int U : 1;
    unsigned int P : 1;
    unsigned int : 3; // Must be 0b010
    CONDITION_CODE cond : 4;
} LoadStoreImmOffsetInstruction;

// Load and Store Word or Unsigned Byte Instruction format
typedef struct {
    REGISTER Rm : 4; // Offset register
    unsigned int : 1; // Must be 0b0
    SHIFT shift : 2;
    unsigned int shift_amount : 5; 
    REGISTER Rd : 4; // Source
    REGISTER Rn : 4; // Base register
    unsigned int L : 1; // Distinguishes between a Load (L == 1) and a Store instruction (L == 0)
    unsigned int W : 1; 
    unsigned int B : 1; // Distinguishes unsigned unsigned byte (B == 1) and word (B == 0) access
    unsigned int U : 1;
    unsigned int P : 1;
    unsigned int : 3; // Must be 0b011
    CONDITION_CODE cond : 4;
} LoadStoreRegOffsetInstruction;

// NEEDS INFO
typedef struct {
    unsigned int : 4;
    unsigned int : 1; // Must be 0b1
    unsigned int : 20;
    unsigned int : 3; // Must be 0b011
    CONDITION_CODE cond : 4;
} MediaInstruction;

// NEEDS INFO
typedef struct {
    unsigned int : 4;
    unsigned int : 4; // Must be 0b1111
    unsigned int : 12;
    unsigned int : 5; // Must be 0b11111
    unsigned int : 3; // Must be 0b011
    CONDITION_CODE cond : 4;
} ArchUndefinedInstruction;

// Load and Store Multiple Instruction format
typedef struct {
    uint16_t register_list; // Each register is represented by a bit
    REGISTER Rn : 4; // Base register
    unsigned int L : 1; // Distinguishes between a Load (L == 1) and a Store (L == 0) instruction
    unsigned int W : 1; // Addressing mode specifier
    unsigned int S : 1;
    unsigned int U : 1; // Addressing mode specifier
    unsigned int P : 1; // Addressing mode specifier
    unsigned int : 3; // Must be 0b100
    CONDITION_CODE cond : 4;
} LoadStoreMultInstruction;

// Branch and Branch with Link Instruction format
typedef struct {
    unsigned int offset : 24;
    unsigned int L : 1; // Preserve return in Link
    unsigned int : 3; // Must be 0b101
    CONDITION_CODE cond : 4;
} BranchInstruction;


// NEEDS INFO
typedef struct {
    unsigned int offset : 8;
    unsigned int cp_num : 4;
    REGISTER CRd : 4; // Source
    REGISTER Rn : 4; // Base register
    unsigned int L : 1; // Distinguishes between a Load (L == 1) and a Store (L == 0) instruction
    unsigned int W : 1;
    unsigned int N : 1;
    unsigned int U : 1; 
    unsigned int P : 1; 
    unsigned int : 3; // Must be 0b110
    CONDITION_CODE cond : 4;
} CoprocLoadStoreInstruction;

// NEEDS INFO
typedef struct {
    unsigned int offset : 8;
    unsigned int cp_num : 4;
    REGISTER CRd : 4; 
    REGISTER Rn : 4; 
    unsigned int L : 1; 
    unsigned int W : 1;
    unsigned int N : 1;
    unsigned int U : 1; 
    unsigned int P : 1; 
    unsigned int : 3; // Must be 0b110
    CONDITION_CODE cond : 4;
} DoubleRegTransInstruction; 

// NEEDS INFO
typedef struct {
    REGISTER CRm : 4;
    unsigned int : 1; // Must be 0b0
    unsigned int opcode2 : 3;
    unsigned int cp_num : 4;
    REGISTER CRd : 4; 
    REGISTER CRn : 4; 
    unsigned int opcode1 : 4;
    unsigned int : 4; // Must be 0b1110
    CONDITION_CODE cond : 4;
} CoprocDataProcInstruction; 

// NEEDS INFO
typedef struct {
    REGISTER CRm : 4;
    unsigned int : 1; // Must be 0b1
    unsigned int opcode2 : 3;
    unsigned int cp_num : 4;
    REGISTER Rd : 4; 
    REGISTER CRn : 4; 
    unsigned int L : 1;
    unsigned int opcode1 : 3;
    unsigned int : 4; // Must be 0b1110
    CONDITION_CODE cond : 4;
} CoprocRegTransInstruction; 

// NEEDS INFO
typedef struct {
    unsigned int swi_number : 24;
    unsigned int : 4; // Must be 0b1111
    CONDITION_CODE cond : 4;
} SoftwareIntInstruction;

// NEEDS INFO
typedef struct {
    unsigned int : 28;
    unsigned int : 4; // Must be 0b1111
} UnconditionalInstruction;

typedef struct {
    unsigned int : 4;
    unsigned int b_4 : 1;
    unsigned int b_6_5: 2;
    unsigned int b_7 : 1;
    unsigned int : 12;
    unsigned int b_20 : 1;
    unsigned int b_22_21 : 2;
    unsigned int b_24_23 : 2;
    unsigned int b_27_25 : 3;
    CONDITION_CODE cond : 4;
} DecodeBits;

typedef struct {
    union {
        uint32_t word;
        struct {
            uint16_t hword_low;
            uint16_t hword_high;
        };

        DataProcImmShiftInstruction data_proc_imm_shift_instruction;
        Misc1Instruction misc_1_instruction;
        DataProcRegShiftInstruction data_proc_reg_shift_instruction;
        Misc2Instruction misc_2_instruction;
        MultipliesInstruction multiplies_instruction;
        ExLoadStoreInstruction ex_load_store_instruction;
        DataProcImmInstruction data_proc_imm_instruction;
        UndefinedInstruction undefined_instruction;
        MoveImmToStatInstruction move_imm_to_stat_instruction;
        LoadStoreImmOffsetInstruction load_store_imm_offset_instruction;
        LoadStoreRegOffsetInstruction load_store_reg_offset_instruction;
        MediaInstruction media_instruction;
        ArchUndefinedInstruction arch_undefined_instruction;
        LoadStoreMultInstruction load_store_mult_instruction;
        BranchInstruction branch_instruction;
        CoprocLoadStoreInstruction coproc_load_store_instruction;
        DoubleRegTransInstruction double_reg_trans_instruction;
        CoprocDataProcInstruction coproc_data_proc_instruction;
        CoprocRegTransInstruction coproc_reg_trans_instruction;
        SoftwareIntInstruction software_int_instruction;
        UnconditionalInstruction unconditional_instruction;

        DecodeBits decode_bits;
    };
} InstructionWord;

INSTRUCTION_GROUP ARM7TDMI_decode_group(InstructionWord inst);
bool ARM7TDMI_check_condition(ARM7TDMI* cpu, CONDITION_CODE cond);
void ARM7TDMI_execute(ARM7TDMI* cpu, InstructionWord inst);
PSR ARM7TDMI_calculate_shifter_operand(uint32_t* shifter_operand, PSR old_flags, SHIFT shift, uint32_t shift_amount, bool Imm);

void ARM7TDMI_execute_data_proc_imm_shift_instruction(ARM7TDMI* cpu, InstructionWord inst);
void ARM7TDMI_execute_misc_1_instruction(ARM7TDMI* cpu, InstructionWord inst);
void ARM7TDMI_execute_data_proc_reg_shift_instruction(ARM7TDMI* cpu, InstructionWord inst);
void ARM7TDMI_execute_misc_2_instruction(ARM7TDMI* cpu, InstructionWord inst);
void ARM7TDMI_execute_multiplies_ex_load_store_instruction(ARM7TDMI* cpu, InstructionWord inst);
void ARM7TDMI_execute_data_proc_imm_instruction(ARM7TDMI* cpu, InstructionWord inst);
void ARM7TDMI_execute_undefined_instruction(ARM7TDMI* cpu, InstructionWord inst);
void ARM7TDMI_execute_move_imm_to_stat_instruction(ARM7TDMI* cpu, InstructionWord inst);
void ARM7TDMI_execute_load_store_imm_offset_instruction(ARM7TDMI* cpu, InstructionWord inst);
void ARM7TDMI_execute_load_store_reg_offset_instruction(ARM7TDMI* cpu, InstructionWord inst);
void ARM7TDMI_execute_media_instruction(ARM7TDMI* cpu, InstructionWord inst);
void ARM7TDMI_execute_arch_undefined_instruction(ARM7TDMI* cpu, InstructionWord inst);
void ARM7TDMI_execute_load_store_mult_instruction(ARM7TDMI* cpu, InstructionWord inst);
void ARM7TDMI_execute_branch_instruction(ARM7TDMI* cpu, InstructionWord inst);
void ARM7TDMI_execute_coproc_load_store_and_double_reg_trans_instruction(ARM7TDMI* cpu, InstructionWord inst);
void ARM7TDMI_execute_coproc_data_proc_instruction(ARM7TDMI* cpu, InstructionWord inst);
void ARM7TDMI_execute_coproc_reg_trans_instruction(ARM7TDMI* cpu, InstructionWord inst);
void ARM7TDMI_execute_software_int_instruction(ARM7TDMI* cpu, InstructionWord inst);
void ARM7TDMI_execute_unconditional_instruction(ARM7TDMI* cpu, InstructionWord inst);

void ARM7TDMI_execute_addressing_mode_1_instruction(
    ARM7TDMI* cpu,
    DATA_PROC_OPCODE opcode, 
    REGISTER Rd, 
    REGISTER Rn, 
    uint32_t shifter_operand, 
    PSR shifter_carry_out, 
    bool S
);

void ARM7TDMI_execute_addressing_mode_2_instruction(
    ARM7TDMI* cpu, 
    REGISTER Rd, 
    REGISTER Rn, 
    size_t address_offset, 
    bool P, 
    bool U, 
    bool B, 
    bool W, 
    bool L, 
    CONDITION_CODE cond
);

PSR ARM7TDMI_data_proc_and(ARM7TDMI* cpu, REGISTER Rd, uint32_t Rn, uint32_t shifter_operand);
PSR ARM7TDMI_data_proc_eor(ARM7TDMI* cpu, REGISTER Rd, uint32_t Rn, uint32_t shifter_operand);
PSR ARM7TDMI_data_proc_sub(ARM7TDMI* cpu, REGISTER Rd, uint32_t Rn, uint32_t shifter_operand);
PSR ARM7TDMI_data_proc_rsb(ARM7TDMI* cpu, REGISTER Rd, uint32_t Rn, uint32_t shifter_operand);
PSR ARM7TDMI_data_proc_add(ARM7TDMI* cpu, REGISTER Rd, uint32_t Rn, uint32_t shifter_operand);
PSR ARM7TDMI_data_proc_adc(ARM7TDMI* cpu, REGISTER Rd, uint32_t Rn, uint32_t shifter_operand);
PSR ARM7TDMI_data_proc_sbc(ARM7TDMI* cpu, REGISTER Rd, uint32_t Rn, uint32_t shifter_operand);
PSR ARM7TDMI_data_proc_rsc(ARM7TDMI* cpu, REGISTER Rd, uint32_t Rn, uint32_t shifter_operand);
PSR ARM7TDMI_data_proc_tst(ARM7TDMI* cpu, REGISTER Rd, uint32_t Rn, uint32_t shifter_operand);
PSR ARM7TDMI_data_proc_teq(ARM7TDMI* cpu, REGISTER Rd, uint32_t Rn, uint32_t shifter_operand);
PSR ARM7TDMI_data_proc_cmp(ARM7TDMI* cpu, REGISTER Rd, uint32_t Rn, uint32_t shifter_operand);
PSR ARM7TDMI_data_proc_cmn(ARM7TDMI* cpu, REGISTER Rd, uint32_t Rn, uint32_t shifter_operand);
PSR ARM7TDMI_data_proc_orr(ARM7TDMI* cpu, REGISTER Rd, uint32_t Rn, uint32_t shifter_operand);
PSR ARM7TDMI_data_proc_mov(ARM7TDMI* cpu, REGISTER Rd, uint32_t Rn, uint32_t shifter_operand);
PSR ARM7TDMI_data_proc_bic(ARM7TDMI* cpu, REGISTER Rd, uint32_t Rn, uint32_t shifter_operand);
PSR ARM7TDMI_data_proc_mvn(ARM7TDMI* cpu, REGISTER Rd, uint32_t Rn, uint32_t shifter_operand);