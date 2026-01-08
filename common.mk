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

CFLAGS   := -g -O0 -fno-omit-frame-pointer -Wall -Wextra -Wpedantic  -std=c23 -I$(LIB_DIR)
DEPFLAGS = -MMD -MP

LIB_SOURCE := $(wildcard $(LIB_DIR)/*.c)
LIB_OBJ    := $(patsubst %.c,%.o,$(LIB_SOURCE))
LIB_DEP    := $(patsubst %.o,%.d,$(LIB_OBJ))

# $? only uses the pre-reqs that were determined to be newer.
$(LIB_PATH): $(LIB_OBJ)
	ar rcs $@ $?

$(LIB_OBJ): %.o: %.c
	$(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

# Because no recipe is specified, Make merges this dependency
# with the above generic rule for error_functions.o.
$(LIB_DIR)/error_functions.o: $(LIB_DIR)/ename.c.inc

$(LIB_DIR)/ename.c.inc:
	sh $(LIB_DIR)/Build_ename.sh > $@
	echo 1>&2 "ename.c.inc built"
# Output to stderr so this message can still be seen
# with 'make -s'.

lclean:
	rm -f $(LIB_DIR)/*.o $(LIB_PATH) $(LIB_DIR)/ename.c.inc

%.bin: %.c $(LIB_PATH)
	$(CC) $(CFLAGS) $(DEPFLAGS) $< $(LIB_PATH) -o $@

-include $(LIB_DEP)

.PHONY: cc
cc:
	bear --output ../compile_commands.json -- $(MAKE) clean lclean all
