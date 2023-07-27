.PHONY: clean fmt check_fmt valgrind compile_rust deps_macos docker_build docker_run docker_test_and_format docker_clean

TARGET=cairo_vm
TEST_TARGET=cairo_vm_test

CC=cc
CXX=g++
SANITIZER_FLAGS=-fsanitize=address -fno-omit-frame-pointer
CFLAGS=-std=c11 -Wall -Wextra -Wimplicit-fallthrough -Werror -pedantic -g -O0
CXX_FLAGS=-std=c++14 -Wall -Wextra -Wimplicit-fallthrough -Werror -pedantic -g -O0
CFLAGS_TEST=-I./src
LN_FLAGS=-L./lambdaworks/lib/lambdaworks/target/release/ -Bstatic -llambdaworks -ldl -lpthread -lm

BUILD_DIR=./build
SRC_DIR=./src
TEST_DIR=./test
LIB_DIR=./lib

SOURCE = $(wildcard $(SRC_DIR)/*.c)
SOURCE := $(filter-out %main.c, $(SOURCE))
SOURCE_CPP = $(wildcard $(SRC_DIR)/*.cpp)
LIB_SOURCE_CPP = $(wildcard $(LIB_DIR)/*.cpp)
TEST_SOURCE = $(wildcard $(TEST_DIR)/*.c)
TEST_SOURCE_CPP = $(wildcard $(TEST_DIR)/*.cpp) $(wildcard $(SRC_DIR)/*.cpp)

HEADERS = $(wildcard $(SRC_DIR)/*.h)
TEST_HEADERS = $(wildcard $(TEST_DIR)/*.h)
OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SOURCE))
OBJECTS_CPP = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SOURCE_CPP))
TEST_OBJECTS = $(patsubst $(TEST_DIR)/%.c, $(BUILD_DIR)/%.o, $(TEST_SOURCE))
LIB_OBJECTS_CPP = $(patsubst $(LIB_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(LIB_SOURCE_CPP))

# Gcc/Clang will create these .d files containing dependencies.
DEP = $(OBJECTS:%.o=%.d)

default: compile_rust $(TARGET)

$(TARGET): $(BUILD_DIR)/$(TARGET)

$(BUILD_DIR)/$(TARGET): $(OBJECTS) $(OBJECTS_CPP) $(LIB_OBJECTS_CPP) $(BUILD_DIR)/main.o
	$(CXX) $(CXX_FLAGS) $(SANITIZER_FLAGS) $^ -o $@ $(LN_FLAGS)

$(TEST_TARGET): $(BUILD_DIR)/$(TEST_TARGET)

$(BUILD_DIR)/$(TEST_TARGET): $(OBJECTS) $(OBJECTS_CPP) $(LIB_OBJECTS_CPP) $(TEST_OBJECTS)
	$(CXX) $(CXX_FLAGS) $(CFLAGS_TEST) $(SANITIZER_FLAGS) $^ -o $@ $(LN_FLAGS)

-include $(DEP)

# The potential dependency on header files is covered
# by calling `-include $(DEP)`.
# The -MMD flags additionaly creates a .d file with
# the same name as the .o file.
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(SANITIZER_FLAGS) -MMD -c $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXX_FLAGS) $(SANITIZER_FLAGS) -MMD -c $< -o $@

$(BUILD_DIR)/%.o: $(TEST_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(CFLAGS_TEST) $(SANITIZER_FLAGS) -MMD -c $< -o $@

$(BUILD_DIR)/%.o: $(LIB_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXX_FLAGS) $(SANITIZER_FLAGS) -MMD -c $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%main.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(SANITIZER_FLAGS) -MMD -c $< -o $@

deps_macos:
	brew install clang-format

run: default
	@$(BUILD_DIR)/$(TARGET) $(program);

test: compile_rust $(TEST_TARGET)
	$(BUILD_DIR)/$(TEST_TARGET)

clean:
	rm -rf $(BUILD_DIR) && \
	cd lambdaworks/lib/lambdaworks && cargo clean

fmt:
	clang-format --style=file -i $(SOURCE) $(TEST_SOURCE) $(HEADERS) $(TEST_HEADERS)

check_fmt:
	clang-format --style=file -Werror -n $(SOURCE) $(TEST_SOURCE) $(HEADERS) $(TEST_HEADERS)

valgrind: clean compile_rust $(TEST_TARGET)
	valgrind --leak-check=full --show-reachable=yes --show-leak-kinds=all --track-origins=yes --error-exitcode=1 ./build/cairo_vm_test

compile_rust: 
	cd lambdaworks/lib/lambdaworks && cargo build --release

docker_build:
	docker build . -t cairo-vm_in_c

docker_run:
	docker run --rm -it -v `pwd`:/usr/cairo-vm_in_C cairo-vm_in_c

docker_test_and_format:
	docker create --name test -t -v `pwd`:/usr/cairo-vm_in_C cairo-vm_in_c
	docker start test
	docker exec -t test bash -c "make && make test && make SANITIZER_FLAGS=-fno-omit-frame-pointer valgrind"
	docker stop test
	docker rm test

docker_clean:
	docker rmi cairo-vm_in_c
