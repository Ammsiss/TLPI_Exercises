include ../common.mk

CFLAGS += $(LIBRT)

CFILES := $(wildcard *.c)
BINS   := $(addsuffix .bin, $(basename $(CFILES)))

.PHONY: all
all: $(BINS)

.PHONY: clean
clean:
	rm -f $(BINS) compile_commands.json

.DEFAULT_GOAL := all
