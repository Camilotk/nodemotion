# ===== NodeMotion Makefile =====

# ────────────────────────────────
# Configuration
# ────────────────────────────────
BUILD_DIR := build
EXE := $(BUILD_DIR)/node_demo
FORMAT_DIRS := src include examples
CLANG_FORMAT := clang-format
CONAN := conan
CMAKE := cmake

# ────────────────────────────────
# Color codes
# ────────────────────────────────
RESET  := \033[0m
BOLD   := \033[1m
BLUE   := \033[34m
CYAN   := \033[36m
GREEN  := \033[32m
YELLOW := \033[33m
RED    := \033[31m
GRAY   := \033[90m

# ────────────────────────────────
# Default target
# ────────────────────────────────
all: setup build

# ────────────────────────────────
# Step 1: Conan install and CMake configure
# ────────────────────────────────
setup:
	@echo "$(CYAN)--- Setting up dependencies via Conan ---$(RESET)"
	@$(CONAN) install . --output-folder=$(BUILD_DIR) --build=missing
	@$(CMAKE) --preset conan-release -S . -B $(BUILD_DIR)
	@echo "$(GREEN)✔ Setup complete.$(RESET)"

# ────────────────────────────────
# Step 2: Build project
# ────────────────────────────────
build:
	@echo "$(BLUE)--- Building NodeMotion ---$(RESET)"
	@$(CMAKE) --build $(BUILD_DIR) -j
	@echo "$(GREEN)✔ Build successful.$(RESET)"

# ────────────────────────────────
# Step 3: Run demo
# ────────────────────────────────
run:
	@echo "$(YELLOW)--- Running node_demo ---$(RESET)"
	@$(EXE)
	@echo "$(GREEN)✔ Execution finished.$(RESET)"

# ────────────────────────────────
# Step 4: Format all source files
# ────────────────────────────────
format:
	@echo "$(CYAN)--- Running clang-format ---$(RESET)"
	@find $(FORMAT_DIRS) -type f \( -name "*.cpp" -o -name "*.hpp" -o -name "*.h" \) -exec $(CLANG_FORMAT) -i {} \;
	@echo "$(GREEN)✔ All source files formatted.$(RESET)"

# ────────────────────────────────
# Step 5: Clean build artifacts
# ────────────────────────────────
clean:
	@echo "$(RED)--- Cleaning build directory ---$(RESET)"
	@rm -rf $(BUILD_DIR)
	@echo "$(GREEN)✔ Clean complete.$(RESET)"

# ────────────────────────────────
# Step 6: Rebuild from scratch
# ────────────────────────────────
rebuild: clean all

.PHONY: all setup build run format clean rebuild
