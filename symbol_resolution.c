#include "symbol_resolution.h"
#include <stdlib.h>
#include <string.h>

global_symbol_t *resolve_symbols(object_file_t *object_list) {
    global_symbol_t *global_table = NULL;
    uint64_t current_code_offset = 0;
    uint64_t current_data_offset = 0;

    for (object_file_t *obj = object_list; obj != NULL; obj = obj->next) {
        // Process global symbols from the current object file
        for (int i = 0; i < obj->symbol_table_size; ++i) {
            object_symbol_t *sym = &((object_symbol_t *)obj->local_symbols)[i]; // Assuming local_symbols points to an array

            if (sym->scope == SYMBOL_SCOPE_GLOBAL) {
                uint64_t symbol_address;
                if (sym->type == SYMBOL_TYPE_LABEL) {
                    symbol_address = current_code_offset + sym->value; // Value is relative offset in object file
                } else if (sym->type == SYMBOL_TYPE_DATA) {
                    symbol_address = current_code_offset + obj->code_size + current_data_offset + sym->value; // Assuming data follows code
                } else {
                    fprintf(stderr, "Warning: Unknown symbol type %d for symbol %s in %s\n",
                            sym->type, sym->name, obj->filename);
                    continue;
                }

                if (!add_global_symbol(&global_table, sym->name, sym->type, symbol_address)) {
                    fprintf(stderr, "Error: Duplicate global symbol '%s' found in %s\n",
                            sym->name, obj->filename);
                    // Handle error (e.g., exit)
                    free_global_symbol_table(global_table);
                    return NULL;
                }
            }
        }
        current_code_offset += obj->code_size;
        current_data_offset += obj->data_size;
        // In a more complex linker, you might need to handle alignment and other factors
    }

    return global_table;
}

bool add_global_symbol(global_symbol_t **head, const char *name, uint32_t type, uint64_t address) {
    if (find_global_symbol(*head, name) != NULL) {
        // Symbol already exists
        return false;
    }

    global_symbol_t *new_symbol = (global_symbol_t *)malloc(sizeof(global_symbol_t));
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

    new_symbol->type = type;
    new_symbol->address = address;
    new_symbol->next = *head;
    *head = new_symbol;

    return true;
}

global_symbol_t *find_global_symbol(global_symbol_t *head, const char *name) {
    global_symbol_t *current = head;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void free_global_symbol_table(global_symbol_t *head) {
    global_symbol_t *current = head;
    while (current != NULL) {
        global_symbol_t *next = current->next;
        free(current->name);
        free(current);
        current = next;
    }
}