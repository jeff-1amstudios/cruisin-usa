#include "../../core/cpu.h"
#include "../../core/machine.h"
#include "../include/macs.h"
#include "../include/mproc.h"
#include "../include/obj.h"
#include "../include/vunit.h"
#include "../include/cmos.h"
#include "../include/sysid.h"
#include "../include/sys.h"
#include "../include/globals.h"
#include "../include/sndtab.h"
#include "../include/pall.h"
#include "../include/objects.h"
#include "../include/text.h"
#include "../include/delta.h"
#include "../include/comm.h"
#include "../include/racer.h"
#include "../include/h2hobj.h"
#include "../include/commdrne.h"

/*
 * Source module: asm/COMMDRNE.ASM
 */

// *----------------------------------------------------------------------------
// *
// *
// *COPYRIGHT (C) 1994 BY  TV GAMES, INC.
// *ALL RIGHTS RESERVED
// *
// *----------------------------------------------------------------------------
// *Startup Position Table
// *
// *39 words per plyr (13*3)
// *
// *
/* asm: STARTUP_POS_TABLE:	;PLYR1  (on PLYR2) */
/* asm: 	.float	576,-180,3287			;GG */
/* asm: 	.float	-1928700,4654,584166		;SF */
/* asm: 	.float	-2611877,14000,-628750		;H280 */
/* asm: 	.float	-3277335,15063,-1078194		;RW */
/* asm: 	.float	-2257363,21013,-1158765		;BH */
/* asm: 	.float	-2426363,10814,-2164529		;LA */
/* asm: 	.float	-4120644,20316,-3599815		;DV */
/* asm: 	.float	-4073652,24315,-3414549		;AZ */
/* asm: 	.float	-2802001,12395,-3353785		;GC */
/* asm: 	.float	-2465719,6191,-4562865		;IOWA */
/* asm: 	.float	-3156184,-9609,-4101941		;CH */
/* asm: 	.float	-3514890,-3107,-3573365		;IN */
/* asm: 	.float	-2353033,-16317,-2927294	;APPAL */
/* asm: 	;PLYR2 (on PLYR1) */
/* asm: 	.float	1728,-177,2875			;GG */
/* asm: 	.float	-1929780,4638,583767		;SF */
/* asm: 	.float	-2611381,13982,-627690		;H280 */
/* asm: 	.float	-3276459,15050,-1078966		;RW */
/* asm: 	.float	-2257834,21013,-1157630		;BH */
/* asm: 	.float	-2426125,10798,-2163396		;LA */
/* asm: 	.float	-4121699,20312,-3600288		;DV */
/* asm: 	.float	-4074714,24350,-3414037		;AZ */
/* asm: 	.float	-2802209,12410,-3355048		;GC */
/* asm: 	.float	-2466458,6193,-4563828		;IOWA */
/* asm: 	.float	-3157144,-9607,-4101204		;CH */
/* asm: 	.float	-3515279,-3107,-3574450		;IN */
/* asm: 	.float	-2353891,-16335,-2928112	;APPAL */
float STARTUP_POS_TABLE[78] = {
    576.0f, -180.0f, 3287.0f, // GG
    -1928700.0f, 4654.0f, 584166.0f, // SF
    -2611877.0f, 14000.0f, -628750.0f, // H280
    -3277335.0f, 15063.0f, -1078194.0f, // RW
    -2257363.0f, 21013.0f, -1158765.0f, // BH
    -2426363.0f, 10814.0f, -2164529.0f, // LA
    -4120644.0f, 20316.0f, -3599815.0f, // DV
    -4073652.0f, 24315.0f, -3414549.0f, // AZ
    -2802001.0f, 12395.0f, -3353785.0f, // GC
    -2465719.0f, 6191.0f, -4562865.0f, // IOWA
    -3156184.0f, -9609.0f, -4101941.0f, // CH
    -3514890.0f, -3107.0f, -3573365.0f, // IN
    -2353033.0f, -16317.0f, -2927294.0f, // APPAL
    1728.0f, -177.0f, 2875.0f, // GG
    -1929780.0f, 4638.0f, 583767.0f, // SF
    -2611381.0f, 13982.0f, -627690.0f, // H280
    -3276459.0f, 15050.0f, -1078966.0f, // RW
    -2257834.0f, 21013.0f, -1157630.0f, // BH
    -2426125.0f, 10798.0f, -2163396.0f, // LA
    -4121699.0f, 20312.0f, -3600288.0f, // DV
    -4074714.0f, 24350.0f, -3414037.0f, // AZ
    -2802209.0f, 12410.0f, -3355048.0f, // GC
    -2466458.0f, 6193.0f, -4563828.0f, // IOWA
    -3157144.0f, -9607.0f, -4101204.0f, // CH
    -3515279.0f, -3107.0f, -3574450.0f, // IN
    -2353891.0f, -16335.0f, -2928112.0f, // APPAL
};
/* asm: PLY2CAR	.bss	PLY2CAR,1 */
int PLY2CAR;
// *----------------------------------------------------------------------------
// *Communications drone
// *
// *
// *
#endif
// *STORE OBJECT POINTER
// *SET PROPER PALETTE
// ;	LDI	*+AR7(INITINDEX),AR0
// ;
// ;	LDI	*+AR0(RD_PALETTE),R0
// ;	BZ	NOOTHERPAL
// ;
// ;	LDI	R0,AR2
// ;	CALL	PAL_FIND_RAW
// ;	STI	R0,*+AR4(OPAL)
// ;	ORM	O_1PAL,*+AR4(OFLAGS)
// ;NOOTHERPAL
// *
// *INIT POSITION N ROAD PIECES FROM BEGINNING
// *
// 	;initialize Ytheta to the intentional direction
// *INIT THROTTLE AND BRAKE
// 	;Z == MASTER
// ;	LDF	*AR2++,R0
// ;	STF	R0,*+AR4(OPOSX)
// ;	LDF	*AR2++,R0
// ;	STF	R0,*+AR4(OPOSY)
// ;	LDF	*AR2++,R0
// ;	STF	R0,*+AR4(OPOSZ)
// ;	LDI	AR4,AR2
// ;	CALL	OBJ_PULL
// ;	CALL	OBJ_INSERT
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------
// *----------------------------------------------------------------------------

