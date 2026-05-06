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
