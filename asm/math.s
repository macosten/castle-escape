.export _atan2,_sin_lookup,_cos_lookup,_brads_lookup,_abs_subtract

octant		= TEMP			;; temporary zeropage variable
x1			= TEMP+1
x2			= TEMP+2
y1			= TEMP+3
y2			= TEMP+4
dx 			= TEMP+5
dy 			= TEMP+6

; void __fastcall__ atan2(unsigned char x1, unsigned char x2, unsigned char y1, unsigned char y2)
;; Adapted from https://codebase64.org/doku.php?id=base:8bit_atan2_8-bit_angle -- 

_atan2:	
	; Move all the appropriate values to the appropriate locations.
	sta y2

	jsr popa
	sta y1

	jsr popa
	sta x2

	jsr popa
	; sta x1
	; Here's the start of the actual routine.
	; lda x1 ; x1 is already in the accumulator.
	;sec
	sbc x2
	bcs @1
	eor #$ff
	clc
	adc #$01
@1:
	; Take only the top 4 bits.
	; >> 4
	lsr
	lsr
	lsr
	lsr

	sta dx
	; rol octant

	lda y1
	;sec
	sbc y2
	bcs @2
	eor #$ff
	clc
	adc #$01

@2:
	; Take only the top 4 bits.
	ora #$f0

	; add A with dx
	adc dx

	; now we should have a working offset, so...
	tax

	lda brads_table,x

	rts

	; The accumulator (should) now store the angle between (x1, y1) and (x2, y2)
	; in binary radians (brad).
	; I.e 90 degrees = 64 (0x40) brad, 45 (0x20) degrees = 0x20 brad...

; Subtract b from a. Negate the result if b > a.
; unsigned char __fastcall__ abs_subtract(unsigned char a, unsigned char b)
_abs_subtract:
	; Store b in TEMP
	sta TEMP
	; get a in the Accumulator
	jsr popa
	; a - b
	sec
	sbc TEMP
	bcs @1
	eor #$ff
	adc #$01
@1:
	rts


; Look up the value of sin(theta), and return an unsigned char meant to be a subpixel offset.
; Subtract this from your x if theta >= 128.

; unsigned char __fastcall__ sin_lookup(unsigned char theta)
_sin_lookup:
	; theta = theta mod 128, because the top bit really just tells us if we should subtract or not.
	and #%01111111
	; Store theta in x.
	tax
	; Fetch the value from the lookup table.
	lda half_sin_table,x
	rts

; unsigned char __fastcall__ cos_lookup(unsigned char theta)
; basically sin_lookup, but adding offsetting theta first to turn sin into cos
_cos_lookup:
	adc #$3F
	; If we overflowed, we want to add one to the accumulator. Otherwise, we don't.
	bcc _sin_lookup
 	; Note that since the carry bit is set, this will actually add one
	adc #$00
	jmp _sin_lookup

.segment "RODATA"

; unsigned char brads_lookup(i)
; If you're moving positive in this direction, add the rval. Else, subtract it.
_brads_lookup:
	tax
	lda brads_table,x
	rts
	

brads_table:
	.byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
	.byte $40,$20,$13,$0d,$0a,$08,$07,$06,$05,$04,$04,$04,$03,$03,$03,$03
	.byte $40,$2d,$20,$18,$13,$0f,$0d,$0b,$0a,$09,$08,$07,$07,$06,$06,$05
	.byte $40,$33,$28,$20,$1a,$16,$13,$10,$0f,$0d,$0c,$0b,$0a,$09,$09,$08
	.byte $40,$36,$2d,$26,$20,$1b,$18,$15,$13,$11,$0f,$0e,$0d,$0c,$0b,$0b
	.byte $40,$38,$30,$2a,$24,$20,$1c,$19,$17,$15,$13,$11,$10,$0f,$0e,$0d
	.byte $40,$39,$33,$2d,$28,$24,$20,$1d,$1a,$18,$16,$14,$13,$12,$10,$0f
	.byte $40,$3a,$34,$2f,$2b,$27,$23,$20,$1d,$1b,$19,$17,$15,$14,$13,$12
	.byte $40,$3b,$36,$31,$2d,$29,$26,$23,$20,$1d,$1b,$1a,$18,$16,$15,$14
	.byte $40,$3b,$37,$33,$2f,$2b,$28,$25,$22,$20,$1e,$1c,$1a,$19,$17,$16
	.byte $40,$3c,$38,$34,$30,$2d,$2a,$27,$24,$22,$20,$1e,$1c,$1b,$19,$18
	.byte $40,$3c,$38,$35,$32,$2e,$2b,$29,$26,$24,$22,$20,$1e,$1d,$1b,$1a
	.byte $40,$3c,$39,$36,$33,$30,$2d,$2a,$28,$26,$24,$22,$20,$1e,$1d,$1b
	.byte $40,$3d,$3a,$37,$34,$31,$2e,$2c,$29,$27,$25,$23,$21,$20,$1e,$1d
	.byte $40,$3d,$3a,$37,$34,$32,$2f,$2d,$2b,$29,$27,$25,$23,$21,$20,$1e
	.byte $40,$3d,$3a,$38,$35,$33,$30,$2e,$2c,$2a,$28,$26,$24,$23,$21,$20

; Used in sin_lookup and cos_lookup
half_sin_table:
	.byte $00,$06,$0C,$12,$19,$1F,$25,$2B
	.byte $31,$38,$3E,$44,$4A,$50,$56,$5C
	.byte $61,$67,$6D,$73,$78,$7E,$83,$88
	.byte $8E,$93,$98,$9D,$A2,$A7,$AB,$B0
	.byte $B5,$B9,$BD,$C1,$C5,$C9,$CD,$D1
	.byte $D4,$D8,$DB,$DE,$E1,$E4,$E7,$EA
	.byte $EC,$EE,$F1,$F3,$F4,$F6,$F8,$F9
	.byte $FB,$FC,$FD,$FE,$FE,$FF,$FF,$FF 
	.byte $FF,$FF,$FF,$FE,$FE,$FD,$FC,$FB
	.byte $F9,$F8,$F6,$F4,$F3,$F1,$EE,$EC
	.byte $EA,$E7,$E4,$E1,$DE,$DB,$D8,$D4
	.byte $D1,$CD,$C9,$C5,$C1,$BD,$B9,$B5
	.byte $B0,$AB,$A7,$A2,$9D,$98,$93,$8E
	.byte $88,$83,$7E,$78,$73,$6D,$67,$61
	.byte $5C,$56,$50,$4A,$44,$3E,$38,$31
	.byte $2B,$25,$1F,$19,$12,$0C,$06,$00


