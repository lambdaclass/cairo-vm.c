#include "run_context.h"
#include "instruction.h"

relocatable compute_dst_addr(run_context run_ctx, Instruction instr) {
	struct relocatable base_addr;
	switch (instr.dst_register) {
	case REG_AP: {
		base_addr = run_ctx.ap;
	} break;
	default: {
		base_addr = run_ctx.fp;
	} break;
	}

	relocatable res_base_addr;
	if (instr.off0 < 0) {
		res_base_addr = sub_relocatable(base_addr, (uint64_t)absolute(instr.off0));
	} else {
		res_base_addr = add_relocatable(base_addr, (uint64_t)instr.off0);
	}

	return res_base_addr;
}

relocatable compute_op0_addr(run_context run_ctx, Instruction instr) {
	struct relocatable base_addr;
	switch (instr.op0_register) {
	case REG_AP: {
		base_addr = run_ctx.ap;
	} break;
	default: {
		base_addr = run_ctx.fp;
	} break;
	}

	relocatable res_base_addr;
	if (instr.off0 < 0) {
		res_base_addr = sub_relocatable(base_addr, (uint64_t)absolute(instr.off1));
	} else {
		res_base_addr = add_relocatable(base_addr, (uint64_t)instr.off1);
	}
	return res_base_addr;
}

relocatable compute_op1_addr(run_context run_ctx, Instruction instr, maybe_relocatable op0) {
	relocatable base_addr;
	switch (instr.op1_addr) {
	case OP_FP:
		base_addr = run_ctx.fp;
		break;
	case OP_AP:
		base_addr = run_ctx.ap;
		break;
	case Imm:
		if (instr.off2 == 1)
			base_addr = run_ctx.pc;
		break;
	case Op0: {
		if (!op0.is_felt) {
			base_addr = op0.value.relocatable;
		}
	} break;
	default: {
		base_addr = {.segment_index = 0, .offset = 0};
	} break;
	}

	relocatable res_base_addr;
	if (instr.off2 < 0) {
		res_base_addr = sub_relocatable(base_addr, (uint64_t)absolute(instr.off2));
	} else {
		res_base_addr = add_relocatable(base_addr, (uint64_t)instr.off2);
	}
	return res_base_addr;
}