void COMM_DRONE_PTR_SORT(void)
{
    // asm: 	LDI	@COMM_DRONE_PTR,AR6
    // asm: 	LDI	@PLY2CAR,AR4
REGULAR:
    // asm: LDI	*+AR4(ODIST),IR1
    // asm: 	ASH	-4,IR1			;quickly divide by 16
    // asm: 	LDI	@INVTABI,AR2		;inverse table dedicated ptr
    // asm: 	LDF	*+AR2(IR1),R0
    // asm: 	CALL	INV_F30
    // asm: 	CMPF	16,R0			;
    // asm: 	LDFLT	16,R0			;
    // asm: 	STF	R0,*+AR6(OMAT00)
    // asm: 	STF	R0,*+AR6(OMAT11)
    // asm: 	STF	R0,*+AR6(OMAT22)
    // asm: 	LDF	*+AR4(OPOSX),R0
    // asm: 	STF	R0,*+AR6(OPOSX)
    // asm: 	LDF	*+AR4(OPOSY),R0
    // asm: 	FLOAT	35,R1
    // asm: 	MPYF	*+AR6(OMAT00),R1
    // asm: 	SUBF	R1,R0
    // asm: 	SUBF	20,R0
    // asm: 	STF	R0,*+AR6(OPOSY)
    // asm: 	LDF	*+AR4(OPOSZ),R0
    // asm: 	STF	R0,*+AR6(OPOSZ)
    // asm: 	LDI	AR6,AR2
    // asm: 	CALL	OBJ_PULL
    // asm: 	LDI	*AR4,R0
    // asm: 	STI	R0,*AR6
    // asm: 	STI	AR6,*AR4
    // asm: 	LDI	*+AR4(OFLAGS),R0
    // asm: 	AND	O_LIST_M,R0
    // asm: 	OR	*+AR6(OFLAGS),R0
    // asm: 	STI	R0,*+AR6(OFLAGS)
    // ;
    // ;	CALL	OBJ_INSERT
    // asm: 	RETS
    TRACE_EVENT(&g_crusn_machine->trace, "function", "COMM_DRONE_PTR_SORT", 0, 0);
    UNIMPL();
}

void OM_DRONE(void)
{
    // *----------------------------------------------------------------------------
    // *
    // *OTHER MACHINE DRONE LOOP
    // *
    // asm: 	LDI	1,R0			;OTHER GUYS CAR NOW....
    // asm: 	STI	R0,*+AR5(CAR_OM)
OM_DRONEL:
    // asm: 	LDI	*+AR4(OCARBLK),R3	;GET CAR DATA AREA
    // asm: 	PUSH	AR4
    // asm: 	PUSH	AR5
    // asm: 	CALL	ROADSCAN		;MAKE SORT WORK RIGHT
    // asm: 	POP	AR5
    // asm: 	POP	AR4
    // asm: 	LDF	0,R0			;CLEAR OUT SOME SHIT
    // asm: 	STF	R0,*+AR5(CARDROT)
    // asm: 	STF	R0,*+AR5(CARSPRAD)
    // asm: 	STF	R0,*+AR5(CARSKID)
    // asm: 	LDF	*+AR5(CARYROT),R0
    // asm: 	STF	R0,*+AR4(ORADY)		;STORE CAR OBJECT RADY
    // asm: 	LDI	0,R0
    // asm: 	STF	R0,*+AR5(CAR_SPIN)
    // ******************************
    // *TEST FOR NO UPDATE
    // asm: 	LDI	0,R0
    // asm: 	STI	R0,*+AR5(CARUPD)
    // asm: 	SLEEP	1
    // asm: 	LDI	*+AR5(CARUPD),R0
    // asm: 	BNE	DRONELL
    // asm: 	NOP
DRONELL:
    // ************
    // asm: 	B	OM_DRONEL
    TRACE_EVENT(&g_crusn_machine->trace, "function", "OM_DRONE", 0, 0);
    UNIMPL();
}
