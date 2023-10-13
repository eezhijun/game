$(info MAKE START)

CC                    := gcc
EXEC                  := main

MK_PATH               := $(abspath $(lastword $(MAKEFILE_LIST)))
ROOT_DIR              := $(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))
BUILD_DIR             := $(ROOT_DIR)/out
BUILD_ABS_DIR         := $(abspath $(BUILD_DIR))

# include
INCLUDE_DIRS          := -I.
INCLUDE_DIRS          += -I$(ROOT_DIR)/src
INCLUDE_DIRS          += -I$(ROOT_DIR)/src/demo
INCLUDE_DIRS          += -I$(ROOT_DIR)/src/utils
INCLUDE_DIRS          += -I$(ROOT_DIR)/src/test


# src
SOURCE_FILES          := $(wildcard *.c)
SOURCE_FILES          += $(wildcard src/utils/*.c)
SOURCE_FILES          += $(wildcard src/test/*.c)
SOURCE_FILES          += $(wildcard src/demo/snake/*.c)


CPPFLAGS              := -Wall
CPPFLAGS              += $(INCLUDE_DIRS)
CPPFLAGS              += -D_WINDOWS_


CFLAGS                += -m32 # gcc 32bit


LDFLAGS               := -pthread
LDFLAGS               += -m32 # gcc 32bit
LDFLAGS               += -lm # to link againt the math library (libm)

OBJ_FILES             = $(SOURCE_FILES:%.c=$(BUILD_DIR)/%.o)
DEP_FILE              = $(OBJ_FILES:%.o=%.d)


$(EXEC) : $(BUILD_DIR)/$(EXEC)

$(BUILD_DIR)/$(EXEC) : $(OBJ_FILES)
	@-mkdir -p $(@D)
	@$(CC) $^ $(LDFLAGS) -o $@
	@echo "EXEC $@"
	@echo "+--------------------------------------------+"
	@echo "|            Finish Compilation              |"
	@echo "+--------------------------------------------+"

-include $(DEP_FILE)

$(BUILD_DIR)/%.o : %.c Makefile
	@-mkdir -p $(@D)
	@$(CC) $(CPPFLAGS) $(CFLAGS) -MMD -c $< -o $@
	@echo "CC $<"

# Run clang-format on source code
.PHONY: format
format:
	@echo "Running clang-format"
	@clang-format -i \
	$(shell find src/demo -name "*.c") \
	$(shell find src/test -name "*.c") \
	$(shell find src/utils -name "*.c")

.PHONY: all
all: $(BUILD_DIR)/$(EXEC)

# Build and run
.PHONY: run
run: all
	@echo "Starting program: $(BUILD_DIR)/$(EXEC)"
	@cd $(BUILD_DIR) && ./$(EXEC)

.PHONY : clean
clean:
	-rm -rf $(BUILD_DIR)
