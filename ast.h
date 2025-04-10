#ifndef AST_H
#define AST_H

#include "lexer.h" // AST nodes might hold tokens

// Forward declaration of AST node structure
typedef struct ast_node_s ast_node_t;

// Define the types of AST nodes
typedef enum {
    AST_PROGRAM,
    AST_FUNCTION_DEFINITION,
    AST_VARIABLE_DECLARATION,
    AST_ASSIGNMENT_STATEMENT,
    AST_RETURN_STATEMENT,
    AST_IF_STATEMENT,
    AST_WHILE_LOOP,
    AST_BLOCK_STATEMENT,
    AST_EXPRESSION_STATEMENT, // For expressions that are statements (e.g., function calls)
    AST_IDENTIFIER,
    AST_INTEGER_LITERAL,
    AST_BINARY_OPERATION,
    AST_UNARY_OPERATION,
    AST_FUNCTION_CALL,
    // Add more AST node types as needed for your language
} ast_node_type_t;

// Structure for an AST node
struct ast_node_s {
    ast_node_type_t type;
    token_t token;       // The token that corresponds to this node (optional)
    ast_node_t *left;    // Typically for the left operand, condition, etc.
    ast_node_t *right;   // Typically for the right operand
    ast_node_t *condition; // For if and while statements
    ast_node_t *body;    // For function definitions, loops, if/else blocks
    ast_node_t *else_body; // For if-else statements
    ast_node_t *arguments; // For function calls, a list of argument expressions
    ast_node_t *next;    // For lists of statements, parameters, etc.
    // Add more fields as needed, e.g., for variable types, function parameters
};

// Helper function to create a new AST node
ast_node_t *create_ast_node(ast_node_type_t type);

// Helper function to free the AST (recursively)
void free_ast(ast_node_t *node);

#endif // AST_H