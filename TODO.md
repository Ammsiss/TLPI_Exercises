## TODO

- Replace replace use of strtol with util function in all
relevant files

- Explore the idea of a library specific make file instead of 
duplicating that logic in the common make file. Something like this 
could work in the per chapter make files (Recurse then execute make):

- Maybe add -D tracking to make file (is it even worth it)?

- Figure out how to not auto compile c files in chapters that are just
sym linked library source files.
