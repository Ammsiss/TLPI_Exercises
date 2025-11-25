## Syscalls

getrlimit
setrlimit

## Tables

**Listing 36-1**: Definition of the rusage structure
**Table 36-1**: Resource values for getrlimit() and setrlimit()

## More Info

Details of specific resource limits **SECTION 36.3**

Process limit associated with real user id only applies to the
process that actually called setrlimit, but it does take into account
all the processes globally associated wit hteh real user id. So
even if the process limit is hit the actual user could still spawn
a separate process.

## TODO

- Come back to the RLIMIT_NICE test to see how to fully check it
    (maybe use capabilities?)
