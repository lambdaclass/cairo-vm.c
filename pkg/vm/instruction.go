package vm

import (
	"errors"
)

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
type Instruction struct {
	offDst   int
	offOp0   int
	offOp1   int
	dstReg   Register
	op0Reg   Register
	op1Addr  Op1Src
	resLogic ResLogic
	pcUpdate PcUpdate
	apUpdate ApUpdate
	fpUpdate FpUpdate
	opcode   Opcode
}

// x-----------------------------x
// x----- Instruction flags -----x
// x-----------------------------x

type Register uint

const (
	AP Register = 0
	FP Register = 1
)

type Op1Src uint

const (
	Op1SrcImm Op1Src = 0
	Op1SrcAP  Op1Src = 1
	Op1SrcFP  Op1Src = 2
	Op1SrcOp0 Op1Src = 4
)

type ResLogic uint

const (
	ResOp1           ResLogic = 0
	ResAdd           ResLogic = 1
	ResMul           ResLogic = 2
	ResUnconstrained ResLogic = 3
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
	Ret      Opcode = 4
)

var NonZeroHighBitError = errors.New("Instruction high bit was not set to zero")
var InvalidOp1RegError = errors.New("Instruction had invalid Op1 Register")
var InvalidPcUpdateError = errors.New("Instruction had invalid Pc update")
var InvalidResError = errors.New("Instruction had an invalid res")
var InvalidOpcodeError = errors.New("Instruction had an invalid opcode")
var InvalidApUpdateError = errors.New("Instruction had an invalid Ap Update")

func decodeInstruction(encodedInstruction uint64) (Instruction, error) {
	const HighBit uint64 = 1 << 63
	const DstRegMask uint64 = 0x0001
	const DstRegOff uint64 = 0
	const Op0RegMask uint64 = 0x0002
	const Op0RegOff uint64 = 1
	const Op1SrcMask uint64 = 0x001C
	const Op1SrcOff uint64 = 2
	const ResLogicMask uint64 = 0x0060
	const ResLogicOff uint64 = 5
	const PcUpdateMask uint64 = 0x0380
	const PcUpdateOff uint64 = 7
	const ApUpdateMask uint64 = 0x0C00
	const ApUpdateOff uint64 = 10
	const OpcodeMask uint64 = 0x7000
	const OpcodeOff uint64 = 12

	if encodedInstruction&HighBit != 0 {
		return Instruction{}, NonZeroHighBitError
	}

	var offset0 = fromBiasedRepresentation(uint16((encodedInstruction)))
	var offset1 = fromBiasedRepresentation(uint16((encodedInstruction) >> 16))
	var offset2 = fromBiasedRepresentation(uint16((encodedInstruction) >> 32))

	var flags = encodedInstruction >> 48

	var dstRegNum = (flags & DstRegMask) >> DstRegOff
	var op0RegNum = (flags & Op0RegMask) >> Op0RegOff
	var op1SrcNum = (flags & Op1SrcMask) >> Op1SrcOff
	var resLogicNum = (flags & ResLogicMask) >> ResLogicOff
	var pcUpdateNum = (flags & PcUpdateMask) >> PcUpdateOff
	var apUpdateNum = (flags & ApUpdateMask) >> ApUpdateOff
	var opCodeNum = (flags & OpcodeMask) >> OpcodeOff

	var dstRegister Register
	var op0Register Register
	var op1Src Op1Src
	var pcUpdate PcUpdate
	var res ResLogic
	var opcode Opcode
	var apUpdate ApUpdate
	var fpUpdate FpUpdate

	if dstRegNum == 1 {
		dstRegister = FP
	} else {
		dstRegister = AP
	}

	if op0RegNum == 1 {
		op0Register = FP
	} else {
		op0Register = AP
	}

	switch op1SrcNum {
	case 0:
		op1Src = Op1SrcOp0
	case 1:
		op1Src = Op1SrcImm
	case 2:
		op1Src = Op1SrcFP
	case 3:
		op1Src = Op1SrcAP
	default:
		return Instruction{}, InvalidOp1RegError
	}

	switch pcUpdateNum {
	case 0:
		pcUpdate = PcUpdateRegular
	case 1:
		pcUpdate = PcUpdateJump
	case 2:
		pcUpdate = PcUpdateJumpRel
	case 3:
		pcUpdate = PcUpdateJnz
	default:
		return Instruction{}, InvalidPcUpdateError
	}

	switch resLogicNum {
	case 0:
		if pcUpdate == PcUpdateJnz {
			res = ResUnconstrained
		} else {
			res = ResOp1
		}
	case 1:
		res = ResAdd
	case 2:
		res = ResMul
	default:
		return Instruction{}, InvalidResError
	}

	switch opCodeNum {
	case 0:
		opcode = NOp
	case 1:
		opcode = Call
	case 2:
		opcode = Ret
	case 4:
		opcode = AssertEq
	default:
		return Instruction{}, InvalidOpcodeError
	}

	switch apUpdateNum {
	case 0:
		if opcode == Call {
			apUpdate = ApUpdateAdd2
		} else {
			apUpdate = ApUpdateRegular
		}
	case 1:
		apUpdate = ApUpdateAdd
	case 2:
		apUpdate = ApUpdateAdd1
	default:
		return Instruction{}, InvalidApUpdateError
	}

	switch opcode {
	case Call:
		fpUpdate = FpUpdateAPPlus2
	case Ret:
		fpUpdate = FpUpdateDst
	default:
		fpUpdate = FpUpdateRegular
	}

	return Instruction{
		offDst:   offset0,
		offOp0:   offset1,
		offOp1:   offset2,
		dstReg:   dstRegister,
		op0Reg:   op0Register,
		op1Addr:  op1Src,
		resLogic: res,
		pcUpdate: pcUpdate,
		apUpdate: apUpdate,
		fpUpdate: fpUpdate,
		opcode:   opcode,
	}, nil
}

func fromBiasedRepresentation(offset uint16) int {
	var bias uint16 = 1 << 15
	return int(offset - bias)
}
