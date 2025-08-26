## Syscalls

**acct(2)** -> Enable or disable process accounting

**clone(2)** -> Linux specific *fork()* with more control

## Tables

**Table 28-1**: Bit values for the ac_flag field of process accounting records
**Table 28-2**: The clone() flags bit-mask values
**Table 28-3**: Time required to create 100k processes using *fork()*, *vfork*,
                and *clone()*
**Table 28-4**: Effect of *exec()* and *fork()* on process attributes
**Figure 28-1**: A thread gruop containing four threads

## More Info

See *proc(5)*

## TODO

- Make a progrma using *clone()* where the child starts in main. (careful about recursion)
- Checkout *procexec/demo_clone.c* for a more complex demo of *clone()*
- Figure out why the timing exersize on my machine does not align at all with **Table 28-3**
