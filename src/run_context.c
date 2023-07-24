#include "run_context.h"

relocatable compute_dst_addr(run_context run_ctx, Instruction instr) {
	relocatable base_addr;
	switch (instr.dst_register) {
	case REG_AP: {
		base_addr = {.segment_index = 1, .offset = run_ctx.ap};
	} break;
	default: {
		base_addr = {.segment_index = 1, .offset = run_ctx.fp};
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
