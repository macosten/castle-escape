.define NO_CHR_BANK 255

.segment "ZEROPAGE"
	BP_BANK: .res 1
    nmiChrTileBank: .res 1
    .exportzp nmiChrTileBank, BP_BANK

.segment "CODE"

.export _set_prg_bank, _get_prg_bank, _set_chr_bank_0, _set_chr_bank_1
.export _set_nmi_chr_tile_bank, _unset_nmi_chr_tile_bank
.export _set_mirroring, _set_mmc1_ctrl, _banked_call


; Converted from C:

.define MAX_BANK_DEPTH 10

BANK_LEVEL: .res 1
BANK_BUFFER: .res MAX_BANK_DEPTH

.export _bank_pop, _bank_push

; sets the bank at $8000-bfff
_set_prg_bank:
    sta BP_BANK
    mmc1_register_write MMC1_PRG
    rts

; returns the current bank at $8000-bfff	
_get_prg_bank:
    lda BP_BANK
	ldx #0
    rts

; sets the first CHR bank
_set_chr_bank_0:
    mmc1_register_write MMC1_CHR0
    rts

; sets the second CHR bank	
_set_chr_bank_1:
    mmc1_register_write MMC1_CHR1
    rts

; for split screens with different CHR bank at top
_set_nmi_chr_tile_bank: 
    sta nmiChrTileBank
    rts

; for split screens with different CHR bank at top... disable it
_unset_nmi_chr_tile_bank:
    lda #NO_CHR_BANK
    sta nmiChrTileBank
    rts

_set_mirroring:
    ; Limit this to mirroring bits, so we can add our bytes safely.
    and #%00000011
    ; Now, set this to have 4k chr banking, and not mess up which prg bank is which.
    ora #%00011100
Ctrl_common:
    mmc1_register_write MMC1_CTRL
    rts
	
; write all 5 bits of the $8000 control register
; in case you want to change the MMC1 settings besides mirroring (not recommended)
_set_mmc1_ctrl:
	and #$1f ;remove upper 3 bits
	jmp Ctrl_common
	
; "Hand-compiled" from C to 6502 asm (but untested):

; void __fastcall__ bank_pop(void);
_bank_pop:
    ; --BANK_LEVEL;
    dec     BANK_LEVEL
    ; if (BANK_LEVEL > 0) { (beq will branch if dec set the zero flag) 
    ; (i.e if BANK_LEVEL is now zero)
    beq     bankLevelIsZero
    ; set_prg_bank(BANK_BUFFER[BANK_LEVEL-1]);
    ldy     BANK_LEVEL
    dey
    lda     BANK_BUFFER,y
    jmp     _set_prg_bank
    ; }
bankLevelIsZero:  rts
	
; void __fastcall__ bank_push(unsigned char bankId);
_bank_push:
    ; Accumulator is already bankId.
    ; BANK_BUFFER[BANK_LEVEL] = bankID;
    ldy BANK_LEVEL
    sta BANK_BUFFER,y
    ; ++BANK_LEVEL;
    inc BANK_LEVEL
    ; set_prg_bank(bankID);
    jmp _set_prg_bank
	
; Call a function pointed to by TEMP.
calltemp:
    jmp (TEMP)

; void __fastcall__ banked_call(unsigned char bankId, void (*method)(void));
_banked_call:
    ; Save the pointer for later.
    sta TEMP
    stx TEMP+1
    ; bank_push(bankId)
    jsr popa
    jsr _bank_push
    ; (*method)();
    jsr calltemp
    ; bank_pop();
    jmp _bank_pop ; rts will be executed eventually in _set_prg_bank
