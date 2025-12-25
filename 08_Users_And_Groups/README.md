# Chapter 8: Exercises

## Ex. 8-1

**Question**

When we execute the following code, we find that it displays the
same number twice, even though the two users have different IDs
in the password file. Why is this?

```c
printf("%ld %ld\n", (long) (getpwnam("avr")->pw_uid),
                    (long) (getpwnam("tsr")->pw_uid));
```

**Answer**

This is becuase each call to `getpwnam()` updates a statically
allocated buffer. Each call happens before the `printf()` call
resulting in the second `getpwnam()` call overwriting the same
location. This results in 2 character pointers pointing at the
same location.

## Ex. 8-2

**Question**

Implement `getpwnam()` using `setpwent()`, `getpwent()`, and
`endpwent()`.

**Answer**

See *getpwnam_imp.c*
