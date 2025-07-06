#include <stddef.h>
#include <string.h>

char* str_insert(char *buf, const char *base, const char *str,
    size_t index)
{
    if (!buf || !base || !str)
        return NULL;

    size_t i;
    for (i = 0; i < index; ++i) {
        buf[i] = base[i];
    }
    buf[i] = '\0';

    strcat(buf, str);

    for (i = index; base[i] != '\0'; ++i) {
        buf[i + strlen(str)] = base[i];
    }
    buf[i + strlen(str)] = '\0';

    return buf;
}
