#ifndef ASSEMBLY_PARSER_H
#define ASSEMBLY_PARSER_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "assembly_lexer.h"

// Forward declaration of instruction structure (from instruction_set.h)
typedef struct instruction_s instruction_t;

// Structure to represent a parsed assembly instruction
typedef struct parsed_instruction_s {
    char *label;
    char *mnemonic;
    char *operand1;
    char *operand2;
    char *operand3;
    int line_number;
} parsed_instruction_t;

// Function to parse a single line of assembly code
parsed_instruction_t *parse_line();

// Function to get the current token being processed by the parser
token_t get_current_parse_token();

// Function to consume the current token and get the next one
token_t consume_token();

// Function to check if the current token type matches the expected type
bool check_token(token_type_t expected_type);

// Function to report a parsing error
void parser_error(const char *message);

#endif // ASSEMBLY_PARSER_H