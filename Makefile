# DSP Algorithms Library
# Author: suman
# Date:   2024/2/28

CC       = gcc
CSTD     = -std=c11
WARNINGS = -Wall -Wextra -Wpedantic

SRC_DIR    = src
COMMON_DIR = common
INC_DIR    = include
OBJ_DIR    = obj
BIN_DIR    = bin
TEST_DIR   = test

INCLUDES = -I$(INC_DIR) -I$(COMMON_DIR)

# make DEBUG=1 for debug build
ifdef DEBUG
  OPTFLAGS = -O0 -g3 -DDEBUG
else
  OPTFLAGS = -O2 -DNDEBUG
endif

CFLAGS  = $(CSTD) $(WARNINGS) $(INCLUDES) $(OPTFLAGS)
LDFLAGS = -lm

# Library sources / objects
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES))
LIB       = $(OBJ_DIR)/libdsp.a

# Test sources / binaries
TEST_SRC = $(wildcard $(TEST_DIR)/test_*.c)
TEST_BIN = $(patsubst $(TEST_DIR)/%.c, $(BIN_DIR)/%, $(TEST_SRC))

# Auto-dependency files
DEPS = $(OBJ_FILES:.o=.d)

# ------- targets -------

.PHONY: all clean test dirs

all: dirs $(LIB) $(TEST_BIN)

dirs:
	@mkdir -p $(OBJ_DIR) $(BIN_DIR)

# compile library objects with dependency generation
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | dirs
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

# build static library
$(LIB): $(OBJ_FILES)
	ar rcs $@ $^

# build each test binary against the library
$(BIN_DIR)/test_%: $(TEST_DIR)/test_%.c $(LIB) | dirs
	$(CC) $(CFLAGS) -I$(TEST_DIR) $< -L$(OBJ_DIR) -ldsp $(LDFLAGS) -o $@

# run all tests
test: all
	@echo ""
	@failed=0; \
	for t in $(TEST_BIN); do \
		./$$t || failed=$$((failed + 1)); \
	done; \
	if [ $$failed -eq 0 ]; then \
		echo "All test suites passed."; \
	else \
		echo "$$failed test suite(s) had failures."; \
		exit 1; \
	fi

clean:
	rm -f $(OBJ_DIR)/*.o $(OBJ_DIR)/*.d $(LIB)
	rm -f $(BIN_DIR)/test_*

-include $(DEPS)
