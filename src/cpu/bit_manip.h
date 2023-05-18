#pragma once

#include <stdint.h>

// Rotate right
static inline uint32_t bit_manip_ROR(uint32_t value, unsigned int rotate) {
    return (value >> rotate) | (value << (32 - rotate));
}

// Arithmetic shift right
static inline uint32_t bit_manip_ASR(uint32_t value, unsigned int shift) {
    return (value >> shift) | (value & (1 << 31));
}

static inline uint32_t bit_manip_set_mask(uint32_t value, uint32_t mask, uint32_t bits) {
    return (value & ~mask) | bits;
}