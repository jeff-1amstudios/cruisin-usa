#ifndef STUB_H
#define STUB_H

#include <stdio.h>

#define STUB()                                                     \
    do {                                                           \
        printf("STUB: %s\n", __func__);                         \
        fflush(stdout);                                            \
    } while (0)

#endif
