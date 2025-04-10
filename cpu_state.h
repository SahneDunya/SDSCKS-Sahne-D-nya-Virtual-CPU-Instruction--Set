#ifndef CPU_STATE_H
#define CPU_STATE_H

#include <stdint.h>
#include "instruction_set.h"

// Define the number of general-purpose registers (from instruction_set.h)
#define NUM_REGISTERS 32

// Define the register type (from instruction_set.h)
typedef uint64_t reg_t;

// Structure representing the state of the SDSCKS virtual CPU
typedef struct {
    reg_t registers[NUM_REGISTERS];
    uint64_t program_counter;
    // Add other CPU state components here as needed:
    // - Status flags (e.g., zero flag, negative flag, carry flag, overflow flag)
    uint8_t zero_flag;
    uint8_t negative_flag;
    uint8_t carry_flag;
    uint8_t overflow_flag;
    // - Other control registers if your architecture requires them
} cpu_state_t;

#endif // CPU_STATE_H