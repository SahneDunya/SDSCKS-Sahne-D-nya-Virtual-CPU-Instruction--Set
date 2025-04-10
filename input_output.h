#ifndef INPUT_OUTPUT_H
#define INPUT_OUTPUT_H

#include <stdint.h>
#include <stdbool.h>

// Define addresses for virtual input/output devices (example)
#define INPUT_DEVICE_ADDRESS 0xFFFFFFF0
#define OUTPUT_DEVICE_ADDRESS 0xFFFFFFF8

// Function to read from the virtual input device
uint64_t io_read(uint64_t address);

// Function to write to the virtual output device
void io_write(uint64_t address, uint64_t value);

#endif // INPUT_OUTPUT_H