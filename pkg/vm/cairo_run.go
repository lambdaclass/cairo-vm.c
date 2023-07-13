package vm

type CairoRunConfig struct {
	Entrypoint   string
	TraceEnabled bool
	RelocateMem  bool
	Layout       string
	ProofMode    bool
	SecureRun    bool
}

func CairoRun(programContent []byte, cairoRunConfig *CairoRunConfig) (*CairoRunner, *VirtualMachine, error) {
	program, err := ProgramFromBytes(programContent, &cairoRunConfig.Entrypoint)
	if err != nil {
		return nil, nil, err
	}

	secureRun := cairoRunConfig.SecureRun
	if secureRun == nil {
		secureRun = new(bool)
		*secureRun = !cairoRunConfig.ProofMode
	}

	cairoRunner, err := NewCairoRunner(program)
	if err != nil {
		return nil, nil, err
	}

	vm := NewVirtualMachine(cairoRunConfig.TraceEnabled)
	end, err := cairoRunner.Initialize(vm)
	if err != nil {
		return nil, nil, err
	}

	err = cairoRunner.RunUntilPC(end, vm, hintExecutor)
	if err != nil {
		return nil, nil, err
	}

	err = cairoRunner.EndRun(false, false, vm, hintExecutor)
	if err != nil {
		return nil, nil, err
	}

	err = vm.VerifyAutoDeductions()
	if err != nil {
		return nil, nil, err
	}

	err = cairoRunner.ReadReturnValues(vm)
	if err != nil {
		return nil, nil, err
	}

	if cairoRunConfig.ProofMode {
		err = cairoRunner.FinalizeSegments(vm)
		if err != nil {
			return nil, nil, err
		}
	}

	if *secureRun {
		err = VerifySecureRunner(cairoRunner, true, nil, vm)
		if err != nil {
			return nil, nil, err
		}
	}

	err = cairoRunner.Relocate(vm, cairoRunConfig.RelocateMem)
	if err != nil {
		return nil, nil, err
	}

	return cairoRunner, vm, nil
}
