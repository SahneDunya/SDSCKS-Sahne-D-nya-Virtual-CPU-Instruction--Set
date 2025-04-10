// Virtual Machine/instruction_execution.h
#ifndef INSTRUCTION_EXECUTION_H
#define INSTRUCTION_EXECUTION_H

#include "vm.h"
#include "instruction_decoder.h"

// Function to execute the ADD instruction
void execute_add(vm_state_t *vm, const decoded_instruction_t *decoded);

// Function to execute the SUB instruction
void execute_sub(vm_state_t *vm, const decoded_instruction_t *decoded);

// Function to execute the MUL instruction
void execute_mul(vm_state_t *vm, const decoded_instruction_t *decoded);

// Function to execute the DIV instruction
void execute_div(vm_state_t *vm, const decoded_instruction_t *decoded);

// Function to execute the AND instruction
void execute_and(vm_state_t *vm, const decoded_instruction_t *decoded);

// Function to execute the OR instruction
void execute_or(vm_state_t *vm, const decoded_instruction_t *decoded);

// Function to execute the XOR instruction
void execute_xor(vm_state_t *vm, const decoded_instruction_t *decoded);

// Function to execute the SLL instruction
void execute_sll(vm_state_t *vm, const decoded_instruction_t *decoded);

// Function to execute the SRL instruction
void execute_srl(vm_state_t *vm, const decoded_instruction_t *decoded);

// Function to execute the SRA instruction
void execute_sra(vm_state_t *vm, const decoded_instruction_t *decoded);

// Function to execute the CMP instruction
void execute_cmp(vm_state_t *vm, const decoded_instruction_t *decoded);

// Function to execute the ADDI instruction
void execute_addi(vm_state_t *vm, const decoded_instruction_t *decoded);

// Function to execute the SUBI instruction
void execute_subi(vm_state_t *vm, const decoded_instruction_t *decoded);

// Function to execute the ANDI instruction
void execute_andi(vm_state_t *vm, const decoded_instruction_t *decoded);

// Function to execute the ORI instruction
void execute_ori(vm_state_t *vm, const decoded_instruction_t *decoded);

// Function to execute the XORI instruction
void execute_xori(vm_state_t *vm, const decoded_instruction_t *decoded);

// Function to execute the LI instruction
void execute_li(vm_state_t *vm, const decoded_instruction_t *decoded);

// Function to execute the LOAD instruction
void execute_load(vm_state_t *vm, const decoded_instruction_t *decoded);

// Function to execute the STORE instruction
void execute_store(vm_state_t *vm, const decoded_instruction_t *decoded);

// Function to execute the JMP instruction
void execute_jmp(vm_state_t *vm, const decoded_instruction_t *decoded);

// Function to execute the JR instruction
void execute_jr(vm_state_t *vm, const decoded_instruction_t *decoded);

// Function to execute the BEQ instruction
void execute_beq(vm_state_t *vm, const decoded_instruction_t *decoded);

// Function to execute the BNE instruction
void execute_bne(vm_state_t *vm, const decoded_instruction_t *decoded);

// Function to execute the HALT instruction
void execute_halt(vm_state_t *vm, const decoded_instruction_t *decoded);

#endif // INSTRUCTION_EXECUTION_H