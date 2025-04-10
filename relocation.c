#include "relocation.h"
#include <stdlib.h>
#include <string.h>

bool perform_relocations(object_file_t *object_list, global_symbol_t *global_symbol_table, uint8_t *linked_code_base, uint8_t *linked_data_base) {
    for (object_file_t *obj = object_list; obj != NULL; obj = obj->next) {
        uint64_t code_base_offset = (uint64_t)(linked_code_base - (uint8_t *)0); // Get the address as an offset
        uint64_t data_base_offset = (uint64_t)(linked_data_base - (uint8_t *)0);

        for (int i = 0; i < obj->relocation_table_size; ++i) {
            object_relocation_t *reloc = &((object_relocation_t *)obj->relocations)[i]; // Assuming relocations is an array

            global_symbol_t *target_symbol = find_global_symbol(global_symbol_table, reloc->symbol_name);
            if (!target_symbol) {
                fprintf(stderr, "Error: Undefined symbol '%s' for relocation in %s at offset 0x%llX\n",
                        reloc->symbol_name, obj->filename, reloc->offset);
                return false;
            }

            uint64_t target_address = target_symbol->address;
            uint64_t relocation_address;

            // Determine if the relocation is in the code or data section of the object file
            if (reloc->offset < obj->code_size) {
                relocation_address = code_base_offset + get_object_code_offset(object_list, obj) + reloc->offset;
                // Assuming RELOC_TYPE_ABSOLUTE64: write the 64-bit target address
                write_address_to_memory(linked_code_base, relocation_address, target_address);
            } else if (reloc->offset >= obj->code_size && reloc->offset < obj->code_size + obj->data_size) {
                uint64_t data_offset_in_obj = reloc->offset - obj->code_size;
                relocation_address = data_base_offset + get_object_data_offset(object_list, obj) + data_offset_in_obj;
                write_address_to_memory(linked_data_base, relocation_address, target_address);
            } else {
                fprintf(stderr, "Error: Relocation offset 0x%llX out of bounds in %s\n",
                        reloc->offset, obj->filename);
                return false;
            }
        }
    }
    return true;
}

// Helper function to get the starting offset of an object file's code in the linked code
uint64_t get_object_code_offset(object_file_t *object_list, object_file_t *target_obj) {
    uint64_t offset = 0;
    for (object_file_t *obj = object_list; obj != target_obj; obj = obj->next) {
        offset += obj->code_size;
    }
    return offset;
}

// Helper function to get the starting offset of an object file's data in the linked data
uint64_t get_object_data_offset(object_file_t *object_list, object_file_t *target_obj) {
    uint64_t offset = 0;
    // First, skip all the code sections
    for (object_file_t *obj = object_list; obj != NULL; obj = obj->next) {
        if (obj == target_obj) break;
        offset += obj->code_size;
    }
    // Then, add the data section sizes of the preceding object files
    for (object_file_t *obj = object_list; obj != target_obj; obj = obj->next) {
        offset += obj->data_size;
    }
    // Offset should be relative to the start of the linked data section
    uint64_t data_start = 0;
    for (object_file_t *obj = object_list; obj != target_obj; obj = obj->next) {
        data_start += obj->code_size; // Size of all preceding code sections
    }
    uint64_t current_data_offset = 0;
    for (object_file_t *obj = object_list; obj != target_obj; obj = obj->next) {
        current_data_offset += obj->data_size;
    }
    return current_data_offset;
}

// Helper function to write a 64-bit address to memory at a given location
void write_address_to_memory(uint8_t *base, uint64_t address, uint64_t value) {
    uint8_t *target = base + address;
    for (int i = 0; i < 8; ++i) {
        target[i] = (uint8_t)(value >> (i * 8));
    }
}