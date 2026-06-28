#include "trace.h"

#include <inttypes.h>
#include <stdlib.h>

void crusn_trace_init(crusn_trace* trace, FILE* stream) {
    trace->stream = stream;
    trace->event_counter = 0;
}

void crusn_trace_event(crusn_trace* trace, const char* kind, const char* name, uint32_t a, uint32_t b) {
    if (trace == NULL || trace->stream == NULL) {
        return;
    }

    fprintf(
        trace->stream,
        "{\"seq\":%" PRIu64 ",\"kind\":\"%s\",\"name\":\"%s\",\"a\":\"%08X\",\"b\":\"%08X\"}\n",
        trace->event_counter++,
        kind,
        name,
        a,
        b);
    fflush(trace->stream);
}

noreturn void crusn_unimpl(const char* file, int line, const char* func) {
    fprintf(stderr, "UNIMPL reached at %s:%d in %s\n", file, line, func);
    fflush(stderr);
    abort();
}

noreturn void crusn_slockon(const char* file, int line, char* msg) {
    fprintf(stderr, "SLOCKON: %s at %s:%d\n", msg, file, line);
    abort();
}
