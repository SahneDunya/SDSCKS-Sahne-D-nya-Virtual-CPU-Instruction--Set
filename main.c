#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "assembly_lexer.h"
#include "assembly_parser.h"
#include "symbol_table.h"
#include "opcodes.h"

#define MAX_LINE_LENGTH 256

// Forward declaration
bool assemble_pass2(FILE *inputFile, symbol_t *symbolTable, FILE *outputFile);

// Helper function to perform the first pass of the assembler (symbol table creation)
symbol_t *assemble_pass1(FILE *inputFile, long *program_size) {
    symbol_t *symbolTable = create_symbol_table();
    init_lexer(inputFile);
    token_t current_token = get_next_token();
    uint64_t current_address = 0;

    while (current_token.type != TOKEN_EOF) {
        if (current_token.type == TOKEN_MNEMONIC) {
            token_t next_token = get_next_token();
            if (next_token.type == TOKEN_COLON) {
                // Label definition
                if (!add_symbol(&symbolTable, current_token.value, current_address)) {
                    fprintf(stderr, "Error: Duplicate label '%s' on line %d\n", current_token.value, current_token.line_number);
                    // Handle error
                }
                current_token = get_next_token(); // Consume colon
            } else {
                // Instruction
                current_address += 8; // Assuming fixed 8-byte instruction size
                current_token = next_token;
            }
        } else if (current_token.type == TOKEN_ERROR) {
            fprintf(stderr, "Lexer Error on line %d, column %d: %s\n",
                    current_token.line_number, current_token.column_number, current_token.value);
            // Handle error, maybe stop assembly
            break;
        } else {
            current_token = get_next_token();
        }
    }

    *program_size = current_address;
    rewind(inputFile); // Reset file pointer for the second pass
    init_lexer(inputFile); // Re-initialize lexer for the second pass
    return symbolTable;
}

bool assemble_pass2(FILE *inputFile, symbol_t *symbolTable, FILE *outputFile) {
    init_lexer(inputFile);
    token_t current_token = get_next_token();
    bool success = true;

    while (current_token.type != TOKEN_EOF) {
        if (current_token.type == TOKEN_MNEMONIC) {
            char *mnemonic = current_token.value;
            uint64_t instruction_word = 0;

            if (strcmp(mnemonic, "ADD") == 0) {
                // Example: ADD Rd, Rs1, Rs2
                token_t rd_token = get_next_token();
                if (rd_token.type == TOKEN_REGISTER) {
                    token_t comma1_token = get_next_token();
                    if (comma1_token.type == TOKEN_COMMA) {
                        token_t rs1_token = get_next_token();
                        if (rs1_token.type == TOKEN_REGISTER) {
                            token_t comma2_token = get_next_token();
                            if (comma2_token.type == TOKEN_COMMA) {
                                token_t rs2_token = get_next_token();
                                if (rs2_token.type == TOKEN_REGISTER) {
                                    int rd = atoi(rd_token.value + 1);
                                    int rs1 = atoi(rs1_token.value + 1);
                                    int rs2 = atoi(rs2_token.value + 1);
                                    if (rd >= 0 && rd < 32 && rs1 >= 0 && rs1 < 32 && rs2 >= 0 && rs2 < 32) {
                                        instruction_word = ((uint64_t)OP_ADD << 58) | ((uint64_t)rd << 11) | ((uint64_t)rs1 << 16) | ((uint64_t)rs2 << 21);
                                        fwrite(&instruction_word, sizeof(uint64_t), 1, outputFile);
                                    } else {
                                        fprintf(stderr, "Error: Invalid register operand on line %d\n", current_token.line_number);
                                        success = false;
                                    }
                                    current_token = get_next_token();
                                } else {
                                    fprintf(stderr, "Error: Expected third register operand for ADD on line %d\n", current_token.line_number);
                                    success = false;
                                }
                            } else {
                                fprintf(stderr, "Error: Expected comma after second register operand for ADD on line %d\n", current_token.line_number);
                                success = false;
                            }
                        } else {
                            fprintf(stderr, "Error: Expected second register operand for ADD on line %d\n", current_token.line_number);
                            success = false;
                        }
                    } else {
                        fprintf(stderr, "Error: Expected comma after first register operand for ADD on line %d\n", current_token.line_number);
                        success = false;
                    }
                } else {
                    fprintf(stderr, "Error: Expected first register operand for ADD on line %d\n", current_token.line_number);
                    success = false;
                }
            } else if (strcmp(mnemonic, "HALT") == 0) {
                instruction_word = ((uint64_t)OP_HALT << 58);
                fwrite(&instruction_word, sizeof(uint64_t), 1, outputFile);
                current_token = get_next_token();
            } else {
                fprintf(stderr, "Error: Unknown mnemonic '%s' on line %d\n", mnemonic, current_token.line_number);
                success = false;
                current_token = get_next_token(); // Try to continue
            }
        } else if (current_token.type == TOKEN_ERROR) {
            success = false;
            current_token = get_next_token();
        } else {
            current_token = get_next_token();
        }
    }

    return success;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input_assembly_file> <output_binary_file>\n", argv[0]);
        return 1;
    }

    FILE *inputFile = fopen(argv[1], "r");
    if (!inputFile) {
        perror("Error opening input file");
        return 1;
    }

    long program_size = 0;
    symbol_t *symbolTable = assemble_pass1(inputFile, &program_size);

    FILE *outputFile = fopen(argv[2], "wb");
    if (!outputFile) {
        perror("Error opening output file");
        fclose(inputFile);
        free_symbol_table(symbolTable);
        return 1;
    }

    if (assemble_pass2(inputFile, symbolTable, outputFile)) {
        printf("Assembly successful. Output written to %s\n", argv[2]);
    } else {
        fprintf(stderr, "Assembly failed.\n");
    }

    fclose(inputFile);
    fclose(outputFile);
    free_symbol_table(symbolTable);

    return 0;
}