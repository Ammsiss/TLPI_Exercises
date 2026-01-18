include ../common.mk

# Add any extra libs here
# CFLAGS +=

CFILES := $(foreach FILE, $(wildcard *.c), \
		 $(shell test -L "$(FILE)" || echo "$(FILE)"))
BINS   := $(addsuffix .bin, $(basename $(CFILES)))
DEP    := $(patsubst %.c,%.d,$(CFILES))

-include $(DEP)

.PHONY: all
all: $(BINS)

.PHONY: clean
clean:
	rm -f $(BINS) $(DEP) compile_commands.json

# So not to be set to the first rule in common.mk
.DEFAULT_GOAL := all
