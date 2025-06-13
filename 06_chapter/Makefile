CC = clang
CFLAGS = -Wall -Wextra -Wpedantic -g -O0 -std=c23

CFILES = $(wildcard ./*.c)
TARGETS = $(basename $(CFILES))

all: $(TARGETS)

%: %.c
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean
clean:
	rm -f $(TARGETS)
