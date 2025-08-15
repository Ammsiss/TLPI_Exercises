## Syscalls

**sigsetjmp(3)** -> Set a non local goto point thats safe to jump to from a singal handler
**siglongjmp(3)** -> Safely jump to a non local goto point from a signal handler

**abort(3)** -> Terminate calling process and produce core dump file

**sigaltstack(3)** -> Create or retrieve an alternative signal stack

## Tables

See **Table 21-1** for functions required to be async-signal-safe by by various standards.

See **Table 21-2** for values returned in the *si_code* field of the *siginfo_t* structure

## More Info

- In the case of a stack overflow the SIGSEGV signal is returned. Consider the case
where you define a handler for this signal. The kernel wouldn't be able to add a
stack frame for your handler and so the default behaviour would occur. (program te-
mination). This is a use case for *sigaltstack()*.

- The _POSIX_C_SOURCE feature test macro must be defined with a value greater than or
equal to 199309 in order to make the declaration of the *siginfo_t* structure visible
from <signal.h>

- Use this macro for blocking system calls that shouldn't be interuppted by a singal:
```c
#define NO_EINTR(stmt) while ((stmt) == -1 && errno == EINTR);
```
Make sure you still check for other errno values after the macro.
You can also juse use the **SA_RESTART** flag with sigaction. (though not all system
calls work with this flag)

**siginterrupt(3)** -> Modify the SA_RESTART flag for a signal (Obsolete)

## TODO
