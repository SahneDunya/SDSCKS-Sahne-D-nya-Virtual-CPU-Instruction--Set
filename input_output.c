#include "input_output.h"
#include <stdio.h>

uint64_t io_read(uint64_t address) {
    if (address == INPUT_DEVICE_ADDRESS) {
        // Simulate reading from input (e.g., keyboard)
        printf("Enter a 64-bit value: ");
        uint64_t value;
        scanf("%llu", &value);
        return value;
    } else {
        fprintf(stderr, "Error: Invalid input device address 0x%llX\n", address);
        return 0; // Or some error value
    }
}

void io_write(uint64_t address, uint64_t value) {
    if (address == OUTPUT_DEVICE_ADDRESS) {
        // Simulate writing to output (e.g., console)
        printf("Output: 0x%llX (%llu)\n", value, value);
    } else {
        fprintf(stderr, "Error: Invalid output device address 0x%llX, value 0x%llX\n", address, value);
    }
}