;
;	Sharp specific routines
;	by Stefano Bodrato, Fall 2013
;
;	int set_psg(int reg, int val);
;
;	Play a sound by PSG
;
;
;	$Id: set_psg_callee.asm,v 1.2 2014-09-28 18:26:58 pauloscustodio Exp $
;

	XLIB	set_psg_callee

	XDEF ASMDISP_SET_PSG_CALLEE

	
set_psg_callee:

   pop hl
   pop de
   ex (sp),hl
	
.asmentry

    LD	BC,$20
	OUT	(C),l

	ld	c,$60
	OUT	(C),e
	ret

DEFC ASMDISP_SET_PSG_CALLEE = # asmentry - set_psg_callee

