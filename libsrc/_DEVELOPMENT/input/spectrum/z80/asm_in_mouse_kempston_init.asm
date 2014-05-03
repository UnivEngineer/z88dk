
; ===============================================================
; Sep 2005, Apr 2014
; ===============================================================
; 
; void in_mouse_kempston_init(void)
;
; Reset kempston mouse logic
;
; ===============================================================

XLIB asm_in_mouse_kempston_init
XDEF asm_in_mouse_kempston_reset

LIB asm_in_mouse_kempston

XREF __input_kempston_mouse_x, __input_kempston_mouse_y

asm_in_mouse_kempston_init:
asm_in_mouse_kempston_reset:

   ; uses : af, bc, de, hl
   
   call asm_in_mouse_kempston  ; zero out current delta
   
   ld hl,0
   ld (__input_kempston_mouse_x),hl
   ld (__input_kempston_mouse_y),hl
   
   ret
