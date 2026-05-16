#ifndef TRACE_H
#define TRACE_H

#include <stdio.h>

#define TRACE()                           \
    do {                                  \
        printf("TRACE: %s\n", __func__); \
        fflush(stdout);                   \
    } while (0)

#endif
