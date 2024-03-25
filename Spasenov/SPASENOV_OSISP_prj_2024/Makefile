CC=gcc
CFLAGS=-g -ggdb -std=c11 -pedantic -W -Wall -Wextra
SRC=src
BUILD=build
BIN=bin
DEBUG_DIST=$(BUILD)/debug
RELEASE_DIST=$(BUILD)/release
OUT_DIR=$(BIN)/debug
BUILD_DIST=$(DEBUG_DIST)

ifeq ($(MODE), release)
	CFLAGS=-std=c11 -pedantic -W -Wall -Wextra -Werror
	OUT_DIR=$(BIN)/release
	BUILD_DIST=$(RELEASE_DIST)
endif

SRCS=$(wildcard $(SRC)/**/*.c)
OBJS=$(patsubst $(SRC)/%.c, $(BUILD_DIST)/%.o, $(SRCS))

.PHONY: all clean

all: $(OUT_DIR)

$(OUT_DIR): $(OBJS)
	@mkdir -p $(dir $(OUT_DIR))
	$(CC) -o $(OUT_DIR) $^ -lncurses

$(BUILD_DIST)/%.o : $(SRC)/%.c
	@mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -rf $(BUILD) $(BIN)