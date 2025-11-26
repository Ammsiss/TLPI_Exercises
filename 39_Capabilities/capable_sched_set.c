#include <sched.h>
#include <sys/capability.h>

#include "../lib/error_functions.h"
#include "../lib/utils.h"

int main(int argc, char *argv[])
{
    int j, pol;
    struct sched_param sp;

    if (argc < 3 || strchr("rfo"
#ifdef SCHED_BATCH              /* Linux-specific */
                "b"
#endif
#ifdef SCHED_IDLE               /* Linux-specific */
                "i"
#endif
                , argv[1][0]) == NULL)
        usageErr("%s policy priority [pid...]\n"
                "    policy is 'r' (RR), 'f' (FIFO), "
#ifdef SCHED_BATCH              /* Linux-specific */
                "'b' (BATCH), "
#endif
#ifdef SCHED_IDLE               /* Linux-specific */
                "'i' (IDLE), "
#endif
                "or 'o' (OTHER)\n",
                argv[0]);

    pol = (argv[1][0] == 'r') ? SCHED_RR :
                (argv[1][0] == 'f') ? SCHED_FIFO :
#ifdef SCHED_BATCH              /* Linux-specific, since kernel 2.6.16 */
                (argv[1][0] == 'b') ? SCHED_BATCH :
#endif
#ifdef SCHED_IDLE               /* Linux-specific, since kernel 2.6.23 */
                (argv[1][0] == 'i') ? SCHED_IDLE :
#endif
                SCHED_OTHER;

    sp.sched_priority = getNum(argv[2], "priority");

    /* Load current capability sets into user memory */
    cap_t cap = cap_get_proc();
    if (cap == NULL)
        errExit("cap_get_proc");

    cap_value_t cap_list[1] = { CAP_SYS_NICE };

    /* Modify that structure to add the needed capability (in our case its
     * CAP_SYS_NICE in order to execute sched_setscheduler */
    if (cap_set_flag(cap, CAP_EFFECTIVE, 1, cap_list, CAP_SET) == -1)
        errExit("cap_set_flag");

    /* Send it back to the kernel */
    if (cap_set_proc(cap) == -1)
        errExit("cap_set_proc");

    for (j = 3; j < argc; j++)
        if (sched_setscheduler(getNum(argv[j], "pid"), pol, &sp) == -1)
            errExit("sched_setscheduler");

    if (cap_free(cap) == -1)
        errExit("cap_free");

    exit(EXIT_SUCCESS);
}
