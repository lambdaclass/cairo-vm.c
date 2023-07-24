#include "vm.h"

virtual_machine vm_new(void) {
	virtual_machine vm = {{{0, 0}, {0, 0}, {0, 0}}, memory_new()};
	return vm;
}

vm_result run_instruction(Instruction instr) {

	int64_t e = instr.off0;

	vm_result res = {
	    .is_ok = true,
	    .error = None,
	};

	if (e == 1)
		return res;
	return res;
}
