// Little-endian based

#pragma once

#include <stdint.h>

// fiq - FIQ
// svc - Supervisor
// abt - Abort
// irq - IRQ
// und - Undefined
enum REGISTER {
    R0,
    R1,
    R2,
    R3,
    R4,
    R5,
    R6,
    R7,
    R8, // R8_fiq,
    R9, // R9_fiq,
    R10, // R10_fiq,
    R11, // R11_fiq,
    R12, // R12_fiq,
    R13, // SP, R13_fiq, R13_svc, R13_abt, R13_irq, R13_und,
    R14, // LR, R14_fiq, R14_svc, R14_abt, R14_irq, R14_und,
    R15, // PC
    CPSR,
    SPSR

    // SPSR_fiq,
    // SPSR_svc,
    // SPSR_abt,
    // SPSR_irq,
    // SPSR_und,
};

uint32_t registers[18];

enum CPSR_FLAG {
    M0 = 1, // Mode Bits
    M1 = 1 << 1,
    M2 = 1 << 2,
    M3 = 1 << 3,
    M4 = 1 << 4,

    T = 1 << 5, // State Bit (0=ARM, 1=THUMB)
    F = 1 << 6, // FIQ disable (0=enable, 1=disable)
    I = 1 << 7, // IRQ disable (0=enable, 1=disable)
    A = 1 << 8, // Abort disable (1=Disable Imprecise Data Aborts)
    E = 1 << 9, // Endian
    // 10-23 reserved
    J = 1 << 24, // Jazelle Mode (1=Jazelle Bytecode instructions)
    // 25-26 reserved
    Q = 1 << 27, // Sticky Overflow (1=Sticky Overflow)
    V = 1 << 28, // Overflow Flag
    C = 1 << 29, // Carry/Borrow Flag
    Z = 1 << 30, // Zero Flag
    N = 1 << 31  // Sign Flag
};

enum CONDITION_CODE {
    EQ, // Equal - Z set
    NE, // Not Equal - Z clear
    CS_HS, // Carry set/unsigned higher or same - C set
    CC_LO, // Carry clear/ unsigned lower - C clear
    MI, // Minus/negative - N set
    PL, // Plus/positive or zero - N clear
    VS, // Overflow - V set
    VC, // No overflow - V clear
    HI, // Unsinged higher - C set and Z clear
    LS, // Unsigned lower or same - C clear or Z set
    GE, // Signed greater than or equal - N == V
    LT, // Signed less than - N != V
    GT, // Signed greater than - Z == 0 && N == V
    LE, // Signed less than or equal - Z == 1 && N != V
    AL, // Always
    VAR, // Other info
};

// Data Processing Instruction OpCodes
enum DATA_PROC_OPCODE {
    AND, // Logical AND
    EOR, // Logical Exclusive OR
    SUB, // Subtract
    RSB, // Reverse Subtract
    ADD, // Add
    ADC, // Add with Carry
    SBC, // Subtract with Carry
    RSC, // Reverse Subtract with Carry
    TST, // Test
    TEQ, // Test Equivalence
    CMP, // Compare
    CMN, // Compare Negated
    ORR, // Logical (inclusive) OR
    MOV, // Move
    BIC, // Bit Clear
    MVN // Move Not
};

enum INSTRUCTION_GROUP {
    DATA_PROC_IMM_SHIFT, // Data Processing (Register) Immediate Shift
    MISC_1, // Miscellaneous group 1
    DATA_PROC_REG_SHIFT, // Data Processing (Register) Register Shift
    MISC_2, // Miscellaneous group 2
    MULTIPLIES, // Multiplies              | 
    EX_LOAD_STORE, // Extra Loads/Stores   | These are the same in the documentation table
    DATA_PROC_IMM, // Data Processing Immediate
    UNDEFINED,
    MOVE_IMM_TO_STAT, // Move Immediate to Status Register
    LOAD_STORE_IMM_OFFSET, // Load/Store Immediate offset
    LOAD_STORE_REG_OFFSET, // Load/Store Register offset
    MEDIA, // Media Instructions
    ARCH_UNDEFINED, // Architecturally Undefined
    BRANCH, // Branch and Branch with Link
    COPROC_LOAD_STORE, // Coprocessor Load/Store       |
    DOUBLE_REG_TRANS, // Double Register Transfers     | These were also split apart
    COPROC_DATA_PROC, // Coprocessor Data Processsing  
    COPROC_REG_TRANS, // Coprocessor Data Transfers
    SOFTWARE_INT, // Software Interrupt
    UNCONDITIONAL, // Unconditional Instructions
};

// Data Processing (Register) Immediate Shift Instruction format
typedef struct {
    REGISTER Rm : 4;
    unsigned int : 0; // Must be 0b0
    unsigned int shift;
    unsigned int shift_amount : 5;
    REGISTER Rd : 4; // Destination register
    REGISTER Rn : 4; // First source operand register
    unsigned int S : 1; // Signifies condition codes updates
    DATA_PROC_OPCODE opcode : 4;
    unsigned int : 3; // Must be 0b000
    CONDITION_CODE cond : 4;
} Data_Proc_Imm_Shift_Instruction;

// Data Processing (Register) Register Shift Instruction format
typedef struct {
    REGISTER Rm : 4;
    unsigned int : 1; // Must be 0b1;
    unsigned int shift : 2;
    unsigned int : 1; // Must be 0b0
    REGISTER Rs : 4;
    REGISTER Rd : 4; // Destination register
    REGISTER Rn : 4; // First source operand register
    unsigned int S : 1; // Signifies condition codes updates
    DATA_PROC_OPCODE opcode : 4;
    unsigned int : 3; // Must be 0b000
    CONDITION_CODE cond : 4;
} Data_Proc_Reg_Shift_Instruction;

typedef struct {
    unsigned int : 4;
    unsigned int : 1; // Must be 0b1
    unsigned int : 2;
    unsigned int : 1; // Must be 0b1
    unsigned int : 17;
    unsigned int : 3; // Must be 0b000
    CONDITION_CODE cond : 4;
} Multiplies_Instruction;

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
} Ex_Load_Store_Instruction;

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
} Data_Proc_Imm_Instruction;

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
} Load_Store_Imm_Offset_Instruction;

// Load and Store Word or Unsigned Byte Instruction format
typedef struct {
    REGISTER Rm : 4; // Offset register
    unsigned int : 1; // Must be 0b1
    unsigned int shift : 2;
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
} Load_Store_Reg_Offset_Instruction;

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
} Load_Store_Mult_Instruction;

// Branch Instruction format
typedef struct {
    unsigned int offset : 24; // 
    unsigned int L : 1; // Preserve return in Link
    unsigned int : 3; // Must be 0b101
    CONDITION_CODE cond : 4;
} Branch_Instruction;

struct Instruction {
    union {
        uint32_t word;
        struct {
            uint16_t hword_low;
            uint16_t hword_high;
        };
    };
};

/*
Decoding instruction bit check order: 
27 26 25
21 22 23 24
20 
4
7

*/