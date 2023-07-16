#ifndef INS_H
#define INS_H
#include <stdbool.h>

//  Structure of the 63-bit that form the first word of each instruction.
//  See Cairo whitepaper, page 32 - https://eprint.iacr.org/2021/1063.pdf.
// ┌─────────────────────────────────────────────────────────────────────────┐
// │                     off_dst (biased representation)                     │
// ├─────────────────────────────────────────────────────────────────────────┤
// │                     off_op0 (biased representation)                     │
// ├─────────────────────────────────────────────────────────────────────────┤
// │                     off_op1 (biased representation)                     │
// ├─────┬─────┬───────┬───────┬───────────┬────────┬───────────────────┬────┤
// │ dst │ op0 │  op1  │  res  │    pc     │   ap   │      opcode       │ 0  │
// │ reg │ reg │  src  │ logic │  update   │ update │                   │    │
// ├─────┼─────┼───┬───┼───┬───┼───┬───┬───┼───┬────┼────┬────┬────┬────┼────┤
// │  0  │  1  │ 2 │ 3 │ 4 │ 5 │ 6 │ 7 │ 8 │ 9 │ 10 │ 11 │ 12 │ 13 │ 14 │ 15 │
// └─────┴─────┴───┴───┴───┴───┴───┴───┴───┴───┴────┴────┴────┴────┴────┴────┘

// Instruction is the representation of the first word of each Cairo instruction.
// Some instructions spread over two words when they use an immediate value, so
// representing the first one with this struct is enougth.

// ---------------------
//  Typedef definitions
// ---------------------

typedef enum Res Res;
typedef enum Opcode Opcode;
typedef enum Op1Addr Op1Addr;
typedef enum Register Register;
typedef enum PcUpdate PcUpdate;
typedef enum ApUpdate ApUpdate;
typedef enum FpUpdate FpUpdate;
typedef struct Instruction Instruction;
typedef struct ResultInstruction ResultInstruction;
typedef enum VirtualMachineError VirtualMachineError;
typedef union ResultInstructionValue ResultInstructionValue;

// ---------------------
//     structures
// ---------------------

enum Register { REG_AP, REG_FP };

enum Op1Addr { Imm, OP_AP, OP_FP, Op0 };

enum Res {
	Op1,
	Res_Add,
	Mul,
	Unconstrained,
};

enum PcUpdate {
	PC_Regular,
	Jump,
	JumpRel,
	Jnz,
};

enum ApUpdate {
	AP_Regular,
	Add,
	Add1,
	Add2,
};

enum FpUpdate {
	Regular,
	APPlus2,
	Dst,
};

enum Opcode {
	NOp,
	AssertEq,
	Call,
	Ret,
};

struct Instruction {
	int off0;
	int off1;
	int off2;
	Register dst_reint;
	Register op0_register;
	Op1Addr op1_addr;
	Res res;
	PcUpdate pc_update;
	ApUpdate ap_update;
	FpUpdate fp_update;
	Opcode opcode;
};
// -----------------
// 	Error handling
// -----------------

enum VirtualMachineError {
	InstructionNonZeroHighBit,
};

union ResultInstructionValue {
	VirtualMachineError error;
	Instruction instruction;
};

struct ResultInstruction {
	bool is_error;
	ResultInstructionValue value;
};

// ------------------
// 	  Functions
// ------------------

unsigned int instruction_size(Instruction ins);

#endif
