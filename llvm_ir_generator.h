#ifndef LLVM_IR_GENERATOR_H
#define LLVM_IR_GENERATOR_H

#include "ast.h"
#include "symbol_table.h"

// Function to initialize the LLVM IR generator
void llvm_ir_generator_init();

// Function to generate LLVM IR from the AST
void generate_llvm_ir(ast_node_t *root);

#endif // LLVM_IR_GENERATOR_H