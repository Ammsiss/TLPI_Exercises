#include <stddef.h>
#include <sys/stat.h>

/*
    - Assign path name to 'cwdbuf' (caller allocated of max size, 'size') and
      null terminate it.

    - Return pointer to cwdbuf on success NULL on path size exceeding 'size'
      and errno set to ERANGE

    - If cwd path name exceeds 4096 silently truncate whole directories prefixes
      from the beginning of the string. (i.e. /test/path/to/do/... -> /path/to/do/...)

    - (Optional) add the glibc wrapper function feature of allowing the user
      to pass in NULL for 'cwdbuf' which causes the function to allocate the
      buffer (as large as needed, up to 4096 bytes) for the user.

    Notes: Make sure to compare against inode number and device number in the
    case the that current working directory is a mount point!
*/

char *getcwd(char *cwdbuf, size_t size)
{
}

int main(void)
{
}
