#include "memory_tests.h"
#include "relocatable.h"
#include <assert.h>
#include <stdio.h>

void memory_get_err(void) {
	// Initialize memory
	memory mem = memory_new();
	relocatable ptr = {0, 0};
	ResultMemory result = memory_get(&mem, ptr);
	assert(result.is_error);
	assert(result.value.error == Get);
}

void memory_insert_err_unallocated_segement(void) {
	// Initialize memory
	memory mem = memory_new();
	relocatable ptr = {0, 0};
	maybe_relocatable elem = {.is_felt = true, .value = {.felt = 1}};
	ResultMemory result = memory_insert(&mem, ptr, elem);
	assert(result.is_error);
	assert(result.value.error == Insert);
}

void memory_load_data_one_element(void) {
	// Initialize memory
	memory mem = memory_new();
	memory_add_segment(&mem);
	// Initialize data to load
	struct CList *data = CList_init(sizeof(maybe_relocatable));
	maybe_relocatable elem = {.is_felt = true, .value = {.felt = 1}};
	data->add(data, &elem);
	relocatable ptr = {0, 0};
	// Load data
	relocatable end_ptr = memory_load_data(&mem, ptr, data);
	assert(end_ptr.segment_index == 0 && end_ptr.offset == 1);
	// Check memory
	ResultMemory result = memory_get(&mem, ptr);
	assert(!result.is_error);
	assert(maybe_relocatable_equal(result.value.memory_value, elem));
	printf("OK!\n");
}

void memory_load_data_empty(void) {
	// Initialize memory
	memory mem = memory_new();
	memory_add_segment(&mem);
	// Initialize data to load
	struct CList *data = CList_init(sizeof(maybe_relocatable));
	relocatable ptr = {0, 0};
	// Load data
	relocatable end_ptr = memory_load_data(&mem, ptr, data);
	assert(end_ptr.segment_index == 0 && end_ptr.offset == 0);
	// Check memory
	ResultMemory result = memory_get(&mem, ptr);
	assert(result.is_error);
	printf("OK!\n");
}

void memory_tests(void) {
	printf("--------------------------------- \n");
	printf("Test: memory_get_err \n");
	memory_get_err();
	printf("--------------------------------- \n");
	printf("Test: memory_insert_err_unallocated_segement \n");
	memory_insert_err_unallocated_segement();
	printf("--------------------------------- \n");
	printf("Test: memory_load_data_empty \n");
	memory_load_data_empty();
	printf("--------------------------------- \n");
	printf("Test: memory_load_data_one_element \n");
	memory_load_data_one_element();
}
