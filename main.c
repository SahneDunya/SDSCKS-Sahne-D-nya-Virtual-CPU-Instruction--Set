#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linker.h"
#include "symbol_resolution.h"
#include "relocation.h"
#include "object_file_format.h"
#include "executable_file_format.h"

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <output_executable> <object_file1> [object_file2 ...]\n", argv[0]);
        return 1;
    }

    const char *output_executable_filename = argv[1];
    object_file_t *head = NULL;
    object_file_t *tail = NULL;

    // Load all object files
    for (int i = 2; i < argc; ++i) {
        object_file_t *obj = load_object_file(argv[i]);
        if (obj) {
            if (!head) {
                head = obj;
                tail = obj;
            } else {
                tail->next = obj;
                tail = obj;
            }
        } else {
            fprintf(stderr, "Error loading object file: %s\n", argv[i]);
            // Free any already loaded object files
            object_file_t *current = head;
            while (current) {
                object_file_t *next = current->next;
                free_object_file(current);
                current = next;
            }
            return 1;
        }
    }

    if (!head) {
        fprintf(stderr, "No object files to link.\n");
        return 1;
    }

    // Resolve global symbols
    global_symbol_t *global_symbol_table = resolve_symbols(head);
    if (!global_symbol_table) {
        fprintf(stderr, "Symbol resolution failed.\n");
        // Free loaded object files
        object_file_t *current = head;
        while (current) {
            object_file_t *next = current->next;
            free_object_file(current);
            current = next;
        }
        return 1;
    }

    // Calculate total code and data sizes
    uint64_t total_code_size = 0;
    uint64_t total_data_size = 0;
    object_file_t *current_obj = head;
    while (current_obj) {
        total_code_size += current_obj->code_size;
        total_data_size += current_obj->data_size;
        current_obj = current_obj->next;
    }

    // Allocate memory for the linked code and data
    uint8_t *linked_code = (uint8_t *)malloc(total_code_size);
    uint8_t *linked_data = (uint8_t *)malloc(total_data_size);
    if (!linked_code && total_code_size > 0 || !linked_data && total_data_size > 0) {
        perror("Memory allocation failed for linked sections");
        free_global_symbol_table(global_symbol_table);
        // Free loaded object files
        current_obj = head;
        while (current_obj) {
            object_file_t *next = current_obj->next;
            free_object_file(current_obj);
            current_obj = next;
        }
        free(linked_code);
        free(linked_data);
        return 1;
    }
    if (linked_code) memset(linked_code, 0, total_code_size);
    if (linked_data) memset(linked_data, 0, total_data_size);

    // Copy code and data sections into the linked memory
    uint64_t current_code_offset = 0;
    uint64_t current_data_offset = 0;
    current_obj = head;
    while (current_obj) {
        if (current_obj->code_section) {
            memcpy(linked_code + current_code_offset, current_obj->code_section, current_obj->code_size);
            current_code_offset += current_obj->code_size;
        }
        if (current_obj->data_section) {
            memcpy(linked_data + current_data_offset, current_obj->data_section, current_obj->data_size);
            current_data_offset += current_obj->data_size;
        }
        current_obj = current_obj->next;
    }

    // Perform relocations
    if (!perform_relocations(head, global_symbol_table, linked_code, linked_data)) {
        fprintf(stderr, "Relocation failed.\n");
        free_global_symbol_table(global_symbol_table);
        // Free loaded object files and linked sections
        current_obj = head;
        while (current_obj) {
            object_file_t *next = current_obj->next;
            free_object_file(current_obj);
            current_obj = next;
        }
        free(linked_code);
        free(linked_data);
        return 1;
    }

    // Find the entry point symbol (e.g., "main" or "_start")
    global_symbol_t *entry_point_symbol = find_global_symbol(global_symbol_table, "main"); // Or "_start"
    uint64_t entry_point_address = 0;
    if (entry_point_symbol) {
        entry_point_address = entry_point_symbol->address;
    } else {
        fprintf(stderr, "Warning: Entry point symbol 'main' not found, assuming start at address 0.\n");
    }

    // Create the executable file
    FILE *outfile = fopen(output_executable_filename, "wb");
    if (!outfile) {
        perror("Error opening output executable file");
        free_global_symbol_table(global_symbol_table);
        // Free loaded object files and linked sections
        current_obj = head;
        while (current_obj) {
            object_file_t *next = current_obj->next;
            free_object_file(current_obj);
            current_obj = next;
        }
        free(linked_code);
        free(linked_data);
        return 1;
    }

    // Write the executable file header
    executable_file_header_t header;
    header.magic = SDSCKS_EXECUTABLE_MAGIC;
    header.version = EXECUTABLE_FILE_VERSION;
    header.code_size = total_code_size;
    header.data_size = total_data_size;
    header.entry_point = entry_point_address;

    fwrite(&header, sizeof(executable_file_header_t), 1, outfile);

    // Write the linked code and data sections
    if (linked_code) fwrite(linked_code, 1, total_code_size, outfile);
    if (linked_data) fwrite(linked_data, 1, total_data_size, outfile);

    fclose(outfile);
    printf("Successfully linked and created executable: %s\n", output_executable_filename);

    // Free all allocated memory
    free_global_symbol_table(global_symbol_table);
    current_obj = head;
    while (current_obj) {
        object_file_t *next = current_obj->next;
        free_object_file(current_obj);
        current_obj = next;
    }
    free(linked_code);
    free(linked_data);

    return 0;
}