#include "memory_tests.h"

#include "collectc/cc_array.h"
#include "relocatable.h"
#include "utils.h"
#include <assert.h>
#include <stdio.h>

void memory_get_err(void) {
	// Initialize memory
	memory mem = memory_new();
	relocatable ptr = {0, 0};
	ResultMemory result = memory_get(&mem, ptr);
	assert(result.type == Err);
	assert(result.value.error == Get);
	memory_free(&mem);
	printf("OK!\n");
}

void memory_insert_err_unallocated_segement(void) {
	// Initialize memory
	memory mem = memory_new();
	relocatable ptr = {0, 0};
	felt_t f;
	one(f);
	maybe_relocatable elem = {.is_felt = true, .value = {.felt = {f[0], f[1], f[2], f[3]}}};
	ResultMemory result = memory_insert(&mem, ptr, elem);
	assert(result.type == Err);
	assert(result.value.error == Insert);
	memory_free(&mem);
	printf("OK!\n");
}

void memory_insert_err_ovewrite_attempt(void) {
	// Initialize memory
	memory mem = memory_new();
	memory_add_segment(&mem);
	relocatable ptr = {0, 0};
	felt_t felt_one;
	one(felt_one);
	maybe_relocatable elem = maybe_relocatable_from_felt_limbs(felt_one);
	ResultMemory result = memory_insert(&mem, ptr, elem);
	assert(result.type != Err);
	felt_t felt_two;
	from(felt_two, 2);
	maybe_relocatable elem_b = maybe_relocatable_from_felt_limbs(felt_two);
	ResultMemory result_b = memory_insert(&mem, ptr, elem_b);
	assert(result_b.type == Err);
	assert(result_b.value.error == Insert);
	memory_free(&mem);
	printf("OK!\n");
}

void memory_insert_ok_ovewrite_same_value(void) {
	// Initialize memory
	memory mem = memory_new();
	memory_add_segment(&mem);
	relocatable ptr = {0, 0};
	felt_t felt_one;
	one(felt_one);
	maybe_relocatable elem = maybe_relocatable_from_felt_limbs(felt_one);
	ResultMemory result = memory_insert(&mem, ptr, elem);
	assert(result.type != Err);
	maybe_relocatable elem_b = maybe_relocatable_from_felt_limbs(felt_one);
	ResultMemory result_b = memory_insert(&mem, ptr, elem_b);
	assert(result_b.type != Err);
	memory_free(&mem);
	printf("OK!\n");
}

void memory_insert_ok(void) {
	// Initialize memory
	memory mem = memory_new();
	memory_add_segment(&mem);
	relocatable ptr = {0, 0};
	felt_t felt_one;
	one(felt_one);
	maybe_relocatable elem = maybe_relocatable_from_felt_limbs(felt_one);
	ResultMemory result_insert = memory_insert(&mem, ptr, elem);
	assert(result_insert.type != Err);
	assert(result_insert.value.none == 0);
	ResultMemory result_get = memory_get(&mem, ptr);
	assert(result_get.type != Err);
	assert(felt_equal(result_get.value.memory_value.value.felt, felt_one));
	memory_free(&mem);
	printf("OK!\n");
}

void memory_insert_two_ok(void) {
	// Initialize memory
	memory mem = memory_new();
	memory_add_segment(&mem);
	relocatable ptr = {0, 0};
	felt_t felt_one;
	one(felt_one);
	maybe_relocatable elem = maybe_relocatable_from_felt_limbs(felt_one);
	ResultMemory result_insert = memory_insert(&mem, ptr, elem);
	assert(result_insert.type != Err);
	relocatable ptr_b = {0, 1};
	ResultMemory result_insert_b = memory_insert(&mem, ptr_b, elem);
	assert(result_insert_b.type != Err);
	ResultMemory result_get = memory_get(&mem, ptr);
	assert(result_get.type != Err);
	assert(felt_equal(result_get.value.memory_value.value.felt, felt_one));
	ResultMemory result_get_b = memory_get(&mem, ptr_b);
	assert(result_get_b.type != Err);
	assert(felt_equal(result_get_b.value.memory_value.value.felt, felt_one));
	memory_free(&mem);
	printf("OK!\n");
}

void memory_insert_with_gap(void) {
	// Initialize memory
	memory mem = memory_new();
	memory_add_segment(&mem);
	relocatable ptr = {0, 1};
	felt_t felt_one;
	one(felt_one);
	maybe_relocatable elem = maybe_relocatable_from_felt_limbs(felt_one);
	ResultMemory result_insert = memory_insert(&mem, ptr, elem);
	assert(result_insert.type != Err);
	assert(result_insert.value.none == 0);
	ResultMemory result_get = memory_get(&mem, ptr);
	assert(result_get.type != Err);
	assert(felt_equal(result_get.value.memory_value.value.felt, felt_one));
	memory_free(&mem);
	printf("OK!\n");
}

