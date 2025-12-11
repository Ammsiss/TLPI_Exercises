CC       := clang
ROOT_DIR := ..

LIBRT    := -lrt
LIBDL    := -ldl
LIBACL   := -lacl
LIBCRYPT := -lcrypt
LIBCAP   := -lcap

LIB_NAME := tlpilib.a
LIB_DIR  := $(ROOT_DIR)/lib
LIB_PATH := $(LIB_DIR)/$(LIB_NAME)

CFLAGS   := -Wall -Wextra -Wpedantic -g -O0 -std=c23 -I$(LIB_DIR)
LCFLAGS  := $(CFLAGS)

LIB_SOURCE := $(wildcard $(LIB_DIR)/*.c)
LIB_OBJ    := $(patsubst %.c,%.o,$(LIB_SOURCE))

# $? only uses the pre-reqs that were determined to be newer
# (Potentially problematic? Maybe just use $^)
$(LIB_PATH): $(LIB_OBJ)
	ar rcs $@ $?

$(LIB_DIR)/%.o: $(LIB_DIR)/%.c
	$(CC) $(LCFLAGS) -c $< -o $@

# Because no recipe is specified, Make merges this dependency
# with the above generic rule for error_functions.o. This is
# a great pattern as you can specify a general rule and then
# any targets that need extra requirements can have a separate
# rule specifying what they depend on.
$(LIB_DIR)/error_functions.o: $(LIB_DIR)/ename.c.inc

$(LIB_DIR)/ename.c.inc:
	sh $(LIB_DIR)/Build_ename.sh > $@
	echo 1>&2 "ename.c.inc built"
# Output to stderr so this message can still be seen
# with 'make -s'

lclean:
	rm -f $(LIB_DIR)/*.o $(LIB_PATH) $(LIB_DIR)/ename.c.inc

%.bin: %.c $(LIB_PATH)
	$(CC) $(CFLAGS) $< $(LIB_PATH) -o $@

.PHONY: cc
cc:
	bear -- $(MAKE) clean lclean all
