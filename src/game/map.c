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
#include "port.h"
#include "map.h"

/*
 * Source module: asm/MAP.ASM
 */

void UNFOLDMAP(void);
void UNFOLDMAP_NOPAL(void);
void FOLDMAP(void);
void CLEAR_MAP_PALS(void);
void MAPPAL_ILLUM_INIT(void);
void MAP_ILLUM_COMPUTE(void);
void MAPPAL_ILLUM(void);
void TIME2STR(void);
void CVTTIME(void);
void RADAR_PLOT(void);

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

/* *
*
*
*
*
*
 */
void UNFOLDMAP(void)
{
    // asm 00005E0B: 	CALL	MAPPAL_ILLUM_INIT
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "UNFOLDMAP", 0, 0);
    UNIMPL();
}

void UNFOLDMAP_NOPAL(void)
{
    // asm 00005E0C: 	LDIL	O_3DROT,R6
    // asm 00005E0F: 	OR	O_NOUROT|O_NOUNIV,R6
    // asm 00005E10: 	LDI	@EPALL,AR2
    // asm 00005E11: 	CALL	PAL_FIND_RAW
#if DEBUG
    // asm: 	BNC	$
#endif
    // asm 00005E12: 	LDI	R0,AR3
    // asm 00005E13: 	LDI	@EPALR,AR2
    // asm 00005E14: 	CALL	PAL_FIND_RAW
#if DEBUG
    // asm: 	BNC	$
#endif
    // asm 00005E15: 	LDI	R0,AR4
    // asm 00005E16: 	LDL	map1,AR2
    // asm 00005E17: 	CALL	OBJ_GETE
    // asm 00005E18: 	STI	AR0,*+AR7(MAP1OBJ)
    // asm 00005E19: 	LDI	R6,R0
    // asm 00005E1A: 	OR	*+AR0(OFLAGS),R0
    // asm 00005E1B: 	OR	O_1PAL,R0
    // asm 00005E1C: 	STI	AR3,*+AR0(OPAL)
    // asm 00005E1D: 	STI	R0,*+AR0(OFLAGS)
    // asm 00005E1E: 	LDI	AR0,AR2
    // asm 00005E1F: 	CALL	OBJ_INSERTP
    // asm 00005E20: 	LDL	map2,AR2
    // asm 00005E21: 	CALL	OBJ_GETE
    // asm 00005E22: 	STI	AR0,*+AR7(MAP2OBJ)
    // asm 00005E23: 	LDI	R6,R0
    // asm 00005E24: 	OR	*+AR0(OFLAGS),R0
    // asm 00005E25: 	OR	O_1PAL,R0
    // asm 00005E26: 	STI	AR4,*+AR0(OPAL)
    // asm 00005E27: 	STI	R0,*+AR0(OFLAGS)
    // asm 00005E28: 	LDI	AR0,AR2
    // asm 00005E29: 	CALL	OBJ_INSERTP
    // asm 00005E2A: 	LDL	map3,AR2
    // asm 00005E2B: 	CALL	OBJ_GETE
    // asm 00005E2C: 	STI	AR0,*+AR7(MAP3OBJ)
    // asm 00005E2D: 	LDI	R6,R0
    // asm 00005E2E: 	OR	*+AR0(OFLAGS),R0
    // asm 00005E2F: 	OR	O_1PAL,R0
    // asm 00005E30: 	STI	AR3,*+AR0(OPAL)
    // asm 00005E31: 	STI	R0,*+AR0(OFLAGS)
    // asm 00005E32: 	LDI	AR0,AR2
    // asm 00005E33: 	CALL	OBJ_INSERTP
    // asm 00005E34: 	LDL	map4,AR2
    // asm 00005E35: 	CALL	OBJ_GETE
    // asm 00005E36: 	STI	AR0,*+AR7(MAP4OBJ)
    // asm 00005E37: 	LDI	R6,R0
    // asm 00005E38: 	OR	*+AR0(OFLAGS),R0
    // asm 00005E39: 	OR	O_1PAL,R0
    // asm 00005E3A: 	STI	AR4,*+AR0(OPAL)
    // asm 00005E3B: 	STI	R0,*+AR0(OFLAGS)
    // asm 00005E3C: 	LDI	AR0,AR2
    // asm 00005E3D: 	CALL	OBJ_INSERTP
    // asm 00005E3E: 	LDF	M1ST,R0
    // asm 00005E3F: 	STF	R0,*+AR7(MAP1T)
    // asm 00005E40: 	LDF	M2ST,R0
    // asm 00005E41: 	STF	R0,*+AR7(MAP2T)
    // asm 00005E42: 	LDF	M3ST,R0
    // asm 00005E43: 	STF	R0,*+AR7(MAP3T)
    // asm 00005E44: 	LDF	M4ST,R0
    // asm 00005E45: 	STF	R0,*+AR7(MAP4T)
    // asm 00005E46: 	CLRF	R0
    // asm 00005E47: 	STF	R0,*+AR7(MAPLPX)
    // asm 00005E48: 	STF	R0,*+AR7(MAPLPY)
    // asm 00005E49: 	FLOAT	3368,R0	;368
    // asm 00005E4A: 	STF	R0,*+AR7(MAPLPZ)
    // asm 00005E4B: 	LDF	HALFPI,R2
    // asm 00005E4C: 	STF	R2,*+AR7(MAPLTX)
    // asm 00005E4D: 	LDI	AR7,AR2
    // asm 00005E4E: 	ADDI	MAPLMAT,AR2
    // asm 00005E4F: 	CALL	FIND_XMATRIX
    // asm 00005E50: 	LDF	0.2,R0
    // asm 00005E51: 	STF	R0,@MAPPAL24
    // asm 00005E52: 	LDF	0.6,R0
    // asm 00005E53: 	STF	R0,@MAPPAL13
    // asm 00005E54: 	LDI	MAP_ITERATIONS-1,AR5
    // asm 00005E55: UNFOLD_LP
    // ;	CALL	MAP_ILLUM_COMPUTE
    // asm 00005E55: 	LDF	@MAPPAL24,R0
    // asm 00005E56: 	ADDF	0.0266,R0
    // asm 00005E57: 	STF	R0,@MAPPAL24
    // asm 00005E58: 	LDF	@MAPPAL13,R0
    // asm 00005E59: 	ADDF	0.0133,R0
    // asm 00005E5A: 	STF	R0,@MAPPAL13
    // asm 00005E5B: 	CALL	MAPPAL_ILLUM
    // asm 00005E5C: 	LDF	*+AR7(MAPLPZ),R0
    // asm 00005E5D: 	FLOAT	368,R1
    // asm 00005E5E: 	SUBF	R0,R1
    // asm 00005E5F: 	MPYF	0.25,R1
    // asm 00005E60: 	ADDF	R1,R0
    // asm 00005E61: 	FLOAT	368,R1
    // asm 00005E62: 	CMPI	0,AR5
    // asm 00005E63: 	LDFEQ	R1,R0
    // asm 00005E64: 	STF	R0,*+AR7(MAPLPZ)
    // asm 00005E65: 	LDF	*+AR7(MAPLTX),R2
    // asm 00005E66: 	MPYF	0.9,R2
    // asm 00005E67: 	CLRF	R1
    // asm 00005E68: 	CMPI	0,AR5
    // asm 00005E69: 	LDFEQ	R1,R2
    // asm 00005E6A: 	STF	R2,*+AR7(MAPLTX)
    // asm 00005E6B: 	LDI	AR7,AR2
    // asm 00005E6C: 	ADDI	MAPLMAT,AR2
    // asm 00005E6D: 	CALL	FIND_XMATRIX
    // asm 00005E6E: 	LDI	*+AR7(MAP2OBJ),AR4
    // asm 00005E6F: 	LDF	*+AR7(MAPLPX),R0
    // asm 00005E70: 	STF	R0,*+AR4(OPOSX)
    // asm 00005E71: 	LDF	*+AR7(MAPLPY),R0
    // asm 00005E72: 	STF	R0,*+AR4(OPOSY)
    // asm 00005E73: 	LDF	*+AR7(MAPLPZ),R0
    // asm 00005E74: 	STF	R0,*+AR4(OPOSZ)
    // asm 00005E75: 	LDF	*+AR7(MAP2T),R2
    // asm 00005E76: 	ADDF	@M2STDI,R2
    // asm 00005E77: 	STF	R2,*+AR7(MAP2T)
    // asm 00005E78: 	LDI	AR4,AR2
    // asm 00005E79: 	ADDI	OMATRIX,AR2
    // asm 00005E7A: 	CALL	FIND_YMATRIX
    // asm 00005E7B: 	LDI	AR7,R2
    // asm 00005E7C: 	ADDI	MAPLMAT,R2
    // asm 00005E7D: 	LDI	AR2,R3
    // asm 00005E7E: 	CALL	CONCATMAT
    // asm 00005E7F: 	LDI	*+AR7(MAP3OBJ),AR4
    // asm 00005E80: 	LDF	*+AR7(MAPLPX),R0
    // asm 00005E81: 	STF	R0,*+AR4(OPOSX)
    // asm 00005E82: 	LDF	*+AR7(MAPLPY),R0
    // asm 00005E83: 	STF	R0,*+AR4(OPOSY)
    // asm 00005E84: 	LDF	*+AR7(MAPLPZ),R0
    // asm 00005E85: 	STF	R0,*+AR4(OPOSZ)
    // asm 00005E86: 	LDF	*+AR7(MAP3T),R2
    // asm 00005E87: 	ADDF	@M3STDI,R2
    // asm 00005E88: 	STF	R2,*+AR7(MAP3T)
    // asm 00005E89: 	LDI	AR4,AR2
    // asm 00005E8A: 	ADDI	OMATRIX,AR2
    // asm 00005E8B: 	CALL	FIND_YMATRIX
    // asm 00005E8C: 	LDI	AR7,R2
    // asm 00005E8D: 	ADDI	MAPLMAT,R2
    // asm 00005E8E: 	LDI	AR2,R3
    // asm 00005E8F: 	CALL	CONCATMAT
    // asm 00005E90: 	LDF	*+AR7(MAP2T),R2
    // asm 00005E91: 	LDI	@MATRIXAI,AR2
    // asm 00005E92: 	CALL	FIND_YMATRIX
    // asm 00005E93: 	LDI	AR7,R2
    // asm 00005E94: 	ADDI	MAPLMAT,R2
    // asm 00005E95: 	LDI	AR2,R3
    // asm 00005E96: 	CALL	CONCATMAT
    // asm 00005E97: 	LDI	*+AR7(MAP1OBJ),AR4
    // asm 00005E98: 	CALL	CLR_VECTORA
    // asm 00005E99: 	FLOAT	-127,R0
    // asm 00005E9A: 	STF	R0,*+AR2(X)
    // asm 00005E9B: 	LDI	@MATRIXAI,R2
    // asm 00005E9C: 	LDI	AR4,R3
    // asm 00005E9D: 	ADDI	OPOSX,R3
    // asm 00005E9E: 	CALL	MATRIX_MUL
    // asm 00005E9F: 	LDF	*+AR4(OPOSX),R0
    // asm 00005EA0: 	ADDF	*+AR7(MAPLPX),R0
    // asm 00005EA1: 	STF	R0,*+AR4(OPOSX)
    // asm 00005EA2: 	LDF	*+AR4(OPOSY),R0
    // asm 00005EA3: 	ADDF	*+AR7(MAPLPY),R0
    // asm 00005EA4: 	STF	R0,*+AR4(OPOSY)
    // asm 00005EA5: 	LDF	*+AR4(OPOSZ),R0
    // asm 00005EA6: 	ADDF	*+AR7(MAPLPZ),R0
    // asm 00005EA7: 	STF	R0,*+AR4(OPOSZ)
    // asm 00005EA8: 	LDF	*+AR7(MAP1T),R2
    // asm 00005EA9: 	ADDF	@M1STDI,R2
    // asm 00005EAA: 	STF	R2,*+AR7(MAP1T)
    // asm 00005EAB: 	LDI	AR4,AR2
    // asm 00005EAC: 	ADDI	OMATRIX,AR2
    // asm 00005EAD: 	CALL	FIND_YMATRIX
    // asm 00005EAE: 	LDI	AR7,R2
    // asm 00005EAF: 	ADDI	MAPLMAT,R2
    // asm 00005EB0: 	LDI	AR2,R3
    // asm 00005EB1: 	CALL	CONCATMAT
    // asm 00005EB2: 	LDF	*+AR7(MAP3T),R2
    // asm 00005EB3: 	LDI	@MATRIXAI,AR2
    // asm 00005EB4: 	CALL	FIND_YMATRIX
    // asm 00005EB5: 	LDI	AR7,R2
    // asm 00005EB6: 	ADDI	MAPLMAT,R2
    // asm 00005EB7: 	LDI	AR2,R3
    // asm 00005EB8: 	CALL	CONCATMAT
    // asm 00005EB9: 	LDI	*+AR7(MAP4OBJ),AR4
    // asm 00005EBA: 	CALL	CLR_VECTORA
    // asm 00005EBB: 	FLOAT	128,R0
    // asm 00005EBC: 	STF	R0,*+AR2(X)
    // asm 00005EBD: 	LDI	@MATRIXAI,R2
    // asm 00005EBE: 	LDI	AR4,R3
    // asm 00005EBF: 	ADDI	OPOSX,R3
    // asm 00005EC0: 	CALL	MATRIX_MUL
    // asm 00005EC1: 	LDF	*+AR4(OPOSX),R0
    // asm 00005EC2: 	ADDF	*+AR7(MAPLPX),R0
    // asm 00005EC3: 	STF	R0,*+AR4(OPOSX)
    // asm 00005EC4: 	LDF	*+AR4(OPOSY),R0
    // asm 00005EC5: 	ADDF	*+AR7(MAPLPY),R0
    // asm 00005EC6: 	STF	R0,*+AR4(OPOSY)
    // asm 00005EC7: 	LDF	*+AR4(OPOSZ),R0
    // asm 00005EC8: 	ADDF	*+AR7(MAPLPZ),R0
    // asm 00005EC9: 	STF	R0,*+AR4(OPOSZ)
    // asm 00005ECA: 	LDF	*+AR7(MAP4T),R2
    // asm 00005ECB: 	ADDF	@M4STDI,R2
    // asm 00005ECC: 	STF	R2,*+AR7(MAP4T)
    // asm 00005ECD: 	LDI	AR4,AR2
    // asm 00005ECE: 	ADDI	OMATRIX,AR2
    // asm 00005ECF: 	CALL	FIND_YMATRIX
    // asm 00005ED0: 	LDI	AR7,R2
    // asm 00005ED1: 	ADDI	MAPLMAT,R2
    // asm 00005ED2: 	LDI	AR2,R3
    // asm 00005ED3: 	CALL	CONCATMAT
    // asm 00005ED4: 	SLEEP	1
    // asm 00005ED6: 	DBU	AR5,UNFOLD_LP
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
    // asm 00005ED7: 	LDI	map1_p,R0	;ONE TO OVERWRITE
    // asm 00005ED8: 	LDI	map1_p,R1	;WHAT TO OVERWRITE IT WITH
    // asm 00005ED9: 	CALL	PAL_OVERWRITE
    // asm 00005EDA: 	LDI	*+AR7(MAP1OBJ),AR0
    // asm 00005EDB: 	LDI	*+AR0(OFLAGS),R0
    // asm 00005EDC: 	ANDN	O_1PAL,R0
    // asm 00005EDD: 	STI	R0,*+AR0(OFLAGS)
    // asm 00005EDE: 	LDI	*+AR7(MAP2OBJ),AR0
    // asm 00005EDF: 	LDI	*+AR0(OFLAGS),R0
    // asm 00005EE0: 	ANDN	O_1PAL,R0
    // asm 00005EE1: 	STI	R0,*+AR0(OFLAGS)
    // asm 00005EE2: 	LDI	*+AR7(MAP3OBJ),AR0
    // asm 00005EE3: 	LDI	*+AR0(OFLAGS),R0
    // asm 00005EE4: 	ANDN	O_1PAL,R0
    // asm 00005EE5: 	STI	R0,*+AR0(OFLAGS)
    // asm 00005EE6: 	LDI	*+AR7(MAP4OBJ),AR0
    // asm 00005EE7: 	LDI	*+AR0(OFLAGS),R0
    // asm 00005EE8: 	ANDN	O_1PAL,R0
    // asm 00005EE9: 	STI	R0,*+AR0(OFLAGS)
    // asm 00005EEA: 	DIE
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
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
    // asm 00005EEB: 	SONDFX	WIPE4
    // *ELP CHANGE
    // asm 00005EED: 	CALL	MAPPAL_ILLUM_INIT
    // *ELP END CHANGE
    // asm 00005EEE: 	LDIL	O_3DROT,R6
    // asm 00005EF1: 	OR	O_NOUROT|O_NOUNIV,R6
    // asm 00005EF2: 	LDI	@EPALL,AR2
    // asm 00005EF3: 	CALL	PAL_FIND_RAW
    // asm 00005EF4: 	LDI	R0,AR3
    // asm 00005EF5: 	LDI	@EPALR,AR2
    // asm 00005EF6: 	CALL	PAL_FIND_RAW
    // asm 00005EF7: 	LDI	R0,AR4
    // asm 00005EF8: 	LDL	map1,AR2
    // asm 00005EF9: 	CALL	OBJ_GETE
    // asm 00005EFA: 	STI	AR0,*+AR7(MAP1OBJ)
    // asm 00005EFB: 	LDI	R6,R0
    // asm 00005EFC: 	OR	*+AR0(OFLAGS),R0
    // asm 00005EFD: 	OR	O_1PAL,R0
    // asm 00005EFE: 	STI	AR3,*+AR0(OPAL)
    // asm 00005EFF: 	STI	R0,*+AR0(OFLAGS)
    // asm 00005F00: 	LDI	AR0,AR2
    // asm 00005F01: 	CALL	OBJ_INSERTP
    // asm 00005F02: 	LDL	map2,AR2
    // asm 00005F03: 	CALL	OBJ_GETE
    // asm 00005F04: 	STI	AR0,*+AR7(MAP2OBJ)
    // asm 00005F05: 	LDI	R6,R0
    // asm 00005F06: 	OR	*+AR0(OFLAGS),R0
    // asm 00005F07: 	OR	O_1PAL,R0
    // asm 00005F08: 	STI	AR4,*+AR0(OPAL)
    // asm 00005F09: 	STI	R0,*+AR0(OFLAGS)
    // asm 00005F0A: 	LDI	AR0,AR2
    // asm 00005F0B: 	CALL	OBJ_INSERTP
    // asm 00005F0C: 	LDL	map3,AR2
    // asm 00005F0D: 	CALL	OBJ_GETE
    // asm 00005F0E: 	STI	AR0,*+AR7(MAP3OBJ)
    // asm 00005F0F: 	LDI	R6,R0
    // asm 00005F10: 	OR	*+AR0(OFLAGS),R0
    // asm 00005F11: 	OR	O_1PAL,R0
    // asm 00005F12: 	STI	AR3,*+AR0(OPAL)
    // asm 00005F13: 	STI	R0,*+AR0(OFLAGS)
    // asm 00005F14: 	LDI	AR0,AR2
    // asm 00005F15: 	CALL	OBJ_INSERTP
    // asm 00005F16: 	LDL	map4,AR2
    // asm 00005F17: 	CALL	OBJ_GETE
    // asm 00005F18: 	STI	AR0,*+AR7(MAP4OBJ)
    // asm 00005F19: 	LDI	R6,R0
    // asm 00005F1A: 	OR	*+AR0(OFLAGS),R0
    // asm 00005F1B: 	OR	O_1PAL,R0
    // asm 00005F1C: 	STI	AR4,*+AR0(OPAL)
    // asm 00005F1D: 	STI	R0,*+AR0(OFLAGS)
    // asm 00005F1E: 	LDI	AR0,AR2
    // asm 00005F1F: 	CALL	OBJ_INSERTP
    // asm 00005F20: 	CLRF	R0
    // asm 00005F21: 	STF	R0,*+AR7(MAP1T)
    // asm 00005F22: 	STF	R0,*+AR7(MAP2T)
    // asm 00005F23: 	STF	R0,*+AR7(MAP3T)
    // asm 00005F24: 	STF	R0,*+AR7(MAP4T)
    // asm 00005F25: 	CLRF	R0
    // asm 00005F26: 	STF	R0,*+AR7(MAPLPX)
    // asm 00005F27: 	STF	R0,*+AR7(MAPLPY)
    // asm 00005F28: 	FLOAT	368,R0	;368
    // asm 00005F29: 	STF	R0,*+AR7(MAPLPZ)
    // asm 00005F2A: 	CLRF	R2
    // asm 00005F2B: 	STF	R2,*+AR7(MAPLTX)
    // asm 00005F2C: 	LDI	AR7,AR2
    // asm 00005F2D: 	ADDI	MAPLMAT,AR2
    // asm 00005F2E: 	CALL	FIND_XMATRIX
    // asm 00005F2F: 	LDF	1,R0
    // asm 00005F30: 	STF	R0,*+AR7(MAPXD)
    // asm 00005F31: 	STF	R0,*+AR7(MAPYD)
    // asm 00005F32: 	LDI	MAP_ITERATIONS-1,AR5
    // asm 00005F33: FOLD_LP
    // asm 00005F33: 	CALL	MAP_ILLUM_COMPUTE
    // asm 00005F34: 	LDI	@EPALL,AR2
    // asm 00005F35: 	CALL	PAL_FIND_RAW
#if DEBUG
    // asm: 	BNC	$
#endif
    // asm 00005F36: 	LDI	@EPALR,AR2
    // asm 00005F37: 	CALL	PAL_FIND_RAW
#if DEBUG
    // asm: 	BNC	$
#endif
    // asm 00005F38: 	LDF	*+AR7(MAPXD),R1
    // asm 00005F39: 	MPYF	1.20,R1
    // asm 00005F3A: 	STF	R1,*+AR7(MAPXD)
    // asm 00005F3B: 	LDF	*+AR7(MAPLPX),R0
    // asm 00005F3C: 	ADDF	R1,R0
    // asm 00005F3D: 	STF	R0,*+AR7(MAPLPX)
    // asm 00005F3E: 	LDF	*+AR7(MAPYD),R1
    // asm 00005F3F: 	MPYF	1.2,R1
    // asm 00005F40: 	STF	R1,*+AR7(MAPYD)
    // asm 00005F41: 	LDF	*+AR7(MAPLPY),R0
    // asm 00005F42: 	SUBF	R1,R0
    // asm 00005F43: 	STF	R0,*+AR7(MAPLPY)
    // asm 00005F44: 	LDF	*+AR7(MAPLPZ),R0
    // asm 00005F45: 	FLOAT	3368,R1
    // asm 00005F46: 	SUBF	R0,R1
    // asm 00005F47: 	MPYF	0.05,R1
    // asm 00005F48: 	ADDF	R1,R0
    // asm 00005F49: 	FLOAT	3368,R1
    // asm 00005F4A: 	CMPI	0,AR5
    // asm 00005F4B: 	LDFEQ	R1,R0
    // asm 00005F4C: 	STF	R0,*+AR7(MAPLPZ)
    // asm 00005F4D: 	LDF	*+AR7(MAPLTX),R2
    // asm 00005F4E: 	LDF	HALFPI,R0
    // asm 00005F4F: 	SUBF	R2,R0
    // asm 00005F50: 	MPYF	0.1,R0
    // asm 00005F51: 	ADDF	R0,R2
    // asm 00005F52: 	LDF	HALFPI,R1
    // asm 00005F53: 	CMPI	0,AR5
    // asm 00005F54: 	LDFEQ	R1,R2
    // asm 00005F55: 	STF	R2,*+AR7(MAPLTX)
    // asm 00005F56: 	LDI	AR7,AR2
    // asm 00005F57: 	ADDI	MAPLMAT,AR2
    // asm 00005F58: 	CALL	FIND_XMATRIX
    // asm 00005F59: 	LDI	*+AR7(MAP2OBJ),AR4
    // asm 00005F5A: 	LDF	*+AR7(MAPLPX),R0
    // asm 00005F5B: 	STF	R0,*+AR4(OPOSX)
    // asm 00005F5C: 	LDF	*+AR7(MAPLPY),R0
    // asm 00005F5D: 	STF	R0,*+AR4(OPOSY)
    // asm 00005F5E: 	LDF	*+AR7(MAPLPZ),R0
    // asm 00005F5F: 	STF	R0,*+AR4(OPOSZ)
    // asm 00005F60: 	LDF	*+AR7(MAP2T),R2
    // asm 00005F61: 	SUBF	@M2STDI,R2
    // asm 00005F62: 	STF	R2,*+AR7(MAP2T)
    // asm 00005F63: 	LDI	AR4,AR2
    // asm 00005F64: 	ADDI	OMATRIX,AR2
    // asm 00005F65: 	CALL	FIND_YMATRIX
    // asm 00005F66: 	LDI	AR7,R2
    // asm 00005F67: 	ADDI	MAPLMAT,R2
    // asm 00005F68: 	LDI	AR2,R3
    // asm 00005F69: 	CALL	CONCATMAT
    // 	;					OBJECT 2
    // 	;----------------------------------------------------------------------
    // asm 00005F6A: 	LDI	*+AR7(MAP3OBJ),AR4
    // asm 00005F6B: 	LDF	*+AR7(MAPLPX),R0
    // asm 00005F6C: 	STF	R0,*+AR4(OPOSX)
    // asm 00005F6D: 	LDF	*+AR7(MAPLPY),R0
    // asm 00005F6E: 	STF	R0,*+AR4(OPOSY)
    // asm 00005F6F: 	LDF	*+AR7(MAPLPZ),R0
    // asm 00005F70: 	STF	R0,*+AR4(OPOSZ)
    // asm 00005F71: 	LDF	*+AR7(MAP3T),R2
    // asm 00005F72: 	SUBF	@M3STDI,R2
    // asm 00005F73: 	STF	R2,*+AR7(MAP3T)
    // asm 00005F74: 	LDI	AR4,AR2
    // asm 00005F75: 	ADDI	OMATRIX,AR2
    // asm 00005F76: 	CALL	FIND_YMATRIX
    // asm 00005F77: 	LDI	AR7,R2
    // asm 00005F78: 	ADDI	MAPLMAT,R2
    // asm 00005F79: 	LDI	AR2,R3
    // asm 00005F7A: 	CALL	CONCATMAT
    // 	;					OBJECT 3
    // 	;----------------------------------------------------------------------
    // asm 00005F7B: 	LDF	*+AR7(MAP2T),R2			;MATRIX FOR TRANSLATION
    // asm 00005F7C: 	LDI	@MATRIXAI,AR2
    // asm 00005F7D: 	CALL	FIND_YMATRIX
    // asm 00005F7E: 	LDI	AR7,R2
    // asm 00005F7F: 	ADDI	MAPLMAT,R2
    // asm 00005F80: 	LDI	AR2,R3
    // asm 00005F81: 	CALL	CONCATMAT
    // asm 00005F82: 	LDI	*+AR7(MAP1OBJ),AR4
    // asm 00005F83: 	CALL	CLR_VECTORA
    // asm 00005F84: 	FLOAT	-127,R0
    // asm 00005F85: 	STF	R0,*+AR2(X)
    // asm 00005F86: 	LDI	@MATRIXAI,R2
    // asm 00005F87: 	LDI	AR4,R3
    // asm 00005F88: 	ADDI	OPOSX,R3
    // asm 00005F89: 	CALL	MATRIX_MUL
    // asm 00005F8A: 	LDF	*+AR4(OPOSX),R0
    // asm 00005F8B: 	ADDF	*+AR7(MAPLPX),R0
    // asm 00005F8C: 	STF	R0,*+AR4(OPOSX)
    // asm 00005F8D: 	LDF	*+AR4(OPOSY),R0
    // asm 00005F8E: 	ADDF	*+AR7(MAPLPY),R0
    // asm 00005F8F: 	STF	R0,*+AR4(OPOSY)
    // asm 00005F90: 	LDF	*+AR4(OPOSZ),R0
    // asm 00005F91: 	ADDF	*+AR7(MAPLPZ),R0
    // asm 00005F92: 	STF	R0,*+AR4(OPOSZ)
    // asm 00005F93: 	LDF	*+AR7(MAP1T),R2
    // asm 00005F94: 	SUBF	@M1STDI,R2
    // asm 00005F95: 	STF	R2,*+AR7(MAP1T)
    // asm 00005F96: 	LDI	AR4,AR2
    // asm 00005F97: 	ADDI	OMATRIX,AR2
    // asm 00005F98: 	CALL	FIND_YMATRIX
    // asm 00005F99: 	LDI	AR7,R2
    // asm 00005F9A: 	ADDI	MAPLMAT,R2
    // asm 00005F9B: 	LDI	AR2,R3
    // asm 00005F9C: 	CALL	CONCATMAT
    // 	;					OBJECT 1
    // 	;----------------------------------------------------------------------
    // asm 00005F9D: 	LDF	*+AR7(MAP3T),R2			;MATRIX FOR TRANSLATION
    // asm 00005F9E: 	LDI	@MATRIXAI,AR2
    // asm 00005F9F: 	CALL	FIND_YMATRIX
    // asm 00005FA0: 	LDI	AR7,R2
    // asm 00005FA1: 	ADDI	MAPLMAT,R2
    // asm 00005FA2: 	LDI	AR2,R3
    // asm 00005FA3: 	CALL	CONCATMAT
    // asm 00005FA4: 	LDI	*+AR7(MAP4OBJ),AR4
    // asm 00005FA5: 	CALL	CLR_VECTORA
    // asm 00005FA6: 	FLOAT	128,R0
    // asm 00005FA7: 	STF	R0,*+AR2(X)
    // asm 00005FA8: 	LDI	@MATRIXAI,R2
    // asm 00005FA9: 	LDI	AR4,R3
    // asm 00005FAA: 	ADDI	OPOSX,R3
    // asm 00005FAB: 	CALL	MATRIX_MUL
    // asm 00005FAC: 	LDF	*+AR4(OPOSX),R0
    // asm 00005FAD: 	ADDF	*+AR7(MAPLPX),R0
    // asm 00005FAE: 	STF	R0,*+AR4(OPOSX)
    // asm 00005FAF: 	LDF	*+AR4(OPOSY),R0
    // asm 00005FB0: 	ADDF	*+AR7(MAPLPY),R0
    // asm 00005FB1: 	STF	R0,*+AR4(OPOSY)
    // asm 00005FB2: 	LDF	*+AR4(OPOSZ),R0
    // asm 00005FB3: 	ADDF	*+AR7(MAPLPZ),R0
    // asm 00005FB4: 	STF	R0,*+AR4(OPOSZ)
    // asm 00005FB5: 	LDF	*+AR7(MAP4T),R2
    // asm 00005FB6: 	SUBF	@M4STDI,R2
    // asm 00005FB7: 	STF	R2,*+AR7(MAP4T)
    // asm 00005FB8: 	LDI	AR4,AR2
    // asm 00005FB9: 	ADDI	OMATRIX,AR2
    // asm 00005FBA: 	CALL	FIND_YMATRIX
    // asm 00005FBB: 	LDI	AR7,R2
    // asm 00005FBC: 	ADDI	MAPLMAT,R2
    // asm 00005FBD: 	LDI	AR2,R3
    // asm 00005FBE: 	CALL	CONCATMAT
    // 	;					OBJECT 4
    // 	;--------------------------------------------------------------------
    // asm 00005FBF: 	SLEEP	1
    // asm 00005FC1: 	DBU	AR5,FOLD_LP
    // asm 00005FC2: 	LDI	*+AR7(MAP1OBJ),AR2
    // asm 00005FC3: 	CALL	OBJ_DELETE
    // asm 00005FC4: 	LDI	*+AR7(MAP2OBJ),AR2
    // asm 00005FC5: 	CALL	OBJ_DELETE
    // asm 00005FC6: 	LDI	*+AR7(MAP3OBJ),AR2
    // asm 00005FC7: 	CALL	OBJ_DELETE
    // asm 00005FC8: 	LDI	*+AR7(MAP4OBJ),AR2
    // asm 00005FC9: 	CALL	OBJ_DELETE
    // asm 00005FCA: 	CALL	CLEAR_MAP_PALS
    // asm 00005FCB: 	DIE
    // WARNING CHECK FOR FALLTHROUGH TO NEXT FUNCTION
    TRACE_EVENT(&g_crusn_machine->trace, "function", "FOLDMAP", 0, 0);
    UNIMPL();
}

