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
enum DPI_OPCODE {
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

// Data Processing Instruction format
typedef struct {
    unsigned int shifter_operand : 12;
    REGISTER Rd : 4; // Destination register
    REGISTER Rn : 4; // First source operand register
    unsigned int S : 1; // Signifies condition codes updates
    DPI_OPCODE opcode : 4;
    unsigned int I : 1; // Distinguishes immediate and register form of shifter_operand
    unsigned int : 2; // Must be 0b00
    CONDITION_CODE cond : 4;
} DPI_Instruction;

// Load and Store Word or Unsigned Byte Instruction format
typedef struct {
    unsigned int addr_mode : 12; // Addressing mode specific
    REGISTER Rd : 4; // Source
    REGISTER Rn : 4; // Base register
    unsigned int L : 1; // Distinguishes between a Load (L == 1) and a Store instruction (L == 0)
    unsigned int W : 1; 
    unsigned int B : 1; // Distinguishes unsigned unsigned byte (B == 1) and word (B == 0) access
    unsigned int U : 1;
    unsigned int P : 1;
    unsigned int I : 1;
    unsigned int : 2; // Must be 0b01
    CONDITION_CODE cond : 4;
} LDSTO_WUB_Instruction;

// Load and Store Unsigned Half-Word and Sign-Extend Half-Word or Byte Instruction format
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
} LDSTO_UHW_SEHWB_Instruction;

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
} LDSTO_Mult_Instruction;

struct Instruction {
    union {
        uint32_t word;
        struct {
            uint16_t hword_low;
            uint16_t hword_high;
        };

        DPI_Instruction dpi_instruction;
        LDSTO_WUB_Instruction ldsto_wub_instruction;
        LDSTO_UHW_SEHWB_Instruction ldsto_uhw_sehwb_Instruction;
    };
};