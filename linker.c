#include "linker.h"

// Assume a very simple object file format:
8 bytes: Code size (uint64_t)
Code section (code_size bytes)
8 bytes: Data size (uint64_t)
Data section (data_size bytes)
// - (For simplicity, we'll skip symbol and relocation tables in this basic example)

object_file_t *load_object_file(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Error opening object file");
        return NULL;
    }

    object_file_t *obj_file = (object_file_t *)malloc(sizeof(object_file_t));
    if (!obj_file) {
        perror("Memory allocation failed");
        fclose(file);
        return NULL;
    }
    memset(obj_file, 0, sizeof(object_file_t));
    obj_file->filename = strdup(filename);

    if (fread(&obj_file->code_size, sizeof(uint64_t), 1, file) != 1) {
        fprintf(stderr, "Error reading code size from %s\n", filename);
        free_object_file(obj_file);
        fclose(file);
        return NULL;
    }
    if (obj_file->code_size > 0) {
        obj_file->code_section = (uint8_t *)malloc(obj_file->code_size);
        if (!obj_file->code_section) {
            perror("Memory allocation failed for code section");
            free_object_file(obj_file);
            fclose(file);
            return NULL;
        }
        if (fread(obj_file->code_section, 1, obj_file->code_size, file) != obj_file->code_size) {
            fprintf(stderr, "Error reading code section from %s\n", filename);
            free_object_file(obj_file);
            fclose(file);
            return NULL;
        }
    }

    if (fread(&obj_file->data_size, sizeof(uint64_t), 1, file) != 1) {
        fprintf(stderr, "Error reading data size from %s\n", filename);
        free_object_file(obj_file);
        fclose(file);
        return NULL;
    }
    if (obj_file->data_size > 0) {
        obj_file->data_section = (uint8_t *)malloc(obj_file->data_size);
        if (!obj_file->data_section) {
            perror("Memory allocation failed for data section");
            free_object_file(obj_file);
            fclose(file);
            return NULL;
        }
        if (fread(obj_file->data_section, 1, obj_file->data_size, file) != obj_file->data_size) {
            fprintf(stderr, "Error reading data section from %s\n", filename);
            free_object_file(obj_file);
            fclose(file);
            return NULL;
        }
    }

    fclose(file);
    return obj_file;
}

bool link_object_files(object_file_t *object_list, const char *output_filename) {
    FILE *outfile = fopen(output_filename, "wb");
    if (!outfile) {
        perror("Error opening output file");
        return false;
    }

    uint64_t total_code_size = 0;
    uint64_t total_data_size = 0;
    object_file_t *current = object_list;
    while (current) {
        total_code_size += current->code_size;
        total_data_size += current->data_size;
        current = current->next;
    }

    uint8_t *linked_code = (uint8_t *)malloc(total_code_size);
    uint8_t *linked_data = (uint8_t *)malloc(total_data_size);
    if (!linked_code || !linked_data) {
        perror("Memory allocation failed for linked sections");
        fclose(outfile);
        free(linked_code);
        free(linked_data);
        return false;
    }
    memset(linked_code, 0, total_code_size);
    memset(linked_data, 0, total_data_size);

    uint64_t current_code_offset = 0;
    uint64_t current_data_offset = 0;
    current = object_list;
    while (current) {
        if (current->code_size > 0) {
            memcpy(linked_code + current_code_offset, current->code_section, current->code_size);
            // In a real linker, you would need to keep track of the base address
            // of each object file's code section for relocation purposes.
            current_code_offset += current->code_size;
        }
        if (current->data_size > 0) {
            memcpy(linked_data + current_data_offset, current->data_section, current->data_size);
            // Similarly for the data section.
            current_data_offset += current->data_size;
        }
        current = current->next;
    }

    // For this simple example, we just write the combined code and data sections
    fwrite(linked_code, 1, total_code_size, outfile);
    fwrite(linked_data, 1, total_data_size, outfile);

    fclose(outfile);
    free(linked_code);
    free(linked_data);
    return true;
}

symbol_t *resolve_symbols(object_file_t *object_list) {
    // This is a placeholder. A real linker needs to collect all global symbols
    // from all object files and check for duplicates.
    return NULL;
}

bool perform_relocations(object_file_t *object_list, symbol_t *global_symbol_table, uint8_t *linked_code_base) {
    // This is a placeholder. A real linker needs to iterate through the relocation
    // tables of each object file and update the addresses in the linked code
    // based on the resolved symbol addresses.
    return true;
}

void free_object_file(object_file_t *obj_file) {
    if (obj_file) {
        free(obj_file->filename);
        free(obj_file->code_section);
        free(obj_file->data_section);
        free_symbol_table(obj_file->local_symbols);
        free_symbol_table(obj_file->global_symbols);
        free_relocation_table(obj_file->relocations);
        free(obj_file);
    }
}

void free_symbol_table(symbol_t *head) {
    symbol_t *current = head;
    while (current) {
        symbol_t *next = current->next;
        free(current->name);
        free(current);
        current = next;
    }
}

void free_relocation_table(relocation_t *head) {
    relocation_t *current = head;
    while (current) {
        relocation_t *next = current->next;
        free(current->symbol_name);
        free(current);
        current = next;
    }
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <output_file> <object_file1> [object_file2 ...]\n", argv[0]);
        return 1;
    }

    const char *output_filename = argv[1];
    object_file_t *head = NULL;
    object_file_t *tail = NULL;

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
            // Handle error loading object file
            fprintf(stderr, "Error loading object file: %s\n", argv[i]);
            // You might want to free any already loaded object files here
            return 1;
        }
    }

    if (head) {
        if (link_object_files(head, output_filename)) {
            printf("Successfully linked object files into %s\n", output_filename);
        } else {
            fprintf(stderr, "Linking failed.\n");
        }

        // Free the linked list of object files
        object_file_t *current = head;
        while (current) {
            object_file_t *next = current->next;
            free_object_file(current);
            current = next;
        }
    } else {
        fprintf(stderr, "No object files to link.\n");
        return 1;
    }

    return 0;
}