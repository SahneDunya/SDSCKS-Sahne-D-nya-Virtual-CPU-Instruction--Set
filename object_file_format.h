#ifndef OBJECT_FILE_FORMAT_H
#define OBJECT_FILE_FORMAT_H

#include <stdint.h>
#include <stdbool.h>

// Magic number to identify SDSCKS object files
#define SDSCKS_OBJECT_MAGIC_0xSDSCKS01

// Version of the object file format
#define OBJECT_FILE_VERSION 1

// Symbol types
typedef enum {
    SYMBOL_TYPE_LABEL,
    SYMBOL_TYPE_DATA
} symbol_type_t;

// Symbol scope
typedef enum {
    SYMBOL_SCOPE_LOCAL,
    SYMBOL_SCOPE_GLOBAL
} symbol_scope_t;

// Structure for a symbol table entry
typedef struct {
    char *name;
    uint32_t type;       // symbol_type_t
    uint32_t scope;      // symbol_scope_t
    uint64_t value;      // Address or offset of the symbol
} object_symbol_t;

// Relocation types (example: absolute address)
typedef enum {
    RELOC_TYPE_ABSOLUTE64
} relocation_type_t;

// Structure for a relocation table entry
typedef struct {
    uint64_t offset;      // Offset in the section to be relocated
    char *symbol_name; // Name of the symbol to relocate to
    uint32_t type;       // relocation_type_t
} object_relocation_t;

// Structure for the object file header
typedef struct {
    uint32_t magic;
    uint16_t version;
    uint64_t code_size;
    uint64_t data_size;
    uint64_t symbol_table_size;   // Number of symbols
    uint64_t relocation_table_size; // Number of relocations
} object_file_header_t;

#endif // OBJECT_FILE_FORMAT_H