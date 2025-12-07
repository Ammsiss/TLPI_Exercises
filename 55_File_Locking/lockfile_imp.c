// lockfile

#include "../lib/tlpi_hdr.h" // IWYU pragma: export

#include <fcntl.h>
#include <limits.h>

int main(int argc, char **argv)
{
    if (argc < 4 || strcmp(argv[1], "--help") == 0)
        usageErr("%s sleeptime retries filename ...", argv[0]);

    int sleep_time = getInt(argv[1], GN_GT_0, "sleep-time");
    int retries = getInt(argv[2], 0, "retries");
    if (retries < 0) {
        if (retries == -1)
            retries = INT_MAX;
        else
            cmdLineErr("'retries' must be greater then or equal to -1");

    }

    int fd;
    for (int file_n = 3; file_n < argc; ++file_n) {
        int success = 0;

        for (int retry_n = 0; retry_n < retries; ++retry_n) {
            if ((fd = open(argv[file_n], O_RDONLY | O_CREAT | O_EXCL, 0440)) == -1) {
                if (errno == EEXIST) {
                    printf("Attempt #%d - %s: Failed; Waiting %d sec\n",
                            retry_n + 1, argv[file_n], sleep_time);
                    sleep(sleep_time);
                    continue;
                } else
                    errExit("open");
            }

            printf("Created lockfile %s\n", argv[file_n]);
            success = 1;
            break;
        }

        if (!success)
            fatal("Failed to create lockfile %s", argv[file_n]);
    }

    printf("All lockfiles created\n");
    exit(EXIT_SUCCESS);
}
