#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    while (1) {
        size_t len = 0;
        char *buf = NULL;

        int num_read = getline(&buf, &len, stdin);

        printf("\nnum_read = %d\n", num_read);
        buf[num_read - 1] = '\0';

        printf("%s\n", buf);
    }

    return 0;
}
