#ifndef CRUSN_TRACE_H
#define CRUSN_TRACE_H

#include <stdint.h>
#include <stdio.h>
#include <stdnoreturn.h>

typedef struct crusn_trace {
    FILE* stream;
    uint64_t event_counter;
} crusn_trace;

void crusn_trace_init(crusn_trace* trace, FILE* stream);
void crusn_trace_event(crusn_trace* trace, const char* kind, const char* name, uint32_t a, uint32_t b);
noreturn void crusn_unimpl(const char* file, int line, const char* func);
noreturn void crusn_slockon(const char* file, int line, char* msg);

#if defined(CRUSN_TRACE) && CRUSN_TRACE
#define TRACE_EVENT(trace, kind, name, a, b) crusn_trace_event((trace), (kind), (name), (a), (b))
#else
#define TRACE_EVENT(trace, kind, name, a, b) ((void)(trace), (void)(kind), (void)(name), (void)(a), (void)(b))
#endif

#define UNIMPL() crusn_unimpl(__FILE__, __LINE__, __func__)

#define UNIMPL_TODO()
#endif
