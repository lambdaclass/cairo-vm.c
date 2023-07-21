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
	printf("OK!\n");
}

void memory_insert_err_unallocated_segement(void) {
	// Initialize memory
	memory mem = memory_new();
	relocatable ptr = {0, 0};
	felt_t f;
	one(f);
	maybe_relocatable elem = {.is_felt = true, .value = {.felt = &f}};
	ResultMemory result = memory_insert(&mem, ptr, elem);
	assert(result.is_error);
	assert(result.value.error == Insert);
	printf("OK!\n");
}

void memory_insert_err_ovewrite_attempt(void) {
	// Initialize memory
	memory mem = memory_new();
	memory_add_segment(&mem);
	relocatable ptr = {0, 0};
	felt_t f;
	one(f);
	maybe_relocatable elem = {.is_felt = true, .value = {.felt = &f}};
	ResultMemory result = memory_insert(&mem, ptr, elem);
	assert(!result.is_error);
	felt_t t;
	from(f, 2);
	maybe_relocatable elem_b = {.is_felt = true, .value = {.felt = &t}};
	ResultMemory result_b = memory_insert(&mem, ptr, elem_b);
	assert(result_b.is_error);
	assert(result_b.value.error == Insert);
	printf("OK!\n");
}

void memory_insert_ok_ovewrite_same_value(void) {
	// Initialize memory
	memory mem = memory_new();
	memory_add_segment(&mem);
	relocatable ptr = {0, 0};
	felt_t f;
	one(f);
	maybe_relocatable elem = {.is_felt = true, .value = {.felt = &f}};
	ResultMemory result = memory_insert(&mem, ptr, elem);
	assert(!result.is_error);
	maybe_relocatable elem_b = {.is_felt = true, .value = {.felt = &f}};
	ResultMemory result_b = memory_insert(&mem, ptr, elem_b);
	assert(!result_b.is_error);
	printf("OK!\n");
}

void memory_insert_ok(void) {
	// Initialize memory
	memory mem = memory_new();
	memory_add_segment(&mem);
	relocatable ptr = {0, 0};
	felt_t f;
	one(f);
	maybe_relocatable elem = {.is_felt = true, .value = {.felt = &f}};
	ResultMemory result_insert = memory_insert(&mem, ptr, elem);
	assert(!result_insert.is_error);
	assert(result_insert.value.none == 0);
	ResultMemory result_get = memory_get(&mem, ptr);
	assert(!result_get.is_error);
	felt_t expected_value;
	one(expected_value);
	assert(felt_equal(result_get.value.memory_value.value.felt, &expected_value));
	printf("OK!\n");
}

void memory_load_data_one_element(void) {
	// Initialize memory
	memory mem = memory_new();
	memory_add_segment(&mem);
	// Initialize data to load
	struct CList *data = CList_init(sizeof(maybe_relocatable));
	felt_t f;
	one(f);
	maybe_relocatable elem = {.is_felt = true, .value = {.felt = &f}};
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
	printf("Test: memory_insert_err_ovewrite_attempt \n");
	memory_insert_err_ovewrite_attempt();
	printf("--------------------------------- \n");
	printf("Test: memory_insert_ok_ovewrite_same_value \n");
	memory_insert_ok_ovewrite_same_value();
	printf("--------------------------------- \n");
	printf("Test: memory_insert_ok \n");
	memory_get_err();
	printf("--------------------------------- \n");
	printf("Test: memory_load_data_empty \n");
	memory_load_data_empty();
	printf("--------------------------------- \n");
	printf("Test: memory_load_data_one_element \n");
	memory_load_data_one_element();
}
