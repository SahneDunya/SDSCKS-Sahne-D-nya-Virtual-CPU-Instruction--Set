#include "vm.h"
#include "memory.h"
#include "instruction_decoder.h" // Include the instruction decoder
#include <stdlib.h>
#include <string.h>

// ... (vm_init, vm_load_program, vm_fetch_instruction remain similar)

void vm_execute_instruction(vm_state_t *vm, uint64_t instruction_word) {
    decoded_instruction_t decoded = decode_instruction(instruction_word);

    switch (decoded.opcode) {
        case OP_ADD: { // ADD Rd, Rs1, Rs2
            if (decoded.rd < NUM_REGISTERS && decoded.rs1 < NUM_REGISTERS && decoded.rs2 < NUM_REGISTERS) {
                vm->registers[decoded.rd] = vm->registers[decoded.rs1] + vm->registers[decoded.rs2];
            } else {
                fprintf(stderr, "Error: Invalid register index in ADD instruction.\n");
                vm->running = false;
            }
            break;
        }
        case OP_SUB: { // SUB Rd, Rs1, Rs2
            if (decoded.rd < NUM_REGISTERS && decoded.rs1 < NUM_REGISTERS && decoded.rs2 < NUM_REGISTERS) {
                vm->registers[decoded.rd] = vm->registers[decoded.rs1] - vm->registers[decoded.rs2];
            } else {
                fprintf(stderr, "Error: Invalid register index in SUB instruction.\n");
                vm->running = false;
            }
            break;
        }
        case OP_HALT: { // HALT
            printf("VM halted.\n");
            vm->running = false;
            break;
        }
        case OP_LOAD: { // LOAD Rd, Address
            if (decoded.rd < NUM_REGISTERS && decoded.address < MEMORY_SIZE - sizeof(reg_t) + 1) {
                vm->registers[decoded.rd] = memory_read_word(decoded.address);
            } else {
                fprintf(stderr, "Error: Invalid register index or memory address in LOAD instruction.\n");
                vm->running = false;
            }
            break;
        }
        case OP_STORE: { // STORE Rs, Address
            if (decoded.rd < NUM_REGISTERS && decoded.address < MEMORY_SIZE - sizeof(reg_t) + 1) {
                memory_write_word(decoded.address, vm->registers[decoded.rd]);
            } else {
                fprintf(stderr, "Error: Invalid register index or memory address in STORE instruction.\n");
                vm->running = false;
            }
            break;
        }
        case OP_ADDI: { // ADDI Rd, Rs1, Immediate
            if (decoded.rd < NUM_REGISTERS && decoded.rs1 < NUM_REGISTERS) {
                vm->registers[decoded.rd] = vm->registers[decoded.rs1] + decoded.immediate;
            } else {
                fprintf(stderr, "Error: Invalid register index in ADDI instruction.\n");
                vm->running = false;
            }
            break;
        }
        case OP_LI: { // LI Rd, Immediate
            if (decoded.rd < NUM_REGISTERS) {
                vm->registers[decoded.rd] = decoded.immediate;
            } else {
                fprintf(stderr, "Error: Invalid register index in LI instruction.\n");
                vm->running = false;
            }
            break;
        }
        case OP_JMP: { // JMP Address
            vm->program_counter = decoded.address;
            break;
        }
        case OP_JR: { // JR Rs
            if (decoded.rs1 < NUM_REGISTERS) {
                vm->program_counter = vm->registers[decoded.rs1];
            } else {
                fprintf(stderr, "Error: Invalid register index in JR instruction.\n");
                vm->running = false;
            }
            break;
        }
        case OP_BEQ: { // BEQ Rs1, Rs2, Offset
            if (decoded.rs1 < NUM_REGISTERS && decoded.rs2 < NUM_REGISTERS) {
                if (vm->registers[decoded.rs1] == vm->registers[decoded.rs2]) {
                    vm->program_counter += decoded.immediate; // Assuming offset is relative to current PC
                }
            } else {
                fprintf(stderr, "Error: Invalid register index in BEQ instruction.\n");
                vm->running = false;
            }
            break;
        }
        case OP_BNE: { // BNE Rs1, Rs2, Offset
            if (decoded.rs1 < NUM_REGISTERS && decoded.rs2 < NUM_REGISTERS) {
                if (vm->registers[decoded.rs1] != vm->registers[decoded.rs2]) {
                    vm->program_counter += decoded.immediate; // Assuming offset is relative to current PC
                }
            } else {
                fprintf(stderr, "Error: Invalid register index in BNE instruction.\n");
                vm->running = false;
            }
            break;
        }
        // Implement other instructions here
        default: {
            fprintf(stderr, "Error: Unknown opcode 0x%02X encountered.\n", decoded.opcode);
            vm->running = false;
            break;
        }
    }
}