#include "llvm_ir_generator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static symbol_table_t *current_symbol_table; // To keep track of variables in scope

void llvm_ir_generator_init() {
    current_symbol_table = create_symbol_table(); // Start with a global scope
    printf("; LLVM IR Generation Start\n");
}

void generate_llvm_ir(ast_node_t *root) {
    if (root == NULL) {
        return;
    }

    switch (root->type) {
        case AST_PROGRAM:
            generate_llvm_ir(root->body);
            break;
        case AST_FUNCTION_DEFINITION:
            printf("\ndefine i64 @%s() {\n", root->token.value); // Assuming all functions return i64 for simplicity
            // Create a new symbol table for the function's scope (nested)
            symbol_table_t *previous_symbol_table = current_symbol_table;
            current_symbol_table = create_symbol_table();
            generate_llvm_ir(root->body);
            printf("  ret i64 0\n"); // Default return 0 for now
            printf("}\n");
            destroy_symbol_table(current_symbol_table);
            current_symbol_table = previous_symbol_table; // Restore previous scope
            break;
        case AST_VARIABLE_DECLARATION:
            printf("  %%%s = alloca i64\n", root->token.value); // Allocate space on the stack
            add_symbol(current_symbol_table, root->token.value, AST_IDENTIFIER); // Store in symbol table
            break;
        case AST_ASSIGNMENT_STATEMENT: {
            symbol_t *var_symbol = lookup_symbol(current_symbol_table, root->token.value);
            if (var_symbol) {
                // For simplicity, assuming the right-hand side is an integer literal or identifier
                if (root->right->type == AST_INTEGER_LITERAL) {
                    printf("  store i64 %s, i64* %%%s\n", root->right->token.value, root->token.value);
                } else if (root->right->type == AST_IDENTIFIER) {
                    symbol_t *rhs_symbol = lookup_symbol(current_symbol_table, root->right->token.value);
                    if (rhs_symbol) {
                        printf("  %%%s_val = load i64, i64* %%%s\n", root->right->token.value, root->right->token.value);
                        printf("  store i64 %%%s_val, i64* %%%s\n", root->right->token.value, root->token.value);
                    } else {
                        fprintf(stderr, "Error: Undeclared variable '%s' in assignment.\n", root->right->token.value);
                    }
                } else {
                    fprintf(stderr, "Error: Unsupported right-hand side in assignment.\n");
                }
            } else {
                fprintf(stderr, "Error: Undeclared variable '%s' in assignment.\n", root->token.value);
            }
            break;
        }
        case AST_RETURN_STATEMENT:
            if (root->left) {
                if (root->left->type == AST_INTEGER_LITERAL) {
                    printf("  ret i64 %s\n", root->left->token.value);
                } else if (root->left->type == AST_IDENTIFIER) {
                    symbol_t *return_symbol = lookup_symbol(current_symbol_table, root->left->token.value);
                    if (return_symbol) {
                        printf("  %%%s_ret_val = load i64, i64* %%%s\n", root->left->token.value, root->left->token.value);
                        printf("  ret i64 %%%s_ret_val\n", root->left->token.value);
                    } else {
                        fprintf(stderr, "Error: Undeclared variable '%s' in return statement.\n", root->left->token.value);
                    }
                } else {
                    fprintf(stderr, "Error: Unsupported return expression type.\n");
                }
            }
            break;
        case AST_IDENTIFIER:
            // Handled in assignment and return statements for now
            break;
        case AST_INTEGER_LITERAL:
            // Handled in assignment and return statements for now
            break;
        case AST_BLOCK_STATEMENT:
            generate_llvm_ir(root->body); // Process statements in the block
            break;
        case AST_EXPRESSION_STATEMENT:
            generate_llvm_ir(root->left); // Evaluate the expression (e.g., function call)
            break;
        case AST_BINARY_OPERATION:
            // This is a very basic example, assuming only addition of integer literals
            if (strcmp(root->token.value, "+") == 0 &&
                root->left->type == AST_INTEGER_LITERAL &&
                root->right->type == AST_INTEGER_LITERAL) {
                int val1 = atoi(root->left->token.value);
                int val2 = atoi(root->right->token.value);
                // In a real compiler, you would generate an 'add' instruction and assign a register
                printf("  ; Result of %d + %d = %d (Not directly represented in IR yet)\n", val1, val2, val1 + val2);
            } else {
                fprintf(stderr, "Error: Unsupported binary operation or operand types.\n");
            }
            break;
        // Add cases for other AST node types
        default:
            fprintf(stderr, "Warning: LLVM IR generation not implemented for AST node type %d\n", root->type);
            break;
    }

    generate_llvm_ir(root->next); // Process sibling nodes
}

// Example of a simple main function to test the LLVM IR generator

#include "parser.h" // Assuming parser's main calls lexer and builds AST

int main() {
    const char *source_code = "int main() { int x; x = 10; int y; y = x; return y + 5; }";
    lexer_init(source_code);
    parser_init();
    ast_node_t *root = parse_program();

    semantic_analyzer_init();
    analyze_ast(root); // Perform semantic analysis before IR generation

    llvm_ir_generator_init();
    generate_llvm_ir(root);

    free_ast(root);
    destroy_symbol_table(global_symbol_table);

    return 0;
}
