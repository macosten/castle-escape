.export _strlen

_strlen:
	sta TEMP
	stx TEMP+1

strlen:
    ldy #$00        ; 2 bytes, 2 cycles
    clv             ; 1 byte, 2 cycles
  slloop:
    lda (TEMP), Y    ; 2 bytes, 5 cycles
    beq slend       ; 2 bytes, 2-3 cycles
    iny             ; 1 byte, 2 cycles
    bvc slloop      ; 2 bytes, 3 cycles
  slend:
    tya             ; 1 byte, 2 cycles
    rts             ; 1 byte, 6 cycles