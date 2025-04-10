#include "instruction_execution.h"
#include "memory.h"

void execute_add(vm_state_t *vm, const decoded_instruction_t *decoded) {
    if (decoded->rd < NUM_REGISTERS && decoded->rs1 < NUM_REGISTERS && decoded->rs2 < NUM_REGISTERS) {
        vm->registers[decoded->rd] = vm->registers[decoded->rs1] + vm->registers[decoded->rs2];
    } else {
        fprintf(stderr, "Error: Invalid register index in ADD instruction.\n");
        vm->running = false;
    }
}

void execute_sub(vm_state_t *vm, const decoded_instruction_t *decoded) {
    if (decoded->rd < NUM_REGISTERS && decoded->rs1 < NUM_REGISTERS && decoded->rs2 < NUM_REGISTERS) {
        vm->registers[decoded->rd] = vm->registers[decoded->rs1] - vm->registers[decoded->rs2];
    } else {
        fprintf(stderr, "Error: Invalid register index in SUB instruction.\n");
        vm->running = false;
    }
}

void execute_mul(vm_state_t *vm, const decoded_instruction_t *decoded) {
    if (decoded->rd < NUM_REGISTERS && decoded->rs1 < NUM_REGISTERS && decoded->rs2 < NUM_REGISTERS) {
        vm->registers[decoded->rd] = vm->registers[decoded->rs1] * vm->registers[decoded->rs2];
    } else {
        fprintf(stderr, "Error: Invalid register index in MUL instruction.\n");
        vm->running = false;
    }
}

void execute_div(vm_state_t *vm, const decoded_instruction_t *decoded) {
    if (decoded->rd < NUM_REGISTERS && decoded->rs1 < NUM_REGISTERS && decoded->rs2 < NUM_REGISTERS) {
        if (vm->registers[decoded->rs2] != 0) {
            vm->registers[decoded->rd] = vm->registers[decoded->rs1] / vm->registers[decoded->rs2];
        } else {
            fprintf(stderr, "Error: Division by zero.\n");
            vm->running = false;
        }
    } else {
        fprintf(stderr, "Error: Invalid register index in DIV instruction.\n");
        vm->running = false;
    }
}

void execute_and(vm_state_t *vm, const decoded_instruction_t *decoded) {
    if (decoded->rd < NUM_REGISTERS && decoded->rs1 < NUM_REGISTERS && decoded->rs2 < NUM_REGISTERS) {
        vm->registers[decoded->rd] = vm->registers[decoded->rs1] & vm->registers[decoded->rs2];
    } else {
        fprintf(stderr, "Error: Invalid register index in AND instruction.\n");
        vm->running = false;
    }
}

void execute_or(vm_state_t *vm, const decoded_instruction_t *decoded) {
    if (decoded->rd < NUM_REGISTERS && decoded->rs1 < NUM_REGISTERS && decoded->rs2 < NUM_REGISTERS) {
        vm->registers[decoded->rd] = vm->registers[decoded->rs1] | vm->registers[decoded->rs2];
    } else {
        fprintf(stderr, "Error: Invalid register index in OR instruction.\n");
        vm->running = false;
    }
}

void execute_xor(vm_state_t *vm, const decoded_instruction_t *decoded) {
    if (decoded->rd < NUM_REGISTERS && decoded->rs1 < NUM_REGISTERS && decoded->rs2 < NUM_REGISTERS) {
        vm->registers[decoded->rd] = vm->registers[decoded->rs1] ^ vm->registers[decoded->rs2];
    } else {
        fprintf(stderr, "Error: Invalid register index in XOR instruction.\n");
        vm->running = false;
    }
}

void execute_sll(vm_state_t *vm, const decoded_instruction_t *decoded) {
    if (decoded->rd < NUM_REGISTERS && decoded->rs1 < NUM_REGISTERS && decoded->rs2 < NUM_REGISTERS) {
        vm->registers[decoded->rd] = vm->registers[decoded->rs1] << vm->registers[decoded->rs2];
    } else {
        fprintf(stderr, "Error: Invalid register index in SLL instruction.\n");
        vm->running = false;
    }
}

void execute_srl(vm_state_t *vm, const decoded_instruction_t *decoded) {
    if (decoded->rd < NUM_REGISTERS && decoded->rs1 < NUM_REGISTERS && decoded->rs2 < NUM_REGISTERS) {
        vm->registers[decoded->rd] = vm->registers[decoded->rs1] >> vm->registers[decoded->rs2]; // Logical right shift
    } else {
        fprintf(stderr, "Error: Invalid register index in SRL instruction.\n");
        vm->running = false;
    }
}

void execute_sra(vm_state_t *vm, const decoded_instruction_t *decoded) {
    if (decoded->rd < NUM_REGISTERS && decoded->rs1 < NUM_REGISTERS && decoded->rs2 < NUM_REGISTERS) {
        vm->registers[decoded->rd] = (int64_t)vm->registers[decoded->rs1] >> vm->registers[decoded->rs2]; // Arithmetic right shift
    } else {
        fprintf(stderr, "Error: Invalid register index in SRA instruction.\n");
        vm->running = false;
    }
}

