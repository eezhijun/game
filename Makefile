# only test demo
$(info MAKE START)

CC                := gcc
BIN               := main

MAKEFIILE_PATH                        := $(abspath $(lastword $(MAKEFILE_LIST)))
ROOT_DIR                              := $(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))
BUILD_DIR                             := $(ROOT_DIR)/output
BUILD_ABS_DIR                         := $(abspath $(BUILD_DIR))
DEMO_DIR                              := $(ROOT_DIR)/demo
SRC_DIR                               := $(ROOT_DIR)/src
SRC_LIB_DIR                           := $(SRC_DIR)/lib
SRC_LIB_LOG_DIR                       := $(SRC_LIB_DIR)/log
SRC_LIB_RINGBUFFER_DIR                := $(SRC_LIB_DIR)/ringbuffer
SRC_LIB_CJSON_DIR                     := $(SRC_LIB_DIR)/cjson
SRC_LIB_TLSF_DIR                      := $(SRC_LIB_DIR)/tlsf
SRC_LIB_STACK_DIR                     := $(SRC_LIB_DIR)/stack
SRC_LIB_QUEUE_DIR                     := $(SRC_LIB_DIR)/queue
SRC_UTILS_DIR                         := $(SRC_DIR)/utils
SRC_TEST_DIR                          := $(SRC_DIR)/test


INCLUDE_DIRS          := -I.
INCLUDE_DIRS          += -I$(DEMO_DIR)
INCLUDE_DIRS          += -I$(SRC_DIR)
INCLUDE_DIRS          += -I$(SRC_UTILS_DIR)
INCLUDE_DIRS          += -I$(SRC_TEST_DIR)
INCLUDE_DIRS          += -I$(SRC_LIB_DIR)
INCLUDE_DIRS          += -I$(SRC_LIB_LOG_DIR)
INCLUDE_DIRS          += -I$(SRC_LIB_RINGBUFFER_DIR)
INCLUDE_DIRS          += -I$(SRC_LIB_CJSON_DIR)
INCLUDE_DIRS          += -I$(SRC_LIB_TLSF_DIR)
INCLUDE_DIRS          += -I$(SRC_LIB_STACK_DIR)
INCLUDE_DIRS          += -I$(SRC_LIB_QUEUE_DIR)


SOURCE_FILES          := $(wildcard *.c)
SOURCE_FILES          += $(wildcard demo/snake/*.c)
SOURCE_FILES          += $(wildcard src/lib/log/*.c)
SOURCE_FILES          += $(wildcard src/lib/snake/*.c)
SOURCE_FILES          += $(wildcard src/lib/ringbuffer/*.c)
SOURCE_FILES          += $(wildcard src/lib/stack/*.c)
SOURCE_FILES          += $(wildcard src/lib/queue/*.c)
SOURCE_FILES          += $(wildcard src/utils/*.c)
SOURCE_FILES          += $(wildcard src/test/*.c)



CFLAGS                := -ggdb3
LDFLAGS               := -ggdb3 -pthread
CPPFLAGS              := $(INCLUDE_DIRS) -DBUILD_DIR=\"$(BUILD_ABS_DIR)\"
CPPFLAGS              += -D_WINDOWS_


CFLAGS                += -m32 # gcc 32bit
LDFLAGS               += -m32

LDFLAGS               += -lm # to link againt the math library (libm)
CPPFLAGS              += -Wall

CPPFLAGS              += -lcurses  # graph lib


ifdef SANITIZE_ADDRESS
  CFLAGS              += -fsanitize=address -fsanitize=alignment
  LDFLAGS             += -fsanitize=address -fsanitize=alignment
endif

ifdef SANITIZE_LEAK
  LDFLAGS             += -fsanitize=leak
endif

# user choose demo
ifeq ($(demo),?)
run:
	@echo "tt"
	@echo "sn"
endif

ifeq ($(demo),tt)
  CPPFLAGS            += -DTEST_DEMO=1
endif

ifeq ($(demo),sn)
  CPPFLAGS            += -DSNAKE_DEMO=1
endif



OBJ_FILES             = $(SOURCE_FILES:%.c=$(BUILD_DIR)/%.o)
DEP_FILE              = $(OBJ_FILES:%.o=%.d)

${BIN} : $(BUILD_DIR)/$(BIN)

${BUILD_DIR}/${BIN} : ${OBJ_FILES}
	@-mkdir -p ${@D}
	@$(CC) $^ ${LDFLAGS} -o $@
	@echo "BIN $@"
	@echo "+--------------------------------------------+"
	@echo "|            Finish Compilation              |"
	@echo "+--------------------------------------------+"

-include ${DEP_FILE}

${BUILD_DIR}/%.o : %.c Makefile
	@-mkdir -p $(@D)
	@$(CC) $(CPPFLAGS) $(CFLAGS) -MMD -c $< -o $@
	@echo "CC $<"

.PHONY : clean

clean:
	-rm -rf $(BUILD_DIR)
