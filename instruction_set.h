#ifndef INSTRUCTION_SET_H
#define INSTRUCTION_SET_H

#include <stdint.h>
#include <stdbool.h>

// Define the number of general-purpose registers
#define NUM_REGISTERS 32

// Define the size of a register in bits (64-bit architecture)
#define REGISTER_SIZE 64

// Define data types
typedef uint64_t reg_t; // For general-purpose registers
typedef uint64_t addr_t; // For memory addresses

// Instruction format (simplified example - can be expanded)
typedef enum {
    INST_TYPE_R, // Register-Register
    INST_TYPE_I, // Register-Immediate
    INST_TYPE_MEM // Memory Access
} instruction_type_t;

// Structure to represent a decoded instruction
typedef struct {
    uint32_t opcode;
    instruction_type_t type;

    // Operands (based on instruction type)
    union {
        struct {
            uint8_t rd;  // Destination register
            uint8_t rs1; // Source register 1
            uint8_t rs2; // Source register 2
        } r_type;
        struct {
            uint8_t rd;  // Destination register
            int64_t immediate; // Immediate value (can be signed)
        } i_type;
        struct {
            uint8_t rd;  // Destination register (for load) or source register (for store)
            addr_t address; // Memory address
        } mem_type;
    } operands;
} instruction_t;

// Function to decode a raw instruction word (64-bit) into the instruction_t structure
instruction_t decode_instruction(uint64_t instruction_word);

// Function to get the mnemonic (string representation) of an opcode (for debugging/disassembly)
const char* get_opcode_mnemonic(uint32_t opcode);

#endif // INSTRUCTION_SET_H