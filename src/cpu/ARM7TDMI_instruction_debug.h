#pragma once

#include "ARM7TDMI_instruction.h"

#ifndef NDEBUG
    #include <stdio.h>
    #include <stdarg.h>
    #define LOG(fmt, ...) writeLog(fmt, __VA_ARGS__)

    void writeLog(const char *fmt, ...) {
        FILE* logFile = fopen("partial_disassembly.log", "a");
        if(logFile != NULL) {
            va_list args;
            va_start(args, fmt);
            vfprintf(logFile, fmt, args);
            va_end(args);
            fclose(logFile);
        }
    }
#else
    #define LOG(fmt, ...)
#endif

const char* const CONDITION_CODE_string[] = {
    [CONDITION_CODE_EQ] = "EQ", // Equal - Z set
    [CONDITION_CODE_NE] = "NE", // Not Equal - Z clear
    [CONDITION_CODE_CS_HS] = "CS", // Carry set/unsigned higher or same - C set
    [CONDITION_CODE_CC_LO] = "CC", // Carry clear/ unsigned lower - C clear
    [CONDITION_CODE_MI] = "MI", // Minus/negative - N set
    [CONDITION_CODE_PL] = "PL", // Plus/positive or zero - N clear
    [CONDITION_CODE_VS] = "VS", // Overflow - V set
    [CONDITION_CODE_VC] = "VC", // No overflow - V clear
    [CONDITION_CODE_HI] = "HI", // Unsinged higher - C set and Z clear
    [CONDITION_CODE_LS] = "LS", // Unsigned lower or same - C clear or Z set
    [CONDITION_CODE_GE] = "GE", // Signed greater than or equal - N == V
    [CONDITION_CODE_LT] = "LT", // Signed less than - N != V
    [CONDITION_CODE_GT] = "GT", // Signed greater than - Z == 0 && N == V
    [CONDITION_CODE_LE] = "LE", // Signed less than or equal - Z == 1 && N != V
    [CONDITION_CODE_AL] = "AL", // Always
    [CONDITION_CODE_UN] = "UN" // Unconditional
};
