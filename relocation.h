#ifndef RELOCATION_H
#define RELOCATION_H

#include <stdint.h>
#include <stdbool.h>
#include "object_file_format.h"
#include "symbol_resolution.h"

// Function to perform relocations on the linked code and data
bool perform_relocations(object_file_t *object_list, global_symbol_t *global_symbol_table, uint8_t *linked_code_base, uint8_t *linked_data_base);

#endif // RELOCATION_H