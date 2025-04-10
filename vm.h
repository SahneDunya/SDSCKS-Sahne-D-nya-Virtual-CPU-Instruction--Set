#ifndef VM_H
#define VM_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "instruction_set.h"
#include "opcodes.h"

// Define the size of the virtual memory (example: 16 exebayt)
#define MEMORY_SIZE (4294967296 * 4294967296)

// Define the number of general-purpose registers (from instruction_set.h)
#define NUM_REGISTERS 32

// Define the register type (from instruction_set.h)
typedef uint64_t reg_t;

// Structure representing the state of the SDSCKS virtual CPU
typedef struct {
    reg_t registers[NUM_REGISTERS];
    uint64_t program_counter;
    uint8_t memory[MEMORY_SIZE];
    bool running;
} vm_state_t;

// Function to initialize the virtual machine state
void vm_init(vm_state_t *vm);

// Function to load the program (machine code) into the VM's memory
bool vm_load_program(vm_state_t *vm, const char *filename);

// Function to execute the program loaded in the VM
void vm_run(vm_state_t *vm);

// Helper function to fetch the next instruction from memory
uint64_t vm_fetch_instruction(vm_state_t *vm);

// Helper function to execute a single instruction
void vm_execute_instruction(vm_state_t *vm, uint64_t instruction_word);

#endif // VM_H