#ifndef OPCODES_H
#define OPCODES_H

// Define opcodes for the SDSCKS instruction set
// Keep them relatively sparse for potential future expansion

// R-Type Instructions (Register-Register)
#define OP_ADD  0x01 // Add two registers
#define OP_SUB  0x02 // Subtract two registers
#define OP_MUL  0x03 // Multiply two registers
#define OP_DIV  0x04 // Divide two registers
#define OP_AND  0x05 // Bitwise AND
#define OP_OR   0x06 // Bitwise OR
#define OP_XOR  0x07 // Bitwise XOR
#define OP_SLL  0x08 // Shift Left Logical
#define OP_SRL  0x09 // Shift Right Logical
#define OP_SRA  0x0A // Shift Right Arithmetic
#define OP_CMP  0x0B // Compare two registers (sets flags)

// I-Type Instructions (Register-Immediate)
#define OP_ADDI 0x11 // Add immediate to register
#define OP_SUBI 0x12 // Subtract immediate from register
#define OP_ANDI 0x13 // Bitwise AND with immediate
#define OP_ORI  0x14 // Bitwise OR with immediate
#define OP_XORI 0x15 // Bitwise XOR with immediate
#define OP_LI   0x16 // Load Immediate (move immediate to register)

// Memory Access Instructions
#define OP_LOAD 0x21 // Load from memory to register
#define OP_STORE 0x22 // Store from register to memory

// Control Flow Instructions
#define OP_JMP  0x31 // Jump to address (immediate)
#define OP_JR   0x32 // Jump to address in register
#define OP_BEQ  0x33 // Branch if equal (to zero or another register)
#define OP_BNE  0x34 // Branch if not equal

// System Instructions
#define OP_HALT 0xFF // Halt execution

#endif // OPCODES_H