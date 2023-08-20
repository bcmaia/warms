# MasterMaker
# Author: β
# This Makefile is designed to simplify the compilation and build process for any 
# C/C++ project. It supports both debug and release modes, handles 
# automatic dependency generation, and provides various targets for building, 
# running, cleaning, and even analyzing the program using Valgrind. It will accept
# both c and cpp files and it can handle diferent project file structures.
# For best use, make sure your project has this structure:
#
# your_project_name/
# | src/
# | | main.cpp
# | | module_name/
# | | | module_name.cpp/
# | | | submodule_name_1.cpp/
# | | | submodule_name_2.c/
# | include/
# | | module_name/
# | | | module_name.hpp/
# | | | submodule_name_1.h/
# | | | submodule_name_2.hpp/
# | Makefile
#
# You don't need to care about paths when importing your headers. In the example
# above, writing '#include "module_name.hpp"' will sufice.




#==============================================================================/
#======================| CONFIGURATIONS |======================================/
#==============================================================================/
# NOTE: Edit this configurations according to your needs.

# Compiler and compiler flags
PROGRAM_NAME := myapp
CC := g++
CCFLAGS := -Wall -Wextra
LDFLAGS := -lncurses
STD_MODE := debug
VALGRIND_FLAGS := --leak-check=full --show-leak-kinds=all



#==============================================================================/
#======================| INNER LOGIC |=========================================/
#==============================================================================/
# NOTE: DO NOT EDIT THE LINES BELLOW UNLESS YOU KNOW WHAT YOU ARE DOING.

# Definitions
MKDIR := mkdir -p
MK := make
RM := rm -fr
CLEAR := clear
ECHO := echo

# Determine build mode (debug or release)
MODE := $(STD_MODE)
TARGET_ROOT_DIR := target

DEBUG_CCFLAGS := -Og -g -ggdb3 -DDEBUG
RELEASE_CCFLAGS := -O3 -DNDEBUG -pedantic -Werror

ifneq ($(REQUESTED_RELEASE),)
	RELEASE := true
endif

ifeq ($(RELEASE),)
    MODE := debug
    CCFLAGS += $(DEBUG_CCFLAGS)
else
    MODE := release
    CCFLAGS += $(RELEASE_CCFLAGS)
endif

TARGET_DIR := $(TARGET_ROOT_DIR)/$(MODE)

# Directory structure
SRC_DIR := src
ifneq ($(REQUESTED_SRC),)
    SRC_DIR := $(REQUESTED_SRC)
endif

INC_DIR := include
OBJ_DIR := $(TARGET_DIR)/obj
BIN_DIR := $(TARGET_DIR)/bin
ANALYSIS_DIR := $(TARGET_DIR)/analysis
EXECUTABLE := $(BIN_DIR)/$(PROGRAM_NAME)

ifeq ($(wildcard $(SRC_DIR)).,)
    SRC_DIR := .
    INC_DIR := .
endif

# Include directories
INCLUDE := $(shell find $(INC_DIR)/ -type d -printf "-I%p ") $(shell find $(SRC_DIR)/ -type d -printf "-I%p ")

# Collect source files
CPP_FILES := $(shell find $(SRC_DIR) -name '*.cpp')
C_FILES := $(shell find $(SRC_DIR) -name '*.c')

# Generate corresponding object file names
CPP_OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(CPP_FILES))
C_OBJ_FILES := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(C_FILES))

# Combine all object files
OBJ_FILES := $(CPP_OBJ_FILES) $(C_OBJ_FILES)

# Generate dependency files
CPP_DEP_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.d,$(CPP_FILES))
C_DEP_FILES := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.d,$(C_FILES))

# Combine all dependency files
DEP_FILES := $(CPP_DEP_FILES) $(C_DEP_FILES)





#==============================================================================/
#======================| MAIN TARGETS |========================================/
#==============================================================================/

# Build target
build: $(EXECUTABLE)
ifneq ($(wildcard $(SRC_DIR).),)
	@$(ECHO) " "
	@$(ECHO) "[$(MODE):build]\t WARNING"
	@$(ECHO) ":\t'src' directory was not detected in the current location."
	@$(ECHO) ":\tThis is not a good look :\\"
	@$(ECHO) ":\tI will do my best, but I strongly recommend that,"
	@$(ECHO) ":\twhen it comes to project organization: GET GOOD."
	@$(ECHO) ":\tI will assume '$(dir $(abspath $(lastword $(MAKEFILE_LIST))))'"
	@$(ECHO) ":\tis your source directory for now."
	@$(ECHO) " "
	@$(ECHO) "[$(MODE):build]\t •\`_´• Build done."
else
	@$(ECHO) "[$(MODE):build]\t (ﾉ◕ヮ◕)ﾉ*:・ﾟ✧ Build done!"
endif


# Run target
ifeq ($(REQUESTED_RELEASE),)
run: build
	@$(ECHO) "[$(MODE):run]\t Starting program... :)"
	@$(EXECUTABLE)
else
run:
	@$(EXECUTABLE)
endif

all:
	@$(MK) build -s > /dev/null






#==============================================================================/
#======================| RULES |===============================================/
#==============================================================================/

# Compile source files and link to executable
$(EXECUTABLE): $(OBJ_FILES)
	@$(MKDIR) $(@D)
	@$(ECHO) "[$(MODE):rule:link]\t Linking executable '$(PROGRAM_NAME)'..."
	@$(CC) $(CCFLAGS) $(INCLUDE) $^ -o $@ $(LDFLAGS)

# Compile C++ source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@$(MKDIR) -p $(@D)
	@$(ECHO) "[$(MODE):rule:compile]\t Compiling $(subst $(OBJ_DIR)/,,$@)..."
	@$(CC) $(CCFLAGS) $(INCLUDE) -MMD -MP -c $< -o $@

