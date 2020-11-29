;
; File generated by cc65 v 2.18 - N/A
;
	.fopt		compiler,"cc65 v 2.18 - N/A"
	.setcpu		"6502"
	.smart		on
	.autoimport	on
	.case		on
	.debuginfo	off
	.importzp	sp, sreg, regsave, regbank
	.importzp	tmp1, tmp2, tmp3, tmp4, ptr1, ptr2, ptr3, ptr4
	.macpack	longbranch
	.export		_bankLevel
	.export		_bankBuffer
	.export		_banked_call
	.import		_bank_push
	.import		_bank_pop

.segment	"BSS"

_bankLevel:
	.res	1,$00
_bankBuffer:
	.res	10,$00

; ---------------------------------------------------------------
; void __near__ banked_call (unsigned char, __near__ __near__ function returning void *)
; ---------------------------------------------------------------

.segment	"CODE"

.proc	_banked_call: near

.segment	"CODE"

	jsr     pushax
	ldy     #$02
	lda     (sp),y
	jsr     _bank_push
	ldy     #$01
	lda     (sp),y
	tax
	dey
	lda     (sp),y
	jsr     callax
	jsr     _bank_pop
	jmp     incsp3

.endproc