void memory_load_data_one_element(void) {
	// Initialize memory
	memory mem = memory_new();
	memory_add_segment(&mem);
	// Initialize data to load
	CC_Array *data;
	cc_array_new(&data);
	felt_t felt_one;
	one(felt_one);
	maybe_relocatable elem = maybe_relocatable_from_felt_limbs(felt_one);
	cc_array_add(data, &elem);
	relocatable ptr = {0, 0};
	// Load data
	ResultMemory load_result = memory_load_data(&mem, ptr, data);
	assert(load_result.type != Err);
	relocatable end_ptr = load_result.value.ptr;
	assert(end_ptr.segment_index == 0 && end_ptr.offset == 1);
	// Check memory
	ResultMemory result = memory_get(&mem, ptr);

	assert(result.type != Err);
	assert(maybe_relocatable_equal(&result.value.memory_value, &elem));
	memory_free(&mem);
	cc_array_destroy(data);
	printf("OK!\n");
}

void memory_load_data_twice_one_element(void) {
	// Initialize memory
	memory mem = memory_new();
	memory_add_segment(&mem);
	memory_add_segment(&mem);
	// Initialize data to load
	CC_Array *data;
	cc_array_new(&data);
	felt_t felt_one;
	one(felt_one);
	maybe_relocatable elem = maybe_relocatable_from_felt_limbs(felt_one);
	cc_array_add(data, &elem);
	relocatable ptr = {0, 0};
	// Load data 1
	ResultMemory load_result = memory_load_data(&mem, ptr, data);
	assert(load_result.type != Err);
	relocatable end_ptr = load_result.value.ptr;
	assert(end_ptr.segment_index == 0 && end_ptr.offset == 1);
	relocatable ptr_2 = {1, 0};
	// Load data 2
	ResultMemory load_result_2 = memory_load_data(&mem, ptr_2, data);
	assert(load_result_2.type != Err);
	relocatable end_ptr_2 = load_result_2.value.ptr;
	assert(end_ptr_2.segment_index == 1 && end_ptr_2.offset == 1);
	// Check memory
	ResultMemory result = memory_get(&mem, ptr);
	assert(result.type != Err);
	assert(maybe_relocatable_equal(&result.value.memory_value, &elem));
	ResultMemory result_2 = memory_get(&mem, ptr_2);
	assert(result_2.type != Err);
	assert(maybe_relocatable_equal(&result_2.value.memory_value, &elem));
	cc_array_destroy(data);
	memory_free(&mem);
	printf("OK!\n");
}

void memory_load_data_empty(void) {
	// Initialize memory
	memory mem = memory_new();
	memory_add_segment(&mem);
	// Initialize data to load
	CC_Array *data;
	cc_array_new(&data);
	relocatable ptr = {0, 0};
	// Load data
	ResultMemory load_result = memory_load_data(&mem, ptr, data);
	assert(load_result.type != Err);
	relocatable end_ptr = load_result.value.ptr;
	assert(end_ptr.segment_index == 0 && end_ptr.offset == 0);
	// Check memory
	ResultMemory result = memory_get(&mem, ptr);
	assert(result.type == Err);
	cc_array_destroy(data);
	memory_free(&mem);
	printf("OK!\n");
}

void memory_load_data_err_unallocated_segment(void) {
	// Initialize memory
	memory mem = memory_new();
	// Initialize data to load
	CC_Array *data;
	cc_array_new(&data);
	felt_t felt_one;
	one(felt_one);
	maybe_relocatable elem = maybe_relocatable_from_felt_limbs(felt_one);
	cc_array_add(data, &elem);
	relocatable ptr = {0, 0};
	// Load data
	ResultMemory load_result = memory_load_data(&mem, ptr, data);
	assert(load_result.type == Err);
	assert(load_result.value.error == LoadData);
	cc_array_destroy(data);
	memory_free(&mem);
}

void memory_add_segment_ok(void) {
	// Initialize memory
	memory mem = memory_new();
	memory_add_segment(&mem);
	assert(mem.num_segments == 1);
	memory_free(&mem);
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
	memory_insert_ok();
	printf("--------------------------------- \n");
	printf("Test: memory_insert_two_ok \n");
	memory_insert_two_ok();
	printf("--------------------------------- \n");
	printf("Test: memory_insert_with_gap \n");
	memory_insert_with_gap();
	printf("--------------------------------- \n");
	printf("Test: memory_load_data_empty \n");
	memory_load_data_empty();
	printf("--------------------------------- \n");
	printf("Test: memory_load_data_one_element \n");
	memory_load_data_one_element();
	printf("--------------------------------- \n");
	printf("Test: memory_load_data_twice_one_element \n");
	memory_load_data_twice_one_element();
	printf("--------------------------------- \n");
	printf("Test: memory_load_data_err_unallocated_segment \n");
	memory_load_data_err_unallocated_segment();
	printf("--------------------------------- \n");
	printf("Test: memory_add_segment_ok \n");
	memory_add_segment_ok();
}
