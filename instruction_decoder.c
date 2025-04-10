#include "instruction_decoder.h"
#include "opcodes.h"

decoded_instruction_t decode_instruction(uint64_t instruction_word) {
    decoded_instruction_t decoded;
    decoded.opcode = (uint32_t)(instruction_word >> 58); // Example: Top 6 bits for opcode

    // Initialize operands to a default value (e.g., 0 or -1)
    decoded.rd = 0;
    decoded.rs1 = 0;
    decoded.rs2 = 0;
    decoded.immediate = 0;
    decoded.address = 0;

    switch (decoded.opcode) {
        case OP_ADD: // ADD Rd, Rs1, Rs2
        case OP_SUB:
        case OP_MUL:
        case OP_DIV:
        case OP_AND:
        case OP_OR:
        case OP_XOR:
        case OP_SLL:
        case OP_SRL:
        case OP_SRA:
        case OP_CMP:
            decoded.rd = (instruction_word >> 11) & 0x1F;
            decoded.rs1 = (instruction_word >> 16) & 0x1F;
            decoded.rs2 = (instruction_word >> 21) & 0x1F;
            break;
        case OP_ADDI: // ADDI Rd, Rs1, Immediate
        case OP_SUBI:
        case OP_ANDI:
        case OP_ORI:
        case OP_XORI:
            decoded.rd = (instruction_word >> 21) & 0x1F;
            decoded.rs1 = (instruction_word >> 16) & 0x1F;
            decoded.immediate = (int64_t)(instruction_word & 0x1FFFFF); // Example: Lower 21 bits for immediate (sign-extended)
            break;
        case OP_LI: // LI Rd, Immediate
            decoded.rd = (instruction_word >> 21) & 0x1F;
            decoded.immediate = (int64_t)(instruction_word & 0x1FFFFF);
            break;
        case OP_LOAD: // LOAD Rd, Address
        case OP_STORE:
            decoded.rd = (instruction_word >> 21) & 0x1F;
            decoded.address = (uint64_t)(instruction_word & 0x1FFFFF); // Example: Lower 21 bits for address
            break;
        case OP_JMP: // JMP Address
            decoded.address = (uint64_t)(instruction_word & 0x3FFFFFF); // Example: Lower 26 bits for address
            break;
        case OP_JR: // JR Rs
            decoded.rs1 = (instruction_word >> 21) & 0x1F;
            break;
        case OP_BEQ: // BEQ Rs1, Rs2, Offset
        case OP_BNE:
            decoded.rs1 = (instruction_word >> 21) & 0x1F;
            decoded.rs2 = (instruction_word >> 16) & 0x1F;
            decoded.immediate = (int64_t)(instruction_word & 0xFFFF); // Example: Lower 16 bits for offset (sign-extended)
            break;
        case OP_HALT:
            // No operands to decode for HALT in this example
            break;
        default:
            // Unknown opcode, operands remain at default values
            break;
    }

    return decoded;
}