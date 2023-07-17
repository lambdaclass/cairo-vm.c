.PHONY: clean fmt check_fmt valgrind compile-rust

TARGET=cairo_vm
TEST_TARGET=cairo_vm_test

CC=cc
SANITIZER_FLAGS=-fsanitize=address -fno-omit-frame-pointer
CFLAGS=-std=c11 -Wall -Wextra -Werror -pedantic -g -O0
CFLAGS_TEST=-I./src
LN_FLAGS=-L./lambdaworks/lib/lambdaworks/target/release/ -llambdaworks

BUILD_DIR=./build
SRC_DIR=./src
TEST_DIR=./test

SOURCE = $(wildcard $(SRC_DIR)/*.c)
TEST_SOURCE = $(wildcard $(TEST_DIR)/*.c) $(wildcard $(SRC_DIR)/*.c)
TEST_SOURCE := $(filter-out %main.c, $(TEST_SOURCE))

HEADERS = $(wildcard $(SRC_DIR)/*.h)
TEST_HEADERS = $(wildcard $(TEST_DIR)/*.h)
OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SOURCE))
TEST_OBJECTS = $(patsubst $(TEST_DIR)/%.c, $(BUILD_DIR)/%.o, $(TEST_SOURCE))

# Gcc/Clang will create these .d files containing dependencies.
DEP = $(OBJECTS:%.o=%.d)

default: $(TARGET)

$(TARGET): $(BUILD_DIR)/$(TARGET)

$(BUILD_DIR)/$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(SANITIZER_FLAGS) $(LN_FLAGS) $^ -o $@

$(TEST_TARGET): $(BUILD_DIR)/$(TEST_TARGET)

$(BUILD_DIR)/$(TEST_TARGET): $(TEST_OBJECTS)
	$(CC) $(CFLAGS) $(CFLAGS_TEST) $(SANITIZER_FLAGS) $(LN_FLAGS) $^ -o $@

-include $(DEP)

# The potential dependency on header files is covered
# by calling `-include $(DEP)`.
# The -MMD flags additionaly creates a .d file with
# the same name as the .o file.
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(SANITIZER_FLAGS) -MMD -c $< -o $@

$(BUILD_DIR)/%.o: $(TEST_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(CFLAGS_TEST) $(SANITIZER_FLAGS) -MMD -c $< -o $@

deps-macos:
	brew install clang-format

run: $(TARGET)
	$(BUILD_DIR)/$(TARGET) 

test: $(TEST_TARGET)
	$(BUILD_DIR)/$(TEST_TARGET)

clean:
	rm -rf $(BUILD_DIR) && \
	cd lambdaworks/lib/lambdaworks && cargo clean

fmt:
	clang-format --style=file -i $(SOURCE) $(TEST_SOURCE) $(HEADERS) $(TEST_HEADERS)

check_fmt:
	clang-format --style=file -Werror -n $(SOURCE) $(TEST_SOURCE) $(HEADERS) $(TEST_HEADERS)

valgrind: clean compile-rust test
	valgrind --leak-check=full --show-reachable=yes --show-leak-kinds=all --track-origins=yes --error-exitcode=1 ./build/cairo_vm_test

compile-rust: 
	cd lambdaworks/lib/lambdaworks && cargo build --release
