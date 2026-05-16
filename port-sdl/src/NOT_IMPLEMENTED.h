#ifndef NOT_IMPLEMENTED_H
#define NOT_IMPLEMENTED_H

#include <stdio.h>
#include <stdlib.h>

#define NOT_IMPLEMENTED()                                                          \
    do {                                                                           \
        fprintf(stderr, "NOT_IMPLEMENTED: %s (%s:%d)\n", __func__, __FILE__,    \
                __LINE__);                                                         \
        abort();                                                                   \
    } while (0)

#endif
