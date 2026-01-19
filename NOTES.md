## TODO

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

- Replace replace use of strtol with util function in all
relevant files

- Explore the idea of a library specific make file instead of
duplicating that logic in the common make file. Something like this
could work in the per chapter make files (Recurse then execute make):