void execute_cmp(vm_state_t *vm, const decoded_instruction_t *decoded) {
    if (decoded->rs1 < NUM_REGISTERS && decoded->rs2 < NUM_REGISTERS) {
        // Example: Set flags based on comparison (you'll need to define flags in cpu_state.h)
        if (vm->registers[decoded->rs1] == vm->registers[decoded->rs2]) {
            vm->zero_flag = 1;
        } else {
            vm->zero_flag = 0;
        }
        if ((int64_t)vm->registers[decoded->rs1] < (int64_t)vm->registers[decoded->rs2]) {
            vm->negative_flag = 1;
        } else {
            vm->negative_flag = 0;
        }
        // Implement carry and overflow flags if needed for your CMP instruction
    } else {
        fprintf(stderr, "Error: Invalid register index in CMP instruction.\n");
        vm->running = false;
    }
}

void execute_addi(vm_state_t *vm, const decoded_instruction_t *decoded) {
    if (decoded->rd < NUM_REGISTERS && decoded->rs1 < NUM_REGISTERS) {
        vm->registers[decoded->rd] = vm->registers[decoded->rs1] + decoded->immediate;
    } else {
        fprintf(stderr, "Error: Invalid register index in ADDI instruction.\n");
        vm->running = false;
    }
}

void execute_subi(vm_state_t *vm, const decoded_instruction_t *decoded) {
    if (decoded->rd < NUM_REGISTERS && decoded->rs1 < NUM_REGISTERS) {
        vm->registers[decoded->rd] = vm->registers[decoded->rs1] - decoded->immediate;
    } else {
        fprintf(stderr, "Error: Invalid register index in SUBI instruction.\n");
        vm->running = false;
    }
}

void execute_andi(vm_state_t *vm, const decoded_instruction_t *decoded) {
    if (decoded->rd < NUM_REGISTERS && decoded->rs1 < NUM_REGISTERS) {
        vm->registers[decoded->rd] = vm->registers[decoded->rs1] & decoded->immediate;
    } else {
        fprintf(stderr, "Error: Invalid register index in ANDI instruction.\n");
        vm->running = false;
    }
}

void execute_ori(vm_state_t *vm, const decoded_instruction_t *decoded) {
    if (decoded->rd < NUM_REGISTERS && decoded->rs1 < NUM_REGISTERS) {
        vm->registers[decoded->rd] = vm->registers[decoded->rs1] | decoded->immediate;
    } else {
        fprintf(stderr, "Error: Invalid register index in ORI instruction.\n");
        vm->running = false;
    }
}

void execute_xori(vm_state_t *vm, const decoded_instruction_t *decoded) {
    if (decoded->rd < NUM_REGISTERS && decoded->rs1 < NUM_REGISTERS) {
        vm->registers[decoded->rd] = vm->registers[decoded->rs1] ^ decoded->immediate;
    } else {
        fprintf(stderr, "Error: Invalid register index in XORI instruction.\n");
        vm->running = false;
    }
}

void execute_li(vm_state_t *vm, const decoded_instruction_t *decoded) {
    if (decoded->rd < NUM_REGISTERS) {
        vm->registers[decoded->rd] = decoded->immediate;
    } else {
        fprintf(stderr, "Error: Invalid register index in LI instruction.\n");
        vm->running = false;
    }
}

void execute_load(vm_state_t *vm, const decoded_instruction_t *decoded) {
    if (decoded->rd < NUM_REGISTERS && decoded->address < MEMORY_SIZE - sizeof(reg_t) + 1) {
        vm->registers[decoded->rd] = memory_read_word(decoded->address);
    } else {
        fprintf(stderr, "Error: Invalid register index or memory address in LOAD instruction.\n");
        vm->running = false;
    }
}

void execute_store(vm_state_t *vm, const decoded_instruction_t *decoded) {
    if (decoded->rd < NUM_REGISTERS && decoded->address < MEMORY_SIZE - sizeof(reg_t) + 1) {
        memory_write_word(decoded->address, vm->registers[decoded->rd]);
    } else {
        fprintf(stderr, "Error: Invalid register index or memory address in STORE instruction.\n");
        vm->running = false;
    }
}

void execute_jmp(vm_state_t *vm, const decoded_instruction_t *decoded) {
    vm->program_counter = decoded->address;
}

void execute_jr(vm_state_t *vm, const decoded_instruction_t *decoded) {
    if (decoded->rs1 < NUM_REGISTERS) {
        vm->program_counter = vm->registers[decoded->rs1];
    } else {
        fprintf(stderr, "Error: Invalid register index in JR instruction.\n");
        vm->running = false;
    }
}

void execute_beq(vm_state_t *vm, const decoded_instruction_t *decoded) {
    if (decoded->rs1 < NUM_REGISTERS && decoded->rs2 < NUM_REGISTERS) {
        if (vm->registers[decoded->rs1] == vm->registers[decoded->rs2]) {
            vm->program_counter += decoded->immediate;
        }
    } else {
        fprintf(stderr, "Error: Invalid register index in BEQ instruction.\n");
        vm->running = false;
    }
}

void execute_bne(vm_state_t *vm, const decoded_instruction_t *decoded) {
    if (decoded->rs1 < NUM_REGISTERS && decoded->rs2 < NUM_REGISTERS) {
        if (vm->registers[decoded->rs1] != vm->registers[decoded->rs2]) {
            vm->program_counter += decoded->immediate;
        }
    } else {
        fprintf(stderr, "Error: Invalid register index in BNE instruction.\n");
        vm->running = false;
    }
}

void execute_halt(vm_state_t *vm, const decoded_instruction_t *decoded) {
    printf("VM halted.\n");
    vm->running = false;
}