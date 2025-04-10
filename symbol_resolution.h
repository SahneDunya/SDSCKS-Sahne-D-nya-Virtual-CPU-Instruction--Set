#ifndef SYMBOL_RESOLUTION_H
#define SYMBOL_RESOLUTION_H

#include <stdint.h>
#include <stdbool.h>
#include "object_file_format.h"

// Structure for an entry in the global symbol table (similar to object_symbol_t)
typedef struct global_symbol_s {
    char *name;
    uint32_t type;       // symbol_type_t
    uint64_t address;    // Final linked address of the symbol
    struct global_symbol_s *next;
} global_symbol_t;

// Function to resolve symbols from a list of object files
global_symbol_t *resolve_symbols(object_file_t *object_list);

// Helper function to add a symbol to the global symbol table
bool add_global_symbol(global_symbol_t **head, const char *name, uint32_t type, uint64_t address);

// Helper function to find a symbol in the global symbol table by name
global_symbol_t *find_global_symbol(global_symbol_t *head, const char *name);

// Helper function to free the global symbol table
void free_global_symbol_table(global_symbol_t *head);

#endif // SYMBOL_RESOLUTION_H