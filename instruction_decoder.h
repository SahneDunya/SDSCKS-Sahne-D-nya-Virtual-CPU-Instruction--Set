#ifndef INSTRUCTION_DECODER_H
#define INSTRUCTION_DECODER_H

#include <stdint.h>
#include "instruction_set.h"

// Structure to represent a decoded instruction for the VM
typedef struct decoded_instruction_s {
    uint32_t opcode;
    // Operands (you might define a union or separate fields depending on your needs)
    uint8_t rd;
    uint8_t rs1;
    uint8_t rs2;
    int64_t immediate;
    uint64_t address;
    // ... other operand fields as needed
} decoded_instruction_t;

// Function to decode a raw instruction word into a decoded_instruction_t structure
decoded_instruction_t decode_instruction(uint64_t instruction_word);

#endif // INSTRUCTION_DECODER_H