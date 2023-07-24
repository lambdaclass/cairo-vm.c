#ifndef INS_H
#define INS_H
#include <stdbool.h>
#include <stdint.h>

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
	FP_Regular,
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
	int64_t off0;
	int64_t off1;
	int64_t off2;
	enum Register dst_register;
	enum Register op0_register;
	enum Op1Addr op1_addr;
	enum Res res;
	enum PcUpdate pc_update;
	enum ApUpdate ap_update;
	enum FpUpdate fp_update;
	enum Opcode opcode;
};
// -----------------
// 	Error handling
// -----------------

enum VirtualMachineError {
	InstructionNonZeroHighBit,
	InvalidOp1Reg,
	InvalidPcUpdate,
	InvalidRes,
	InvalidOpcode,
	InvalidApUpdate,
};

union ResultInstructionValue {
	enum VirtualMachineError error;
	struct Instruction instruction;
};

struct ResultInstruction {
	bool is_error;
	union ResultInstructionValue value;
};

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

uint64_t instruction_size(Instruction instruction);

#endif
