#include "semantic_analyzer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static symbol_table_t *global_symbol_table;

void semantic_analyzer_init() {
    global_symbol_table = create_symbol_table();
    // You might want to pre-populate the global symbol table with built-in functions or types
}

symbol_table_t *create_symbol_table() {
    symbol_table_t *table = (symbol_table_t *)malloc(sizeof(symbol_table_t));
    if (!table) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    table->head = NULL;
    return table;
}

void destroy_symbol_table(symbol_table_t *table) {
    symbol_t *current = table->head;
    while (current) {
        symbol_t *next = current->next;
        free(current->name);
        free(current);
        current = next;
    }
    free(table);
}

void add_symbol(symbol_table_t *table, const char *name, ast_node_type_t type) {
    if (lookup_symbol(table, name)) {
        fprintf(stderr, "Error: Symbol '%s' already declared.\n", name);
        exit(EXIT_FAILURE);
    }
    symbol_t *new_symbol = (symbol_t *)malloc(sizeof(symbol_t));
    if (!new_symbol) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    new_symbol->name = strdup(name);
    if (!new_symbol->name) {
        perror("Memory allocation failed");
        free(new_symbol);
        exit(EXIT_FAILURE);
    }
    new_symbol->type = type;
    new_symbol->next = table->head;
    table->head = new_symbol;
}

symbol_t *lookup_symbol(symbol_table_t *table, const char *name) {
    symbol_t *current = table->head;
    while (current) {
        if (strcmp(current->name, name) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void analyze_ast(ast_node_t *root) {
    if (root == NULL) {
        return;
    }

    switch (root->type) {
        case AST_PROGRAM:
            analyze_ast(root->body); // Analyze function definitions
            break;
        case AST_FUNCTION_DEFINITION:
            // Create a new scope (symbol table) for the function
            symbol_table_t *function_scope = create_symbol_table();
            // Add function name to the global scope (for now, assuming global functions)
            add_symbol(global_symbol_table, root->token.value, AST_FUNCTION_DEFINITION);
            analyze_ast(root->body); // Analyze statements within the function
            destroy_symbol_table(function_scope);
            break;
        case AST_VARIABLE_DECLARATION:
            // Add variable to the current scope (for now, assuming global scope)
            add_symbol(global_symbol_table, root->token.value, AST_IDENTIFIER); // Assuming all variables are of a generic 'identifier' type for now
            break;
        case AST_ASSIGNMENT_STATEMENT:
            // Check if the variable on the left-hand side is declared
            if (!lookup_symbol(global_symbol_table, root->token.value)) {
                fprintf(stderr, "Error: Undeclared variable '%s' at line %d, column %d\n",
                        root->token.value, root->token.line, root->token.column);
                exit(EXIT_FAILURE);
            }
            analyze_ast(root->right); // Analyze the expression on the right-hand side
            break;
        case AST_RETURN_STATEMENT:
            analyze_ast(root->left); // Analyze the returned expression
            // You might want to check if the return type matches the function's declared return type
            break;
        case AST_IDENTIFIER:
            // Check if the identifier is declared
            if (!lookup_symbol(global_symbol_table, root->token.value)) {
                fprintf(stderr, "Error: Undeclared identifier '%s' at line %d, column %d\n",
                        root->token.value, root->token.line, root->token.column);
                exit(EXIT_FAILURE);
            }
            break;
        case AST_INTEGER_LITERAL:
            // Integer literals are always valid (for now)
            break;
        case AST_BINARY_OPERATION:
            analyze_ast(root->left);
            analyze_ast(root->right);
            // You would perform type checking here based on the operator and operand types
            break;
        // Add cases for other AST node types as needed
        default:
            // Handle unhandled node types
            break;
    }

    analyze_ast(root->next); // Process sibling nodes (e.g., in a list of statements)
}

// Example of a simple main function to test the semantic analyzer

#include "parser.h" // Assuming parser's main calls lexer and builds AST

int main() {
    const char *source_code = "int main() { int x; x = 10; return y; }";
    lexer_init(source_code);
    parser_init();
    ast_node_t *root = parse_program();

    semantic_analyzer_init();
    analyze_ast(root);

    printf("Semantic analysis complete.\n");

    free_ast(root);
    destroy_symbol_table(global_symbol_table);

    return 0;
}
