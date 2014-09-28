; int __CALLEE__ vscanf_callee(const char *format, void *arg_ptr)
; 05.2008 aralbrec

XLIB vscanf_callee
XDEF ASMDISP_VSCANF_CALLEE

LIB vfscanf_callee
XREF ASMDISP_VFSCANF_CALLEE, _stdin

.vscanf_callee

   pop hl
   pop bc
   pop de
   push hl

.asmentry

   ; enter : de  = format string
   ;         bc  = & parameter list (arg_ptr)
   ; exit  : hl  = number of conversions done
   ;         carry if stream error: ERRNO set and hl=-1

   ld ix,(_stdin)
   jp vfscanf_callee + ASMDISP_VFSCANF_CALLEE

defc ASMDISP_VSCANF_CALLEE = # asmentry - vscanf_callee
