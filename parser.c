#include "parser.h"
#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static token_t current_token;

void parser_init() {
    current_token = get_next_token();
}

token_t consume_token(token_type_t expected_type, const char *error_message) {
    if (current_token.type == expected_type) {
        token_t consumed_token = current_token;
        current_token = get_next_token();
        return consumed_token;
    } else {
        fprintf(stderr, "Error at line %d, column %d: %s. Expected token type %d, but got %d (value: '%s')\n",
                current_token.line, current_token.column, error_message, expected_type, current_token.type, current_token.value);
        // Handle error recovery (e.g., exit or try to synchronize)
        exit(EXIT_FAILURE);
    }
}

token_t peek_token() {
    return current_token;
}

ast_node_t *create_ast_node(ast_node_type_t type) {
    ast_node_t *node = (ast_node_t *)malloc(sizeof(ast_node_t));
    if (!node) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    memset(node, 0, sizeof(ast_node_t));
    node->type = type;
    return node;
}

ast_node_t *parse_program() {
    ast_node_t *program_node = create_ast_node(AST_PROGRAM);
    program_node->body = NULL;
    ast_node_t *current = NULL;

    while (peek_token().type != TOKEN_EOF) {
        ast_node_t *function_def = parse_function_definition();
        if (program_node->body == NULL) {
            program_node->body = function_def;
            current = function_def;
        } else {
            current->next = function_def;
            current = function_def;
        }
    }

    return program_node;
}

ast_node_t *parse_function_definition() {
    consume_token(TOKEN_KEYWORD, "Expected keyword 'int' for function definition");
    token_t function_name_token = consume_token(TOKEN_IDENTIFIER, "Expected function name");
    consume_token(TOKEN_PUNCTUATOR, "Expected '(' after function name");
    consume_token(TOKEN_PUNCTUATOR, "Expected ')' after '(' (for now, no parameters)");
    consume_token(TOKEN_PUNCTUATOR, "Expected '{' to start function body");

    ast_node_t *func_def_node = create_ast_node(AST_FUNCTION_DEFINITION);
    func_def_node->token = function_name_token;
    func_def_node->body = NULL;
    ast_node_t *current = NULL;

    while (peek_token().type != TOKEN_PUNCTUATOR || (peek_token().value != NULL && strcmp(peek_token().value, "}") != 0)) {
        ast_node_t *statement = parse_statement();
        if (func_def_node->body == NULL) {
            func_def_node->body = statement;
            current = statement;
        } else {
            current->next = statement;
            current = statement;
        }
    }

    consume_token(TOKEN_PUNCTUATOR, "Expected '}' to end function body");
    return func_def_node;
}

ast_node_t *parse_statement() {
    if (peek_token().type == TOKEN_KEYWORD && strcmp(peek_token().value, "int") == 0) {
        return parse_variable_declaration();
    } else if (peek_token().type == TOKEN_IDENTIFIER) {
        // Assuming assignment statement starts with an identifier
        return parse_assignment_statement();
    } else if (peek_token().type == TOKEN_KEYWORD && strcmp(peek_token().value, "return") == 0) {
        return parse_return_statement();
    } else {
        fprintf(stderr, "Error at line %d, column %d: Expected statement, but got token type %d (value: '%s')\n",
                peek_token().line, peek_token().column, peek_token().type, peek_token().value);
        exit(EXIT_FAILURE);
    }
}

ast_node_t *parse_variable_declaration() {
    consume_token(TOKEN_KEYWORD, "Expected keyword 'int'");
    token_t var_name_token = consume_token(TOKEN_IDENTIFIER, "Expected variable name");
    consume_token(TOKEN_PUNCTUATOR, "Expected ';' after variable declaration");

    ast_node_t *var_decl_node = create_ast_node(AST_VARIABLE_DECLARATION);
    var_decl_node->token = var_name_token;
    return var_decl_node;
}

ast_node_t *parse_assignment_statement() {
    token_t var_name_token = consume_token(TOKEN_IDENTIFIER, "Expected identifier for assignment");
    consume_token(TOKEN_OPERATOR, "Expected '=' in assignment");
    ast_node_t *expression_node = parse_expression();
    consume_token(TOKEN_PUNCTUATOR, "Expected ';' after assignment statement");

    ast_node_t *assign_node = create_ast_node(AST_ASSIGNMENT_STATEMENT);
    assign_node->token = var_name_token;
    assign_node->right = expression_node; // Assuming assignment is like identifier = expression
    return assign_node;
}

ast_node_t *parse_return_statement() {
    consume_token(TOKEN_KEYWORD, "Expected keyword 'return'");
    ast_node_t *expression_node = parse_expression();
    consume_token(TOKEN_PUNCTUATOR, "Expected ';' after return statement");

    ast_node_t *return_node = create_ast_node(AST_RETURN_STATEMENT);
    return_node->left = expression_node; // Assuming return is like return expression;
    return_node;
}

ast_node_t *parse_expression() {
    // For simplicity, let's just handle integer literals for now
    if (peek_token().type == TOKEN_INTEGER_LITERAL) {
        ast_node_t *literal_node = create_ast_node(AST_INTEGER_LITERAL);
        literal_node->token = consume_token(TOKEN_INTEGER_LITERAL, "Expected integer literal");
        return literal_node;
    } else if (peek_token().type == TOKEN_IDENTIFIER) {
        ast_node_t *identifier_node = create_ast_node(AST_IDENTIFIER);
        identifier_node->token = consume_token(TOKEN_IDENTIFIER, "Expected identifier");
        return identifier_node;
    } else {
        fprintf(stderr, "Error at line %d, column %d: Expected expression, but got token type %d (value: '%s')\n",
                peek_token().line, peek_token().column, peek_token().type, peek_token().value);
        exit(EXIT_FAILURE);
    }
}

// Function to free the AST (recursively)
void free_ast(ast_node_t *node) {
    if (node == NULL) return;
    free_ast(node->left);
    free_ast(node->right);
    free_ast(node->body);
    free_ast(node->next);
    free_token(&node->token);
    free(node);
}

// Example of a simple main function to test the parser

int main() {
    const char *source_code = "int main() { int x; x = 10; return x; }";
    lexer_init(source_code);
    parser_init();
    ast_node_t *root = parse_program();
    // You would then traverse and use the AST for further compilation phases
    printf("Parsing complete. AST root: %p\n", root);
    free_ast(root);
    return 0;
}