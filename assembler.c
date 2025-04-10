#include "assembler.h"
#include "instruction_set.h"
#include "opcodes.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 256

// Helper function to trim leading and trailing whitespace from a string
char *trim(char *str) {
    char *start = str;
    while (*start && isspace(*start)) {
        start++;
    }
    char *end = start + strlen(start) - 1;
    while (end > start && isspace(*end)) {
        end--;
    }
    *(end + 1) = '\0';
    return start;
}

assembly_line_t *tokenize_line(char *line, int line_number) {
    assembly_line_t *tokens = (assembly_line_t *)malloc(sizeof(assembly_line_t));
    if (!tokens) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    tokens->label = NULL;
    tokens->mnemonic = NULL;
    tokens->operand1 = NULL;
    tokens->operand2 = NULL;
    tokens->operand3 = NULL;
    tokens->line_number = line_number;

    line = trim(line);
    if (*line == '\0' || *line == ';') { // Empty line or comment
        return tokens;
    }

    char *saveptr;
    char *token = strtok_r(line, " \t,", &saveptr);

    // Check for label (ends with ':')
    if (token && token[strlen(token) - 1] == ':') {
        tokens->label = strdup(token);
        tokens->label[strlen(tokens->label) - 1] = '\0'; // Remove the ':'
        token = strtok_r(NULL, " \t,", &saveptr);
    }

    if (token) {
        tokens->mnemonic = strdup(token);
        token = strtok_r(NULL, " \t,", &saveptr);
        if (token) {
            tokens->operand1 = strdup(token);
            token = strtok_r(NULL, " \t,", &saveptr);
            if (token) {
                tokens->operand2 = strdup(token);
                token = strtok_r(NULL, " \t,", &saveptr);
                if (token) {
                    tokens->operand3 = strdup(token);
                }
            }
        }
    }

    return tokens;
}

void free_assembly_line(assembly_line_t *line) {
    if (line) {
        free(line->label);
        free(line->mnemonic);
        free(line->operand1);
        free(line->operand2);
        free(line->operand3);
        free(line);
    }
}

void add_symbol(symbol_t **head, const char *name, uint64_t address) {
    symbol_t *new_symbol = (symbol_t *)malloc(sizeof(symbol_t));
    if (!new_symbol) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    new_symbol->name = strdup(name);
    new_symbol->address = address;
    new_symbol->next = *head;
    *head = new_symbol;
}

symbol_t *find_symbol(symbol_t *head, const char *name) {
    symbol_t *current = head;
    while (current) {
        if (strcmp(current->name, name) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

symbol_t *assemble_pass1(FILE *inputFile, long *program_size) {
    symbol_t *symbolTable = NULL;
    char line[MAX_LINE_LENGTH];
    uint64_t current_address = 0;
    int line_number = 1;

    while (fgets(line, sizeof(line), inputFile)) {
        assembly_line_t *tokens = tokenize_line(line, line_number++);
        if (tokens->label) {
            if (find_symbol(symbolTable, tokens->label)) {
                fprintf(stderr, "Error: Duplicate label '%s' on line %d\n", tokens->label, tokens->line_number);
                // Handle error appropriately
            } else {
                add_symbol(&symbolTable, tokens->label, current_address);
            }
        }
        if (tokens->mnemonic) {
            // For simplicity, assume each instruction takes a fixed size (e.g., 8 bytes for 64-bit)
            current_address += 8; // Adjust based on your instruction size
        }
        free_assembly_line(tokens);
    }

    *program_size = current_address;
    rewind(inputFile); // Reset file pointer for the second pass
    return symbolTable;
}

bool assemble_pass2(FILE *inputFile, symbol_t *symbolTable, FILE *outputFile) {
    char line[MAX_LINE_LENGTH];
    int line_number = 1;
    bool success = true;

    while (fgets(line, sizeof(line), inputFile)) {
        assembly_line_t *tokens = tokenize_line(line, line_number++);
        if (tokens->mnemonic) {
            uint64_t instruction_word = 0;
            // Here you would implement the logic to translate the assembly mnemonic
            // and operands into the 64-bit machine code instruction based on
            // your SDSCKS instruction set definition (opcodes.h and instruction format).

            // Example for a simple ADD instruction: ADD Rd, Rs1, Rs2
            if (strcmp(tokens->mnemonic, "ADD") == 0) {
                if (tokens->operand1 && tokens->operand2 && tokens->operand3) {
                    int rd = atoi(tokens->operand1 + 1); // Assuming registers are R0, R1, etc.
                    int rs1 = atoi(tokens->operand2 + 1);
                    int rs2 = atoi(tokens->operand3 + 1);
                    if (rd >= 0 && rd < NUM_REGISTERS && rs1 >= 0 && rs1 < NUM_REGISTERS && rs2 >= 0 && rs2 < NUM_REGISTERS) {
                        instruction_word = ((uint64_t)OP_ADD << 58) | ((uint64_t)rd << 11) | ((uint64_t)rs1 << 16) | ((uint64_t)rs2 << 21);
                        fwrite(&instruction_word, sizeof(uint64_t), 1, outputFile);
                    } else {
                        fprintf(stderr, "Error: Invalid register operand on line %d\n", tokens->line_number);
                        success = false;
                    }
                } else {
                    fprintf(stderr, "Error: Incorrect number of operands for ADD on line %d\n", tokens->line_number);
                    success = false;
                }
            }
            // Implement similar logic for other instructions (SUB, LOAD, STORE, JMP, etc.)
            // You'll need to parse operands (registers, immediate values, addresses)
            // and encode them into the instruction word according to your format.
            else if (strcmp(tokens->mnemonic, "HALT") == 0) {
                instruction_word = ((uint64_t)OP_HALT << 58);
                fwrite(&instruction_word, sizeof(uint64_t), 1, outputFile);
            }
            else {
                fprintf(stderr, "Error: Unknown mnemonic '%s' on line %d\n", tokens->mnemonic, tokens->line_number);
                success = false;
            }
        }
        free_assembly_line(tokens);
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
        // Free symbol table
        return 1;
    }

    rewind(inputFile); // Ensure we read from the beginning for the second pass

    if (assemble_pass2(inputFile, symbolTable, outputFile)) {
        printf("Assembly successful. Output written to %s\n", argv[2]);
    } else {
        fprintf(stderr, "Assembly failed.\n");
    }

    fclose(inputFile);
    fclose(outputFile);

    // Free the symbol table
    symbol_t *current = symbolTable;
    while (current) {
        symbol_t *next = current->next;
        free(current->name);
        free(current);
        current = next;
    }

    return 0;
}