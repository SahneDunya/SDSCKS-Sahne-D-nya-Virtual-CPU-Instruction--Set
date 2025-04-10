#include "assembly_parser.h"
#include "assembly_lexer.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static token_t current_token;

token_t get_current_parse_token() {
    return current_token;
}

token_t consume_token() {
    token_t previous_token = current_token;
    current_token = get_next_token();
    return previous_token;
}

bool check_token(token_type_t expected_type) {
    return current_token.type == expected_type;
}

void parser_error(const char *message) {
    fprintf(stderr, "Parser Error at line %d, column %d: %s (found '%s')\n",
            current_token.line_number, current_token.column_number, message, current_token.value);
    // You might want to exit or handle errors differently
}

parsed_instruction_t *parse_line() {
    parsed_instruction_t *parsed_inst = (parsed_instruction_t *)malloc(sizeof(parsed_instruction_t));
    if (!parsed_inst) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    parsed_inst->label = NULL;
    parsed_inst->mnemonic = NULL;
    parsed_inst->operand1 = NULL;
    parsed_inst->operand2 = NULL;
    parsed_inst->operand3 = NULL;
    parsed_inst->line_number = current_token.line_number;

    // Check for a label
    if (check_token(TOKEN_MNEMONIC) && get_next_token().type == TOKEN_COLON) {
        parsed_inst->label = strdup(current_token.value);
        consume_token(); // Consume the label
        consume_token(); // Consume the colon
        parsed_inst->mnemonic = strdup(consume_token().value); // Consume the mnemonic
    } else if (check_token(TOKEN_MNEMONIC)) {
        parsed_inst->mnemonic = strdup(consume_token().value); // Consume the mnemonic
    } else if (check_token(TOKEN_EOF)) {
        free(parsed_inst);
        return NULL; // End of file
    } else {
        parser_error("Expected mnemonic or label");
        // Recover or exit
        free(parsed_inst);
        return NULL;
    }

    // Parse operands based on the mnemonic (this will need a more comprehensive implementation)
    if (parsed_inst->mnemonic) {
        if (strcmp(parsed_inst->mnemonic, "ADD") == 0 ||
            strcmp(parsed_inst->mnemonic, "SUB") == 0 ||
            strcmp(parsed_inst->mnemonic, "MUL") == 0 ||
            strcmp(parsed_inst->mnemonic, "DIV") == 0 ||
            strcmp(parsed_inst->mnemonic, "AND") == 0 ||
            strcmp(parsed_inst->mnemonic, "OR") == 0 ||
            strcmp(parsed_inst->mnemonic, "XOR") == 0 ||
            strcmp(parsed_inst->mnemonic, "SLL") == 0 ||
            strcmp(parsed_inst->mnemonic, "SRL") == 0 ||
            strcmp(parsed_inst->mnemonic, "SRA") == 0 ||
            strcmp(parsed_inst->mnemonic, "CMP") == 0) {
            // Expecting three register operands: Rd, Rs1, Rs2
            if (check_token(TOKEN_REGISTER)) {
                parsed_inst->operand1 = strdup(consume_token().value);
                if (check_token(TOKEN_COMMA)) consume_token();
                if (check_token(TOKEN_REGISTER)) {
                    parsed_inst->operand2 = strdup(consume_token().value);
                    if (check_token(TOKEN_COMMA)) consume_token();
                    if (check_token(TOKEN_REGISTER)) {
                        parsed_inst->operand3 = strdup(consume_token().value);
                    } else {
                        parser_error("Expected third register operand");
                    }
                } else {
                    parser_error("Expected second register operand");
                }
            } else {
                parser_error("Expected first register operand");
            }
        } else if (strcmp(parsed_inst->mnemonic, "ADDI") == 0 ||
                   strcmp(parsed_inst->mnemonic, "SUBI") == 0 ||
                   strcmp(parsed_inst->mnemonic, "ANDI") == 0 ||
                   strcmp(parsed_inst->mnemonic, "ORI") == 0 ||
                   strcmp(parsed_inst->mnemonic, "XORI") == 0 ||
                   strcmp(parsed_inst->mnemonic, "LI") == 0) {
            // Expecting register and immediate: Rd, immediate
            if (check_token(TOKEN_REGISTER)) {
                parsed_inst->operand1 = strdup(consume_token().value);
                if (check_token(TOKEN_COMMA)) consume_token();
                if (check_token(TOKEN_IMMEDIATE)) {
                    parsed_inst->operand2 = strdup(consume_token().value);
                } else {
                    parser_error("Expected immediate operand");
                }
            } else {
                parser_error("Expected register operand");
            }
        } else if (strcmp(parsed_inst->mnemonic, "LOAD") == 0 ||
                   strcmp(parsed_inst->mnemonic, "STORE") == 0) {
            // Expecting register and address (for simplicity, assume immediate is address): Rd/Rs, address
            if (check_token(TOKEN_REGISTER)) {
                parsed_inst->operand1 = strdup(consume_token().value);
                if (check_token(TOKEN_COMMA)) consume_token();
                if (check_token(TOKEN_IMMEDIATE)) {
                    parsed_inst->operand2 = strdup(consume_token().value);
                } else {
                    parser_error("Expected address (immediate) operand");
                }
            } else {
                parser_error("Expected register operand");
            }
        } else if (strcmp(parsed_inst->mnemonic, "JMP") == 0) {
            // Expecting immediate address (or label)
            if (check_token(TOKEN_IMMEDIATE) || check_token(TOKEN_MNEMONIC)) { // MNEMONIC could be a label
                parsed_inst->operand1 = strdup(consume_token().value);
            } else {
                parser_error("Expected jump address (immediate or label)");
            }
        } else if (strcmp(parsed_inst->mnemonic, "JR") == 0) {
            // Expecting register with address
            if (check_token(TOKEN_REGISTER)) {
                parsed_inst->operand1 = strdup(consume_token().value);
            } else {
                parser_error("Expected register operand for jump");
            }
        } else if (strcmp(parsed_inst->mnemonic, "BEQ") == 0 ||
                   strcmp(parsed_inst->mnemonic, "BNE") == 0) {
            // Expecting two registers and a target (immediate or label)
            if (check_token(TOKEN_REGISTER)) {
                parsed_inst->operand1 = strdup(consume_token().value);
                if (check_token(TOKEN_COMMA)) consume_token();
                if (check_token(TOKEN_REGISTER)) {
                    parsed_inst->operand2 = strdup(consume_token().value);
                    if (check_token(TOKEN_COMMA)) consume_token();
                    if (check_token(TOKEN_IMMEDIATE) || check_token(TOKEN_MNEMONIC)) { // MNEMONIC could be a label
                        parsed_inst->operand3 = strdup(consume_token().value);
                    } else {
                        parser_error("Expected branch target (immediate or label)");
                    }
                } else {
                    parser_error("Expected second register operand");
                }
            } else {
                parser_error("Expected first register operand");
            }
        } else if (strcmp(parsed_inst->mnemonic, "HALT") == 0) {
            // No operands expected
        } else if (parsed_inst->mnemonic != NULL) {
            parser_error("Unknown mnemonic");
        }
    }

    return parsed_inst;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input_assembly_file>\n", argv[0]);
        return 1;
    }

    FILE *inputFile = fopen(argv[1], "r");
    if (!inputFile) {
        perror("Error opening input file");
        return 1;
    }

    init_lexer(inputFile);
    current_token = get_next_token(); // Get the first token

    parsed_instruction_t *instruction;
    while ((instruction = parse_line()) != NULL) {
        if (instruction->mnemonic) {
            printf("Label: %s, Mnemonic: %s, Operand1: %s, Operand2: %s, Operand3: %s, Line: %d\n",
                   instruction->label ? instruction->label : "(none)",
                   instruction->mnemonic ? instruction->mnemonic : "(none)",
                   instruction->operand1 ? instruction->operand1 : "(none)",
                   instruction->operand2 ? instruction->operand2 : "(none)",
                   instruction->operand3 ? instruction->operand3 : "(none)",
                   instruction->line_number);
        } else if (instruction->label) {
            printf("Label: %s (no instruction), Line: %d\n", instruction->label, instruction->line_number);
        }
        // Free the allocated memory for the parsed instruction
        free(instruction->label);
        free(instruction->mnemonic);
        free(instruction->operand1);
        free(instruction->operand2);
        free(instruction->operand3);
        free(instruction);
        if (check_token(TOKEN_ERROR)) {
            // Stop parsing on error for this example
            break;
        }
    }

    fclose(inputFile);
    return 0;
}