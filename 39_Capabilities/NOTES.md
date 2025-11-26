## Syscalls
## Tables

Table 39-1: Operations permitted by each linux capability
Table 39-2: The securebits flags

## More Info

We can view hexadecimal representations of the three capability sets for any process
in the three fields CapInh, CapPrm, and CapEff in the Linux-specific */proc/PID/status*
file.

The setcap(8) and getcap(8) commands, contained in the libcap package described in
Section 39.7, can be used to manipulate file capabilities sets (The syntax used by
setcap and getcap for representing capability sets is described in the cap_from_text(3)
manual page provided in the libcap package.)

Kernel capability calculation formulas:
    P'(permitted) = (P(inheritable) & F(inheritable)) | (F(permitted) & cap_bset)
    P'(effective) = F(effective) ? P'(permitted) : 0
    P'(inheritable) = P(inheritable)
Kernel capability calculation formulas for a set-user-ID-*root* program:
    P'(permitted) = P(inheritable) | cap_bset
    P'(effective) = P'(permitted)

**General vibe of libcap API**:
1. Use the cap_get_proc() function to retrieve a copy of the process’s current capa-
bility sets from the kernel and place it in a structure that the function allocates
in user space. (Alternatively, we may use the cap_init() function to create a new,
empty capability set structure.) In the libcap API, the cap_t data type is a pointer
used to refer to such structures.
2. Use the cap_set_flag() function to update the user-space structure to raise
(CAP_SET) and drop (CAP_CLEAR) capabilities from the permitted, effective, and
inheritable sets stored in the user-space structure retrieved in the previous step.
808 Chapter 39
3. Use the cap_set_proc() function to pass the user-space structure back to the kernel
in order to change the process’s capabilities.
4. Use the cap_free() function to free the structure that was allocated by the libcap
API in the first step.

## TODO
