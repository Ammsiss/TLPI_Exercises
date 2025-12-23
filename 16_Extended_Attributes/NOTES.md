## Syscalls

`setxattr(2)` - sets the EAs (Extended attributes) of a file  
`lsetxattr(2)`  
`fsetxattr(2)`  
`setfattr(1)`

`getxattr(2)` - retrieve an EA of a file  
`lgetxattr(2)`  
`fgetxattr(2)`  
`getfattr(1)`

`removexattr(2)` - remove an EA of a file  
`lremovexattr(2)`  
`xremovexattr(2)`  
`setfattr(1)`

`listxattr(2)` - retrieve all of the EAs of a file  
`llistxattr(2)`  
`flistxattr(2)`  
`getfattr(1)`

## TODO

- Add multiple file parsing to **Exersise 16-1** and the option
  to remove an EA. (After adding remove option, update
  'Examples' for `removexattr(2)` syscall above)
