#ifndef ASSEMBLY_LEXER_H
#define ASSEMBLY_LEXER_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

// Define the types of tokens
typedef enum {
    TOKEN_LABEL,
    TOKEN_MNEMONIC,
    TOKEN_REGISTER,
    TOKEN_IMMEDIATE,
    TOKEN_COMMA,
    TOKEN_COLON,
    TOKEN_EOF,
    TOKEN_ERROR
} token_type_t;

// Structure to represent a token
typedef struct {
    token_type_t type;
    char *value; // The actual text of the token (e.g., "ADD", "R5", "10")
    int line_number;
    int column_number;
} token_t;

// Function to initialize the lexer with an input file
void init_lexer(FILE *inputFile);

// Function to get the next token from the input
token_t get_next_token();

// Function to free the memory allocated for a token
void free_token(token_t *token);

#endif // ASSEMBLY_LEXER_H