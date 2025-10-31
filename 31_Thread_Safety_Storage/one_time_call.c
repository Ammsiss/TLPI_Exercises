#define GNU_SOURCE

#include <pthread.h>

#include "../lib/error_functions.h"

struct Control
{
    int flag;
    pthread_mutex_t mtx;
};

static struct Control control = { 0, PTHREAD_MUTEX_INITIALIZER };

int one_time_init(struct Control* control, void (* init)())
{
    int s;
    int rv;

    if ((s = pthread_mutex_lock(&control->mtx)) != 0)
        errExitEN(s, "pthread_mutex_lock");

    if (control->flag)
        rv = -1;
    else {
        init();
        rv = 0;
        control->flag = 1;
    }

    if ((s = pthread_mutex_unlock(&control->mtx)) != 0)
        errExitEN(s, "pthread_mutex_unlock");

    return rv;
}
