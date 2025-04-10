#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

// Define the different types of tokens
typedef enum {
    TOKEN_EOF,
    TOKEN_KEYWORD,
    TOKEN_IDENTIFIER,
    TOKEN_INTEGER_LITERAL,
    TOKEN_STRING_LITERAL,
    TOKEN_OPERATOR,
    TOKEN_PUNCTUATOR,
    // Add more token types as needed for your language
} token_type_t;

// Define a structure to represent a token
typedef struct {
    token_type_t type;
    char *value;       // The actual text of the token
    int line;          // Line number where the token appears
    int column;        // Column number where the token starts
} token_t;

// Function to initialize the lexer with the source code
void lexer_init(const char *source_code);

// Function to get the next token from the source code
token_t get_next_token();

#endif // LEXER_H