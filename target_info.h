#ifndef TARGET_INFO_H
#define TARGET_INFO_H

#include <stdint.h>
#include <stddef.h>

// Define the target triple for our SDSCKS VM (assuming a hypothetical architecture)
#define SDSCKS_TARGET_TRIPLE "sdscks-unknown-unknown-eabi" // Example: sdscks architecture, unknown vendor/OS, embedded ABI

// Define the data layout string (specifies how data is laid out in memory)
// This is highly dependent on the SDSCKS architecture. Here's a placeholder example:
// e - little endian
// m:e - name mangling format
// p:64:64 - 64-bit pointers, 64-bit aligned
// i64:64 - 64-bit integers, 64-bit aligned
// n64 - native integer widths are 64-bit
// S64 - stack natural alignment is 64-bit
#define SDSCKS_DATA_LAYOUT "e-m:e-p:64:64-i64:64-n64-S64"

// Function to get the target triple string
const char *get_target_triple();

// Function to get the data layout string
const char *get_data_layout();

// Add more functions to get other target-specific information if needed
// For example, pointer size, integer sizes, etc.

#endif // TARGET_INFO_H