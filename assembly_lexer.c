#include "assembly_lexer.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static FILE *input_file;
static int current_line = 1;
static int current_column = 0;
static char current_char = ' '; // Initialize with a space to trigger reading

// Helper function to read the next character from the input file
static char read_char() {
    char c = fgetc(input_file);
    if (c == '\n') {
        current_line++;
        current_column = 0;
    } else if (c != EOF) {
        current_column++;
    }
    return c;
}

void init_lexer(FILE *inputFile) {
    input_file = inputFile;
    current_line = 1;
    current_column = 0;
    current_char = ' ';
}

token_t get_next_token() {
    token_t token;
    token.value = NULL;
    token.line_number = current_line;
    token.column_number = current_column;

    // Skip whitespace and comments
    while (isspace(current_char) || current_char == ';') {
        if (current_char == ';') {
            while (current_char != '\n' && current_char != EOF) {
                current_char = read_char();
            }
        }
        current_char = read_char();
    }

    if (current_char == EOF) {
        token.type = TOKEN_EOF;
        return token;
    }

    // Identify tokens
    if (isalpha(current_char)) {
        // Could be a label or a mnemonic or a register
        char buffer[64];
        int i = 0;
        while (isalnum(current_char)) {
            if (i < sizeof(buffer) - 1) {
                buffer[i++] = current_char;
            }
            current_char = read_char();
        }
        buffer[i] = '\0';

        // Check if it's a register (assuming format 'R' followed by a number)
        if (buffer[0] == 'R' && isdigit(buffer[1])) {
            token.type = TOKEN_REGISTER;
        } else {
            // Could be a mnemonic or a label (we'll differentiate later in the parser)
            // For now, treat as a generic identifier
            // The parser will need to check if it's a valid mnemonic
            token.type = TOKEN_MNEMONIC;
        }
        token.value = strdup(buffer);
        return token;
    } else if (isdigit(current_char) || (current_char == '-' && isdigit(read_char()))) {
        // Immediate value
        char buffer[64];
        int i = 0;
        buffer[i++] = current_char;
        if (current_char == '-') current_char = read_char(); // Consume the digit after '-'
        while (isdigit(current_char)) {
            if (i < sizeof(buffer) - 1) {
                buffer[i++] = current_char;
            }
            current_char = read_char();
        }
        buffer[i] = '\0';
        token.type = TOKEN_IMMEDIATE;
        token.value = strdup(buffer);
        return token;
    } else if (current_char == ',') {
        token.type = TOKEN_COMMA;
        token.value = strdup(",");
        current_char = read_char();
        return token;
    } else if (current_char == ':') {
        token.type = TOKEN_COLON;
        token.value = strdup(":");
        current_char = read_char();
        return token;
    } else {
        // Unknown character
        token.type = TOKEN_ERROR;
        char error_buffer[2];
        error_buffer[0] = current_char;
        error_buffer[1] = '\0';
        token.value = strdup(error_buffer);
        current_char = read_char();
        return token;
    }
}

void free_token(token_t *token) {
    if (token && token->value) {
        free(token->value);
        token->value = NULL;
    }
}