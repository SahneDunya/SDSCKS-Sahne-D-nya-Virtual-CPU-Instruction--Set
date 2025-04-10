#ifndef SEMANTIC_ANALYZER_H
#define SEMANTIC_ANALYZER_H

#include "ast.h"

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

// Function to initialize the semantic analyzer
void semantic_analyzer_init();

// Function to analyze the AST
void analyze_ast(ast_node_t *root);

// Helper function to create a new symbol table
symbol_table_t *create_symbol_table();

// Helper function to destroy a symbol table
void destroy_symbol_table(symbol_table_t *table);

// Helper function to add a symbol to the symbol table
void add_symbol(symbol_table_t *table, const char *name, ast_node_type_t type);

// Helper function to lookup a symbol in the symbol table
symbol_t *lookup_symbol(symbol_table_t *table, const char *name);

#endif // SEMANTIC_ANALYZER_H