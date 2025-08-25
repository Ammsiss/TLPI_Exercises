## Syscalls

### exec() Library Functions

These functions replace the calling process with a new program.

p (PATH)    : Denotes that the *pathname* argument can be a basename instead of
              an entire relative or absolute path. PATH will be searched for matches.

v (vector)  : Denotes that the *argv* argument must be a null terminated array.

l (list)    : Denotes that the *argv* argument must be a list (variadic arguments).

e (env)     : Allows the programmer to specify the env for the new program (the other
              calls use the existing environment).

**execve(2)**
**execle(3)**
**execlp(3)**
**execvp(3)**
**execv(3)**
**execl(3)**

**fexecve(3)**

**system(3)** -> Execute a shell command (Never use with s-uid/gid programs)

## Tables
## More Info

## TODO

- Why does 'clear' work with t_system.c program?
- Add the security measure noted in *imp_execlp.c*
