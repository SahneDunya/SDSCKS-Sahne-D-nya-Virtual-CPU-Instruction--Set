#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "parser.h"
#include "ast.h"
#include "semantic_analyzer.h"
#include "llvm_ir_generator.h"
#include "error_handling.h"
#include "target_info.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input_source_file>\n", argv[0]);
        return 1;
    }

    const char *input_filename = argv[1];
    FILE *input_file = fopen(input_filename, "r");
    if (!input_file) {
        perror("Error opening input file");
        return 1;
    }

    // Read the entire source file into a buffer
    fseek(input_file, 0, SEEK_END);
    long file_size = ftell(input_file);
    fseek(input_file, 0, SEEK_SET);

    char *source_code = (char *)malloc(file_size + 1);
    if (!source_code) {
        perror("Memory allocation failed for source code");
        fclose(input_file);
        return 1;
    }

    if (fread(source_code, 1, file_size, input_file) != file_size) {
        perror("Error reading source file");
        free(source_code);
        fclose(input_file);
        return 1;
    }
    source_code[file_size] = '\0'; // Null-terminate the source code

    fclose(input_file);

    // Initialize the lexer with the source code
    lexer_init(source_code);

    // Initialize the parser
    parser_init();

    // Parse the source code into an Abstract Syntax Tree
    ast_node_t *root = parse_program();

    if (get_error_count() > 0) {
        fprintf(stderr, "Parsing failed due to %d errors.\n", get_error_count());
        free_ast(root);
        free(source_code);
        return 1;
    }

    // Initialize the semantic analyzer
    semantic_analyzer_init();

    // Perform semantic analysis on the AST
    analyze_ast(root);

    if (get_error_count() > 0) {
        fprintf(stderr, "Semantic analysis failed due to %d errors.\n", get_error_count());
        free_ast(root);
        free(source_code);
        return 1;
    }

    // Initialize the LLVM IR generator
    llvm_ir_generator_init();

    // Generate LLVM IR from the AST
    generate_llvm_ir(root);

    // Optionally, you could output the generated LLVM IR to a file here
     For example:
     FILE *output_ir_file = fopen("output.ll", "w");
    // (You would need to modify generate_llvm_ir to write to this file)
     fclose(output_ir_file);

    // Clean up allocated memory
    free_ast(root);
    free(source_code);

    return 0;
}