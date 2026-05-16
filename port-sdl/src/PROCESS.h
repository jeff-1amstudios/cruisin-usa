#ifndef PROCESS_H
#define PROCESS_H

typedef void (*ProcFn)(void);

void PRC_INIT(void);
void PRC_CREATE(ProcFn fn);
void PRC_DISPATCH(void);
void PLYR_INTRO(void);

#endif
