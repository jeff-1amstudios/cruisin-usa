#include "../core/cpu.h"
#include "../core/machine.h"
#include "macs.h"
#include "obj.h"
#include "mproc.h"
#include "vunit.h"
#include "sysid.h"
#include "sys.h"
#include "globals.h"
#include "sndtab.h"
#include "pall.h"
#include "objects.h"
#include "text.h"
#include "h2hobj.h"
#include "map.h"
#include "discovered_defines.h"

/*
 * Source module: asm/MAP.ASM
 */

/* *----------------------------------------------------------------------------
*
*
*
*
*
*
 */
#define MAP_ITERATIONS 30
/* 	;INITIAL THETAs
 */
#define M1ST (-HALFPI)
#define M2ST HALFPI
#define M3ST (-HALFPI)
#define M4ST HALFPI
/* 	;THETA DELTAs
 */
#define M1STD 0.052359877 //HALFPI/30
#define M2STD (-0.052359877) //-HALFPI/30
#define M3STD 0.052359877 //HALFPI/30
#define M4STD (-0.052359877) //-HALFPI/30
/* asm: M3STDI	.float	M1STD */
float M3STDI = M1STD;
/* asm: M4STDI	.float	M2STD */
float M4STDI = M2STD;
/* 	;PROCESS DATA DEFINEs
 */
#define MAP1OBJ (PDATA+0)
#define MAP2OBJ (PDATA+1)
#define MAP3OBJ (PDATA+2)
#define MAP4OBJ (PDATA+3)
#define MAPLPX (PDATA+4) //MAP LEAD POSITION X
#define MAPLPY (PDATA+5) //MAP LEAD POSITION Y
#define MAPLPZ (PDATA+6) //MAP LEAD POSITION Z
#define MAP1T (PDATA+7) //MAP 1 THETA
#define MAP2T (PDATA+8) //MAP 2 THETA
#define MAP3T (PDATA+9) //MAP 3 THETA
#define MAP4T (PDATA+10) //MAP 4 THETA
#define MAPLTX (PDATA+11) //MAP LEAD THETA
#define MAPLMAT (PDATA+12) //MAP LEAD MATRIX
#define MAPLMATEND (PDATA+20)
#define MAPXD (PDATA+21)
#define MAPYD (PDATA+22)
/* *----------------------------------------------------------------------------
 */
/* asm: FORMULA1	.float	0.318309886 */
float FORMULA1 = 0.318309886f;
/* *----------------------------------------------------------------------------
 */
/* asm: MAPPAL13	.bss	MAPPAL13,1 */
int MAPPAL13;
/* asm: MAPPAL24	.bss	MAPPAL24,1 */
int MAPPAL24;
/* asm: STOPWATCH	.bss	STOPWATCH,1 */
int STOPWATCH;
/* asm: STOPWATCH_CNTL	.bss	STOPWATCH_CNTL,1 */
int STOPWATCH_CNTL;
const char *COLON = ":";
/* *----------------------------------------------------------------------------
*RETURNS
*	AR2	POINTING TO LAP BUFFER
*
 */
/* asm: lap_buffer	.bss	lap_buffer,4 */
int lap_buffer[4];
/* asm: tmp_buffer	.bss	tmp_buffer,2 */
int tmp_buffer[2];
/* *----------------------------------------------------------------------------
*CVTTIME	CVT TIMECODE TO COMPONENTS
*
*PARAMETERS
*	R0
*
*RETURNS
*	R0	(INT) HUNDERTHS
*	R1	(INT) SECONDS
*	R2	(INT) MINUTES
*
 */
/* asm: MINFACT	.float	0.000303030303		;1/(55*60) */
float MINFACT = 0.000303030303f;
/* asm: SECFACT	.float	0.018181818		;1/55 */
float SECFACT = 0.018181818f;
/* asm: HUNFACT	.float	1.818181818		;100/55 */
float HUNFACT = 1.818181818f;
#define RADAR_XMIN 460
#define RADAR_XMAX 500
#define RADAR_XCNTR (((RADAR_XMAX-RADAR_XMIN)/2)+RADAR_XMIN)
#define RADAR_YMIN 100
#define RADAR_YMAX 200
#define RADAR_YCNTR (((RADAR_YMAX-RADAR_YMIN)/2)+RADAR_YMIN)
#define BLIPSIZE_X 4
#define BLIPSIZE_Y 4
#define BLIPSIZE_XH 2
#define BLIPSIZE_YH 2
/* asm: THIS_MACHINE_AHEAD	.bss	THIS_MACHINE_AHEAD,1 */
int THIS_MACHINE_AHEAD;

/* *
*
*
*
*
*
 */
void UNFOLDMAP(void)
{
    // asm: 	CALL	MAPPAL_ILLUM_INIT
    TRACE_EVENT(&g_crusn_machine->trace, "function", "UNFOLDMAP", 0, 0);
    UNIMPL();
}

