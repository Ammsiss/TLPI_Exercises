# Chapter 30: Threads: Thread Synchronization
## Ex. 30-1
**Question**

Modify the program in **Listing 30-1** (thread_incr.c) so that each loop in the thread’s
start function outputs the current value of glob and some identifier that uniquely
identifies the thread. The unique identifier for the thread could be specified as an
argument to the pthread_create() call used to create the thread. For this program,
that would require changing the argument of the thread’s start function to be a
pointer to a structure containing the unique identifier and a loop limit value. Run the
program, redirecting output to a file, and then inspect the file to see what happens to
glob as the kernel scheduler alternates execution between the two threads.

**Answer**

## Ex. 30-2
**Question**

Implement a set of thread-safe functions that update and search an unbalanced
binary tree. This library should include functions (with the obvious purposes) of
the following form:

```c
initialize(tree);
add(tree, char *key, void *value);
delete(tree, char *key)
Boolean lookup(char *key, void **value)
```

In the above prototypes, tree is a structure that points to the root of the tree (you
will need to define a suitable structure for this purpose). Each element of the tree
holds a key-value pair. You will also need to define the structure for each element
to include a mutex that protects that element so that only one thread at a time can
access it. The initialize(), add(), and lookup() functions are relatively simple to imple-
ment. The delete() operation requires a little more effort.

*Removing the need to maintain a balanced tree greatly simplifies the locking
requirements of the implementation, but carries the risk that certain patterns
of input would result in a tree that performs poorly. Maintaining a balanced
tree necessitates moving nodes between subtrees during the add() and delete()
operations, which requires much more complex locking strategies.*

**Answer**

See *test_ts_tree.c* and *ts_btree.h*
