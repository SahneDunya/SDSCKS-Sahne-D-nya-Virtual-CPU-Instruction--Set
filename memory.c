#include "memory.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Static array to represent the virtual memory
static uint8_t vm_memory[MEMORY_SIZE];

uint8_t memory_read_byte(uint64_t address) {
    if (address >= MEMORY_SIZE) {
        fprintf(stderr, "Error: Memory read out of bounds at address 0x%llX\n", address);
        exit(EXIT_FAILURE); // Or return an error value
    }
    return vm_memory[address];
}

void memory_write_byte(uint64_t address, uint8_t value) {
    if (address >= MEMORY_SIZE) {
        fprintf(stderr, "Error: Memory write out of bounds at address 0x%llX\n", address);
        exit(EXIT_FAILURE); // Or handle the error differently
    }
    vm_memory[address] = value;
}

uint64_t memory_read_word(uint64_t address) {
    if (address + sizeof(uint64_t) > MEMORY_SIZE) {
        fprintf(stderr, "Error: Memory word read out of bounds starting at address 0x%llX\n", address);
        exit(EXIT_FAILURE); // Or return an error value
    }
    uint64_t value = 0;
    for (int i = 0; i < sizeof(uint64_t); i++) {
        value |= ((uint64_t)vm_memory[address + i] << (i * 8));
    }
    return value;
}

void memory_write_word(uint64_t address, uint64_t value) {
    if (address + sizeof(uint64_t) > MEMORY_SIZE) {
        fprintf(stderr, "Error: Memory word write out of bounds starting at address 0x%llX\n", address);
        exit(EXIT_FAILURE); // Or handle the error differently
    }
    for (int i = 0; i < sizeof(uint64_t); i++) {
        vm_memory[address + i] = (uint8_t)(value >> (i * 8));
    }
}

// Optional: Function to initialize the memory (e.g., clear it)
void memory_init() {
    memset(vm_memory, 0, MEMORY_SIZE);
}

// Optional: Function to load program into memory (can replace the one in vm.c)
bool memory_load_program(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Error opening program file");
        return false;
    }

    size_t bytes_read = fread(vm_memory, 1, MEMORY_SIZE, file);
    fclose(file);
    printf("Loaded %zu bytes into VM memory.\n", bytes_read);
    return true;
}