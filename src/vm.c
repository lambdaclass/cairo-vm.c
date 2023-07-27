#include "vm.h"

virtual_machine vm_new(void) {
	virtual_machine vm = {{{0, 0}, {0, 0}, {0, 0}}, memory_new()};
	return vm;
}

maybe_relocatable compute_res(Instruction instr, maybe_relocatable op0, maybe_relocatable op1) {
	maybe_relocatable res;
	switch (instr.res) {
	case Op1:
		res = op1;
		break;
	case Res_Add:
		res = add_maybe_relocatable(op0, op1);
		break;
	case Mul:
		if (op0.is_felt && op1.is_felt) {
			felt_t a = {op0.value.felt[0], op0.value.felt[1], op0.value.felt[2], op0.value.felt[3]};
			felt_t b = {op1.value.felt[0], op1.value.felt[1], op1.value.felt[2], op1.value.felt[3]};
			felt_t result;
			mul(a, b, result);
			res = (maybe_relocatable){.is_felt = true,
			                          .value = {.felt = {result[0], result[1], result[2], result[3]}}};
		} else {
			res = (maybe_relocatable){.is_felt = false, .value = {.relocatable = {0, 0}}};
		}
		break;
	default:
		// TODO: in rust this is a none, we should check what to do in this cases
		res = (maybe_relocatable){.is_felt = false, .value = {.relocatable = {0, 0}}};
		break;
	}
	return res;
}

computed_operands_res compute_operands(virtual_machine vm, Instruction instr) {
	relocatable dst_addr = compute_dst_addr(vm.run_context, instr);
	ResultMemory dst_op = memory_get(&vm.memory, dst_addr);
	if (dst_op.type == Err) {
		computed_operands_res res = {.value = {.error = MemoryError}, .is_error = true};
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

	felt_t value;
	from(value, 0);
	// maybe_relocatable res = {.is_felt = false, .value = {.felt = {value[0], value[1], value[2],
	// value[3]}}};
	uint8_t deduced_operands = 0;

	if (op0_op.type == Err) {
		computed_operands_res res = {.value = {.error = MemoryError}, .is_error = true};
		return res;
	}

	// for now this is always pre computed. we should handle the case when it is not
	maybe_relocatable op0 = op0_op.value.memory_value;
	maybe_relocatable op1 = op1_op.value.memory_value;

	// compute res
	maybe_relocatable res = compute_res(instr, op0, op1);

	// compute dst
	maybe_relocatable dst = dst_op.value.memory_value;

	operands_addresses accessed_addresses = {
	    .dst_addr = {.offset = dst_addr.offset, .segment_index = dst_addr.segment_index},
	    .op0_addr = {.offset = op0_addr.offset, .segment_index = op0_addr.segment_index},
	    .op1_addr = {.offset = op1_addr.offset, .segment_index = op1_addr.segment_index},
	};

	operands operands = {.dst = dst, .op0 = op0, .op1 = op1, .res = res};
	computed_operands_res result = {
	    .is_error = false,
	    .value = {.ops = {.oprs = operands, .op_addrs = accessed_addresses, .deduced_operands = deduced_operands}}};

	return result;
}
// TODO: uncomment once all functions are done
// vm_result run_instruction(virtual_machine vm, Instruction instr) {

// 	computed_operands_res com_op_res = compute_operands(vm, instr);
// 	if (com_op_res.is_error) {
// 		VirtualMachineError err = com_op_res.value.error;
// 		vm_result res = {.is_ok = false, .error = err};
// 		return res;
// 	}

// 	vm_result res = {
// 	    .is_ok = true,
// 	    .error = None,
// 	};

// 	return res;
// }
