CC = clang
CFLAGS = -Wall -Wextra -Wpedantic -g -O0 -std=c23 -I../lib

CFILES = $(wildcard ./*.c)
LIBS = $(wildcard ../lib/*.c)
BASE_NAME = $(basename $(CFILES))
BIN_NAME = $(addsuffix .bin, $(BASE_NAME))

.PHONY: all
all: $(BASE_NAME)

%: %.c
	$(CC) -lcrypt $(CFLAGS) $< $(LIBS) -o $@.bin

.PHONY: clean
clean:
	rm -f $(BIN_NAME)
