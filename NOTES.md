## TODO

- The reason can't build 44 is cause that was before the library port so need
  to add the lib functions from tlpi in the sym link style we adopted.

RANDOM
- Change the make files to use dynamic library instead of static

- Clean up git history (linearize merges, compress small
  commits). Perhaps with this method: "rebuild a brand-new
  linear history from selected “checkpoint” commits (chapter
  commits), by taking the full tree snapshot at those points and
  committing it in a fresh branch. This preserves the content
  exactly at each checkpoint, just rearranges history
  into a clean line."

- Replace replace use of strtol with util function in all
relevant files

- Explore the idea of a library specific make file instead of
duplicating that logic in the common make file. Something like this
could work in the per chapter make files (Recurse then execute make):

- Make your own program using sockets or posix message queues to
  send the chat.html or conversations.json file to the linux
  box.

- Continue updating NOTES.md and README.md in each chapter
  directory. (Current directory: 17)

- Replace the rest of the Makefiles in all chapters

- Explore the idea of a library specific make file instead of
duplicating that logic in the common make file. Something like this
could work in the per chapter make files (Recurse then execute make):

- Do the chapter 40 exersizes. Learning the utmpx api while
  outdated will still probably give you an idea of the shape of
  the problem.

- Check out alpine or void linux for a no systemd distro
