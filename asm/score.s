; Adapted from a snippet I found on http://forum.6502.org/viewtopic.php?f=2&t=4894

.export _convert_to_decimal, _score_string

.segment "BSS"

_score_string: .res 5 ; Max score of "65535" - 1 extra byte for debug

.segment "CODE"

; void _fastcall_ convert_to_decimal(unsigned int score);
_convert_to_decimal: ; A contains the low byte, X contains the high byte
    ldy #0 ; Load the index for score_string into Y
    ; Store A and X at TEMP and TEMP+1, respectively
    sta TEMP
    txa
    sta TEMP+1
    ; TEMP now contains the number we're looking to convert (in the correct byte order, too, I think). 
    ; clear the score string buffer
    lda #0
    sta _score_string
    sta _score_string+1
    sta _score_string+2
    sta _score_string+3
    sta _score_string+4
    ; Other variables for the loop:
loop_head: ; prnum2
    lda #0
    sta TEMP+3 ; gthan+1 - clear the BCD digit
    lda #16
    sta TEMP+2 ; gthan - {>} = loop counter
step_1: ; prdiv1
	asl TEMP	; Gradually, we replace the saved value at TEMP with the quotient of TEMP/10
	rol TEMP+1 	;
	rol TEMP+3	; ...and the BCD value will be replaced with the remainder of the division.
	lda TEMP+3
	sec
	sbc #10		; Partial BCD >= 10?
	bcc step_2  ; If not: skip ahead
	sta TEMP+3	; If yes: Update the partial result...
	inc TEMP	; ...and set the low bit in the partial quotient. (?)
step_2: ; prdiv2
	dec TEMP+2
	bne step_1	; Loop 16 times
	lda TEMP+3
	;ora #'0' ; If we were converting the BCD result to ASCII (but we're not)
	; Now for where we actually diverge somewhat: instead of pha-ing, we're going to store A in the _score_string buffer.
	sta _score_string,y 
	iny ; increment y since we just put something at y's old index.
	lda TEMP ; check if the number is zero...
	ora TEMP+1
	bne loop_head ; if not, go  back to the loop head.
    rts