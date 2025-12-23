# Chapter 10: Exercises

## Ex. 10-1

**Question**

Assume a system where the value returned by the call
`sysconf(_SC_CLK_TCK)` is 100. Assuming that the clock_t value
returned by `times()` is an unsigned 32-bit integer, how long
will it take before this value cycles so that it restarts at 0?
Perform the same calculation for the `CLOCKS_PER_SEC` value
returned by `clock()`.

**Answer**

A 32 bit unsigned integer has a range from [0, 2^32)

In order to get the time in seconds from the return value of
times() we must divide by `sysconf(_SC_CLK_TCK)`. If this value
is 100 then the total amount of time before wrapping is
(2^32 - 1) / 100 ~= 4.29E7 seconds or roughly 497 days.

In order to get the time in seconds from the return value of
`clock()` we must divide by `CLOCKS_PER_SEC`. Therefore the
total amount of time before wrapping would be (2^32 - 1) /
1,000,000 ~= 4294.97 seconds or roughly 72 minutes.
