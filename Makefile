# DSP Algorithms Library
# Author: suman
# Date:   2024/2/28

CC       = gcc
CSTD     = -std=c11
WARNINGS = -Wall -Wextra -Wpedantic -Werror

SRC_DIR    = src
COMMON_DIR = common
INC_DIR    = include
OBJ_DIR    = obj
BIN_DIR    = bin
TEST_DIR   = test
DATA_DIR   = data
SCRIPT_DIR = scripts

INCLUDES = -I$(INC_DIR) -I$(COMMON_DIR)

# make DEBUG=1 for debug build
ifdef DEBUG
  OPTFLAGS = -O0 -g3 -DDEBUG
else
  OPTFLAGS = -O2 -DNDEBUG
endif

CFLAGS  = $(CSTD) $(WARNINGS) $(INCLUDES) $(OPTFLAGS)
LDFLAGS = -lm

# Library sources / objects  (src/ + common/*.c)
SRC_FILES    = $(wildcard $(SRC_DIR)/*.c)
COMMON_FILES = $(wildcard $(COMMON_DIR)/*.c)
OBJ_FILES    = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES)) \
               $(patsubst $(COMMON_DIR)/%.c, $(OBJ_DIR)/%.o, $(COMMON_FILES))
LIB          = $(OBJ_DIR)/libdsp.a

# Test sources / binaries  (test_*.c -> unit tests)
TEST_SRC = $(wildcard $(TEST_DIR)/test_*.c)
TEST_BIN = $(patsubst $(TEST_DIR)/%.c, $(BIN_DIR)/%, $(TEST_SRC))

# Demo sources / binaries  (demo_*.c -> CSV generators)
DEMO_SRC = $(wildcard $(TEST_DIR)/demo_*.c)
DEMO_BIN = $(patsubst $(TEST_DIR)/%.c, $(BIN_DIR)/%, $(DEMO_SRC))

# Auto-dependency files
DEPS = $(OBJ_FILES:.o=.d)

# ------- targets -------

.PHONY: all clean test demos plot dirs

all: dirs $(LIB) $(TEST_BIN) $(DEMO_BIN)

dirs:
	@mkdir -p $(OBJ_DIR) $(BIN_DIR) $(DATA_DIR)

# compile library objects from src/
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | dirs
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

# compile library objects from common/
$(OBJ_DIR)/%.o: $(COMMON_DIR)/%.c | dirs
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

# build static library
$(LIB): $(OBJ_FILES)
	ar rcs $@ $^

# build each test binary against the library
$(BIN_DIR)/test_%: $(TEST_DIR)/test_%.c $(LIB) | dirs
	$(CC) $(CFLAGS) -I$(TEST_DIR) $< -L$(OBJ_DIR) -ldsp $(LDFLAGS) -o $@

# build each demo binary against the library
$(BIN_DIR)/demo_%: $(TEST_DIR)/demo_%.c $(LIB) | dirs
	$(CC) $(CFLAGS) $< -L$(OBJ_DIR) -ldsp $(LDFLAGS) -o $@

# run all unit tests
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

# run demo programs to generate CSV data in data/
demos: all
	@echo "--- Running demos ---"
	@for d in $(DEMO_BIN); do ./$$d; done
	@echo "CSV files written to $(DATA_DIR)/"

# generate plots from CSV data (requires gnuplot)
plot: demos
	@command -v gnuplot >/dev/null 2>&1 || { echo "gnuplot not found, install with: sudo apt install gnuplot"; exit 1; }
	@echo "--- Generating plots ---"
	@for gp in $(SCRIPT_DIR)/*.gp; do gnuplot "$$gp"; done
	@echo "Plots written to $(DATA_DIR)/"

clean:
	rm -f $(OBJ_DIR)/*.o $(OBJ_DIR)/*.d $(LIB)
	rm -f $(BIN_DIR)/test_* $(BIN_DIR)/demo_*
	rm -f $(DATA_DIR)/*.csv $(DATA_DIR)/*.png

-include $(DEPS)
