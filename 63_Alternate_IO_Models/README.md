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
