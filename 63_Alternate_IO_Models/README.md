# Chapter 63: Alternate I/O Models

## Ex. 63-1

**Question**

Modify the program in **Listing 63-2** (*poll_pipes.c*) to use
`select()` instead of `poll()`.

**Answer**

See *select_pipes.c*

Run the following command to see the diff with the original
listing.

```bash
diff ../tlpi-dist/altio/poll_pipes.c select_pipes.c
```

## Ex. 63-8

**Question**

Modify the program in **Listing 63-3** (*demo_sigio.c*) to use a
realtime signal instead of `SIGIO`. Modify the signal handler to
accept a `siginfo_t` argument and display the values of the
`si_fd` and `si_code` fields of this structure.

**Answer**

See *sigio_rts_demo.c*
