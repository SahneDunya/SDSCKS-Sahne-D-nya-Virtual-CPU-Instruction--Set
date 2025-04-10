#include "symbol_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

bool add_symbol(symbol_table_t *table, const char *name, ast_node_type_t type) {
    if (lookup_symbol(table, name)) {
        return false; // Symbol already exists
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
    return true;
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

void print_symbol_table(symbol_table_t *table) {
    printf("Symbol Table:\n");
    symbol_t *current = table->head;
    while (current) {
        printf("  Name: %s, Type: %d\n", current->name, current->type);
        current = current->next;
    }
}