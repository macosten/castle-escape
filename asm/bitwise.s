;
;  bitwise.s
;  Castle Escape
;
;  Created by Zaccari Silverman on 6/30/20.
;

.export _clear_object_bitfield, _set_object_bit

.segment "BSS"

OBJECT_BITFIELD: .res 32

.segment "CODE"

;void __fastcall__ clear_object_bitfield(void)
_clear_object_bitfield:
	lda #0 ; load 0 into A and 0 into y
	ldy #0
	clear_object_bitfield_loop:
		cpy #32 ; if y >= 32, branch to rts
		beq clear_object_bitfield_done
		sta OBJECT_BITFIELD,y ; Store A at the object bitfield offset by Y
		iny ; increment y
		jmp clear_object_bitfield_loop ; loop

	clear_object_bitfield_done:
	rts

;void __fastcall__ set_object_bit(unsigned char index)
_set_object_bit:
    sta TEMP ; We'll need this original index value later so let's store it.
    
    ; divide by 8 to get the byte offset.
    lsr a
    lsr a
    lsr a
    tax ; Transfer A (the byte offset) to X (the register); eventually we'll want to put it in Y
    
    ; Now let's calculate the bit offset, then get the correct bit mask.

    lda TEMP ; Retrieve the original index value
    and #7 ; mask away all but the bottom 3 bits; the A register now contains this masked value
    tay ; Transfer A (the bit offset) to Y (the register)

    lda bitsettable,y ; Load the correct set bit value into A
    sta TEMP ; Save this at TEMP

    txa ; Transfer X (the register with the byte offset) to Y (since we don't need the bit offset anymore now that we have the mask in A).
    tay

    ; Unfortunately the lack of a txy means that A just got destroyed, so let's restore it:
    lda TEMP

    ; Logical or A with the correct byte in the object bitfield; the result is stored in A.
    ora OBJECT_BITFIELD,y 
    sta OBJECT_BITFIELD,y ; overwrite the original byte with the new one. 
    
    rts

; Each value in this table is 2^n, where n is the offset. 
; Stored as a table rather than calculated on the fly just to save the hassle of looping.
bitsettable:
    .byte 1, 2, 4, 8, 16, 32, 64, 128
