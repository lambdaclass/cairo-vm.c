#include "vm.h"
#include "memory.h"
#include "run_context.h"

virtual_machine vm_new(void) {
	virtual_machine vm = {{{0, 0}, {0, 0}, {0, 0}}, memory_new()};
	return vm;
}

computed_operands_res compute_operands(virtual_machine vm, Instruction instr) {
	relocatable dst_addr = compute_dst_addr(vm.run_context, instr);
	ResultMemory mem_res = memory_get(&vm.memory, dst_addr);
	if (mem_res.type == Err) {
		computed_operands_res res = {.value = { .error = MemoryError}, .is_error = true};
		return res;
	}

	relocatable op0_addr = compute_op0_addr(vm.run_context, instr);
	ResultMemory op0_op = memory_get(&vm.memory, op0_addr);
	if (op0_op.type == Err) {
		computed_operands_res res = {.value = {.error = MemoryError}, .is_error = true};
		return res;
	}

	relocatable op1_addr = compute_op1_addr(vm.run_context, instr, op0_op.value.memory_value);
	ResultMemory op1_op = memory_get(&vm.memory, op1_addr);

	uint8_t deduced_operands = 0;
}

vm_result run_instruction(virtual_machine vm, Instruction instr) {

	computed_operands_res com_op_res = compute_operands(vm, instr);
	if (com_op_res.is_error) {
		VirtualMachineError err = com_op_res.value.error;
		vm_result res = {.is_ok = false, .error = err};
		return res;
	}

	vm_result res = {
	    .is_ok = true,
	    .error = None,
	};

	return res;
}
