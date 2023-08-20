# Compiler and compiler flags
PROGRAM_NAME := myapp
CC := g++
CCFLAGS := -Wall -Wextra
LDFLAGS := -lncurses
STD_MODE := debug
VALGRIND_FLAGS := --leak-check=full --show-leak-kinds=all

# Definitions
MKDIR := mkdir -p
MK := make
RM := rm -fr
CLEAR := clear

# Mode definitions
MODE := $(STD_MODE)
TARGET_ROOT_DIR := target

DEBUG_CCFLAGS := -Og -g -ggdb3 -DDEBUG
RELEASE_CCFLAGS := -O3 -DNDEBUG -pedantic -Werror

ifeq ($(RELEASE),)
    MODE := debug
    CCFLAGS += $(DEBUG_CCFLAGS)
else
    MODE := release
    CCFLAGS += $(RELEASE_CCFLAGS)
endif

TARGET_DIR := $(TARGET_ROOT_DIR)/$(MODE)


# Directories
SRC_DIR := src
INC_DIR := include
OBJ_DIR := $(TARGET_DIR)/obj
BIN_DIR := $(TARGET_DIR)/bin
ANALYSIS_DIR := $(TARGET_DIR)/analysis
EXECUTABLE := $(BIN_DIR)/$(PROGRAM_NAME)

# Include directories
INCLUDE := $(shell find $(INC_DIR)/ -type d -printf "-I%p ")

CPP_FILES := $(shell find $(SRC_DIR) -name '*.cpp')
C_FILES := $(shell find $(SRC_DIR) -name '*.c')

CPP_OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(CPP_FILES))
C_OBJ_FILES := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(C_FILES))

OBJ_FILES := $(CPP_OBJ_FILES) $(C_OBJ_FILES)

CPP_DEP_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.d,$(CPP_FILES))
C_DEP_FILES := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.d,$(C_FILES))

DEP_FILES := $(CPP_DEP_FILES) $(C_DEP_FILES)

# Build target
build: $(EXECUTABLE)

# Run target
run: build
ifeq ($(RELEASE), 1)
	$(EXECUTABLE)
else
	$(EXECUTABLE) && clear
endif

# Rules
$(EXECUTABLE): $(OBJ_FILES)
	@$(MKDIR) $(@D)
	$(CC) $(CCFLAGS) $(INCLUDE) $^ -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@$(MKDIR) -p $(@D)
	$(CC) $(CCFLAGS) $(INCLUDE) -MMD -MP -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@$(MKDIR) -p $(@D)
	$(CC) $(CCFLAGS) $(INCLUDE) -MMD -MP -c $< -o $@

-include $(DEP_FILES)

.PHONY: clean

clean:
	$(RM) $(TARGET_DIR)

cleanr:
	$(RM) $(TARGET_DIR)

fresh: clean build

analysis: build
	@$(MKDIR) $(ANALYSIS_DIR)
	@$(eval ANALYSIS_COUNT := $(shell ls -1 $(ANALYSIS_DIR) | grep -oE 'valgrind_analysis_[0-9]+' | sort -rn | head -n 1 | grep -oE '[0-9]+'))
	@$(eval NEXT_ANALYSIS_COUNT := $(shell echo $$(($(ANALYSIS_COUNT) + 1))))
	@$(eval ANALYSIS_FILENAME := valgrind_analysis_$(shell printf "%03d" $(NEXT_ANALYSIS_COUNT)).txt)
	valgrind $(VALGRIND_FLAGS) $(EXECUTABLE) > $(ANALYSIS_DIR)/$(ANALYSIS_FILENAME) 2>&1