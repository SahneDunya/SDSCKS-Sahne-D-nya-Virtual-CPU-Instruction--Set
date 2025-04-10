#include "symbol_table.h"
#include <stdlib.h>
#include <string.h>

symbol_t *create_symbol_table() {
    return NULL; // An empty symbol table is represented by a NULL head
}

bool add_symbol(symbol_t **head, const char *name, uint64_t address) {
    if (find_symbol(*head, name) != NULL) {
        // Symbol already exists
        return false;
    }

    symbol_t *new_symbol = (symbol_t *)malloc(sizeof(symbol_t));
    if (!new_symbol) {
        perror("Memory allocation failed");
        return false;
    }

    new_symbol->name = strdup(name);
    if (!new_symbol->name) {
        perror("Memory allocation failed");
        free(new_symbol);
        return false;
    }

    new_symbol->address = address;
    new_symbol->next = *head;
    *head = new_symbol;

    return true;
}

symbol_t *find_symbol(symbol_t *head, const char *name) {
    symbol_t *current = head;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void free_symbol_table(symbol_t *head) {
    symbol_t *current = head;
    while (current != NULL) {
        symbol_t *next = current->next;
        free(current->name);
        free(current);
        current = next;
    }
}