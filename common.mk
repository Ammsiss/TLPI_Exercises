CC      ?= clang
CFLAGS  ?= -Wall -Wextra -Wpedantic -g -O0 -std=c23

SRC_DIR ?= .

LIB_DIR := $(ROOT_DIR)/lib
LIBS    := $(wildcard $(LIB_DIR)/*.c)
INCLUDE := -I$(LIB_DIR)

CFILES = $(wildcard $(SRC_DIR)/*.c)
BASES = $(basename $(notdir $(CFILES)))
BINS = $(addsuffix .bin, $(BASES))

.PHONY: all clean

all: $(BINS)

%.bin: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDE) $< $(LIBS) -o $@

clean:
	rm -f $(BINS)