void UNFOLDMAP_NOPAL(void)
{
    // asm: 	LDIL	O_3DROT,R6
    // asm: 	OR	O_NOUROT|O_NOUNIV,R6
    // asm: 	LDI	@EPALL,AR2
    // asm: 	CALL	PAL_FIND_RAW
#if DEBUG
    // asm: 	BNC	$
#endif
    // asm: 	LDI	R0,AR3
    // asm: 	LDI	@EPALR,AR2
    // asm: 	CALL	PAL_FIND_RAW
#if DEBUG
    // asm: 	BNC	$
#endif
    // asm: 	LDI	R0,AR4
    // asm: 	LDL	map1,AR2
    // asm: 	CALL	OBJ_GETE
    // asm: 	STI	AR0,*+AR7(MAP1OBJ)
    // asm: 	LDI	R6,R0
    // asm: 	OR	*+AR0(OFLAGS),R0
    // asm: 	OR	O_1PAL,R0
    // asm: 	STI	AR3,*+AR0(OPAL)
    // asm: 	STI	R0,*+AR0(OFLAGS)
    // asm: 	LDI	AR0,AR2
    // asm: 	CALL	OBJ_INSERTP
    // asm: 	LDL	map2,AR2
    // asm: 	CALL	OBJ_GETE
    // asm: 	STI	AR0,*+AR7(MAP2OBJ)
    // asm: 	LDI	R6,R0
    // asm: 	OR	*+AR0(OFLAGS),R0
    // asm: 	OR	O_1PAL,R0
    // asm: 	STI	AR4,*+AR0(OPAL)
    // asm: 	STI	R0,*+AR0(OFLAGS)
    // asm: 	LDI	AR0,AR2
    // asm: 	CALL	OBJ_INSERTP
    // asm: 	LDL	map3,AR2
    // asm: 	CALL	OBJ_GETE
    // asm: 	STI	AR0,*+AR7(MAP3OBJ)
    // asm: 	LDI	R6,R0
    // asm: 	OR	*+AR0(OFLAGS),R0
    // asm: 	OR	O_1PAL,R0
    // asm: 	STI	AR3,*+AR0(OPAL)
    // asm: 	STI	R0,*+AR0(OFLAGS)
    // asm: 	LDI	AR0,AR2
    // asm: 	CALL	OBJ_INSERTP
    // asm: 	LDL	map4,AR2
    // asm: 	CALL	OBJ_GETE
    // asm: 	STI	AR0,*+AR7(MAP4OBJ)
    // asm: 	LDI	R6,R0
    // asm: 	OR	*+AR0(OFLAGS),R0
    // asm: 	OR	O_1PAL,R0
    // asm: 	STI	AR4,*+AR0(OPAL)
    // asm: 	STI	R0,*+AR0(OFLAGS)
    // asm: 	LDI	AR0,AR2
    // asm: 	CALL	OBJ_INSERTP
    // asm: 	LDF	M1ST,R0
    // asm: 	STF	R0,*+AR7(MAP1T)
    // asm: 	LDF	M2ST,R0
    // asm: 	STF	R0,*+AR7(MAP2T)
    // asm: 	LDF	M3ST,R0
    // asm: 	STF	R0,*+AR7(MAP3T)
    // asm: 	LDF	M4ST,R0
    // asm: 	STF	R0,*+AR7(MAP4T)
    // asm: 	CLRF	R0
    // asm: 	STF	R0,*+AR7(MAPLPX)
    // asm: 	STF	R0,*+AR7(MAPLPY)
    // asm: 	FLOAT	3368,R0	;368
    // asm: 	STF	R0,*+AR7(MAPLPZ)
    // asm: 	LDF	HALFPI,R2
    // asm: 	STF	R2,*+AR7(MAPLTX)
    // asm: 	LDI	AR7,AR2
    // asm: 	ADDI	MAPLMAT,AR2
    // asm: 	CALL	FIND_XMATRIX
    // asm: 	LDF	0.2,R0
    // asm: 	STF	R0,@MAPPAL24
    // asm: 	LDF	0.6,R0
    // asm: 	STF	R0,@MAPPAL13
    // asm: 	LDI	MAP_ITERATIONS-1,AR5
    // asm: UNFOLD_LP
    // ;	CALL	MAP_ILLUM_COMPUTE
    // asm: 	LDF	@MAPPAL24,R0
    // asm: 	ADDF	0.0266,R0
    // asm: 	STF	R0,@MAPPAL24
    // asm: 	LDF	@MAPPAL13,R0
    // asm: 	ADDF	0.0133,R0
    // asm: 	STF	R0,@MAPPAL13
    // asm: 	CALL	MAPPAL_ILLUM
    // asm: 	LDF	*+AR7(MAPLPZ),R0
    // asm: 	FLOAT	368,R1
    // asm: 	SUBF	R0,R1
    // asm: 	MPYF	0.25,R1
    // asm: 	ADDF	R1,R0
    // asm: 	FLOAT	368,R1
    // asm: 	CMPI	0,AR5
    // asm: 	LDFEQ	R1,R0
    // asm: 	STF	R0,*+AR7(MAPLPZ)
    // asm: 	LDF	*+AR7(MAPLTX),R2
    // asm: 	MPYF	0.9,R2
    // asm: 	CLRF	R1
    // asm: 	CMPI	0,AR5
    // asm: 	LDFEQ	R1,R2
    // asm: 	STF	R2,*+AR7(MAPLTX)
    // asm: 	LDI	AR7,AR2
    // asm: 	ADDI	MAPLMAT,AR2
    // asm: 	CALL	FIND_XMATRIX
    // asm: 	LDI	*+AR7(MAP2OBJ),AR4
    // asm: 	LDF	*+AR7(MAPLPX),R0
    // asm: 	STF	R0,*+AR4(OPOSX)
    // asm: 	LDF	*+AR7(MAPLPY),R0
    // asm: 	STF	R0,*+AR4(OPOSY)
    // asm: 	LDF	*+AR7(MAPLPZ),R0
    // asm: 	STF	R0,*+AR4(OPOSZ)
    // asm: 	LDF	*+AR7(MAP2T),R2
    // asm: 	ADDF	@M2STDI,R2
    // asm: 	STF	R2,*+AR7(MAP2T)
    // asm: 	LDI	AR4,AR2
    // asm: 	ADDI	OMATRIX,AR2
    // asm: 	CALL	FIND_YMATRIX
    // asm: 	LDI	AR7,R2
    // asm: 	ADDI	MAPLMAT,R2
    // asm: 	LDI	AR2,R3
    // asm: 	CALL	CONCATMAT
    // asm: 	LDI	*+AR7(MAP3OBJ),AR4
    // asm: 	LDF	*+AR7(MAPLPX),R0
    // asm: 	STF	R0,*+AR4(OPOSX)
    // asm: 	LDF	*+AR7(MAPLPY),R0
    // asm: 	STF	R0,*+AR4(OPOSY)
    // asm: 	LDF	*+AR7(MAPLPZ),R0
    // asm: 	STF	R0,*+AR4(OPOSZ)
    // asm: 	LDF	*+AR7(MAP3T),R2
    // asm: 	ADDF	@M3STDI,R2
    // asm: 	STF	R2,*+AR7(MAP3T)
    // asm: 	LDI	AR4,AR2
    // asm: 	ADDI	OMATRIX,AR2
    // asm: 	CALL	FIND_YMATRIX
    // asm: 	LDI	AR7,R2
    // asm: 	ADDI	MAPLMAT,R2
    // asm: 	LDI	AR2,R3
    // asm: 	CALL	CONCATMAT
    // asm: 	LDF	*+AR7(MAP2T),R2
    // asm: 	LDI	@MATRIXAI,AR2
    // asm: 	CALL	FIND_YMATRIX
    // asm: 	LDI	AR7,R2
    // asm: 	ADDI	MAPLMAT,R2
    // asm: 	LDI	AR2,R3
    // asm: 	CALL	CONCATMAT
    // asm: 	LDI	*+AR7(MAP1OBJ),AR4
    // asm: 	CALL	CLR_VECTORA
    // asm: 	FLOAT	-127,R0
    // asm: 	STF	R0,*+AR2(X)
    // asm: 	LDI	@MATRIXAI,R2
    // asm: 	LDI	AR4,R3
    // asm: 	ADDI	OPOSX,R3
    // asm: 	CALL	MATRIX_MUL
    // asm: 	LDF	*+AR4(OPOSX),R0
    // asm: 	ADDF	*+AR7(MAPLPX),R0
    // asm: 	STF	R0,*+AR4(OPOSX)
    // asm: 	LDF	*+AR4(OPOSY),R0
    // asm: 	ADDF	*+AR7(MAPLPY),R0
    // asm: 	STF	R0,*+AR4(OPOSY)
    // asm: 	LDF	*+AR4(OPOSZ),R0
    // asm: 	ADDF	*+AR7(MAPLPZ),R0
    // asm: 	STF	R0,*+AR4(OPOSZ)
    // asm: 	LDF	*+AR7(MAP1T),R2
    // asm: 	ADDF	@M1STDI,R2
    // asm: 	STF	R2,*+AR7(MAP1T)
    // asm: 	LDI	AR4,AR2
    // asm: 	ADDI	OMATRIX,AR2
    // asm: 	CALL	FIND_YMATRIX
    // asm: 	LDI	AR7,R2
    // asm: 	ADDI	MAPLMAT,R2
    // asm: 	LDI	AR2,R3
    // asm: 	CALL	CONCATMAT
    // asm: 	LDF	*+AR7(MAP3T),R2
    // asm: 	LDI	@MATRIXAI,AR2
    // asm: 	CALL	FIND_YMATRIX
    // asm: 	LDI	AR7,R2
    // asm: 	ADDI	MAPLMAT,R2
    // asm: 	LDI	AR2,R3
    // asm: 	CALL	CONCATMAT
    // asm: 	LDI	*+AR7(MAP4OBJ),AR4
    // asm: 	CALL	CLR_VECTORA
    // asm: 	FLOAT	128,R0
    // asm: 	STF	R0,*+AR2(X)
    // asm: 	LDI	@MATRIXAI,R2
    // asm: 	LDI	AR4,R3
    // asm: 	ADDI	OPOSX,R3
    // asm: 	CALL	MATRIX_MUL
    // asm: 	LDF	*+AR4(OPOSX),R0
    // asm: 	ADDF	*+AR7(MAPLPX),R0
    // asm: 	STF	R0,*+AR4(OPOSX)
    // asm: 	LDF	*+AR4(OPOSY),R0
    // asm: 	ADDF	*+AR7(MAPLPY),R0
    // asm: 	STF	R0,*+AR4(OPOSY)
    // asm: 	LDF	*+AR4(OPOSZ),R0
    // asm: 	ADDF	*+AR7(MAPLPZ),R0
    // asm: 	STF	R0,*+AR4(OPOSZ)
    // asm: 	LDF	*+AR7(MAP4T),R2
    // asm: 	ADDF	@M4STDI,R2
    // asm: 	STF	R2,*+AR7(MAP4T)
    // asm: 	LDI	AR4,AR2
    // asm: 	ADDI	OMATRIX,AR2
    // asm: 	CALL	FIND_YMATRIX
    // asm: 	LDI	AR7,R2
    // asm: 	ADDI	MAPLMAT,R2
    // asm: 	LDI	AR2,R3
    // asm: 	CALL	CONCATMAT
    // asm: 	SLEEP	1
    // asm: 	DBU	AR5,UNFOLD_LP
    // ;
    // ;	LDF	1.0,R0
    // ;	STF	R0,@MAPPAL24
    // ;	STF	R0,@MAPPAL13
    // ;	CALL	MAPPAL_ILLUM
    // ;
    // ;
    // ;	LDI	15,AR5
    // ;LU76	LDF	@MAPPAL13,R0
    // ;	SUBRF	1.0,R0
    // ;	MPYF	0.1,R0
    // ;	ADDF	@MAPPAL13,R0
    // ;	STF	R0,@MAPPAL13
    // ;
    // ;	LDF	@MAPPAL24,R0
    // ;	SUBRF	1.0,R0
    // ;	MPYF	0.1,R0
    // ;	ADDF	@MAPPAL24,R0
    // ;	STF	R0,@MAPPAL24
    // ;
    // ;	CALL	MAPPAL_ILLUM
    // ;
    // ;	SLEEP	1
    // ;	DBU	AR5,LU76
    // ;
    // asm: 	LDI	map1_p,R0	;ONE TO OVERWRITE
    // asm: 	LDI	map1_p,R1	;WHAT TO OVERWRITE IT WITH
    // asm: 	CALL	PAL_OVERWRITE
    // asm: 	LDI	*+AR7(MAP1OBJ),AR0
    // asm: 	LDI	*+AR0(OFLAGS),R0
    // asm: 	ANDN	O_1PAL,R0
    // asm: 	STI	R0,*+AR0(OFLAGS)
    // asm: 	LDI	*+AR7(MAP2OBJ),AR0
    // asm: 	LDI	*+AR0(OFLAGS),R0
    // asm: 	ANDN	O_1PAL,R0
    // asm: 	STI	R0,*+AR0(OFLAGS)
    // asm: 	LDI	*+AR7(MAP3OBJ),AR0
    // asm: 	LDI	*+AR0(OFLAGS),R0
    // asm: 	ANDN	O_1PAL,R0
    // asm: 	STI	R0,*+AR0(OFLAGS)
    // asm: 	LDI	*+AR7(MAP4OBJ),AR0
    // asm: 	LDI	*+AR0(OFLAGS),R0
    // asm: 	ANDN	O_1PAL,R0
    // asm: 	STI	R0,*+AR0(OFLAGS)
    // asm: 	DIE
    TRACE_EVENT(&g_crusn_machine->trace, "function", "UNFOLDMAP_NOPAL", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*
*
*
 */
void FOLDMAP(void)
{
    // asm: 	SONDFX	WIPE4
    // *ELP CHANGE
    // asm: 	CALL	MAPPAL_ILLUM_INIT
    // *ELP END CHANGE
    // asm: 	LDIL	O_3DROT,R6
    // asm: 	OR	O_NOUROT|O_NOUNIV,R6
    // asm: 	LDI	@EPALL,AR2
    // asm: 	CALL	PAL_FIND_RAW
    // asm: 	LDI	R0,AR3
    // asm: 	LDI	@EPALR,AR2
    // asm: 	CALL	PAL_FIND_RAW
    // asm: 	LDI	R0,AR4
    // asm: 	LDL	map1,AR2
    // asm: 	CALL	OBJ_GETE
    // asm: 	STI	AR0,*+AR7(MAP1OBJ)
    // asm: 	LDI	R6,R0
    // asm: 	OR	*+AR0(OFLAGS),R0
    // asm: 	OR	O_1PAL,R0
    // asm: 	STI	AR3,*+AR0(OPAL)
    // asm: 	STI	R0,*+AR0(OFLAGS)
    // asm: 	LDI	AR0,AR2
    // asm: 	CALL	OBJ_INSERTP
    // asm: 	LDL	map2,AR2
    // asm: 	CALL	OBJ_GETE
    // asm: 	STI	AR0,*+AR7(MAP2OBJ)
    // asm: 	LDI	R6,R0
    // asm: 	OR	*+AR0(OFLAGS),R0
    // asm: 	OR	O_1PAL,R0
    // asm: 	STI	AR4,*+AR0(OPAL)
    // asm: 	STI	R0,*+AR0(OFLAGS)
    // asm: 	LDI	AR0,AR2
    // asm: 	CALL	OBJ_INSERTP
    // asm: 	LDL	map3,AR2
    // asm: 	CALL	OBJ_GETE
    // asm: 	STI	AR0,*+AR7(MAP3OBJ)
    // asm: 	LDI	R6,R0
    // asm: 	OR	*+AR0(OFLAGS),R0
    // asm: 	OR	O_1PAL,R0
    // asm: 	STI	AR3,*+AR0(OPAL)
    // asm: 	STI	R0,*+AR0(OFLAGS)
    // asm: 	LDI	AR0,AR2
    // asm: 	CALL	OBJ_INSERTP
    // asm: 	LDL	map4,AR2
    // asm: 	CALL	OBJ_GETE
    // asm: 	STI	AR0,*+AR7(MAP4OBJ)
    // asm: 	LDI	R6,R0
    // asm: 	OR	*+AR0(OFLAGS),R0
    // asm: 	OR	O_1PAL,R0
    // asm: 	STI	AR4,*+AR0(OPAL)
    // asm: 	STI	R0,*+AR0(OFLAGS)
    // asm: 	LDI	AR0,AR2
    // asm: 	CALL	OBJ_INSERTP
    // asm: 	CLRF	R0
    // asm: 	STF	R0,*+AR7(MAP1T)
    // asm: 	STF	R0,*+AR7(MAP2T)
    // asm: 	STF	R0,*+AR7(MAP3T)
    // asm: 	STF	R0,*+AR7(MAP4T)
    // asm: 	CLRF	R0
    // asm: 	STF	R0,*+AR7(MAPLPX)
    // asm: 	STF	R0,*+AR7(MAPLPY)
    // asm: 	FLOAT	368,R0	;368
    // asm: 	STF	R0,*+AR7(MAPLPZ)
    // asm: 	CLRF	R2
    // asm: 	STF	R2,*+AR7(MAPLTX)
    // asm: 	LDI	AR7,AR2
    // asm: 	ADDI	MAPLMAT,AR2
    // asm: 	CALL	FIND_XMATRIX
    // asm: 	LDF	1,R0
    // asm: 	STF	R0,*+AR7(MAPXD)
    // asm: 	STF	R0,*+AR7(MAPYD)
    // asm: 	LDI	MAP_ITERATIONS-1,AR5
    // asm: FOLD_LP
    // asm: 	CALL	MAP_ILLUM_COMPUTE
    // asm: 	LDI	@EPALL,AR2
    // asm: 	CALL	PAL_FIND_RAW
#if DEBUG
    // asm: 	BNC	$
#endif
    // asm: 	LDI	@EPALR,AR2
    // asm: 	CALL	PAL_FIND_RAW
#if DEBUG
    // asm: 	BNC	$
#endif
    // asm: 	LDF	*+AR7(MAPXD),R1
    // asm: 	MPYF	1.20,R1
    // asm: 	STF	R1,*+AR7(MAPXD)
    // asm: 	LDF	*+AR7(MAPLPX),R0
    // asm: 	ADDF	R1,R0
    // asm: 	STF	R0,*+AR7(MAPLPX)
    // asm: 	LDF	*+AR7(MAPYD),R1
    // asm: 	MPYF	1.2,R1
    // asm: 	STF	R1,*+AR7(MAPYD)
    // asm: 	LDF	*+AR7(MAPLPY),R0
    // asm: 	SUBF	R1,R0
    // asm: 	STF	R0,*+AR7(MAPLPY)
    // asm: 	LDF	*+AR7(MAPLPZ),R0
    // asm: 	FLOAT	3368,R1
    // asm: 	SUBF	R0,R1
    // asm: 	MPYF	0.05,R1
    // asm: 	ADDF	R1,R0
    // asm: 	FLOAT	3368,R1
    // asm: 	CMPI	0,AR5
    // asm: 	LDFEQ	R1,R0
    // asm: 	STF	R0,*+AR7(MAPLPZ)
    // asm: 	LDF	*+AR7(MAPLTX),R2
    // asm: 	LDF	HALFPI,R0
    // asm: 	SUBF	R2,R0
    // asm: 	MPYF	0.1,R0
    // asm: 	ADDF	R0,R2
    // asm: 	LDF	HALFPI,R1
    // asm: 	CMPI	0,AR5
    // asm: 	LDFEQ	R1,R2
    // asm: 	STF	R2,*+AR7(MAPLTX)
    // asm: 	LDI	AR7,AR2
    // asm: 	ADDI	MAPLMAT,AR2
    // asm: 	CALL	FIND_XMATRIX
    // asm: 	LDI	*+AR7(MAP2OBJ),AR4
    // asm: 	LDF	*+AR7(MAPLPX),R0
    // asm: 	STF	R0,*+AR4(OPOSX)
    // asm: 	LDF	*+AR7(MAPLPY),R0
    // asm: 	STF	R0,*+AR4(OPOSY)
    // asm: 	LDF	*+AR7(MAPLPZ),R0
    // asm: 	STF	R0,*+AR4(OPOSZ)
    // asm: 	LDF	*+AR7(MAP2T),R2
    // asm: 	SUBF	@M2STDI,R2
    // asm: 	STF	R2,*+AR7(MAP2T)
    // asm: 	LDI	AR4,AR2
    // asm: 	ADDI	OMATRIX,AR2
    // asm: 	CALL	FIND_YMATRIX
    // asm: 	LDI	AR7,R2
    // asm: 	ADDI	MAPLMAT,R2
    // asm: 	LDI	AR2,R3
    // asm: 	CALL	CONCATMAT
    // 	;					OBJECT 2
    // 	;----------------------------------------------------------------------
    // asm: 	LDI	*+AR7(MAP3OBJ),AR4
    // asm: 	LDF	*+AR7(MAPLPX),R0
    // asm: 	STF	R0,*+AR4(OPOSX)
    // asm: 	LDF	*+AR7(MAPLPY),R0
    // asm: 	STF	R0,*+AR4(OPOSY)
    // asm: 	LDF	*+AR7(MAPLPZ),R0
    // asm: 	STF	R0,*+AR4(OPOSZ)
    // asm: 	LDF	*+AR7(MAP3T),R2
    // asm: 	SUBF	@M3STDI,R2
    // asm: 	STF	R2,*+AR7(MAP3T)
    // asm: 	LDI	AR4,AR2
    // asm: 	ADDI	OMATRIX,AR2
    // asm: 	CALL	FIND_YMATRIX
    // asm: 	LDI	AR7,R2
    // asm: 	ADDI	MAPLMAT,R2
    // asm: 	LDI	AR2,R3
    // asm: 	CALL	CONCATMAT
    // 	;					OBJECT 3
    // 	;----------------------------------------------------------------------
    // asm: 	LDF	*+AR7(MAP2T),R2			;MATRIX FOR TRANSLATION
    // asm: 	LDI	@MATRIXAI,AR2
    // asm: 	CALL	FIND_YMATRIX
    // asm: 	LDI	AR7,R2
    // asm: 	ADDI	MAPLMAT,R2
    // asm: 	LDI	AR2,R3
    // asm: 	CALL	CONCATMAT
    // asm: 	LDI	*+AR7(MAP1OBJ),AR4
    // asm: 	CALL	CLR_VECTORA
    // asm: 	FLOAT	-127,R0
    // asm: 	STF	R0,*+AR2(X)
    // asm: 	LDI	@MATRIXAI,R2
    // asm: 	LDI	AR4,R3
    // asm: 	ADDI	OPOSX,R3
    // asm: 	CALL	MATRIX_MUL
    // asm: 	LDF	*+AR4(OPOSX),R0
    // asm: 	ADDF	*+AR7(MAPLPX),R0
    // asm: 	STF	R0,*+AR4(OPOSX)
    // asm: 	LDF	*+AR4(OPOSY),R0
    // asm: 	ADDF	*+AR7(MAPLPY),R0
    // asm: 	STF	R0,*+AR4(OPOSY)
    // asm: 	LDF	*+AR4(OPOSZ),R0
    // asm: 	ADDF	*+AR7(MAPLPZ),R0
    // asm: 	STF	R0,*+AR4(OPOSZ)
    // asm: 	LDF	*+AR7(MAP1T),R2
    // asm: 	SUBF	@M1STDI,R2
    // asm: 	STF	R2,*+AR7(MAP1T)
    // asm: 	LDI	AR4,AR2
    // asm: 	ADDI	OMATRIX,AR2
    // asm: 	CALL	FIND_YMATRIX
    // asm: 	LDI	AR7,R2
    // asm: 	ADDI	MAPLMAT,R2
    // asm: 	LDI	AR2,R3
    // asm: 	CALL	CONCATMAT
    // 	;					OBJECT 1
    // 	;----------------------------------------------------------------------
    // asm: 	LDF	*+AR7(MAP3T),R2			;MATRIX FOR TRANSLATION
    // asm: 	LDI	@MATRIXAI,AR2
    // asm: 	CALL	FIND_YMATRIX
    // asm: 	LDI	AR7,R2
    // asm: 	ADDI	MAPLMAT,R2
    // asm: 	LDI	AR2,R3
    // asm: 	CALL	CONCATMAT
    // asm: 	LDI	*+AR7(MAP4OBJ),AR4
    // asm: 	CALL	CLR_VECTORA
    // asm: 	FLOAT	128,R0
    // asm: 	STF	R0,*+AR2(X)
    // asm: 	LDI	@MATRIXAI,R2
    // asm: 	LDI	AR4,R3
    // asm: 	ADDI	OPOSX,R3
    // asm: 	CALL	MATRIX_MUL
    // asm: 	LDF	*+AR4(OPOSX),R0
    // asm: 	ADDF	*+AR7(MAPLPX),R0
    // asm: 	STF	R0,*+AR4(OPOSX)
    // asm: 	LDF	*+AR4(OPOSY),R0
    // asm: 	ADDF	*+AR7(MAPLPY),R0
    // asm: 	STF	R0,*+AR4(OPOSY)
    // asm: 	LDF	*+AR4(OPOSZ),R0
    // asm: 	ADDF	*+AR7(MAPLPZ),R0
    // asm: 	STF	R0,*+AR4(OPOSZ)
    // asm: 	LDF	*+AR7(MAP4T),R2
    // asm: 	SUBF	@M4STDI,R2
    // asm: 	STF	R2,*+AR7(MAP4T)
    // asm: 	LDI	AR4,AR2
    // asm: 	ADDI	OMATRIX,AR2
    // asm: 	CALL	FIND_YMATRIX
    // asm: 	LDI	AR7,R2
    // asm: 	ADDI	MAPLMAT,R2
    // asm: 	LDI	AR2,R3
    // asm: 	CALL	CONCATMAT
    // 	;					OBJECT 4
    // 	;--------------------------------------------------------------------
    // asm: 	SLEEP	1
    // asm: 	DBU	AR5,FOLD_LP
    // asm: 	LDI	*+AR7(MAP1OBJ),AR2
    // asm: 	CALL	OBJ_DELETE
    // asm: 	LDI	*+AR7(MAP2OBJ),AR2
    // asm: 	CALL	OBJ_DELETE
    // asm: 	LDI	*+AR7(MAP3OBJ),AR2
    // asm: 	CALL	OBJ_DELETE
    // asm: 	LDI	*+AR7(MAP4OBJ),AR2
    // asm: 	CALL	OBJ_DELETE
    // asm: 	CALL	CLEAR_MAP_PALS
    // asm: 	DIE
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FOLDMAP", 0, 0);
    UNIMPL();
}

void CLEAR_MAP_PALS(void)
{
    // asm: 	LDI	@EPALL,AR2
    // asm: 	CALL	PAL_FIND_RAW
#if DEBUG
    // asm: 	BNC	$
#endif
    // asm: 	BNC	IBOIBO
    // asm: 	LDI	R0,AR2
    // asm: 	CALL	PAL_DELETE_RAW
IBOIBO:
    // asm: 	LDI	@EPALR,AR2
    // asm: 	CALL	PAL_FIND_RAW
#if DEBUG
    // asm: 	BNC	$
#endif
    // asm: 	RETSNC
    // asm: 	LDI	R0,AR2
    // asm: 	CALL	PAL_DELETE_RAW
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CLEAR_MAP_PALS", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void MAPPAL_ILLUM_INIT(void)
{
    // asm: 	LDI	@EPALR,AR0	;LOAD PALETTES AT
    // asm: 	LDI	@EPALL,AR1	;THE SAME TIME
    // asm: 	LDI	map1_p,AR3
    // asm: 	ADDI	@PALROMI,AR3
    // asm: 	LDI	*AR3,AR3	;NOW HOLDS RAM LOCATION
    // asm: 	LDI	*AR3++,R0
    // asm: 	STI	R0,*AR0++
    // asm: 	STI	R0,*AR1++
    // asm: 	LDI	R0,AR2
    // asm: 	AND	0FFFh,AR2
    // asm: 	RS	1,AR2
    // asm: 	SUBI	1,AR2
L342:
    // asm: LDI	*AR3++,R0
    // asm: 	STI	R0,*AR0++
    // asm: 	STI	R0,*AR1++
    // asm: 	DBU	AR2,L342
    // asm: 	LDI	@EPALR,AR2
    // asm: 	CALL	PAL_ALLOC_RAW
    // asm: 	LDI	@EPALL,AR2
    // asm: 	CALL	PAL_ALLOC_RAW
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MAPPAL_ILLUM_INIT", 0, 0);
    UNIMPL();
}

void MAP_ILLUM_COMPUTE(void)
{
    // asm: 	LDF	*+AR7(MAP1T),R0
    // asm: 	ADDF	HALFPI,R0
    // asm: 	MPYF	@FORMULA1,R0
    // asm: 	SUBRF	1.0,R0
    // asm: 	MPYF	0.7,R0
    // asm: 	ADDF	0.3,R0
    // asm: 	STF	R0,@MAPPAL13
    // asm: 	LDF	*+AR7(MAP2T),R0
    // asm: 	ADDF	HALFPI,R0
    // asm: 	MPYF	@FORMULA1,R0
    // asm: 	SUBRF	1.0,R0
    // asm: 	MPYF	0.7,R0
    // asm: 	ADDF	0.3,R0
    // asm: 	STF	R0,@MAPPAL24
    // asm: 	CALL	MAPPAL_ILLUM
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MAP_ILLUM_COMPUTE", 0, 0);
    UNIMPL();
}

void MAPPAL_ILLUM(void)
{
    // asm: 	PUSH	AR0
    // asm: 	PUSH	AR1
    // asm: 	PUSH	AR2
    // asm: 	PUSH	AR3
    // asm: 	PUSH	AR4
    // asm: 	PUSH	AR5
    // asm: 	PUSH	R0
    // asm: 	PUSH	R1
    // asm: 	PUSH	R2
    // asm: 	PUSH	R3
    // asm: 	PUSH	R4
    // asm: 	PUSH	R5
    // asm: 	PUSHFL	R6
    // asm: 	LDF	@MAPPAL13,R6	;1st multiplier
    // asm: 	LDI	@EPALL,AR0
    // asm: 	LDI	map1_p,AR3
    // asm: 	ADDI	@PALROMI,AR3
    // asm: 	LDI	*AR3,AR3	;NOW HOLDS RAM LOCATION
    // asm: 	LDI	*AR3++,R0
    // asm: 	STI	R0,*AR0++
    // asm: 	LDI	R0,AR5
    // asm: 	LS	1,AR5
    // asm: 	RS	2,AR5
    // asm: L894
    // asm: 	LDI	*AR3++,R4	;get src2 (TRUE)
    // asm: 	LDI	R4,R0
    // asm: 	AND	01Fh,R0
    // asm: 	LS	3,R0
    // asm: 	FLOAT	R0
    // asm: 	MPYF	R6,R0
    // asm: 	FIX	R0
    // asm: 	RS	3,R0
    // asm: 	AND	01Fh,R0
    // asm: 	LDI	R4,R1
    // asm: 	RS	5,R1
    // asm: 	AND	01Fh,R1
    // asm: 	LS	3,R1
    // asm: 	FLOAT	R1
    // asm: 	MPYF	R6,R1
    // asm: 	FIX	R1
    // asm: 	RS	3,R1
    // asm: 	AND	01Fh,R1
    // asm: 	LS	5,R1
    // asm: 	OR	R1,R0
    // asm: 	LDI	R4,R1
    // asm: 	RS	10,R1
    // asm: 	AND	01Fh,R1
    // asm: 	LS	3,R1
    // asm: 	FLOAT	R1
    // asm: 	MPYF	R6,R1
    // asm: 	FIX	R1
    // asm: 	RS	3,R1
    // asm: 	AND	01Fh,R1
    // asm: 	LS	10,R1
    // asm: 	OR	R1,R0
    // 	;second word
    // 	;
    // asm: 	RS	16,R4
    // asm: 	LDI	R4,R2
    // asm: 	AND	01Fh,R2
    // asm: 	LS	3,R2
    // asm: 	FLOAT	R2
    // asm: 	MPYF	R6,R2
    // asm: 	FIX	R2
    // asm: 	RS	3,R2
    // asm: 	AND	01Fh,R2
    // asm: 	LDI	R4,R3
    // asm: 	RS	5,R3
    // asm: 	AND	01Fh,R3
    // asm: 	LS	3,R3
    // asm: 	FLOAT	R3
    // asm: 	MPYF	R6,R3
    // asm: 	FIX	R3
    // asm: 	RS	3,R3
    // asm: 	AND	01Fh,R3
    // asm: 	LS	5,R3
    // asm: 	OR	R3,R2
    // asm: 	LDI	R4,R3
    // asm: 	RS	10,R3
    // asm: 	AND	01Fh,R3
    // asm: 	LS	3,R3
    // asm: 	FLOAT	R3
    // asm: 	MPYF	R6,R3
    // asm: 	FIX	R3
    // asm: 	RS	3,R3
    // asm: 	AND	01Fh,R3
    // asm: 	LS	10,R3
    // asm: 	OR	R3,R2
    // asm: 	LS	16,R2
    // asm: 	OR	R2,R0
    // asm: 	STI	R0,*AR0++
    // asm: 	DBU	AR5,L894
    // asm: 	LDI	@EPALL,AR2
    // asm: 	CALL	PAL_FIND_RAW
    // asm: 	CMPI	-1,R0
    // asm: 	BEQ	JAJA4
    // asm: 	LDI	*AR2++,R3
    // asm: 	LDI	R0,R2
    // asm: 	CALL	PAL_SET
JAJA4:
    // *
    // *
    // *
    // *
    // *
    // asm: 	LDF	@MAPPAL24,R6	;2nd multiplier
    // asm: 	LDI	@EPALR,AR0
    // asm: 	LDI	map1_p,AR3
    // asm: 	ADDI	@PALROMI,AR3
    // asm: 	LDI	*AR3,AR3	;NOW HOLDS RAM LOCATION
    // asm: 	LDI	*AR3++,R0
    // asm: 	STI	R0,*AR0++
    // asm: 	LDI	R0,AR5
    // asm: 	LS	1,AR5
    // asm: 	RS	2,AR5
    // asm: L894D
    // asm: 	LDI	*AR3++,R4	;get src2 (TRUE)
    // asm: 	LDI	R4,R0
    // asm: 	AND	01Fh,R0
    // asm: 	LS	3,R0
    // asm: 	FLOAT	R0
    // asm: 	MPYF	R6,R0
    // asm: 	FIX	R0
    // asm: 	RS	3,R0
    // asm: 	AND	01Fh,R0
    // asm: 	LDI	R4,R1
    // asm: 	RS	5,R1
    // asm: 	AND	01Fh,R1
    // asm: 	LS	3,R1
    // asm: 	FLOAT	R1
    // asm: 	MPYF	R6,R1
    // asm: 	FIX	R1
    // asm: 	RS	3,R1
    // asm: 	AND	01Fh,R1
    // asm: 	LS	5,R1
    // asm: 	OR	R1,R0
    // asm: 	LDI	R4,R1
    // asm: 	RS	10,R1
    // asm: 	AND	01Fh,R1
    // asm: 	LS	3,R1
    // asm: 	FLOAT	R1
    // asm: 	MPYF	R6,R1
    // asm: 	FIX	R1
    // asm: 	RS	3,R1
    // asm: 	AND	01Fh,R1
    // asm: 	LS	10,R1
    // asm: 	OR	R1,R0
    // 	;second word
    // 	;
    // asm: 	RS	16,R4
    // asm: 	LDI	R4,R2
    // asm: 	AND	01Fh,R2
    // asm: 	LS	3,R2
    // asm: 	FLOAT	R2
    // asm: 	MPYF	R6,R2
    // asm: 	FIX	R2
    // asm: 	RS	3,R2
    // asm: 	AND	01Fh,R2
    // asm: 	LDI	R4,R3
    // asm: 	RS	5,R3
    // asm: 	AND	01Fh,R3
    // asm: 	LS	3,R3
    // asm: 	FLOAT	R3
    // asm: 	MPYF	R6,R3
    // asm: 	FIX	R3
    // asm: 	RS	3,R3
    // asm: 	AND	01Fh,R3
    // asm: 	LS	5,R3
    // asm: 	OR	R3,R2
    // asm: 	LDI	R4,R3
    // asm: 	RS	10,R3
    // asm: 	AND	01Fh,R3
    // asm: 	LS	3,R3
    // asm: 	FLOAT	R3
    // asm: 	MPYF	R6,R3
    // asm: 	FIX	R3
    // asm: 	RS	3,R3
    // asm: 	AND	01Fh,R3
    // asm: 	LS	10,R3
    // asm: 	OR	R3,R2
    // asm: 	LS	16,R2
    // asm: 	OR	R2,R0
    // asm: 	STI	R0,*AR0++
    // asm: 	DBU	AR5,L894D
    // asm: 	LDI	@EPALR,AR2
    // asm: 	CALL	PAL_FIND_RAW
    // asm: 	CMPI	-1,R0
    // asm: 	BEQ	JAJA5
    // asm: 	LDI	*AR2++,R3
    // asm: 	LDI	R0,R2
    // asm: 	CALL	PAL_SET
JAJA5:
    // asm: 	POPFL	R6
    // asm: 	POP	R5
    // asm: 	POP	R4
    // asm: 	POP	R3
    // asm: 	POP	R2
    // asm: 	POP	R1
    // asm: 	POP	R0
    // asm: 	POP	AR5
    // asm: 	POP	AR4
    // asm: 	POP	AR3
    // asm: 	POP	AR2
    // asm: 	POP	AR1
    // asm: 	POP	AR0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MAPPAL_ILLUM", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
*TIME2STR	CONVERT TIME CODE TO STRING
*
*
*PARAMETERS
*	R0	TIME (IN 1/60 SECONDS)
*	AR2	STRING SPACE
*
 */
void TIME2STR(void)
{
    // asm: 	PUSH	R0
    // asm: 	PUSH	R1
    // asm: 	PUSH	R2
    // asm: 	PUSH	AR0
    // asm: 	PUSH	AR1
    // asm: 	PUSH	AR2
    // asm: 	CALL	CVTTIME
    // asm: 	PUSH	R0
    // asm: 	PUSH	R1
    // asm: 	CALL	_itoa
    // asm: 	LDI	AR2,AR0
    // asm: 	LDI	@COLONI,AR1
    // asm: 	CALL	STRCAT
    // asm: 	POP	R2
    // asm: 	LDI	@tmp_bufferI,AR2
    // asm: 	CALL	_itoaLZ
    // asm: 	LDI	AR2,AR1
    // asm: 	CALL	STRCAT
    // asm: 	LDI	@COLONI,AR1
    // asm: 	CALL	STRCAT
    // asm: 	POP	R2
    // asm: 	LDI	@tmp_bufferI,AR2
    // asm: 	CALL	_itoaLZ
    // asm: 	LDI	AR2,AR1
    // asm: 	CALL	STRCAT
    // asm: 	LDI	AR0,AR2
    // asm: 	POP	AR2
    // asm: 	POP	AR1
    // asm: 	POP	AR0
    // asm: 	POP	R2
    // asm: 	POP	R1
    // asm: 	POP	R0
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TIME2STR", 0, 0);
    UNIMPL();
}

void CVTTIME(void)
{
    // asm: 	PUSH	R3
    // asm: 	PUSHF	R3
    // asm: 	FLOAT	R0,R3
    // asm: 	MPYF	@MINFACT,R3
    // asm: 	FIX	R3,R3
    // asm: 	CEILI	99,R3
    // asm: 	FLOORI	0,R3
    // asm: 	LDI	R3,R2			;MINUTES
    // asm: 	MPYI	3300,R3
    // asm: 	SUBI 	R3,R0
    // asm: 	LDILT	0,R0
    // asm: 	FLOAT	R0,R3
    // asm: 	MPYF	@SECFACT,R3
    // asm: 	FIX	R3,R3
    // asm: 	CEILI	59,R3
    // asm: 	FLOORI	0,R3
    // asm: 	LDI	R3,R1			;SECONDS
    // asm: 	MPYI	55,R3
    // asm: 	SUBI 	R3,R0
    // asm: 	LDILT	0,R0
    // asm: 	FLOAT	R0,R3
    // asm: 	MPYF	@HUNFACT,R3
    // asm: 	FIX	R3,R0			;HUNDRETHS
    // asm: 	CEILI	99,R0
    // asm: 	FLOORI	0,R0
    // asm: 	POPF	R3
    // asm: 	POP	R3
    // asm: 	RETS
    // ;*THIS CHECKS CVTTIME FOR MONOTONICITY
    // ;CHEK
    // ;	LDI	0,R5
    // ;	LDI	-1,R4
    // ;CKLP
    // ;	LDI	R5,R0
    // ;	CALL	CVTTIME
    // ;	MPYI	100,R1
    // ;	MPYI	6000,R2
    // ;	ADDI	R0,R1
    // ;	ADDI	R1,R2
    // ;	CMPI	R2,R4
    // ;	BGE	$
    // ;	LDI	R2,R4
    // ;	ADDI	1,R5
    // ;	CMPI	32000,R5
    // ;	BLT	CKLP
    // ;	B	$
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CVTTIME", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void RADAR_PLOT(void)
{
    // asm: 	PUSH	AR4
    // asm: 	LDI	@PLYCAR,AR4
    // 	;background
    // asm: 	LDI	CC|DITHER|8,R0
    // asm: 	STI	R0,@_ACNTL
    // asm: 	LDI	RADAR_XMIN,R0
    // asm: 	ADDI	@MOVEIN_OFFSET,R0
    // asm: 	STI	R0,@(_ARPS+(0*3))
    // asm: 	STI	R0,@(_ARPS+(3*3))
    // asm: 	LDI	RADAR_XMAX,R0
    // asm: 	ADDI	@MOVEIN_OFFSET,R0
    // asm: 	STI	R0,@(_ARPS+(1*3))
    // asm: 	STI	R0,@(_ARPS+(2*3))
    // asm: 	LDI	RADAR_YMIN,R0
    // asm: 	STI	R0,@(_ARPS+(0*3)+1)
    // asm: 	STI	R0,@(_ARPS+(1*3)+1)
    // asm: 	LDI	RADAR_YMAX,R0
    // asm: 	STI	R0,@(_ARPS+(3*3)+1)
    // asm: 	STI	R0,@(_ARPS+(2*3)+1)
    // asm: 	CLRI	R0
    // asm: 	STI	R0,@_ADDRL
    // asm: 	CLRI	R0
    // asm: 	STI	R0,@_ACMAP
    // asm: 	CALL	_stuff_fpga
    // asm: 	LDP	@(_CAMERARAD+Y)
    // asm: 	LDF	@(_CAMERARAD+Y),R2
    // asm: 	SETDP
    // asm: 	LDI	@MATRIXAI,AR2
    // asm: 	CALL	FIND_YMATRIX
    // asm: 	LDI	@CAR_LISTI,AR0
    // asm: 	SUBI	OLINK3,AR0
RADAR_LP:
    // asm: 	LDI	*+AR0(OLINK3),R0
    // asm: 	BEQ	RADAR_X
    // asm: 	LDI	R0,AR0
    // asm: 	LDI	CC|3,R0			;red (RACER)
    // asm: 	LDI	*+AR0(OID),R1
    // asm: 	CMPI	DRONE_C|VEHICLE_T|DRNE_RACER,R1
    // asm: 	LDINE	CC|0Ch,R0		;light gray (general racer)
    // asm: 	STI	R0,@_ACNTL
    // asm: 	CALL	CLR_VECTORA
    // asm: 	LDF	*+AR0(OPOSX),R0
    // asm: 	SUBF	*+AR4(OPOSX),R0
    // asm: 	LDLF	0.003,R1
    // asm: 	MPYF	R1,R0
    // asm: 	STF	R0,*+AR2(X)
    // asm: 	LDF	*+AR0(OPOSZ),R0
    // asm: 	SUBF	*+AR4(OPOSZ),R0
    // asm: 	LDLF	0.003,R1
    // asm: 	MPYF	R1,R0
    // asm: 	STF	R0,*+AR2(Z)
    // asm: 	LDI	@MATRIXAI,R2
    // asm: 	LDI	AR2,R3
    // asm: 	CALL	MATRIX_MUL
    // asm: 	LDF	*+AR2(X),R0
    // asm: 	FIX	R0
    // asm: 	ADDI	RADAR_XCNTR-2,R0
    // 	;
    // 	;
    // 	;
    // 	;
    // 	;
    // asm: 	LDI	@HEAD2HEAD_ON,R2		;are we playing HEAD2HEAD?
    // asm: 	BZ	NOTTHEOPLYR
    // asm: 	CMPI	@PLY2CAR,AR0		;is this the other PLAYER?
    // asm: 	BNE	NOTTHEOPLYR
    // asm: 	CLRI	R6
    // asm: 	CMPI	RADAR_XMIN,R0
    // asm: 	LDILT	RADAR_XMIN,R0
    // asm: 	LDILT	1,R6
    // asm: 	CMPI	RADAR_XMAX,R0
    // asm: 	LDIGT	RADAR_XMAX,R0
    // asm: 	LDIGT	1,R6
    // asm: 	ADDI	@MOVEIN_OFFSET,R0
    // asm: 	STI	R0,@(_ARPS+(0*3))
    // asm: 	STI	R0,@(_ARPS+(3*3))
    // asm: 	ADDI	3,R0
    // asm: 	STI	R0,@(_ARPS+(1*3))
    // asm: 	STI	R0,@(_ARPS+(2*3))
    // asm: 	LDF	*+AR2(Z),R0
    // asm: 	MPYF	0.3,R0
    // asm: 	FIX	R0
    // asm: 	NEGI	R0
    // asm: 	ADDI	RADAR_YCNTR-2,R0
    // asm: 	CMPI	RADAR_YMIN,R0
    // asm: 	LDILT	RADAR_YMIN,R0
    // asm: 	LDILT	1,R6
    // asm: 	CMPI	RADAR_YMAX,R0
    // asm: 	LDIGT	RADAR_YMAX,R0
    // asm: 	LDIGT	1,R6
    // asm: 	STI	R0,@(_ARPS+(0*3)+1)
    // asm: 	STI	R0,@(_ARPS+(1*3)+1)
    // asm: 	ADDI	3,R0
    // asm: 	STI	R0,@(_ARPS+(3*3)+1)
    // asm: 	STI	R0,@(_ARPS+(2*3)+1)
    // asm: 	LDL	H2HPAL1,AR2
    // asm: 	CALL	PAL_FIND_RAW
    // asm: 	STI	R0,@_ACMAP
    // asm: 	LDI	TM|ZS,R0		;red (RACER)
    // asm: 	STI	R0,@_ACNTL
    // asm: 	LDI	@DIPRAM,R0
    // asm: 	TSTB	CMDP_MASTER,R0
    // asm: 	BZ	GL14
    // asm: 	LDL	h2p1a_I,R0
    // asm: 	STI	R0,@_ADDRL
    // asm: 	LDI	@_ARPS+(0*3)+1,R0
    // asm: 	SUBI	16,R0
    // asm: 	STI	R0,@_ARPS+(0*3)+1
    // asm: 	LDI	@_ARPS+(1*3)+1,R0
    // asm: 	SUBI	16,R0
    // asm: 	STI	R0,@_ARPS+(1*3)+1
    // asm: 	LDI	@_ARPS+(0*3),R0
    // asm: 	SUBI	13,R0
    // asm: 	STI	R0,@_ARPS+(0*3)
    // asm: 	LDI	@_ARPS+(3*3),R0
    // asm: 	SUBI	13,R0
    // asm: 	STI	R0,@_ARPS+(3*3)
    // asm: 	LDI	00038h,R0
    // asm: 	STI	R0,@_AIVI
    // asm: 	LDI	00041h,R0
    // asm: 	STI	R0,@_AIVI+1
    // asm: 	LDI	00D41h,R0
    // asm: 	STI	R0,@_AIVI+2
    // asm: 	LDI	00D38h,R0
    // asm: 	STI	R0,@_AIVI+3
    // asm: 	BU	GL15
GL14:
    // asm: LDL	h2p2b_I,R0
    // asm: 	STI	R0,@_ADDRL
    // asm: 	LDI	@_ARPS+(0*3)+1,R0
    // asm: 	SUBI	16,R0
    // asm: 	STI	R0,@_ARPS+(0*3)+1
    // asm: 	LDI	@_ARPS+(1*3)+1,R0
    // asm: 	SUBI	16,R0
    // asm: 	STI	R0,@_ARPS+(1*3)+1
    // asm: 	LDI	@_ARPS+(0*3),R0
    // asm: 	SUBI	13,R0
    // asm: 	STI	R0,@_ARPS+(0*3)
    // asm: 	LDI	@_ARPS+(3*3),R0
    // asm: 	SUBI	13,R0
    // asm: 	STI	R0,@_ARPS+(3*3)
    // asm: 	LDI	00041h,R0
    // asm: 	STI	R0,@_AIVI
    // asm: 	LDI	0004Bh,R0
    // asm: 	STI	R0,@_AIVI+1
    // asm: 	LDI	00D4Bh,R0
    // asm: 	STI	R0,@_AIVI+2
    // asm: 	LDI	00D41h,R0
    // asm: 	STI	R0,@_AIVI+3
GL15:
    // asm: 	CMPI	0,R6
    // asm: 	BEQ	DADA4
    // 	;we have a case where the other player is either behind us
    // 	;or in front of us
    // 	;
    // asm: 	LDI	*+AR0(OCARBLK),AR1
    // asm: 	LDI	*+AR1(CAR_OM),R0
#if DEBUG
    // asm: 	BZ	$	;debugging please remove
#endif
    // asm: 	LDI	*+AR1(CARTRACK_ID),R0
    // asm: 	LDI	@PLYCBLK,AR2
    // asm: 	LDI	*+AR2(CARTRAK),AR2
    // asm: 	LDI	*+AR2(OUSR1),R1
    // asm: 	CMPI	R1,R0
    // asm: 	BLT	ISBEHIND
    // ;	BLE	ISBEHIND
    // ;	LDI	RADAR_XCNTR,R0
    // ;	ADDI	9,R0
    // ;	STI	R0,@_ARPS+(1*3)
    // ;	STI	R0,@_ARPS+(2*3)
    // ;	SUBI	18,R0
    // ;	STI	R0,@_ARPS+(0*3)
    // ;	STI	R0,@_ARPS+(3*3)
    // asm: 	LDI	RADAR_YMIN,R0
    // asm: 	ADDI	10,R0
    // asm: 	STI	R0,@_ARPS+(2*3)+1
    // asm: 	STI	R0,@_ARPS+(3*3)+1
    // asm: 	SUBI	20,R0
    // asm: 	STI	R0,@_ARPS+(0*3)+1
    // asm: 	STI	R0,@_ARPS+(1*3)+1
    // asm: 	BU	DADA4
ISBEHIND:
    // ;	LDI	RADAR_XCNTR,R0
    // ;	ADDI	9,R0
    // ;	STI	R0,@_ARPS+(1*3)
    // ;	STI	R0,@_ARPS+(2*3)
    // ;	SUBI	18,R0
    // ;	STI	R0,@_ARPS+(0*3)
    // ;	STI	R0,@_ARPS+(3*3)
    // asm: 	LDI	RADAR_YMAX,R0
    // asm: 	ADDI	10,R0
    // asm: 	STI	R0,@_ARPS+(2*3)+1
    // asm: 	STI	R0,@_ARPS+(3*3)+1
    // asm: 	SUBI	20,R0
    // asm: 	STI	R0,@_ARPS+(0*3)+1
    // asm: 	STI	R0,@_ARPS+(1*3)+1
DADA4:
    // asm: 	CALL	_stuff_fpga
    // asm: 	CLRI	R0
    // asm: 	STI	R0,@_ACMAP
    // asm: 	BU	RADAR_LP
NOTTHEOPLYR:
    // asm: 	CMPI	RADAR_XMIN,R0
    // asm: 	BLT	RADAR_LP
    // asm: 	CMPI	RADAR_XMAX,R0
    // asm: 	BGT	RADAR_LP
    // asm: 	ADDI	@MOVEIN_OFFSET,R0
    // asm: 	STI	R0,@(_ARPS+(0*3))
    // asm: 	STI	R0,@(_ARPS+(3*3))
    // asm: 	ADDI	3,R0
    // asm: 	STI	R0,@(_ARPS+(1*3))
    // asm: 	STI	R0,@(_ARPS+(2*3))
    // asm: 	LDF	*+AR2(Z),R0
    // asm: 	MPYF	0.3,R0
    // asm: 	FIX	R0
    // asm: 	NEGI	R0
    // asm: 	ADDI	RADAR_YCNTR-2,R0
    // asm: 	CMPI	RADAR_YMIN,R0
    // asm: 	BLT	RADAR_LP
    // asm: 	CMPI	RADAR_YMAX,R0
    // asm: 	BGT	RADAR_LP
    // asm: 	STI	R0,@(_ARPS+(0*3)+1)
    // asm: 	STI	R0,@(_ARPS+(1*3)+1)
    // asm: 	ADDI	3,R0
    // asm: 	STI	R0,@(_ARPS+(3*3)+1)
    // asm: 	STI	R0,@(_ARPS+(2*3)+1)
    // asm: 	CALL	_stuff_fpga
    // asm: 	BU	RADAR_LP
RADAR_X:
    // asm: 	LDI	CC|1,R0
    // asm: 	STI	R0,@_ACNTL
    // asm: 	LDI	RADAR_XCNTR-2,R0
    // asm: 	ADDI	@MOVEIN_OFFSET,R0
    // asm: 	STI	R0,@(_ARPS+(0*3))
    // asm: 	STI	R0,@(_ARPS+(3*3))
    // asm: 	LDI	RADAR_XCNTR+1,R0
    // asm: 	ADDI	@MOVEIN_OFFSET,R0
    // asm: 	STI	R0,@(_ARPS+(1*3))
    // asm: 	STI	R0,@(_ARPS+(2*3))
    // asm: 	LDI	RADAR_YCNTR-2,R0
    // asm: 	STI	R0,@(_ARPS+(0*3)+1)
    // asm: 	STI	R0,@(_ARPS+(1*3)+1)
    // asm: 	LDI	RADAR_YCNTR+1,R0
    // asm: 	STI	R0,@(_ARPS+(3*3)+1)
    // asm: 	STI	R0,@(_ARPS+(2*3)+1)
    // asm: 	CALL	_stuff_fpga
    // asm: 	POP	AR4
    // asm: 	LDI	@HEAD2HEAD_ON,R0
    // asm: 	BZ	NODOAP
    // asm: 	LDI	@PLY2CAR,AR0
    // asm: 	LDI	*+AR0(OCARBLK),AR0
    // asm: 	LDI	@PLYCBLK,AR1
    // asm: 	LDI	*+AR0(CARTRACK_ID),R0
    // asm: 	LDI	*+AR1(CARTRACK_ID),R1
    // asm: 	CMPI	R0,R1
    // asm: 	LDIGT	1,R0
    // asm: 	LDILE	0,R0
    // asm: 	STI	R0,@THIS_MACHINE_AHEAD
NODOAP:
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RADAR_PLOT", 0, 0);
    UNIMPL();
}