# Compile C source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@$(MKDIR) -p $(@D)
	@$(ECHO) "[$(MODE):rule:compile]\t Compiling $(subst $(OBJ_DIR)/,,$@)..."
	@$(CC) $(CCFLAGS) $(INCLUDE) -MMD -MP -c $< -o $@

# Include dependency files
-include $(DEP_FILES)






#==============================================================================/
#======================| TARGETS |=============================================/
#==============================================================================/

.PHONY: clean NUKE help setup

# Clean build artifacts
clean:
	@$(ECHO) "ʕノ•ᴥ•ʔノ ︵ ┻━┻"
	@$(ECHO) "[$(MODE):clean]\t Cleaning this mess..."
	@$(RM) $(TARGET_DIR)

# Clean and clear the console
clear: clean
	$(CLEAR)

# Clean and rebuild
fresh: clean build
	@$(ECHO) "[$(MODE):fresh]\t Fresh from the oven :)"

# Run Valgrind memory analysis
analysis: build
	@$(ECHO) "[$(MODE):analysis]\t Starting analysis..."
	@$(MKDIR) $(ANALYSIS_DIR)
	@$(eval ANALYSIS_COUNT := $(shell ls -1 $(ANALYSIS_DIR) | grep -oE 'valgrind_analysis_[0-9]+' | sort -rn | head -n 1 | grep -oE '[0-9]+'))
	@$(eval NEXT_ANALYSIS_COUNT := $(shell echo $$(($(ANALYSIS_COUNT) + 1))))
	@$(eval ANALYSIS_FILENAME := valgrind_analysis_$(shell printf "%03d" $(NEXT_ANALYSIS_COUNT)).txt)
	valgrind $(VALGRIND_FLAGS) $(EXECUTABLE) > $(ANALYSIS_DIR)/$(ANALYSIS_FILENAME) 2>&1

# Prompt for user confirmation before nuking the target folder
NUKE:
	@$(ECHO) " "
	@$(ECHO) "	( 0 _ 0 )"
	@read -p "Are you sure you want to NUKE the target folder? This will delete all build artifacts. (yes/no): " confirmation; \
	if [ "$$confirmation" = "yes" ]; then \
		$(RM) $(TARGET_ROOT_DIR); \
		echo "ヽ(｀Д´)⊃━☆ﾟ. * ･ ｡ﾟ, Target folder nuked."; \
		echo " "; \
	else \
		echo "Aborted."; \
	fi


# Create a .gitignore file with common build artifacts
gitignore:
	@$(ECHO) "Creating .gitignore file..."
	@$(ECHO) "# Common build artifacts" > .gitignore
	@$(ECHO) "$(TARGET_ROOT_DIR)/" >> .gitignore
	@$(ECHO) "*.o" >> .gitignore
	@$(ECHO) "*.d" >> .gitignore
	@$(ECHO) "*.exe" >> .gitignore
	@$(ECHO) "*.out" >> .gitignore
	@$(ECHO) "*.log" >> .gitignore
	@$(ECHO) "analysis/" >> .gitignore
	@$(ECHO) "*.stackdump" >> .gitignore
	@$(ECHO) ".vscode/" >> .gitignore
	@$(ECHO) ".idea/" >> .gitignore
	@$(ECHO) ".clangd/" >> .gitignore
	@$(ECHO) ".DS_Store" >> .gitignore
	@$(ECHO) "*.swp" >> .gitignore
	@$(ECHO) "*.swo" >> .gitignore
	@$(ECHO) ".gitignore" >> .gitignore
	@$(ECHO) "Gitignore file created."

bundle: bundle.zip
	zip -r bundle.zip .

hello:
	@$(ECHO) "( * ^ *) ノシ"
	@$(ECHO) "Hello, World!"

help:
	@$(ECHO) "ʕっ•ᴥ•ʔっ Need some help?"
	@$(ECHO) "Available targets:"
	@$(ECHO) "  build      - Build the program"
	@$(ECHO) "  run        - Build and run the program"
	@$(ECHO) "  clean      - Clean build artifacts"
	@$(ECHO) "  clear      - Clean and clear the console"
	@$(ECHO) "  fresh      - Clean, build, and run"
	@$(ECHO) "  analysis   - Run Valgrind memory analysis"
	@$(ECHO) "  gitignore  - Create a .gitignore file for common build artifacts"
	@$(ECHO) "  NUKE       - Delete the target folder (use with caution)"
	@$(ECHO) "  help       - Display this help message"


h: 
	@$(MK) help --no-print-directory

HELP: 
	@$(ECHO) "Hey, calm down, I can help! ;)"
	@$(MK) help --no-print-directory

r:
	@$(MK) run --no-print-directory

b:
	@$(MK) build --no-print-directory

# Show a warning message for misspelled targets
%: ; @$(if $(filter-out %.d,$@),$($(info ¯\_(ツ)_/¯ Unknown target '$@'. Use 'make help' to see available targets.)))



setup:
	@read -p "Enter the name of the 'src' directory (or press Enter to skip): " requested_src; \
	if [ -n "$$requested_src" ]; then \
		echo "REQUESTED_SRC := $$requested_src" | cat - $(MAKEFILE_LIST) > temp && mv temp $(MAKEFILE_LIST); \
		echo "Setup completed. 'REQUESTED_SRC' set to '$$requested_src'."; \
	else \
		echo "Setup skipped."; \
	fi

reformat:
	@echo "Deleting lines until 'MasterMaker' is found..."
	@awk '/MasterMaker/{found=1} !found' $(MAKEFILE_LIST) > temp && mv temp $(MAKEFILE_LIST)
	@echo "Deletion completed."