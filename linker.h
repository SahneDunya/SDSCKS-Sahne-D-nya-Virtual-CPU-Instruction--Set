#ifndef LINKER_H
#define LINKER_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

// Define a simple structure for a symbol
typedef struct symbol_s {
    char *name;
    uint64_t address;
    bool is_global; // Whether the symbol is visible to other object files
    struct symbol_s *next;
} symbol_t;

// Define a simple structure for a relocation entry
typedef struct relocation_s {
    uint64_t offset; // Offset within the code/data section to be relocated
    char *symbol_name; // Name of the symbol to relocate to
    struct relocation_s *next;
} relocation_t;

// Define a structure to represent a loaded object file (simplified)
typedef struct object_file_s {
    char *filename;
    uint8_t *code_section;
    uint64_t code_size;
    uint8_t *data_section;
    uint64_t data_size;
    symbol_t *local_symbols; // Symbols defined in this file
    symbol_t *global_symbols; // Global symbols defined in this file
    relocation_t *relocations; // Relocations needed for this file
    struct object_file_s *next;
} object_file_t;

// Function to load an object file
object_file_t *load_object_file(const char *filename);

// Function to perform the linking process
bool link_object_files(object_file_t *object_list, const char *output_filename);

// Helper function to resolve symbols across object files
symbol_t *resolve_symbols(object_file_t *object_list);

// Helper function to perform relocations
bool perform_relocations(object_file_t *object_list, symbol_t *global_symbol_table, uint8_t *linked_code_base);

// Helper function to free the memory used by an object file
void free_object_file(object_file_t *obj_file);

// Helper function to free the memory used by a symbol table
void free_symbol_table(symbol_t *head);

// Helper function to free the memory used by a relocation table
void free_relocation_table(relocation_t *head);

#endif // LINKER_H