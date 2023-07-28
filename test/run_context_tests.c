#include "run_context_tests.h"

void compute_dst_addr_test(void) {
	printf("Test: Compute dst address test\n");
	relocatable pc = {.segment_index = 0, .offset = 0};
	relocatable ap = {.segment_index = 5, .offset = 5};
	relocatable fp = {.segment_index = 10, .offset = 10};
	run_context ctx = {.pc = pc, .ap = ap, .fp = fp};
	Instruction instr = {.off0 = 0,
	                     .off1 = 1,
	                     .off2 = 2,
	                     .dst_register = REG_AP,
	                     .op0_register = REG_AP,
	                     .op1_addr = OP_AP,
	                     .res = Res_Add,
	                     .pc_update = PC_Regular,
	                     .ap_update = AP_Regular,
	                     .fp_update = FP_Regular,
	                     .opcode = NOp};

	relocatable result = compute_dst_addr(ctx, instr);
	assert(result.segment_index == ap.segment_index);
	assert(result.offset == 0);
	instr.off0 = -1;
	result = compute_dst_addr(ctx, instr);
	assert(result.offset == 4);
	instr.dst_register = REG_FP;
	result = compute_dst_addr(ctx, instr);
	assert(result.segment_index == fp.segment_index);
	assert(result.offset == 9);
	instr.off0 = 2;
	result = compute_dst_addr(ctx, instr);
	assert(result.offset == 12);

	printf("Ok!\n");
}

void compute_op0_addr_test(void) {
	printf("Test: Compute op0 address test\n");
	relocatable pc = {.segment_index = 0, .offset = 0};
	relocatable ap = {.segment_index = 5, .offset = 5};
	relocatable fp = {.segment_index = 10, .offset = 10};
	run_context ctx = {.pc = pc, .ap = ap, .fp = fp};
	Instruction instr = {.off0 = 0,
	                     .off1 = 1,
	                     .off2 = 2,
	                     .dst_register = REG_AP,
	                     .op0_register = REG_AP,
	                     .op1_addr = OP_AP,
	                     .res = Res_Add,
	                     .pc_update = PC_Regular,
	                     .ap_update = AP_Regular,
	                     .fp_update = FP_Regular,
	                     .opcode = NOp};

	relocatable result = compute_op0_addr(ctx, instr);
	assert(result.segment_index == ap.segment_index);
	assert(result.offset == 6);
	instr.off1 = -1;
	result = compute_op0_addr(ctx, instr);
	assert(result.offset == 4);
	instr.op0_register = REG_FP;
	result = compute_op0_addr(ctx, instr);
	assert(result.segment_index == fp.segment_index);
	assert(result.offset == 9);
	instr.off1 = 2;
	result = compute_op0_addr(ctx, instr);
	assert(result.offset == 12);

	printf("Ok!\n");
}

// void compute_op1_addr_test(void) {
// 	printf("Compute op1 address test\n");
// 	relocatable pc = {.segment_index = 0, .offset = 0};
// 	relocatable ap = {.segment_index = 5, .offset = 5};
// 	relocatable fp = {.segment_index = 10, .offset = 10};
// 	run_context ctx = {.pc = pc, .ap = ap, .fp = fp};
// 	Instruction instr = {.off0 = 0,
// 	                     .off1 = 1,
// 	                     .off2 = 2,
// 	                     .dst_register = REG_AP,
// 	                     .op0_register = REG_AP,
// 	                     .op1_addr = OP_AP,
// 	                     .res = Res_Add,
// 	                     .pc_update = PC_Regular,
// 	                     .ap_update = AP_Regular,
// 	                     .fp_update = FP_Regular,
// 	                     .opcode = NOp};
// 	relocatable aux = {.segment_index = 15, .offset = 15};
// 	maybe_relocatable op0 = {.is_felt = false, .value = {.relocatable = aux}};
// 	relocatable result = compute_op1_addr(ctx, instr, op0);
// 	assert(result.segment_index == ap.segment_index);
// 	assert(result.offset == 7);
// 	instr.off1 = -1;
// 	result = compute_op1_addr(ctx, instr);
// 	assert(result.offset == 4);
// 	instr.op0_register = REG_FP;
// 	result = compute_op1_addr(ctx, instr);
// 	assert(result.segment_index == fp.segment_index);
// 	assert(result.offset == 9);
// 	instr.off1 = 2;
// 	result = compute_op1_addr(ctx, instr);
// 	assert(result.offset == 12);

// 	printf("Ok!\n");
// }

void run_context_test(void) {
	printf("--------------------------------- \n");
	printf("Run context tests\n");
	printf("---------------------------------\n");
	compute_dst_addr_test();
	printf("---------------------------------\n");
	compute_op0_addr_test();
	// compute_op1_addr_test();
}
