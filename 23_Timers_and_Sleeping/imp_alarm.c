/*
    This function aims to replicate the behaviour of alarm(2) using
    setitimer().

    In order this function does the following:

    1. Store the value of any pre-existing timer
    2. If seconds is 0 disable the previous timer or if its not
       set the value of the timer to seconds
    3. Returns the previous time remaining or 0 if it was disabled
*/

#include <time.h>
#include <sys/time.h>

unsigned int imp_alarm(unsigned int seconds)
{
    struct itimerval previous;
    if (getitimer(ITIMER_REAL, &previous) == -1)
        return -1;

    if (seconds == 0) {
        struct itimerval disable;
        disable.it_value.tv_sec = 0;
        disable.it_value.tv_usec = 0;

        if (setitimer(ITIMER_REAL, &disable, NULL) == -1)
            return -1;
    } else {
        struct itimerval tv;
        tv.it_value.tv_sec = seconds;
        tv.it_value.tv_usec = 0;   /* alarm() does not support micro seconds... */
        /* ... or intervels */
        tv.it_interval.tv_sec = 0;
        tv.it_interval.tv_usec = 0;

        setitimer(ITIMER_REAL, &tv, NULL);
    }

    /* Return 0 if the timer was disabled or the seconds remaining */
    if (previous.it_value.tv_sec == 0 && previous.it_value.tv_usec == 0)
        return 0;
    else
        return previous.it_value.tv_sec;
}
