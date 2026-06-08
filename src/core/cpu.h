#ifndef CRUSN_CPU_H
#define CRUSN_CPU_H

#include <stdint.h>

typedef union crusn_reg32 {
    uint32_t u;
    int32_t s;
    float f;
} crusn_reg32;

extern crusn_reg32 R0;
extern crusn_reg32 R1;
extern crusn_reg32 R2;
extern crusn_reg32 R3;
extern crusn_reg32 R4;
extern crusn_reg32 R5;
extern crusn_reg32 R6;
extern crusn_reg32 R7;

extern uint32_t AR0;
extern uint32_t AR1;
extern uint32_t AR2;
extern uint32_t AR3;
extern uint32_t AR4;
extern uint32_t AR5;
extern uint32_t AR6;
extern uint32_t AR7;

extern uint32_t DP;
extern uint32_t SP;
extern uint32_t ST;
extern uint32_t IE;
extern uint32_t IF;
extern uint32_t IOF;
extern uint32_t RC;
extern uint32_t RS;
extern uint32_t RE;
extern uint32_t BK;
extern uint32_t IR0;
extern uint32_t IR1;

void crusn_cpu_reset(void);

#endif
