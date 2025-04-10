#include <stdio.h>
#include <stdlib.h>
#include "vm.h" // Include the main VM header

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <program_binary_file>\n", argv[0]);
        return 1;
    }

    vm_state_t vm;
    vm_init(&vm); // Initialize the VM state

    if (vm_load_program(&vm, argv[1])) {
        printf("Starting VM execution...\n");
        vm_run(&vm); // Start the execution cycle

        printf("\nVM State After Execution:\n");
        printf("Program Counter: 0x%llX\n", vm.program_counter);
        for (int i = 0; i < NUM_REGISTERS; i++) {
            printf("R%d: 0x%llX\n", i, vm.registers[i]);
        }
        // You might want to print some memory contents or other relevant state here
    } else {
        fprintf(stderr, "Failed to load program: %s\n", argv[1]);
        return 1;
    }

    return 0;
}