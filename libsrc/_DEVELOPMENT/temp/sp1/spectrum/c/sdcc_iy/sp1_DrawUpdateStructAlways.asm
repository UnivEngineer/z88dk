
; sp1_DrawUpdateStructAlways(struct sp1_update *u)

PUBLIC _sp1_DrawUpdateStructAlways

_sp1_DrawUpdateStructAlways:

   pop af
   pop hl
   
   push hl
   push af

   INCLUDE "temp/sp1/spectrum/updater/asm_sp1_DrawUpdateStructAlways.asm"
