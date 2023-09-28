$(info "DungeoRush Start")

CC              := gcc
EXEC            := main

MK_PATH         := $(abspath $(lastword $(MAKEFILE_LIST)))
ROOT_DIR        := $(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))
BUILD_DIR       := $(ROOT_DIR)/out


# $(info $(MK_PATH), $(ROOT_DIR), $(BUILD_DIR))

INCLUDE_DIRS := -I.
INCLUDE_DIRS += -I/usr/local/include/SDL2
INCLUDE_DIRS += -I/usr/include/SDL2
INCLUDE_DIRS += -I$(ROOT_DIR)/src

LIB_DIRS := -L.
LIB_DIRS := -L/usr/local/lib/

SOURCE_FILES := $(wildcard src/*.c)

CPPFLAGS := $(INCLUDE_DIRS)
CPPFLAGS += $(LIB_DIRS)
CPPFLAGS += -ggdb3

ifdef DBG
CPPFLAGS += -v
endif

CFLAGS :=

LDFLAGS := -pthread
LDFLAGS += -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lSDL2_net -lm

OBJ_FILES = $(SOURCE_FILES:%.c=$(BUILD_DIR)/%.o)

$(info $(LDFLAGS))

$(BUILD_DIR)/$(EXEC) : $(OBJ_FILES)
	@-mkdir -p $(@D)
	@$(CC) $^ $(LDFLAGS) -o $@
	@echo "EXEC $@"


${BUILD_DIR}/%.o : %.c
	@-mkdir -p $(@D)
	@$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@
	@echo "CC $<"

.PHONY : clean

clean:
	-rm -rf $(BUILD_DIR)
