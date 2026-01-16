# ------------------------------------------------------------
# Makefile for Treasure Runner 
# ------------------------------------------------------------

# ------------------------------------------------------------
# Compiler and tools
# ------------------------------------------------------------
CC      := gcc

# ------------------------------------------------------------
# Flags
# ------------------------------------------------------------
CFLAGS  := -Wall -Wextra  -D_POSIX_C_SOURCE=200809L -Iinclude
LDFLAGS :=

# ------------------------------------------------------------
# Project layout
# ------------------------------------------------------------
SRC_DIR        := src
INCLUDE_DIR    := include
BUILD_DIR      := build
TESTS_DIR      := tests
TEST_BIN_DIR   := build/tests
DIST_DIR       := ../dist
TOOLS_DIR := tools
TOOLS_BIN_DIR := build/tools

# ------------------------------------------------------------
# Puzzlegen shared library
# ------------------------------------------------------------
PUZZLEGEN_SO := $(DIST_DIR)/libpuzzlegen.so

# ------------------------------------------------------------
# Source / object mapping
# ------------------------------------------------------------
STUDENT_SRCS := $(wildcard $(SRC_DIR)/*.c)
STUDENT_OBJS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(STUDENT_SRCS))

# ------------------------------------------------------------
# Check (unit testing)
# ------------------------------------------------------------
CHECK_CFLAGS := $(shell pkg-config --cflags check)
CHECK_LIBS   := $(shell pkg-config --libs check)

TEST_SRCS := $(wildcard $(TESTS_DIR)/*.c)
TEST_OBJS := $(patsubst $(TESTS_DIR)/%.c,$(TEST_BIN_DIR)/%.o,$(TEST_SRCS))
TEST_BIN := $(TEST_BIN_DIR)/tests

# ------------------------------------------------------------
# Phony targets
# ------------------------------------------------------------
.PHONY: all clean help test render dist-check

# ------------------------------------------------------------
# Default target
# ------------------------------------------------------------
all: test
	@echo "✓ Build complete"

# ------------------------------------------------------------
# Directory creation
# ------------------------------------------------------------
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

$(TEST_BIN_DIR):
	@mkdir -p $(TEST_BIN_DIR)

$(TOOLS_BIN_DIR):
	@mkdir -p $(TOOLS_BIN_DIR)
# ------------------------------------------------------------
# Object compilation
# ------------------------------------------------------------
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) \
	    -I$(INCLUDE_DIR) \
	    -c $< -o $@

$(TEST_BIN_DIR)/%.o: $(TESTS_DIR)/%.c | $(TEST_BIN_DIR)
	$(CC) $(CFLAGS) $(CHECK_CFLAGS) \
	    -I$(INCLUDE_DIR) \
	    -c $< -o $@

# ------------------------------------------------------------


# ------------------------------------------------------------
# Testing
# ------------------------------------------------------------
dist-check:
	@if [ ! -f "$(PUZZLEGEN_SO)" ]; then \
	    echo "Missing shared library: $(PUZZLEGEN_SO)"; \
	    echo "Please run make setup-lib."; \
	    exit 1; \
	fi

$(TEST_BIN): $(TEST_OBJS) $(STUDENT_OBJS) | $(TEST_BIN_DIR)
	$(CC) $(CFLAGS) $(CHECK_CFLAGS) \
	    -I$(INCLUDE_DIR) \
	    $^ -o $@ \
	    -L$(DIST_DIR) -lpuzzlegen \
	    -Wl,-rpath,'$$ORIGIN/../../dist' \
	    $(CHECK_LIBS)

test: $(TEST_BIN)
	@echo "Running C unit tests..."
	@LD_LIBRARY_PATH=$(DIST_DIR):$$LD_LIBRARY_PATH $(TEST_BIN)
	@status=$$?; \
	if [ $$status -ne 0 ]; then \
	    echo "✗ One or more tests failed"; \
	    exit $$status; \
	fi; \
	echo "✓ All C tests passed"

# ------------------------------------------------------------
# Clean (safe)
# ------------------------------------------------------------
clean:
	@echo "Cleaning build and binary artifacts..."
	@rm -rf $(BUILD_DIR) $(TEST_BIN_DIR) $(TOOLS_BIN_DIR) 
	@echo "✓ Clean complete"



# ------------------------------------------------------------
# Render Integration check Utility
# ------------------------------------------------------------
$(TOOLS_BIN_DIR)/render: $(TOOLS_DIR)/render_integration.c $(STUDENT_OBJS) dist-check | $(TOOLS_BIN_DIR)
	$(CC) $(CFLAGS) \
	    -I$(INCLUDE_DIR) \
	    $< $(STUDENT_OBJS) \
	    -o $@ \
	    -L$(DIST_DIR) -lpuzzlegen -lm \
	    -Wl,-rpath,'$$ORIGIN/../../../dist'

render: $(TOOLS_BIN_DIR)/render
	@if [ -z "$$LD_LIBRARY_PATH" ]; then \
	    echo "LD_LIBRARY_PATH is not set. Please source ../env.sh before running."; \
	fi
	@echo "Render tool built at $(TOOLS_BIN_DIR)/render"
	@echo "To run: ./$(TOOLS_BIN_DIR)/render <path/to/config.ini>"


# ------------------------------------------------------------
# puzzlegen_demo tool (standalone datagen usage)
# ------------------------------------------------------------
$(TOOLS_BIN_DIR)/puzzlegen_demo: $(TOOLS_DIR)/puzzlegen_demo.c dist-check | $(TOOLS_BIN_DIR)
	$(CC) $(CFLAGS) \
	    -I$(INCLUDE_DIR) \
	    $< \
	    -o $@ \
	    -L$(DIST_DIR) -lpuzzlegen -lm \
	    -Wl,-rpath,'$$ORIGIN../dist'

.PHONY: puzzlegen_demo

puzzlegen_demo: $(TOOLS_BIN_DIR)/puzzlegen_demo
	@echo "Built puzzlegen_demo tool.\n Run with export LD_LIBRARY_PATH=../dist; ./build/tools/puzzlegen_demo <config_file.ini>"

.PHONY: setup-lib
setup-lib:
	@arch=$$(dpkg --print-architecture 2>/dev/null || uname -m); \
	if [ "$$arch" = "amd64" ] || [ "$$arch" = "x86_64" ]; then \
		cp $(DIST_DIR)/libpuzzlegen-linux-amd64.so $(PUZZLEGEN_SO); \
		echo "Using libpuzzlegen-linux-amd64.so"; \
	else \
		cp $(DIST_DIR)/libpuzzlegen-linux-arm64.so $(PUZZLEGEN_SO); \
		echo "Using libpuzzlegen-linux-arm64.so"; \
	fi

# ------------------------------------------------------------
# Help
# ------------------------------------------------------------
help:
	@echo "Available targets:"
	@echo "  all              Build and run tests (default)"
	@echo "  test             Build and run unit tests"
	@echo "  clean            Remove build artifacts and binaries"
	@echo "  render           Build render integration tool"
	@echo "  puzzlegen_demo   Build puzzlegen demo tool"
	@echo "  setup-lib        Copy architecture-specific library"
	@echo "  dist-check       Verify dist artifacts exist"
	@echo ""
	@echo "Note: Source ../env.sh to set up environment variables"
