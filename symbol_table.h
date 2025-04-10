#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

// Structure to represent a symbol (label) in the symbol table
typedef struct symbol_s {
    char *name;
    uint64_t address;
    struct symbol_s *next;
} symbol_t;

// Function to initialize an empty symbol table (returns the head)
symbol_t *create_symbol_table();

// Function to add a new symbol to the symbol table
bool add_symbol(symbol_t **head, const char *name, uint64_t address);

// Function to find a symbol in the symbol table by its name
symbol_t *find_symbol(symbol_t *head, const char *name);

// Function to free all the memory allocated for the symbol table
void free_symbol_table(symbol_t *head);

#endif // SYMBOL_TABLE_H