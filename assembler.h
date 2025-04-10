#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

// Forward declaration of instruction structure
typedef struct instruction_s instruction_t;

// Structure to represent a line of assembly code (after tokenization)
typedef struct {
    char *label;
    char *mnemonic;
    char *operand1;
    char *operand2;
    char *operand3;
    int line_number;
} assembly_line_t;

// Structure to represent a symbol (label)
typedef struct symbol_s {
    char *name;
    uint64_t address;
    struct symbol_s *next;
} symbol_t;

// Function to perform the first pass of the assembler (symbol table creation)
symbol_t *assemble_pass1(FILE *inputFile, long *program_size);

// Function to perform the second pass of the assembler (code generation)
bool assemble_pass2(FILE *inputFile, symbol_t *symbolTable, FILE *outputFile);

// Helper function to tokenize a line of assembly code
assembly_line_t *tokenize_line(char *line, int line_number);

// Helper function to free the memory allocated for an assembly_line_t
void free_assembly_line(assembly_line_t *line);

// Helper function to add a symbol to the symbol table
void add_symbol(symbol_t **head, const char *name, uint64_t address);

// Helper function to find a symbol in the symbol table
symbol_t *find_symbol(symbol_t *head, const char *name);

#endif // ASSEMBLER_H