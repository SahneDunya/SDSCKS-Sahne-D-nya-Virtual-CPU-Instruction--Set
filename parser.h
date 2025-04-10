#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

// Forward declaration of AST node structure
typedef struct ast_node_s ast_node_t;

// Define the types of AST nodes
typedef enum {
    AST_PROGRAM,
    AST_FUNCTION_DEFINITION,
    AST_VARIABLE_DECLARATION,
    AST_ASSIGNMENT_STATEMENT,
    AST_RETURN_STATEMENT,
    AST_IDENTIFIER,
    AST_INTEGER_LITERAL,
    AST_BINARY_OPERATION,
    // Add more AST node types as needed
} ast_node_type_t;

// Structure for an AST node
struct ast_node_s {
    ast_node_type_t type;
    token_t token;       // The token that corresponds to this node (optional)
    ast_node_t *left;    // For binary operations, function calls, etc.
    ast_node_t *right;   // For binary operations
    ast_node_t *body;    // For function definitions, loops, etc.
    ast_node_t *next;    // For lists of statements, parameters, etc.
    // Add more fields as needed, e.g., for variable types, function parameters
};

// Function to initialize the parser with the token stream
void parser_init();

// Function to parse the entire program
ast_node_t *parse_program();

// Function to parse a function definition
ast_node_t *parse_function_definition();

// Function to parse a statement
ast_node_t *parse_statement();

// Function to parse a variable declaration
ast_node_t *parse_variable_declaration();

// Function to parse an assignment statement
ast_node_t *parse_assignment_statement();

// Function to parse a return statement
ast_node_t *parse_return_statement();

// Function to parse an expression
ast_node_t *parse_expression();

// Helper function to consume a specific token type
token_t consume_token(token_type_t expected_type, const char *error_message);

// Helper function to get the current token
token_t peek_token();

#endif // PARSER_H