#ifndef TEXT_UTILS_H
#define TEXT_UTILS_H
#include <string.h>
#include <stdlib.h>

static char *my_strdup(const char *s)
{
    if (!s)
        return NULL;
    size_t n = strlen(s) + 1;
    char *p = malloc(n);
    if (!p)
        return NULL;
    memcpy(p, s, n);
    return p;
}

#endif

