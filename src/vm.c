#include "vm.h"

virtual_machine vm_new(void) {
	virtual_machine vm = {{{0, 0}, {0, 0}, {0, 0}}, memory_new()};
	return vm;
}

vm_result run_instruction(Instruction instr) {

	vm_result res = {
	    .is_ok = true,
	    .error = None,
	};

	return res;
}