void CLEAR_MAP_PALS(void)
{
    // asm 00005FCC: 	LDI	@EPALL,AR2
    // asm 00005FCD: 	CALL	PAL_FIND_RAW
#if DEBUG
    // asm: 	BNC	$
#endif
    // asm 00005FCE: 	BNC	IBOIBO
    // asm 00005FCF: 	LDI	R0,AR2
    // asm 00005FD0: 	CALL	PAL_DELETE_RAW
IBOIBO:
    // asm 00005FD1: 	LDI	@EPALR,AR2
    // asm 00005FD2: 	CALL	PAL_FIND_RAW
#if DEBUG
    // asm: 	BNC	$
#endif
    // asm 00005FD3: 	RETSNC
    // asm 00005FD4: 	LDI	R0,AR2
    // asm 00005FD5: 	CALL	PAL_DELETE_RAW
    // asm 00005FD6: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "CLEAR_MAP_PALS", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
void MAPPAL_ILLUM_INIT(void)
{
    // asm 00005FD7: 	LDI	@EPALR,AR0	;LOAD PALETTES AT
    // asm 00005FD8: 	LDI	@EPALL,AR1	;THE SAME TIME
    // asm 00005FD9: 	LDI	map1_p,AR3
    // asm 00005FDA: 	ADDI	@PALROMI,AR3
    // asm 00005FDB: 	LDI	*AR3,AR3	;NOW HOLDS RAM LOCATION
    // asm 00005FDC: 	LDI	*AR3++,R0
    // asm 00005FDD: 	STI	R0,*AR0++
    // asm 00005FDE: 	STI	R0,*AR1++
    // asm 00005FDF: 	LDI	R0,AR2
    // asm 00005FE0: 	AND	0FFFh,AR2
    // asm 00005FE1: 	RS	1,AR2
    // asm 00005FE2: 	SUBI	1,AR2
L342:
    // asm 00005FE3: LDI	*AR3++,R0
    // asm 00005FE4: 	STI	R0,*AR0++
    // asm 00005FE5: 	STI	R0,*AR1++
    // asm 00005FE6: 	DBU	AR2,L342
    // asm 00005FE7: 	LDI	@EPALR,AR2
    // asm 00005FE8: 	CALL	PAL_ALLOC_RAW
    // asm 00005FE9: 	LDI	@EPALL,AR2
    // asm 00005FEA: 	CALL	PAL_ALLOC_RAW
    // asm 00005FEB: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MAPPAL_ILLUM_INIT", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
/* asm: FORMULA1	.float	0.318309886 */
float FORMULA1 = 0.318309886f;

void MAP_ILLUM_COMPUTE(void)
{
    // asm 00005FED: 	LDF	*+AR7(MAP1T),R0
    // asm 00005FEE: 	ADDF	HALFPI,R0
    // asm 00005FEF: 	MPYF	@FORMULA1,R0
    // asm 00005FF0: 	SUBRF	1.0,R0
    // asm 00005FF1: 	MPYF	0.7,R0
    // asm 00005FF2: 	ADDF	0.3,R0
    // asm 00005FF3: 	STF	R0,@MAPPAL13
    // asm 00005FF4: 	LDF	*+AR7(MAP2T),R0
    // asm 00005FF5: 	ADDF	HALFPI,R0
    // asm 00005FF6: 	MPYF	@FORMULA1,R0
    // asm 00005FF7: 	SUBRF	1.0,R0
    // asm 00005FF8: 	MPYF	0.7,R0
    // asm 00005FF9: 	ADDF	0.3,R0
    // asm 00005FFA: 	STF	R0,@MAPPAL24
    // asm 00005FFB: 	CALL	MAPPAL_ILLUM
    // asm 00005FFC: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MAP_ILLUM_COMPUTE", 0, 0);
    UNIMPL();
}

/* *----------------------------------------------------------------------------
 */
/* asm: MAPPAL13	.bss	MAPPAL13,1 */
int MAPPAL13;
/* asm: MAPPAL24	.bss	MAPPAL24,1 */
int MAPPAL24;

void MAPPAL_ILLUM(void)
{
    // asm 00005FFD: 	PUSH	AR0
    // asm 00005FFE: 	PUSH	AR1
    // asm 00005FFF: 	PUSH	AR2
    // asm 00006000: 	PUSH	AR3
    // asm 00006001: 	PUSH	AR4
    // asm 00006002: 	PUSH	AR5
    // asm 00006003: 	PUSH	R0
    // asm 00006004: 	PUSH	R1
    // asm 00006005: 	PUSH	R2
    // asm 00006006: 	PUSH	R3
    // asm 00006007: 	PUSH	R4
    // asm 00006008: 	PUSH	R5
    // asm 00006009: 	PUSHFL	R6
    // asm 0000600B: 	LDF	@MAPPAL13,R6	;1st multiplier
    // asm 0000600C: 	LDI	@EPALL,AR0
    // asm 0000600D: 	LDI	map1_p,AR3
    // asm 0000600E: 	ADDI	@PALROMI,AR3
    // asm 0000600F: 	LDI	*AR3,AR3	;NOW HOLDS RAM LOCATION
    // asm 00006010: 	LDI	*AR3++,R0
    // asm 00006011: 	STI	R0,*AR0++
    // asm 00006012: 	LDI	R0,AR5
    // asm 00006013: 	LS	1,AR5
    // asm 00006014: 	RS	2,AR5
    // asm 00006015: L894
    // asm 00006015: 	LDI	*AR3++,R4	;get src2 (TRUE)
    // asm 00006016: 	LDI	R4,R0
    // asm 00006017: 	AND	01Fh,R0
    // asm 00006018: 	LS	3,R0
    // asm 00006019: 	FLOAT	R0
    // asm 0000601A: 	MPYF	R6,R0
    // asm 0000601B: 	FIX	R0
    // asm 0000601C: 	RS	3,R0
    // asm 0000601D: 	AND	01Fh,R0
    // asm 0000601E: 	LDI	R4,R1
    // asm 0000601F: 	RS	5,R1
    // asm 00006020: 	AND	01Fh,R1
    // asm 00006021: 	LS	3,R1
    // asm 00006022: 	FLOAT	R1
    // asm 00006023: 	MPYF	R6,R1
    // asm 00006024: 	FIX	R1
    // asm 00006025: 	RS	3,R1
    // asm 00006026: 	AND	01Fh,R1
    // asm 00006027: 	LS	5,R1
    // asm 00006028: 	OR	R1,R0
    // asm 00006029: 	LDI	R4,R1
    // asm 0000602A: 	RS	10,R1
    // asm 0000602B: 	AND	01Fh,R1
    // asm 0000602C: 	LS	3,R1
    // asm 0000602D: 	FLOAT	R1
    // asm 0000602E: 	MPYF	R6,R1
    // asm 0000602F: 	FIX	R1
    // asm 00006030: 	RS	3,R1
    // asm 00006031: 	AND	01Fh,R1
    // asm 00006032: 	LS	10,R1
    // asm 00006033: 	OR	R1,R0
    // 	;second word
    // 	;
    // asm 00006034: 	RS	16,R4
    // asm 00006035: 	LDI	R4,R2
    // asm 00006036: 	AND	01Fh,R2
    // asm 00006037: 	LS	3,R2
    // asm 00006038: 	FLOAT	R2
    // asm 00006039: 	MPYF	R6,R2
    // asm 0000603A: 	FIX	R2
    // asm 0000603B: 	RS	3,R2
    // asm 0000603C: 	AND	01Fh,R2
    // asm 0000603D: 	LDI	R4,R3
    // asm 0000603E: 	RS	5,R3
    // asm 0000603F: 	AND	01Fh,R3
    // asm 00006040: 	LS	3,R3
    // asm 00006041: 	FLOAT	R3
    // asm 00006042: 	MPYF	R6,R3
    // asm 00006043: 	FIX	R3
    // asm 00006044: 	RS	3,R3
    // asm 00006045: 	AND	01Fh,R3
    // asm 00006046: 	LS	5,R3
    // asm 00006047: 	OR	R3,R2
    // asm 00006048: 	LDI	R4,R3
    // asm 00006049: 	RS	10,R3
    // asm 0000604A: 	AND	01Fh,R3
    // asm 0000604B: 	LS	3,R3
    // asm 0000604C: 	FLOAT	R3
    // asm 0000604D: 	MPYF	R6,R3
    // asm 0000604E: 	FIX	R3
    // asm 0000604F: 	RS	3,R3
    // asm 00006050: 	AND	01Fh,R3
    // asm 00006051: 	LS	10,R3
    // asm 00006052: 	OR	R3,R2
    // asm 00006053: 	LS	16,R2
    // asm 00006054: 	OR	R2,R0
    // asm 00006055: 	STI	R0,*AR0++
    // asm 00006056: 	DBU	AR5,L894
    // asm 00006057: 	LDI	@EPALL,AR2
    // asm 00006058: 	CALL	PAL_FIND_RAW
    // asm 00006059: 	CMPI	-1,R0
    // asm 0000605A: 	BEQ	JAJA4
    // asm 0000605B: 	LDI	*AR2++,R3
    // asm 0000605C: 	LDI	R0,R2
    // asm 0000605D: 	CALL	PAL_SET
JAJA4:
    // *
    // *
    // *
    // *
    // *
    // asm 0000605E: 	LDF	@MAPPAL24,R6	;2nd multiplier
    // asm 0000605F: 	LDI	@EPALR,AR0
    // asm 00006060: 	LDI	map1_p,AR3
    // asm 00006061: 	ADDI	@PALROMI,AR3
    // asm 00006062: 	LDI	*AR3,AR3	;NOW HOLDS RAM LOCATION
    // asm 00006063: 	LDI	*AR3++,R0
    // asm 00006064: 	STI	R0,*AR0++
    // asm 00006065: 	LDI	R0,AR5
    // asm 00006066: 	LS	1,AR5
    // asm 00006067: 	RS	2,AR5
    // asm 00006068: L894D
    // asm 00006068: 	LDI	*AR3++,R4	;get src2 (TRUE)
    // asm 00006069: 	LDI	R4,R0
    // asm 0000606A: 	AND	01Fh,R0
    // asm 0000606B: 	LS	3,R0
    // asm 0000606C: 	FLOAT	R0
    // asm 0000606D: 	MPYF	R6,R0
    // asm 0000606E: 	FIX	R0
    // asm 0000606F: 	RS	3,R0
    // asm 00006070: 	AND	01Fh,R0
    // asm 00006071: 	LDI	R4,R1
    // asm 00006072: 	RS	5,R1
    // asm 00006073: 	AND	01Fh,R1
    // asm 00006074: 	LS	3,R1
    // asm 00006075: 	FLOAT	R1
    // asm 00006076: 	MPYF	R6,R1
    // asm 00006077: 	FIX	R1
    // asm 00006078: 	RS	3,R1
    // asm 00006079: 	AND	01Fh,R1
    // asm 0000607A: 	LS	5,R1
    // asm 0000607B: 	OR	R1,R0
    // asm 0000607C: 	LDI	R4,R1
    // asm 0000607D: 	RS	10,R1
    // asm 0000607E: 	AND	01Fh,R1
    // asm 0000607F: 	LS	3,R1
    // asm 00006080: 	FLOAT	R1
    // asm 00006081: 	MPYF	R6,R1
    // asm 00006082: 	FIX	R1
    // asm 00006083: 	RS	3,R1
    // asm 00006084: 	AND	01Fh,R1
    // asm 00006085: 	LS	10,R1
    // asm 00006086: 	OR	R1,R0
    // 	;second word
    // 	;
    // asm 00006087: 	RS	16,R4
    // asm 00006088: 	LDI	R4,R2
    // asm 00006089: 	AND	01Fh,R2
    // asm 0000608A: 	LS	3,R2
    // asm 0000608B: 	FLOAT	R2
    // asm 0000608C: 	MPYF	R6,R2
    // asm 0000608D: 	FIX	R2
    // asm 0000608E: 	RS	3,R2
    // asm 0000608F: 	AND	01Fh,R2
    // asm 00006090: 	LDI	R4,R3
    // asm 00006091: 	RS	5,R3
    // asm 00006092: 	AND	01Fh,R3
    // asm 00006093: 	LS	3,R3
    // asm 00006094: 	FLOAT	R3
    // asm 00006095: 	MPYF	R6,R3
    // asm 00006096: 	FIX	R3
    // asm 00006097: 	RS	3,R3
    // asm 00006098: 	AND	01Fh,R3
    // asm 00006099: 	LS	5,R3
    // asm 0000609A: 	OR	R3,R2
    // asm 0000609B: 	LDI	R4,R3
    // asm 0000609C: 	RS	10,R3
    // asm 0000609D: 	AND	01Fh,R3
    // asm 0000609E: 	LS	3,R3
    // asm 0000609F: 	FLOAT	R3
    // asm 000060A0: 	MPYF	R6,R3
    // asm 000060A1: 	FIX	R3
    // asm 000060A2: 	RS	3,R3
    // asm 000060A3: 	AND	01Fh,R3
    // asm 000060A4: 	LS	10,R3
    // asm 000060A5: 	OR	R3,R2
    // asm 000060A6: 	LS	16,R2
    // asm 000060A7: 	OR	R2,R0
    // asm 000060A8: 	STI	R0,*AR0++
    // asm 000060A9: 	DBU	AR5,L894D
    // asm 000060AA: 	LDI	@EPALR,AR2
    // asm 000060AB: 	CALL	PAL_FIND_RAW
    // asm 000060AC: 	CMPI	-1,R0
    // asm 000060AD: 	BEQ	JAJA5
    // asm 000060AE: 	LDI	*AR2++,R3
    // asm 000060AF: 	LDI	R0,R2
    // asm 000060B0: 	CALL	PAL_SET
JAJA5:
    // asm 000060B1: 	POPFL	R6
    // asm 000060B3: 	POP	R5
    // asm 000060B4: 	POP	R4
    // asm 000060B5: 	POP	R3
    // asm 000060B6: 	POP	R2
    // asm 000060B7: 	POP	R1
    // asm 000060B8: 	POP	R0
    // asm 000060B9: 	POP	AR5
    // asm 000060BA: 	POP	AR4
    // asm 000060BB: 	POP	AR3
    // asm 000060BC: 	POP	AR2
    // asm 000060BD: 	POP	AR1
    // asm 000060BE: 	POP	AR0
    // asm 000060BF: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "MAPPAL_ILLUM", 0, 0);
    UNIMPL();
}

/* asm: STOPWATCH	.bss	STOPWATCH,1 */
int STOPWATCH;
/* asm: STOPWATCH_CNTL	.bss	STOPWATCH_CNTL,1 */
int STOPWATCH_CNTL;
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
    // asm 000060C4: 	PUSH	R0
    // asm 000060C5: 	PUSH	R1
    // asm 000060C6: 	PUSH	R2
    // asm 000060C7: 	PUSH	AR0
    // asm 000060C8: 	PUSH	AR1
    // asm 000060C9: 	PUSH	AR2
    // asm 000060CA: 	CALL	CVTTIME
    // asm 000060CB: 	PUSH	R0
    // asm 000060CC: 	PUSH	R1
    // asm 000060CD: 	CALL	_itoa
    // asm 000060CE: 	LDI	AR2,AR0
    // asm 000060CF: 	LDI	@COLONI,AR1
    // asm 000060D0: 	CALL	STRCAT
    // asm 000060D1: 	POP	R2
    // asm 000060D2: 	LDI	@tmp_bufferI,AR2
    // asm 000060D3: 	CALL	_itoaLZ
    // asm 000060D4: 	LDI	AR2,AR1
    // asm 000060D5: 	CALL	STRCAT
    // asm 000060D6: 	LDI	@COLONI,AR1
    // asm 000060D7: 	CALL	STRCAT
    // asm 000060D8: 	POP	R2
    // asm 000060D9: 	LDI	@tmp_bufferI,AR2
    // asm 000060DA: 	CALL	_itoaLZ
    // asm 000060DB: 	LDI	AR2,AR1
    // asm 000060DC: 	CALL	STRCAT
    // asm 000060DD: 	LDI	AR0,AR2
    // asm 000060DE: 	POP	AR2
    // asm 000060DF: 	POP	AR1
    // asm 000060E0: 	POP	AR0
    // asm 000060E1: 	POP	R2
    // asm 000060E2: 	POP	R1
    // asm 000060E3: 	POP	R0
    // asm 000060E4: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "TIME2STR", 0, 0);
    UNIMPL();
}

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

void CVTTIME(void)
{
    // asm 000060E8: 	PUSH	R3
    // asm 000060E9: 	PUSHF	R3
    // asm 000060EA: 	FLOAT	R0,R3
    // asm 000060EB: 	MPYF	@MINFACT,R3
    // asm 000060EC: 	FIX	R3,R3
    // asm 000060ED: 	CEILI	99,R3
    // asm 000060EF: 	FLOORI	0,R3
    // asm 000060F1: 	LDI	R3,R2			;MINUTES
    // asm 000060F2: 	MPYI	3300,R3
    // asm 000060F3: 	SUBI 	R3,R0
    // asm 000060F4: 	LDILT	0,R0
    // asm 000060F5: 	FLOAT	R0,R3
    // asm 000060F6: 	MPYF	@SECFACT,R3
    // asm 000060F7: 	FIX	R3,R3
    // asm 000060F8: 	CEILI	59,R3
    // asm 000060FA: 	FLOORI	0,R3
    // asm 000060FC: 	LDI	R3,R1			;SECONDS
    // asm 000060FD: 	MPYI	55,R3
    // asm 000060FE: 	SUBI 	R3,R0
    // asm 000060FF: 	LDILT	0,R0
    // asm 00006100: 	FLOAT	R0,R3
    // asm 00006101: 	MPYF	@HUNFACT,R3
    // asm 00006102: 	FIX	R3,R0			;HUNDRETHS
    // asm 00006103: 	CEILI	99,R0
    // asm 00006105: 	FLOORI	0,R0
    // asm 00006107: 	POPF	R3
    // asm 00006108: 	POP	R3
    // asm 00006109: 	RETS
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

/* *----------------------------------------------------------------------------
 */
void RADAR_PLOT(void)
{
    // asm 0000610A: 	PUSH	AR4
    // asm 0000610B: 	LDI	@PLYCAR,AR4
    // 	;background
    // asm 0000610C: 	LDI	CC|DITHER|8,R0
    // asm 0000610D: 	STI	R0,@_ACNTL
    // asm 0000610E: 	LDI	RADAR_XMIN,R0
    // asm 0000610F: 	ADDI	@MOVEIN_OFFSET,R0
    // asm 00006110: 	STI	R0,@(_ARPS+(0*3))
    // asm 00006111: 	STI	R0,@(_ARPS+(3*3))
    // asm 00006112: 	LDI	RADAR_XMAX,R0
    // asm 00006113: 	ADDI	@MOVEIN_OFFSET,R0
    // asm 00006114: 	STI	R0,@(_ARPS+(1*3))
    // asm 00006115: 	STI	R0,@(_ARPS+(2*3))
    // asm 00006116: 	LDI	RADAR_YMIN,R0
    // asm 00006117: 	STI	R0,@(_ARPS+(0*3)+1)
    // asm 00006118: 	STI	R0,@(_ARPS+(1*3)+1)
    // asm 00006119: 	LDI	RADAR_YMAX,R0
    // asm 0000611A: 	STI	R0,@(_ARPS+(3*3)+1)
    // asm 0000611B: 	STI	R0,@(_ARPS+(2*3)+1)
    // asm 0000611C: 	CLRI	R0
    // asm 0000611D: 	STI	R0,@_ADDRL
    // asm 0000611E: 	CLRI	R0
    // asm 0000611F: 	STI	R0,@_ACMAP
    // asm 00006120: 	CALL	_stuff_fpga
    // asm 00006121: 	LDP	@(_CAMERARAD+Y)
    // asm 00006122: 	LDF	@(_CAMERARAD+Y),R2
    // asm 00006123: 	SETDP
    // asm 00006124: 	LDI	@MATRIXAI,AR2
    // asm 00006125: 	CALL	FIND_YMATRIX
    // asm 00006126: 	LDI	@CAR_LISTI,AR0
    // asm 00006127: 	SUBI	OLINK3,AR0
RADAR_LP:
    // asm 00006128: 	LDI	*+AR0(OLINK3),R0
    // asm 00006129: 	BEQ	RADAR_X
    // asm 0000612A: 	LDI	R0,AR0
    // asm 0000612B: 	LDI	CC|3,R0			;red (RACER)
    // asm 0000612C: 	LDI	*+AR0(OID),R1
    // asm 0000612D: 	CMPI	DRONE_C|VEHICLE_T|DRNE_RACER,R1
    // asm 0000612E: 	LDINE	CC|0Ch,R0		;light gray (general racer)
    // asm 0000612F: 	STI	R0,@_ACNTL
    // asm 00006130: 	CALL	CLR_VECTORA
    // asm 00006131: 	LDF	*+AR0(OPOSX),R0
    // asm 00006132: 	SUBF	*+AR4(OPOSX),R0
    // asm 00006133: 	LDLF	0.003,R1
    // asm 00006134: 	MPYF	R1,R0
    // asm 00006135: 	STF	R0,*+AR2(X)
    // asm 00006136: 	LDF	*+AR0(OPOSZ),R0
    // asm 00006137: 	SUBF	*+AR4(OPOSZ),R0
    // asm 00006138: 	LDLF	0.003,R1
    // asm 00006139: 	MPYF	R1,R0
    // asm 0000613A: 	STF	R0,*+AR2(Z)
    // asm 0000613B: 	LDI	@MATRIXAI,R2
    // asm 0000613C: 	LDI	AR2,R3
    // asm 0000613D: 	CALL	MATRIX_MUL
    // asm 0000613E: 	LDF	*+AR2(X),R0
    // asm 0000613F: 	FIX	R0
    // asm 00006140: 	ADDI	RADAR_XCNTR-2,R0
    // 	;
    // 	;
    // 	;
    // 	;
    // 	;
    // asm 00006141: 	LDI	@HEAD2HEAD_ON,R2		;are we playing HEAD2HEAD?
    // asm 00006142: 	BZ	NOTTHEOPLYR
    // asm 00006143: 	CMPI	@PLY2CAR,AR0		;is this the other PLAYER?
    // asm 00006144: 	BNE	NOTTHEOPLYR
    // asm 00006145: 	CLRI	R6
    // asm 00006146: 	CMPI	RADAR_XMIN,R0
    // asm 00006147: 	LDILT	RADAR_XMIN,R0
    // asm 00006148: 	LDILT	1,R6
    // asm 00006149: 	CMPI	RADAR_XMAX,R0
    // asm 0000614A: 	LDIGT	RADAR_XMAX,R0
    // asm 0000614B: 	LDIGT	1,R6
    // asm 0000614C: 	ADDI	@MOVEIN_OFFSET,R0
    // asm 0000614D: 	STI	R0,@(_ARPS+(0*3))
    // asm 0000614E: 	STI	R0,@(_ARPS+(3*3))
    // asm 0000614F: 	ADDI	3,R0
    // asm 00006150: 	STI	R0,@(_ARPS+(1*3))
    // asm 00006151: 	STI	R0,@(_ARPS+(2*3))
    // asm 00006152: 	LDF	*+AR2(Z),R0
    // asm 00006153: 	MPYF	0.3,R0
    // asm 00006154: 	FIX	R0
    // asm 00006155: 	NEGI	R0
    // asm 00006156: 	ADDI	RADAR_YCNTR-2,R0
    // asm 00006157: 	CMPI	RADAR_YMIN,R0
    // asm 00006158: 	LDILT	RADAR_YMIN,R0
    // asm 00006159: 	LDILT	1,R6
    // asm 0000615A: 	CMPI	RADAR_YMAX,R0
    // asm 0000615B: 	LDIGT	RADAR_YMAX,R0
    // asm 0000615C: 	LDIGT	1,R6
    // asm 0000615D: 	STI	R0,@(_ARPS+(0*3)+1)
    // asm 0000615E: 	STI	R0,@(_ARPS+(1*3)+1)
    // asm 0000615F: 	ADDI	3,R0
    // asm 00006160: 	STI	R0,@(_ARPS+(3*3)+1)
    // asm 00006161: 	STI	R0,@(_ARPS+(2*3)+1)
    // asm 00006162: 	LDL	H2HPAL1,AR2
    // asm 00006163: 	CALL	PAL_FIND_RAW
    // asm 00006164: 	STI	R0,@_ACMAP
    // asm 00006165: 	LDI	TM|ZS,R0		;red (RACER)
    // asm 00006166: 	STI	R0,@_ACNTL
    // asm 00006167: 	LDI	@DIPRAM,R0
    // asm 00006168: 	TSTB	CMDP_MASTER,R0
    // asm 00006169: 	BZ	GL14
    // asm 0000616A: 	LDL	h2p1a_I,R0
    // asm 0000616B: 	STI	R0,@_ADDRL
    // asm 0000616C: 	LDI	@_ARPS+(0*3)+1,R0
    // asm 0000616D: 	SUBI	16,R0
    // asm 0000616E: 	STI	R0,@_ARPS+(0*3)+1
    // asm 0000616F: 	LDI	@_ARPS+(1*3)+1,R0
    // asm 00006170: 	SUBI	16,R0
    // asm 00006171: 	STI	R0,@_ARPS+(1*3)+1
    // asm 00006172: 	LDI	@_ARPS+(0*3),R0
    // asm 00006173: 	SUBI	13,R0
    // asm 00006174: 	STI	R0,@_ARPS+(0*3)
    // asm 00006175: 	LDI	@_ARPS+(3*3),R0
    // asm 00006176: 	SUBI	13,R0
    // asm 00006177: 	STI	R0,@_ARPS+(3*3)
    // asm 00006178: 	LDI	00038h,R0
    // asm 00006179: 	STI	R0,@_AIVI
    // asm 0000617A: 	LDI	00041h,R0
    // asm 0000617B: 	STI	R0,@_AIVI+1
    // asm 0000617C: 	LDI	00D41h,R0
    // asm 0000617D: 	STI	R0,@_AIVI+2
    // asm 0000617E: 	LDI	00D38h,R0
    // asm 0000617F: 	STI	R0,@_AIVI+3
    // asm 00006180: 	BU	GL15
GL14:
    // asm 00006181: LDL	h2p2b_I,R0
    // asm 00006182: 	STI	R0,@_ADDRL
    // asm 00006183: 	LDI	@_ARPS+(0*3)+1,R0
    // asm 00006184: 	SUBI	16,R0
    // asm 00006185: 	STI	R0,@_ARPS+(0*3)+1
    // asm 00006186: 	LDI	@_ARPS+(1*3)+1,R0
    // asm 00006187: 	SUBI	16,R0
    // asm 00006188: 	STI	R0,@_ARPS+(1*3)+1
    // asm 00006189: 	LDI	@_ARPS+(0*3),R0
    // asm 0000618A: 	SUBI	13,R0
    // asm 0000618B: 	STI	R0,@_ARPS+(0*3)
    // asm 0000618C: 	LDI	@_ARPS+(3*3),R0
    // asm 0000618D: 	SUBI	13,R0
    // asm 0000618E: 	STI	R0,@_ARPS+(3*3)
    // asm 0000618F: 	LDI	00041h,R0
    // asm 00006190: 	STI	R0,@_AIVI
    // asm 00006191: 	LDI	0004Bh,R0
    // asm 00006192: 	STI	R0,@_AIVI+1
    // asm 00006193: 	LDI	00D4Bh,R0
    // asm 00006194: 	STI	R0,@_AIVI+2
    // asm 00006195: 	LDI	00D41h,R0
    // asm 00006196: 	STI	R0,@_AIVI+3
GL15:
    // asm 00006197: 	CMPI	0,R6
    // asm 00006198: 	BEQ	DADA4
    // 	;we have a case where the other player is either behind us
    // 	;or in front of us
    // 	;
    // asm 00006199: 	LDI	*+AR0(OCARBLK),AR1
    // asm 0000619A: 	LDI	*+AR1(CAR_OM),R0
#if DEBUG
    // asm: 	BZ	$	;debugging please remove
#endif
    // asm 0000619B: 	LDI	*+AR1(CARTRACK_ID),R0
    // asm 0000619C: 	LDI	@PLYCBLK,AR2
    // asm 0000619D: 	LDI	*+AR2(CARTRAK),AR2
    // asm 0000619E: 	LDI	*+AR2(OUSR1),R1
    // asm 0000619F: 	CMPI	R1,R0
    // asm 000061A0: 	BLT	ISBEHIND
    // ;	BLE	ISBEHIND
    // ;	LDI	RADAR_XCNTR,R0
    // ;	ADDI	9,R0
    // ;	STI	R0,@_ARPS+(1*3)
    // ;	STI	R0,@_ARPS+(2*3)
    // ;	SUBI	18,R0
    // ;	STI	R0,@_ARPS+(0*3)
    // ;	STI	R0,@_ARPS+(3*3)
    // asm 000061A1: 	LDI	RADAR_YMIN,R0
    // asm 000061A2: 	ADDI	10,R0
    // asm 000061A3: 	STI	R0,@_ARPS+(2*3)+1
    // asm 000061A4: 	STI	R0,@_ARPS+(3*3)+1
    // asm 000061A5: 	SUBI	20,R0
    // asm 000061A6: 	STI	R0,@_ARPS+(0*3)+1
    // asm 000061A7: 	STI	R0,@_ARPS+(1*3)+1
    // asm 000061A8: 	BU	DADA4
ISBEHIND:
    // ;	LDI	RADAR_XCNTR,R0
    // ;	ADDI	9,R0
    // ;	STI	R0,@_ARPS+(1*3)
    // ;	STI	R0,@_ARPS+(2*3)
    // ;	SUBI	18,R0
    // ;	STI	R0,@_ARPS+(0*3)
    // ;	STI	R0,@_ARPS+(3*3)
    // asm 000061A9: 	LDI	RADAR_YMAX,R0
    // asm 000061AA: 	ADDI	10,R0
    // asm 000061AB: 	STI	R0,@_ARPS+(2*3)+1
    // asm 000061AC: 	STI	R0,@_ARPS+(3*3)+1
    // asm 000061AD: 	SUBI	20,R0
    // asm 000061AE: 	STI	R0,@_ARPS+(0*3)+1
    // asm 000061AF: 	STI	R0,@_ARPS+(1*3)+1
DADA4:
    // asm 000061B0: 	CALL	_stuff_fpga
    // asm 000061B1: 	CLRI	R0
    // asm 000061B2: 	STI	R0,@_ACMAP
    // asm 000061B3: 	BU	RADAR_LP
NOTTHEOPLYR:
    // asm 000061B4: 	CMPI	RADAR_XMIN,R0
    // asm 000061B5: 	BLT	RADAR_LP
    // asm 000061B6: 	CMPI	RADAR_XMAX,R0
    // asm 000061B7: 	BGT	RADAR_LP
    // asm 000061B8: 	ADDI	@MOVEIN_OFFSET,R0
    // asm 000061B9: 	STI	R0,@(_ARPS+(0*3))
    // asm 000061BA: 	STI	R0,@(_ARPS+(3*3))
    // asm 000061BB: 	ADDI	3,R0
    // asm 000061BC: 	STI	R0,@(_ARPS+(1*3))
    // asm 000061BD: 	STI	R0,@(_ARPS+(2*3))
    // asm 000061BE: 	LDF	*+AR2(Z),R0
    // asm 000061BF: 	MPYF	0.3,R0
    // asm 000061C0: 	FIX	R0
    // asm 000061C1: 	NEGI	R0
    // asm 000061C2: 	ADDI	RADAR_YCNTR-2,R0
    // asm 000061C3: 	CMPI	RADAR_YMIN,R0
    // asm 000061C4: 	BLT	RADAR_LP
    // asm 000061C5: 	CMPI	RADAR_YMAX,R0
    // asm 000061C6: 	BGT	RADAR_LP
    // asm 000061C7: 	STI	R0,@(_ARPS+(0*3)+1)
    // asm 000061C8: 	STI	R0,@(_ARPS+(1*3)+1)
    // asm 000061C9: 	ADDI	3,R0
    // asm 000061CA: 	STI	R0,@(_ARPS+(3*3)+1)
    // asm 000061CB: 	STI	R0,@(_ARPS+(2*3)+1)
    // asm 000061CC: 	CALL	_stuff_fpga
    // asm 000061CD: 	BU	RADAR_LP
RADAR_X:
    // asm 000061CE: 	LDI	CC|1,R0
    // asm 000061CF: 	STI	R0,@_ACNTL
    // asm 000061D0: 	LDI	RADAR_XCNTR-2,R0
    // asm 000061D1: 	ADDI	@MOVEIN_OFFSET,R0
    // asm 000061D2: 	STI	R0,@(_ARPS+(0*3))
    // asm 000061D3: 	STI	R0,@(_ARPS+(3*3))
    // asm 000061D4: 	LDI	RADAR_XCNTR+1,R0
    // asm 000061D5: 	ADDI	@MOVEIN_OFFSET,R0
    // asm 000061D6: 	STI	R0,@(_ARPS+(1*3))
    // asm 000061D7: 	STI	R0,@(_ARPS+(2*3))
    // asm 000061D8: 	LDI	RADAR_YCNTR-2,R0
    // asm 000061D9: 	STI	R0,@(_ARPS+(0*3)+1)
    // asm 000061DA: 	STI	R0,@(_ARPS+(1*3)+1)
    // asm 000061DB: 	LDI	RADAR_YCNTR+1,R0
    // asm 000061DC: 	STI	R0,@(_ARPS+(3*3)+1)
    // asm 000061DD: 	STI	R0,@(_ARPS+(2*3)+1)
    // asm 000061DE: 	CALL	_stuff_fpga
    // asm 000061DF: 	POP	AR4
    // asm 000061E0: 	LDI	@HEAD2HEAD_ON,R0
    // asm 000061E1: 	BZ	NODOAP
    // asm 000061E2: 	LDI	@PLY2CAR,AR0
    // asm 000061E3: 	LDI	*+AR0(OCARBLK),AR0
    // asm 000061E4: 	LDI	@PLYCBLK,AR1
    // asm 000061E5: 	LDI	*+AR0(CARTRACK_ID),R0
    // asm 000061E6: 	LDI	*+AR1(CARTRACK_ID),R1
    // asm 000061E7: 	CMPI	R0,R1
    // asm 000061E8: 	LDIGT	1,R0
    // asm 000061E9: 	LDILE	0,R0
    // asm 000061EA: 	STI	R0,@THIS_MACHINE_AHEAD
NODOAP:
    // asm 000061EB: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "RADAR_PLOT", 0, 0);
    UNIMPL();
}
