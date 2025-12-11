## TODO

- Centralize make file and add dependency tracking with .d files.
  This relies on going through the make book, which in turn relies
  on going through the bash tutorials.

- Finish skipped filesystem based exersises.

- Add a string <-> character comparison function. One that takes a
  string of individual characters and compares each one to a single
  specified character.

- Finish standardizing chapters. (rename, add readme and notes, and
  change exersise filenames)

- Replace replace use of strtol with util function in all
relevant files

- Explore the idea of a library specific make file instead of 
duplicating that logic in the common make file. Something like this 
could work in the per chapter make files (Recurse then execute make):

```make
$(LIB_PATH):
	$(MAKE) -C $(LIB_DIR) $(LIB_NAME)
```
