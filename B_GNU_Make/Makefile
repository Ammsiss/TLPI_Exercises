include ../common.mk

# Add any extra libs here
# CFLAGS +=

CFILES := $(wildcard *.c)
BINS   := $(addsuffix .bin, $(basename $(CFILES)))

.PHONY: all
all: $(BINS)

.PHONY: clean
clean:
	rm -f $(BINS) compile_commands.json

.DEFAULT_GOAL := all # So not to be set to the first rule in common.mk
