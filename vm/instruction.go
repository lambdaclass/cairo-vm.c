package vm

type Instruction struct {
	off0      int
	off1      int
	off2      int
	dst_reg   Register
	op0_reg   Register
	op1_addr  Op1Addr
	res       Res
	pc_update PcUpdate
	ap_update ApUpdate
	fp_update FpUpdate
	opcode    Opcode
}

type Register uint

const (
	AP Register = 0
	FP Register = 1
)

type Op1Addr uint

const (
	Op1AddrImm Op1Addr = 0
	Op1AddrAP  Op1Addr = 1
	Op1AddrFP  Op1Addr = 2
	Op1AddrOp0 Op1Addr = 3
)

type Res uint

const (
	ResOp1           Res = 0
	ResAdd           Res = 1
	ResMul           Res = 2
	ResUnconstrained Res = 3
)

type PcUpdate uint

const (
	PcUpdateRegular PcUpdate = 0
	PcUpdateJump    PcUpdate = 1
	PcUpdateJumpRel PcUpdate = 2
	PcUpdateJnz     PcUpdate = 3
)

type ApUpdate uint

const (
	ApUpdateRegular ApUpdate = 0
	ApUpdateAdd     ApUpdate = 1
	ApUpdateAdd1    ApUpdate = 2
	ApUpdateAdd2    ApUpdate = 3
)

type FpUpdate uint

const (
	FpUpdateRegular FpUpdate = 0
	FpUpdateAPPlus2 FpUpdate = 1
	FpUpdateDst     FpUpdate = 2
)

type Opcode uint

const (
	NOp      Opcode = 0
	AssertEq Opcode = 1
	Call     Opcode = 2
	Ret      Opcode = 3
)
