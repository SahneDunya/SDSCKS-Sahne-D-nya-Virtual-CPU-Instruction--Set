#include "lexer.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static const char *source;
static int current_pos;
static int line;
static int column;

void lexer_init(const char *source_code) {
    source = source_code;
    current_pos = 0;
    line = 1;
    column = 1;
}

static char peek_char() {
    if (source[current_pos] == '\0') {
        return '\0';
    }
    return source[current_pos];
}

static char consume_char() {
    char c = peek_char();
    if (c != '\0') {
        current_pos++;
        column++;
        if (c == '\n') {
            line++;
            column = 1;
        }
    }
    return c;
}

static void skip_whitespace() {
    char c = peek_char();
    while (isspace(c)) {
        consume_char();
        c = peek_char();
    }
}

token_t get_next_token() {
    skip_whitespace();

    token_t token;
    token.value = NULL;
    token.line = line;
    token.column = column;

    char current_char = peek_char();

    if (current_char == '\0') {
        token.type = TOKEN_EOF;
        return token;
    }

    if (isalpha(current_char) || current_char == '_') {
        // Identifier or keyword
        int start_pos = current_pos;
        while (isalnum(peek_char()) || peek_char() == '_') {
            consume_char();
        }
        int length = current_pos - start_pos;
        token.value = (char *)malloc(length + 1);
        strncpy(token.value, source + start_pos, length);
        token.value[length] = '\0';

        // Check for keywords (you'll need to define these)
        if (strcmp(token.value, "int") == 0 || strcmp(token.value, "return") == 0 || /* ... other keywords ... */) {
            token.type = TOKEN_KEYWORD;
        } else {
            token.type = TOKEN_IDENTIFIER;
        }
        return token;
    }

    if (isdigit(current_char)) {
        // Integer literal
        int start_pos = current_pos;
        while (isdigit(peek_char())) {
            consume_char();
        }
        int length = current_pos - start_pos;
        token.value = (char *)malloc(length + 1);
        strncpy(token.value, source + start_pos, length);
        token.value[length] = '\0';
        token.type = TOKEN_INTEGER_LITERAL;
        return token;
    }

    if (current_char == '"') {
        // String literal
        consume_char(); // Consume the opening quote
        int start_pos = current_pos;
        while (peek_char() != '"' && peek_char() != '\0') {
            consume_char();
        }
        if (peek_char() == '"') {
            consume_char(); // Consume the closing quote
            int length = current_pos - start_pos - 1; // Exclude the quotes
            token.value = (char *)malloc(length + 1);
            strncpy(token.value, source + start_pos, length);
            token.value[length] = '\0';
            token.type = TOKEN_STRING_LITERAL;
        } else {
            fprintf(stderr, "Error: Unterminated string literal at line %d, column %d\n", line, column);
            token.type = TOKEN_EOF; // Or some error token
        }
        return token;
    }

    // Operators and punctuators (you'll need to define these)
    if (strchr("+-*/=;", current_char) != NULL) {
        token.value = (char *)malloc(2);
        token.value[0] = consume_char();
        token.value[1] = '\0';
        token.type = (strchr("+-*/=", current_char) != NULL) ? TOKEN_OPERATOR : TOKEN_PUNCTUATOR;
        return token;
    }

    // Handle other characters or report an error
    fprintf(stderr, "Error: Unknown character '%c' at line %d, column %d\n", current_char, line, column);
    consume_char(); // Move past the unknown character
    token.type = TOKEN_EOF; // Or some error token
    return token;
}

// Example of a function to free the memory allocated for a token
void free_token(token_t *token) {
    if (token->value != NULL) {
        free(token->value);
        token->value = NULL;
    }
}

// Example of a simple main function to test the lexer

int main() {
    const char *source_code = "int main() {\n  int x = 10;\n  return x + 5;\n}";
    lexer_init(source_code);
    token_t token;
    do {
        token = get_next_token();
        printf("Token: type=%d, value='%s', line=%d, column=%d\n",
               token.type, token.value, token.line, token.column);
        free_token(&token);
    } while (token.type != TOKEN_EOF);
    return 0;
}
