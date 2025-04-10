#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>
#include <stdbool.h>

// Define the size of the virtual memory (same as in vm.h)
#define MEMORY_SIZE (4294967296 * 4294967296)

// Function to read a byte from the virtual memory
uint8_t memory_read_byte(uint64_t address);

// Function to write a byte to the virtual memory
void memory_write_byte(uint64_t address, uint8_t value);

// Function to read a word (64-bit) from the virtual memory
uint64_t memory_read_word(uint64_t address);

// Function to write a word (64-bit) to the virtual memory
void memory_write_word(uint64_t address, uint64_t value);

#endif // MEMORY_H