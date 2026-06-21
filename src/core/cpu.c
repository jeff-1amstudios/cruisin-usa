#include "cpu.h"

#include <string.h>

crusn_reg32 R0;
crusn_reg32 R1;
crusn_reg32 R2;
crusn_reg32 R3;
crusn_reg32 R4;
crusn_reg32 R5;
crusn_reg32 R6;
crusn_reg32 R7;

uintptr_t AR0;
uintptr_t AR1;
uintptr_t AR2;
uintptr_t AR3;
uintptr_t AR4;
uintptr_t AR5;
uintptr_t AR6;
uintptr_t AR7;

uint32_t DP;
uint32_t SP;
uint32_t ST;
uint32_t IE;
uint32_t IF;
uint32_t IOF;
uint32_t RC;
uint32_t RS;
uint32_t RE;
uint32_t BK;
uint32_t IR0;
uint32_t IR1;

void crusn_cpu_reset(void)
{
    memset(&R0, 0, sizeof(R0));
    memset(&R1, 0, sizeof(R1));
    memset(&R2, 0, sizeof(R2));
    memset(&R3, 0, sizeof(R3));
    memset(&R4, 0, sizeof(R4));
    memset(&R5, 0, sizeof(R5));
    memset(&R6, 0, sizeof(R6));
    memset(&R7, 0, sizeof(R7));

    AR0 = 0;
    AR1 = 0;
    AR2 = 0;
    AR3 = 0;
    AR4 = 0;
    AR5 = 0;
    AR6 = 0;
    AR7 = 0;

    DP = 0;
    SP = 0;
    ST = 0;
    IE = 0;
    IF = 0;
    IOF = 0;
    RC = 0;
    RS = 0;
    RE = 0;
    BK = 0;
    IR0 = 0;
    IR1 = 0;
}
