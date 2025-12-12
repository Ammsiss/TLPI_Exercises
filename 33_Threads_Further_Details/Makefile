include ../common.mk

# Add any extra libs here
# CFLAGS +=

# Sym links refer to function only library files; No entry point
CFILES := $(foreach FILE, $(wildcard *.c), \
		 $(shell test -L "$(FILE)" || echo "$(FILE)"))
BINS   := $(addsuffix .bin, $(basename $(CFILES)))

.PHONY: all
all: $(BINS)

.PHONY: clean
clean:
	rm -f $(BINS) compile_commands.json

# So not to be set to the first rule in common.mk
.DEFAULT_GOAL := all
