## Syscalls

`posix_openpt()` -> Open a pty master device
`grantpt()` -> Change a pty slave ownership and perms
`unlockpt()` -> Unlock a pty slave
`ptsname()` -> Return name of a pty slave

`ptyMasterOpen()` -> TLPI function
`ptyFork()` -> Create a pty connected child process

## TODO

- Instead of using a pty pair for the vi scripting language
  program, create one that uses noevims modern message pack rpc.
- While replay_script.c does work, the typescript.timed file could be made more
  human readable by converting escape sequences (and optionally ansii escapes)
  to a 2 char representation and back. Currently \n is the only one that is
  printed literally.
