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

.PHONY: build_deploy
build_deploy: nv_sv.bin nv_cl.bin
	./nv_sv.bin&

.PHONY: kill_sv
kill_sv:
	pkill -e nv_sv.bin

.PHONY: clean
clean:
	rm -f $(BINS) $(DEP) compile_commands.json

# So not to be set to the first rule in common.mk
.DEFAULT_GOAL := all
