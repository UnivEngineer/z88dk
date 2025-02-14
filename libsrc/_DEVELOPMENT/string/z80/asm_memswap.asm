
; ===============================================================
; Dec 2013 / Dec 2021 feilipu
; ===============================================================
; 
; void *memswap(void *s1, void *s2, size_t n)
;
; Swap n bytes pointed at by s1 and s2.
;
; ===============================================================

SECTION code_clib
SECTION code_string

PUBLIC asm_memswap
PUBLIC asm0_memswap

asm_memswap:

   ; enter : bc = size_t n
   ;         de = void *s2
   ;         hl = void *s1
   ;
   ; exit  : hl = void *s1
   ;         de = ptr in s2 to byte after last one written
   ;         bc = 0
   ;         carry reset
   ;
   ; uses  : af, bc, de

   ld a,b
   or c
   ret Z

asm0_memswap:
   push hl

IF __CPU_INTEL__ || __CPU_GBZ80__

   dec bc
   inc b
   inc c

loop:
   push bc

   ld a,(de)
   ld c,a

   ld a,(hl)
   ld (de+),a

   ld a,c
   ld (hl+),a

   pop bc
   dec c
   jr NZ,loop
   dec b
   jr NZ,loop

ELSE

loop:
   ld a,(de)
   ldi
   dec hl
   ld (hl),a
   inc hl
   jp PE,loop

ENDIF

   pop hl
   ret
