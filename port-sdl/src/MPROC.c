#include <string.h>

#include "PROCESS.h"
#include "TRACE.h"

#include "NOT_IMPLEMENTED.h"
static ProcFn g_proc_queue[64];
static int g_proc_head;
static int g_proc_tail;

void PRC_INIT(void) {
    TRACE();
    g_proc_head = 0;
    g_proc_tail = 0;
    memset(g_proc_queue, 0, sizeof(g_proc_queue));
}

void PRC_CREATE(ProcFn fn) {
    TRACE();
    if (!fn) {
        return;
    }
    const int next = (g_proc_tail + 1) % (int)(sizeof(g_proc_queue) / sizeof(g_proc_queue[0]));
    if (next == g_proc_head) {
        return;
    }
    g_proc_queue[g_proc_tail] = fn;
    g_proc_tail = next;
}

void PRC_DISPATCH(void) {
    TRACE();
    const int cap = (int)(sizeof(g_proc_queue) / sizeof(g_proc_queue[0]));
    int pending = g_proc_tail - g_proc_head;
    if (pending < 0) {
        pending += cap;
    }

    /* Run only the processes that were pending at dispatch entry.
       Processes created during execution run on later frames. */
    while (pending > 0 && g_proc_head != g_proc_tail) {
        ProcFn fn = g_proc_queue[g_proc_head];
        g_proc_head = (g_proc_head + 1) % cap;
        if (fn) {
            fn();
        }
        pending--;
    }
}

void PRC_DEBUG_CHECK(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void PRC_CREATE_CHILD(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void PRC_SLEEP(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void PRC_SUICIDE(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void PRC_KILL(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void PRC_KILLALL(void) {
    TRACE();
    PRC_INIT();
}

void PRC_EXISTP(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void PRC_XFER(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void PRC_FINDNEXT(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void PRC_FIND(void) {
    TRACE();
    NOT_IMPLEMENTED();
}

void PRC_FOLLOW(void) {
    TRACE();
    NOT_IMPLEMENTED();
}
