include ../common.mk

# Add any extra libs here
# CFLAGS +=

CFILES := $(foreach FILE, $(wildcard *.c), \
		 $(shell test -L "$(FILE)" || echo "$(FILE)"))
BINS   := $(addsuffix .bin, $(basename $(CFILES)))
DEP    := $(patsubst %.c,%.d,$(CFILES))

IS_SHELL_SV := is_shell_sv.bin
IS_SHELL_CL := is_shell_cl.bin

-include $(DEP)

.PHONY: is_server
is_server: $(IS_SHELL_SV) $(IS_SHELL_CL)

.PHONY: all
all: $(BINS)

.PHONY: clean
clean:
	rm -f $(BINS) $(DEP) compile_commands.json

# So not to be set to the first rule in common.mk
.DEFAULT_GOAL := all
