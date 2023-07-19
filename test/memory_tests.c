#include "memory_tests.h"
#include "relocatable.h"
#include <assert.h>
#include <stdio.h>

void load_data_one_element(void) {
	// Initialize memory
	memory mem = memory_new();
	memory_add_segment(&mem);
	// Initialize data to load
	struct CList *data = CList_init(sizeof(maybe_relocatable));
	felt_t f;
	one(f);
	maybe_relocatable elem = {.felt = &f};
	data->add(data, &elem);
	relocatable ptr = {0, 0};
	// Load data
	relocatable end_ptr = memory_load_data(&mem, ptr, data);
	assert(end_ptr.segment_index == 0 && end_ptr.offset == 1);
	// Check memory
	assert(mem.data->count(mem.data) == 1);
	ResultMemory result = memory_get(&mem, ptr);
	assert(!result.is_error);
	assert(maybe_relocatable_equal(result.value.memory_value, elem));
	printf("OK!\n");
}

void load_data_empty(void) {
	// Initialize memory
	memory mem = memory_new();
	memory_add_segment(&mem);
	// Initialize data to load
	struct CList *data = CList_init(sizeof(maybe_relocatable));
	relocatable ptr = {0, 0};
	// Load data
	relocatable end_ptr = memory_load_data(&mem, ptr, data);
	assert(end_ptr.segment_index == 0 && end_ptr.offset == 1);
	// Check memory
	assert(mem.data->count(mem.data) == 0);
	printf("OK!\n");
}

void memory_tests(void) {
	printf("--------------------------------- \n");
	printf("Test: load_data_empty \n");
	load_data_one_element();
	printf("--------------------------------- \n");
	printf("Test: load_data_one_element \n");
	load_data_one_element();
}
