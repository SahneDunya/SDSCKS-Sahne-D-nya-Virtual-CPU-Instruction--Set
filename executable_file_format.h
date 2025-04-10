#ifndef EXECUTABLE_FILE_FORMAT_H
#define EXECUTABLE_FILE_FORMAT_H

#include <stdint.h>

// Magic number to identify SDSCKS executable files
#define SDSCKS_EXECUTABLE_MAGIC_0xSDSCKSEXEC

// Version of the executable file format
#define EXECUTABLE_FILE_VERSION 1

// Structure for the executable file header
typedef struct {
    uint32_t magic;
    uint16_t version;
    uint64_t code_size;
    uint64_t data_size;
    uint64_t entry_point; // Address of the first instruction to execute
} executable_file_header_t;

#endif // EXECUTABLE_FILE_FORMAT_H