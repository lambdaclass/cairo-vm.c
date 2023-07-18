#include "vm.h"
#include "memory.h"

virtual_machine vm_new(void) {
	virtual_machine vm = {{{0, 0}, {0, 0}, {0, 0}}, memory_new()};
	return vm;
}
