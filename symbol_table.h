#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "ast.h" // Symbol table entries might refer to AST node types
#include <stdbool.h>

// Forward declaration of symbol table structure
typedef struct symbol_table_s symbol_table_t;

// Structure for a symbol table entry
typedef struct symbol_s {
    char *name;
    ast_node_type_t type; // For variables, the type; for functions, maybe a special type
    // Add more attributes as needed, e.g., scope, size, number of parameters
    struct symbol_s *next;
} symbol_t;

// Structure for the symbol table (a linked list of symbols for simplicity)
struct symbol_table_s {
    symbol_t *head;
};

// Function to create a new symbol table
symbol_table_t *create_symbol_table();

// Function to destroy a symbol table
void destroy_symbol_table(symbol_table_t *table);

// Function to add a symbol to the symbol table
bool add_symbol(symbol_table_t *table, const char *name, ast_node_type_t type);

// Function to lookup a symbol in the symbol table
symbol_t *lookup_symbol(symbol_table_t *table, const char *name);

// Function to print the symbol table (for debugging)
void print_symbol_table(symbol_table_t *table);

#endif // SYMBOL_TABLE_H