CC = clang
CFLAGS = -Wall -Wextra -Wpedantic -g -O0 -std=c23

CFILES = $(wildcard ./*.c)
BASE_NAME = $(basename $(CFILES))
BIN_NAME = $(addsuffix .bin, $(BASE_NAME))

.PHONY: all
all: $(BASE_NAME)

%: %.c
	$(CC) $(CFLAGS) $< -o $@.bin

.PHONY: clean
clean:
	rm -f $(BIN_NAME)
