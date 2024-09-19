; This file is for the FamiTone2 library and was generated by FamiStudio

music_data_castle_escape:
	.byte 7
	.word @instruments
	.word @samples-3
; 00 : Menu Music
	.word @song0ch0
	.word @song0ch1
	.word @song0ch2
	.word @song0ch3
	.word @song0ch4
	.word 307,256
; 01 : Pizzicato
	.word @song1ch0
	.word @song1ch1
	.word @song1ch2
	.word @song1ch3
	.word @song1ch4
	.word 307,256
; 02 : Level Music 3
	.word @song2ch0
	.word @song2ch1
	.word @song2ch2
	.word @song2ch3
	.word @song2ch4
	.word 307,256
; 03 : Boss
	.word @song3ch0
	.word @song3ch1
	.word @song3ch2
	.word @song3ch3
	.word @song3ch4
	.word 307,256
; 04 : Death
	.word @song4ch0
	.word @song4ch1
	.word @song4ch2
	.word @song4ch3
	.word @song4ch4
	.word 307,256
; 05 : Level Begin
	.word @song5ch0
	.word @song5ch1
	.word @song5ch2
	.word @song5ch3
	.word @song5ch4
	.word 307,256
; 06 : Level Music 1
	.word @song6ch0
	.word @song6ch1
	.word @song6ch2
	.word @song6ch3
	.word @song6ch4
	.word 307,256

.export music_data_castle_escape
.global FAMISTUDIO_DPCM_PTR

@instruments:
	.byte $b0 ; 00 : Bass
	.word @env6, @env8, @env8
	.byte $00
	.byte $b0 ; 01 : Bleeps
	.word @env17, @env8, @env8
	.byte $00
	.byte $30 ; 02 : Crash Cymbal / Whistle
	.word @env12, @env4, @env8
	.byte $00
	.byte $30 ; 03 : GBA Snare (8-#)
	.word @env3, @env13, @env8
	.byte $00
	.byte $b0 ; 04 : Harpsichord-Alt1
	.word @env15, @env8, @env8
	.byte $00
	.byte $b0 ; 05 : LeadChorus
	.word @env16, @env8, @env0
	.byte $00
	.byte $70 ; 06 : LeadVerse1
	.word @env18, @env8, @env0
	.byte $00
	.byte $30 ; 07 : LeadVerse2
	.word @env18, @env8, @env0
	.byte $00
	.byte $30 ; 08 : NoiseBassDrum
	.word @env5, @env2, @env8
	.byte $00
	.byte $30 ; 09 : NoiseCymbal
	.word @env1, @env11, @env8
	.byte $00
	.byte $30 ; 0a : NoiseHiHat
	.word @env7, @env14, @env8
	.byte $00
	.byte $30 ; 0b : NoiseSnare
	.word @env10, @env9, @env8
	.byte $00

@env0:
	.byte $c0,$1d,$c0,$c1,$c2,$c2,$c1,$c0,$bf,$be,$bd,$be,$bf,$c0,$00,$02
@env1:
	.byte $ca,$c6,$c3,$c0,$00,$03
@env2:
	.byte $c0,$bf,$c1,$00,$02
@env3:
	.byte $cf,$c6,$cd,$c9,$c7,$c5,$c3,$c2,$c1,$02,$c0,$00,$0a
@env4:
	.byte $cb,$cc,$cd,$ce,$cd,$00,$04
@env5:
	.byte $cf,$cd,$c6,$c4,$c3,$c0,$00,$05
@env6:
	.byte $cf,$7f,$00,$00
@env7:
	.byte $ca,$ca,$c7,$c6,$c6,$c5,$04,$c4,$c3,$c2,$c1,$05,$c0,$00,$0c
@env8:
	.byte $c0,$7f,$00,$01
@env9:
	.byte $c0,$c2,$c5,$00,$02
@env10:
	.byte $cb,$cb,$c5,$03,$c4,$03,$c3,$03,$c2,$00,$08
@env11:
	.byte $c0,$c1,$c2,$00,$02
@env12:
	.byte $ca,$cd,$cc,$cc,$cb,$02,$ca,$ca,$c9,$c8,$c8,$c7,$c7,$c6,$02,$c5,$c5,$c4,$02,$c3,$04,$c2,$13,$c1,$22,$c0,$00,$19
@env13:
	.byte $b9,$c0,$00,$01
@env14:
	.byte $c0,$c1,$03,$c2,$c1,$c2,$c1,$c2,$c1,$c2,$00,$09
@env15:
	.byte $c7,$c6,$c5,$c5,$c4,$c4,$c3,$00,$06
@env16:
	.byte $c5,$c6,$c7,$00,$02
@env17:
	.byte $ca,$c9,$c8,$c6,$c4,$00,$04
@env18:
	.byte $c6,$c6,$c7,$02,$c6,$03,$c5,$00,$06

@samples:

@song0ch0:
	.byte $fb, $01, $f9, $f9, $cb, $f9, $f9, $cb, $f9, $f9, $cb, $f9, $f9, $cb, $f9, $f9, $cb
@song0ch0loop:
	.byte $f9, $f9, $cb, $f9, $f9, $cb, $f9, $f9, $cb, $8e, $1a, $f9, $f1, $00, $28, $cb, $00, $24, $f9, $cb, $24, $a3, $00, $18
	.byte $cb, $00, $1a, $f9, $f1, $00, $28, $cb, $00, $24, $f9, $f1, $00, $2e, $a5, $2e, $a3, $00, $28, $f9, $f1, $00, $1a, $cb
	.byte $00, $24, $f9, $f3, $24, $cb, $00, $93
@song0ref75:
	.byte $24, $8f, $00, $20, $8f, $00, $1a, $8f, $00, $24, $8f, $00, $20, $8f, $00, $1a, $8f, $00, $24, $a3, $00
	.byte $ff, $13
	.word @song0ref75
	.byte $91, $91, $00, $28, $8f, $00, $24, $8f, $00, $1e, $8f, $00, $28, $8f, $00, $24, $8f, $00, $1e, $8f, $00, $28, $a3, $00
	.byte $2e, $8f, $00, $2c, $8f, $00, $24, $8f, $00, $2e, $8f, $00, $2c, $8f, $00, $24, $8f, $00, $2e, $91, $a7, $2e, $f9, $c9
	.byte $00, $cf, $fd
	.word @song0ch0loop
@song0ch1:
	.byte $f9, $f9, $cb, $f9, $f9, $cb, $f9, $f9, $cb, $f9, $f9, $cb, $88
@song0ref165:
	.byte $1a, $8f, $00, $1a, $8f, $00, $32, $8f, $00, $32, $8f, $00, $1a, $8f, $00, $1a, $8f, $00, $32, $91, $32, $8f, $00
	.byte $ff, $17
	.word @song0ref165
@song0ch1loop:
@song0ref192:
	.byte $1e, $8f, $00, $1e, $8f, $00, $36, $8f, $00, $36, $8f, $00, $1e, $8f, $00, $1e, $8f, $00, $36, $91, $36, $8f, $00
	.byte $ff, $17
	.word @song0ref192
@song0ref218:
	.byte $20, $8f, $00, $20, $8f, $00, $38, $8f, $00, $38, $8f, $00, $20, $8f, $00, $20, $8f, $00, $38, $91, $38, $8f, $00
	.byte $ff, $17
	.word @song0ref218
@song0ref244:
	.byte $24, $8f, $00, $24, $8f, $00, $3c, $8f, $00, $3c, $8f, $00, $24, $8f, $00, $24, $8f, $00, $3c, $91, $3c, $8f, $00, $28
	.byte $8f, $00, $28, $8f, $00, $40, $8f, $00, $40, $8f, $00, $16, $8f, $00, $16, $8f, $00, $2e, $91, $2e, $8f, $00
	.byte $ff, $17
	.word @song0ref165
	.byte $ff, $17
	.word @song0ref165
	.byte $ff, $17
	.word @song0ref192
	.byte $ff, $17
	.word @song0ref192
	.byte $ff, $17
	.word @song0ref218
	.byte $ff, $17
	.word @song0ref218
	.byte $ff, $2e
	.word @song0ref244
	.byte $ff, $17
	.word @song0ref165
	.byte $ff, $17
	.word @song0ref165
	.byte $ff, $17
	.word @song0ref192
	.byte $ff, $17
	.word @song0ref192
	.byte $ff, $17
	.word @song0ref218
	.byte $ff, $17
	.word @song0ref218
	.byte $ff, $2e
	.word @song0ref244
	.byte $ff, $17
	.word @song0ref165
	.byte $ff, $17
	.word @song0ref165
	.byte $fd
	.word @song0ch1loop
@song0ch2:
	.byte $80, $4a, $f9, $f9, $c9
@song0ref346:
	.byte $4e, $f9, $f9, $c9, $50, $f9, $f9, $c9, $54, $f9, $a3, $58, $cd, $46, $cd, $4a, $f9, $f9, $c9
@song0ch2loop:
	.byte $ff, $13
	.word @song0ref346
	.byte $ff, $13
	.word @song0ref346
	.byte $ff, $13
	.word @song0ref346
	.byte $fd
	.word @song0ch2loop
@song0ch3:
@song0ref378:
	.byte $90, $5a, $91, $5a, $91, $86, $50, $91, $50, $91, $90, $5a, $91, $5a, $91, $86, $50, $91, $00, $91, $90, $5a, $91, $5a
	.byte $91, $86, $50, $91, $50, $91, $90, $5a, $91, $5a, $91, $86, $50, $91, $00, $91
	.byte $ff, $20
	.word @song0ref378
	.byte $ff, $20
	.word @song0ref378
	.byte $ff, $20
	.word @song0ref378
	.byte $ff, $20
	.word @song0ref378
@song0ch3loop:
	.byte $ff, $20
	.word @song0ref378
	.byte $ff, $20
	.word @song0ref378
	.byte $ff, $20
	.word @song0ref378
	.byte $ff, $20
	.word @song0ref378
	.byte $ff, $20
	.word @song0ref378
	.byte $ff, $20
	.word @song0ref378
	.byte $ff, $20
	.word @song0ref378
	.byte $ff, $20
	.word @song0ref378
	.byte $ff, $20
	.word @song0ref378
	.byte $ff, $20
	.word @song0ref378
	.byte $ff, $20
	.word @song0ref378
	.byte $ff, $20
	.word @song0ref378
	.byte $fd
	.word @song0ch3loop
@song0ch4:
	.byte $f9, $f9, $cb, $f9, $f9, $cb, $f9, $f9, $cb, $f9, $f9, $cb, $f9, $f9, $cb
@song0ch4loop:
@song0ref486:
	.byte $f9, $f9, $cb, $f9, $f9, $cb, $f9, $f9, $cb, $f9, $f9, $cb, $f9, $f9, $cb, $f9, $f9, $cb
	.byte $ff, $12
	.word @song0ref486
	.byte $fd
	.word @song0ch4loop
@song1ch0:
	.byte $fb, $01, $f9, $f9, $cb, $f9, $f9, $cb
@song1ch0loop:
@song1ref10:
	.byte $f9, $f9, $cb, $f9, $f9, $cb, $f9, $f9, $cb, $f9, $f9, $cb, $f9, $f9, $cb
	.byte $ff, $0f
	.word @song1ref10
	.byte $ff, $0f
	.word @song1ref10
	.byte $ff, $0f
	.word @song1ref10
	.byte $ff, $0f
	.word @song1ref10
	.byte $ff, $0f
	.word @song1ref10
	.byte $ff, $0f
	.word @song1ref10
	.byte $ff, $0f
	.word @song1ref10
	.byte $ff, $0f
	.word @song1ref10
	.byte $fd
	.word @song1ch0loop
@song1ch1:
	.byte $f9, $f9, $cb, $f9, $f9, $b7, $88, $10, $91
@song1ch1loop:
@song1ref62:
	.byte $87, $00, $89, $16, $91, $00, $91, $0c, $99, $00, $89, $12, $91, $00, $91, $08, $99, $00, $89, $10, $91, $00, $91, $16
	.byte $99, $00, $89, $1a, $91, $00, $91, $20, $91, $00, $91, $1e, $a3, $00, $a7, $24, $a5, $00, $a5, $2e, $b7, $00, $bb, $1a
	.byte $91, $87, $00, $89, $20, $91, $00, $91, $16
@song1ref119:
	.byte $99, $00, $89, $1e, $91, $00, $91, $12, $99, $00, $89, $1a, $91, $00, $91, $1e, $99, $00, $89, $24, $91, $00, $91, $2a
	.byte $91, $00, $91, $28, $a3, $00, $a7, $2e, $a5, $00, $a5, $38, $b7, $00, $bb, $10, $91
	.byte $ff, $24
	.word @song1ref62
	.byte $a5, $00, $a5, $2c, $a5, $00, $a5, $36, $b7, $00, $bb, $1e, $91, $87, $00, $89, $28, $91, $00, $91, $16, $99, $00, $89
	.byte $1a, $91, $00, $91, $20, $99, $00, $89, $1e, $91, $00, $91, $10, $99, $00, $89, $14, $91, $00, $91, $16, $91, $87, $00
	.byte $89, $10, $97, $00, $8b, $16, $91, $00, $91, $14, $99, $00, $89, $16, $91, $00, $91, $10, $99, $00, $89, $16, $91, $00
	.byte $91, $12, $99, $00, $89, $10, $91
	.byte $ff, $60
	.word @song1ref62
@song1ref245:
	.byte $16, $91, $87, $00, $89, $20, $91, $00, $91, $16, $99, $00, $89, $1c, $91, $00, $91, $1a
@song1ref263:
	.byte $99, $00, $89, $24, $91, $00, $91, $1a, $99, $00, $89, $20, $91, $00, $91, $1e, $91, $87, $00, $89, $28, $91, $00, $91
	.byte $1e, $99, $00, $89, $24, $91, $00, $91, $1e, $99, $00, $89, $20, $91, $00, $91, $24, $99, $00, $89, $28, $91, $00, $91
	.byte $2a, $91, $87, $00, $89, $2e, $91, $00, $91, $28, $99, $00, $89, $2a, $91, $00, $91, $2c, $99, $00, $89, $2e, $91, $00
	.byte $91, $30, $99, $00, $89, $32, $91, $00, $91, $36, $91, $87, $00, $89, $38, $99, $00, $b1, $2e, $9b, $00, $af, $38, $9b
	.byte $00, $d7, $20, $91, $87
@song1ref364:
	.byte $00, $89, $1e, $91, $00, $91, $14, $99, $00, $89, $16, $91, $00, $91, $1a, $99
	.byte $ff, $0f
	.word @song1ref364
	.byte $91, $87, $00, $89, $1e, $99, $00, $b1, $2e, $9b, $00, $af, $36, $9b, $00, $d7, $28, $91, $87
@song1ref402:
	.byte $00, $89, $24, $91, $00, $91, $1a, $99, $00, $89, $1e, $91, $00, $91, $20, $99
	.byte $ff, $0f
	.word @song1ref402
	.byte $91, $87, $00, $89, $24, $91, $00, $91, $28
	.byte $ff, $0f
	.word @song1ref263
	.byte $24, $99, $00, $89, $20, $91, $00, $91, $16, $91, $87, $00, $89, $1e, $91, $00, $91, $20
	.byte $ff, $13
	.word @song1ref119
	.byte $1a, $91, $00, $93, $10, $8f
	.byte $ff, $60
	.word @song1ref62
	.byte $ff, $73
	.word @song1ref245
	.byte $eb, $93, $8c
@song1ref469:
	.byte $32, $f9, $cb, $34, $a5, $24, $a5, $28, $91, $93, $32, $f9, $cb, $34, $a5, $24, $a5, $28, $91, $93, $32, $a5, $34, $a5
	.byte $3c, $a5, $38, $a5, $40, $a5, $3c, $a5, $38, $a5, $32, $91, $93, $32, $a5, $34, $a5, $24, $a5, $32, $a5, $2e, $f9, $8f
	.byte $93
	.byte $ff, $1c
	.word @song1ref469
	.byte $42, $a5, $38, $a5, $32, $a5, $34, $91, $93, $2e, $cd, $2e, $a5, $32, $a5, $2a, $cd, $38, $b9
@song1ref540:
	.byte $93, $4a, $f9, $cb, $4c, $a5, $3c, $a5, $40, $91, $93, $4a, $f9, $cb, $4c, $a5, $3c, $a5, $40, $91, $93, $4a, $a5, $4c
	.byte $a5, $54, $a5, $50, $a5, $58, $a5, $54, $a5, $50, $a5, $4a, $91, $93, $4a, $a5, $4c, $a5, $3c, $a5, $4a, $a5, $46, $f9
	.byte $8f
	.byte $ff, $1d
	.word @song1ref540
	.byte $5a, $a5, $50, $a5, $4a, $a5, $4c, $91, $93, $46, $cd, $46, $a5, $4a, $a5, $42, $cd, $00, $a5, $88, $10, $91, $fd
	.word @song1ch1loop
@song1ch2:
	.byte $00, $91, $80
@song1ref620:
	.byte $20, $b9, $00, $91, $2e, $b9, $00, $91, $16, $b9, $00, $91, $2e, $b9, $00, $91
	.byte $ff, $0e
	.word @song1ref620
@song1ch2loop:
	.byte $00, $91
	.byte $ff, $10
	.word @song1ref620
	.byte $ff, $10
	.word @song1ref620
	.byte $ff, $10
	.word @song1ref620
@song1ref651:
	.byte $20, $b9, $00, $91, $38, $b9, $00, $91, $20, $b9, $00, $91, $38, $b9, $00, $91
	.byte $ff, $10
	.word @song1ref620
	.byte $1e, $b9, $00, $91, $36, $b9, $00, $91, $14, $b9, $00, $91, $36, $b9, $00, $91
	.byte $ff, $10
	.word @song1ref620
	.byte $ff, $10
	.word @song1ref620
	.byte $ff, $10
	.word @song1ref620
	.byte $ff, $10
	.word @song1ref620
	.byte $ff, $10
	.word @song1ref620
	.byte $ff, $10
	.word @song1ref651
	.byte $ff, $10
	.word @song1ref651
@song1ref707:
	.byte $24, $b9, $00, $91, $3c, $b9, $00, $91, $24, $b9, $00, $91, $3c, $b9, $00, $91, $28, $b9, $00, $91, $40, $b9, $00, $91
	.byte $2e, $b9, $00, $91, $46, $b9, $00, $91, $50, $b9, $00, $91, $46, $b9, $00, $91, $50, $b9, $00, $cd, $93, $1e, $f9, $f9
	.byte $b5, $00, $91, $1e, $b9, $00, $91, $2e, $b9, $00, $91, $36, $b9, $00, $cd, $93, $24, $f9, $f9, $b5, $00, $91, $3c, $e1
	.byte $00, $91, $32, $99, $00, $89, $38, $e1, $00, $91, $2e, $91, $87, $00, $89, $36, $e1, $00, $91, $2a, $99, $00, $89, $32
	.byte $f5, $00, $91, $00, $91
	.byte $ff, $10
	.word @song1ref620
	.byte $ff, $10
	.word @song1ref620
	.byte $ff, $10
	.word @song1ref620
	.byte $ff, $10
	.word @song1ref651
	.byte $ff, $10
	.word @song1ref651
	.byte $ff, $2d
	.word @song1ref707
@song1ref826:
	.byte $12, $b9, $00, $91, $20, $a5, $00, $91, $2a, $a5, $00, $91, $12, $b9, $00, $a5, $93
	.byte $ff, $11
	.word @song1ref826
	.byte $12, $a5, $20, $a5, $12, $a5, $20, $a5, $28
@song1ref855:
	.byte $a5, $20, $a5, $12, $a5, $20, $91, $93, $12, $a5, $20, $a5, $12, $a5, $20, $a5, $12, $cd, $00, $b9, $93
	.byte $ff, $11
	.word @song1ref826
	.byte $ff, $11
	.word @song1ref826
	.byte $12, $a5, $20, $a5, $12, $a5, $20, $a5, $2a
	.byte $ff, $15
	.word @song1ref855
@song1ref894:
	.byte $2a, $b9, $00, $91, $38, $a5, $00, $91, $42, $a5, $00, $91, $2a, $b9, $00, $a5, $93
	.byte $ff, $10
	.word @song1ref894
	.byte $95, $2a, $a5, $38, $a5, $2a, $a5, $38, $a5, $40, $a5, $38, $a5, $2a, $a5, $38, $8f, $93, $2a, $a5, $38, $a5, $2a, $a5
	.byte $38, $a5, $2a, $cd, $00, $b9, $93
	.byte $ff, $11
	.word @song1ref894
	.byte $ff, $11
	.word @song1ref894
	.byte $2a, $a5, $38, $a5, $2a, $a5, $38, $a5, $42, $a5, $38, $a5, $2a, $a5, $38, $91, $93, $2a, $a5, $38, $a5, $2a, $a5, $38
	.byte $a5, $2a, $cd, $00, $b9, $fd
	.word @song1ch2loop
@song1ch3:
	.byte $f9, $f9, $cb, $92, $38, $83, $38, $83, $38, $83, $00, $38, $89, $00, $85, $38, $89, $00, $85, $38, $8b, $00, $83, $84
	.byte $42, $b9, $92, $36, $91, $38, $91, $3c, $91, $3c, $91, $38, $91, $84, $42, $cd
@song1ch3loop:
@song1ref1024:
	.byte $00, $91, $92
@song1ref1027:
	.byte $38, $91, $00, $91, $38, $8b, $00, $38, $8b, $00, $87, $38, $95, $00, $8d, $38, $9d, $00, $85, $38, $95, $00, $8d, $38
	.byte $8b, $00, $38, $8b, $00, $87, $38, $95, $00, $8d, $38, $91, $83, $00, $8d
	.byte $ff, $13
	.word @song1ref1027
	.byte $84, $42, $f5, $92, $38, $91, $00, $91
	.byte $ff, $27
	.word @song1ref1027
	.byte $ff, $13
	.word @song1ref1027
	.byte $84, $42, $f5, $92, $38, $91, $00, $91
	.byte $ff, $27
	.word @song1ref1027
	.byte $ff, $13
	.word @song1ref1027
	.byte $84, $42, $f5, $92, $38, $91, $00, $91
	.byte $ff, $27
	.word @song1ref1027
	.byte $ff, $27
	.word @song1ref1027
	.byte $ff, $27
	.word @song1ref1027
	.byte $ff, $13
	.word @song1ref1027
	.byte $84, $42, $f5, $92, $38, $91, $00, $91
	.byte $ff, $27
	.word @song1ref1027
	.byte $ff, $13
	.word @song1ref1027
	.byte $84, $42, $f5, $92, $38, $91, $00, $91
	.byte $ff, $27
	.word @song1ref1027
	.byte $ff, $27
	.word @song1ref1027
	.byte $ff, $27
	.word @song1ref1027
@song1ref1148:
	.byte $84, $42, $cd, $92, $38, $87, $3a, $87, $38, $87, $3a, $87, $38, $87, $3a, $87, $38, $87, $3a, $87, $84, $42, $f5, $00
	.byte $91
	.byte $ff, $29
	.word @song1ref1024
	.byte $ff, $16
	.word @song1ref1148
	.byte $ff, $29
	.word @song1ref1024
	.byte $ff, $27
	.word @song1ref1027
	.byte $ff, $27
	.word @song1ref1027
	.byte $ff, $27
	.word @song1ref1027
	.byte $ff, $13
	.word @song1ref1027
	.byte $84, $42, $f5, $92, $38, $91, $00, $91
	.byte $ff, $27
	.word @song1ref1027
	.byte $ff, $13
	.word @song1ref1027
	.byte $84, $42, $f5, $92, $38, $91, $00, $91
	.byte $ff, $27
	.word @song1ref1027
	.byte $ff, $27
	.word @song1ref1027
	.byte $ff, $27
	.word @song1ref1027
	.byte $ff, $16
	.word @song1ref1148
	.byte $00, $91
@song1ref1230:
	.byte $42, $cd, $92, $38, $91, $38, $91, $38, $91, $38, $91, $84, $42, $cd, $92, $38, $91, $00, $91, $38, $91, $00, $91, $84
	.byte $ff, $14
	.word @song1ref1230
@song1ref1257:
	.byte $84, $42, $a5, $92, $38, $91, $38, $91, $84, $42, $a5, $92, $38, $91, $00, $91, $84, $42, $a5, $92, $38, $91, $38, $91
	.byte $84, $42, $a5, $92, $38, $91, $00, $91, $84, $42, $a5, $92, $38, $91, $38, $91, $84, $42, $a5, $92, $38, $91, $00, $91
	.byte $84, $42, $cd, $92, $38, $91, $00, $a5, $00, $91, $84
	.byte $ff, $14
	.word @song1ref1230
	.byte $84
	.byte $ff, $14
	.word @song1ref1230
	.byte $ff, $2c
	.word @song1ref1257
	.byte $84
	.byte $ff, $14
	.word @song1ref1230
	.byte $84
	.byte $ff, $14
	.word @song1ref1230
	.byte $ff, $2c
	.word @song1ref1257
	.byte $84
	.byte $ff, $14
	.word @song1ref1230
	.byte $84
	.byte $ff, $14
	.word @song1ref1230
	.byte $ff, $2a
	.word @song1ref1257
	.byte $fd
	.word @song1ch3loop
@song1ch4:
	.byte $f9, $f9, $cb, $f9, $f9, $cb
@song1ch4loop:
	.byte $ff, $0f
	.word @song1ref10
	.byte $ff, $0f
	.word @song1ref10
	.byte $ff, $0f
	.word @song1ref10
	.byte $ff, $0f
	.word @song1ref10
	.byte $ff, $0f
	.word @song1ref10
	.byte $ff, $0f
	.word @song1ref10
	.byte $ff, $0f
	.word @song1ref10
	.byte $ff, $0f
	.word @song1ref10
	.byte $ff, $0f
	.word @song1ref10
	.byte $fd
	.word @song1ch4loop
@song2ch0:
	.byte $fb, $01, $f9, $f9, $cb, $f9, $f9, $cb
@song2ch0loop:
@song2ref10:
	.byte $f9, $f9, $cb, $f9, $f9, $cb, $f9, $f9, $cb, $f9, $f9, $cb, $f9, $f9, $cb, $f9, $f9, $cb
	.byte $ff, $12
	.word @song2ref10
	.byte $ff, $12
	.word @song2ref10
	.byte $ff, $12
	.word @song2ref10
	.byte $ff, $12
	.word @song2ref10
	.byte $f9, $f9, $cb, $f9, $f9, $cb, $fd
	.word @song2ch0loop
@song2ch1:
	.byte $f9, $f9, $cb, $f9, $f9, $cb
@song2ch1loop:
	.byte $f9, $f9, $cb, $f9, $f9, $cb, $88
@song2ref63:
	.byte $1e, $c9, $01, $1e, $c9, $01, $36, $c9, $00, $a9, $36, $a3, $00
@song2ref76:
	.byte $2c, $c9, $01, $30, $a1, $01, $32, $a1, $01, $30, $b5, $00, $95, $28, $c9, $00, $81
	.byte $ff, $11
	.word @song2ref76
	.byte $24, $c3, $00, $87, $28, $c3, $00, $87, $22, $c3, $00, $87, $24, $c3, $00, $87
	.byte $ff, $1e
	.word @song2ref63
	.byte $ff, $11
	.word @song2ref76
	.byte $28, $c3, $00, $87, $24, $c3, $00, $87, $22, $c3, $00, $87, $1a, $c3, $00, $87, $1e, $c3, $00, $f9, $f9, $83, $f9, $f9
	.byte $cb, $f9, $f9, $cb, $f9, $f9, $cb
@song2ref149:
	.byte $10, $a5, $14, $a5, $1a, $a5, $28, $a5, $00, $cd, $28, $cd, $f9, $91, $00, $f9, $b7
	.byte $ff, $11
	.word @song2ref149
	.byte $ff, $11
	.word @song2ref149
	.byte $24, $a3, $00, $26, $a3, $00, $26, $a3, $00, $28, $a3, $00, $cf, $28, $cd, $f9, $91, $00, $f9, $b7
	.byte $ff, $11
	.word @song2ref149
	.byte $ff, $11
	.word @song2ref149
	.byte $ff, $11
	.word @song2ref149
	.byte $30, $a5, $2e, $a5, $2c, $a5, $28, $a5, $00, $cd, $28, $cd, $f9, $91, $00, $f9, $b7, $f9, $f9, $cb, $f9, $f9, $cb, $fd
	.word @song2ch1loop
@song2ch2:
	.byte $80
@song2ref228:
	.byte $1e, $a3, $00, $a7, $10, $9f, $00, $83, $10, $a3, $00, $a7, $1e, $a3, $00, $10, $9f, $00, $83, $10, $a3, $00, $1e, $a3
	.byte $00, $10, $9f, $00, $83, $10, $9f, $00, $83, $10, $9f, $00, $ab, $1e, $a3, $00, $10, $9f, $00, $83, $10, $a3, $00
@song2ch2loop:
	.byte $ff, $2f
	.word @song2ref228
	.byte $ff, $2f
	.word @song2ref228
	.byte $ff, $2f
	.word @song2ref228
	.byte $ff, $2f
	.word @song2ref228
	.byte $ff, $2f
	.word @song2ref228
	.byte $ff, $2f
	.word @song2ref228
	.byte $ff, $2f
	.word @song2ref228
	.byte $ff, $2f
	.word @song2ref228
	.byte $ff, $2f
	.word @song2ref228
	.byte $ff, $2f
	.word @song2ref228
	.byte $ff, $2f
	.word @song2ref228
	.byte $ff, $2f
	.word @song2ref228
	.byte $ff, $2f
	.word @song2ref228
	.byte $ff, $2f
	.word @song2ref228
	.byte $ff, $2f
	.word @song2ref228
	.byte $ff, $2f
	.word @song2ref228
	.byte $fd
	.word @song2ch2loop
@song2ch3:
	.byte $f9, $f9, $cb, $f9, $f9, $cb
@song2ch3loop:
@song2ref334:
	.byte $86, $50, $a5, $90, $5a, $91, $5a, $91, $5a, $91, $5a, $91, $86, $50, $a5, $90, $5a, $91, $5a, $91, $5a, $91, $5a, $91
	.byte $86, $50, $a5, $90, $5a, $91, $00, $91
	.byte $ff, $1a
	.word @song2ref334
	.byte $ff, $1a
	.word @song2ref334
	.byte $ff, $1a
	.word @song2ref334
	.byte $ff, $1a
	.word @song2ref334
	.byte $ff, $1a
	.word @song2ref334
	.byte $ff, $1a
	.word @song2ref334
	.byte $ff, $1a
	.word @song2ref334
	.byte $ff, $1a
	.word @song2ref334
	.byte $ff, $1a
	.word @song2ref334
	.byte $ff, $1a
	.word @song2ref334
	.byte $ff, $1a
	.word @song2ref334
	.byte $ff, $1a
	.word @song2ref334
	.byte $ff, $1a
	.word @song2ref334
	.byte $ff, $1a
	.word @song2ref334
	.byte $ff, $1a
	.word @song2ref334
	.byte $ff, $1a
	.word @song2ref334
	.byte $ff, $1a
	.word @song2ref334
	.byte $ff, $1a
	.word @song2ref334
	.byte $ff, $1a
	.word @song2ref334
	.byte $ff, $1a
	.word @song2ref334
	.byte $ff, $1a
	.word @song2ref334
	.byte $ff, $1a
	.word @song2ref334
	.byte $ff, $1a
	.word @song2ref334
	.byte $ff, $1a
	.word @song2ref334
	.byte $ff, $1a
	.word @song2ref334
	.byte $ff, $1a
	.word @song2ref334
	.byte $ff, $1a
	.word @song2ref334
	.byte $ff, $1a
	.word @song2ref334
	.byte $ff, $1a
	.word @song2ref334
	.byte $ff, $1a
	.word @song2ref334
	.byte $ff, $1a
	.word @song2ref334
	.byte $fd
	.word @song2ch3loop
@song2ch4:
	.byte $f9, $f9, $cb, $f9, $f9, $cb
@song2ch4loop:
	.byte $ff, $12
	.word @song2ref10
	.byte $ff, $12
	.word @song2ref10
	.byte $ff, $12
	.word @song2ref10
	.byte $ff, $12
	.word @song2ref10
	.byte $ff, $12
	.word @song2ref10
	.byte $f9, $f9, $cb, $f9, $f9, $cb, $fd
	.word @song2ch4loop
@song3ch0:
	.byte $fb, $01, $f9, $f9, $f9, $91, $f9, $f9, $f9, $91
@song3ch0loop:
	.byte $f9, $f9, $f9, $91, $f9, $f9, $f9, $91, $8a, $32, $f9, $93, $40, $95, $3c, $95, $3a, $ad, $30, $ad, $44, $c5, $48, $95
	.byte $40, $f9, $93, $40, $95, $3c, $95, $3a, $ad, $30, $ad, $44, $c5, $48, $95, $40, $f9, $93, $40, $95, $3c, $95, $3a, $ad
	.byte $30, $ad, $40, $c5, $30, $95, $32, $f9, $93, $40, $95, $3c, $95, $3a, $ad, $30, $ad, $40, $c5, $30, $95, $32, $f9, $93
	.byte $82, $1a, $93, $00, $1a, $93, $00, $1a, $93, $00, $97, $02, $93, $00, $97, $02, $93, $00, $c7, $f9, $95, $1a, $93, $00
	.byte $1a, $93, $00, $1a, $93, $00, $97, $02, $93, $00, $97, $02, $93, $00, $97, $02, $93, $00, $97, $fd
	.word @song3ch0loop
@song3ch1:
	.byte $88
@song3ref131:
	.byte $32, $89, $1a, $95, $32, $89, $1a, $95, $32, $89, $1a, $89, $32, $89, $1a, $95, $32, $89, $1a, $95, $32, $89, $1a, $89
	.byte $32, $89, $1a, $95, $32, $89, $1a, $95, $32, $89, $1a, $89, $2e, $89, $1a, $95, $2e, $89, $1a, $95, $2e, $89, $1a, $89
	.byte $32, $89, $1a, $95, $32, $89, $1a, $95, $32, $89, $1a, $89, $32, $89, $1a, $95, $32, $89, $1a, $95, $32, $89, $1a, $89
	.byte $32, $89, $1a, $95, $32, $89, $1a, $95, $32, $89, $1a, $89, $36, $89, $1a, $95, $36, $89, $1a, $95, $36, $89, $1a, $89
@song3ch1loop:
	.byte $ff, $54
	.word @song3ref131
	.byte $38, $89, $1a, $95, $38, $89, $1a, $95, $38, $89, $1a, $89
	.byte $ff, $60
	.word @song3ref131
	.byte $ff, $54
	.word @song3ref131
	.byte $38, $89, $1a, $95, $38, $89, $1a, $95, $38, $89, $1a, $89
	.byte $ff, $60
	.word @song3ref131
	.byte $fd
	.word @song3ch1loop
@song3ch2:
	.byte $80
	.byte $ff, $60
	.word @song3ref131
@song3ch2loop:
	.byte $ff, $54
	.word @song3ref131
	.byte $38, $89, $1a, $95, $38, $89, $1a, $95, $38, $89, $1a, $89
	.byte $ff, $60
	.word @song3ref131
	.byte $ff, $54
	.word @song3ref131
	.byte $38, $89, $1a, $95, $38, $89, $1a, $95, $38, $89, $1a, $89
	.byte $ff, $60
	.word @song3ref131
	.byte $fd
	.word @song3ch2loop
@song3ch3:
@song3ref311:
	.byte $92, $5a, $89, $86, $5a, $89, $5a, $89, $92, $5a, $89, $86, $5a, $89, $5a, $89, $92, $5a, $89, $86, $5a, $89, $92, $5a
	.byte $89, $86, $5a, $89, $5a, $89, $92, $5a, $89, $86, $5a, $89, $5a, $89, $92, $5a, $89, $86, $5a, $89, $92, $5a, $89, $86
	.byte $5a, $89, $5a, $89, $92, $5a, $89, $86, $5a, $89, $5a, $89, $92, $5a, $89, $86, $5a, $89, $94, $58, $89, $86, $5a, $89
	.byte $5a, $89, $94, $58, $89, $86, $5a, $89, $5a, $89, $94, $5a, $89, $56, $89
	.byte $ff, $40
	.word @song3ref311
@song3ch3loop:
	.byte $ff, $40
	.word @song3ref311
	.byte $ff, $40
	.word @song3ref311
	.byte $ff, $40
	.word @song3ref311
	.byte $ff, $40
	.word @song3ref311
	.byte $ff, $40
	.word @song3ref311
	.byte $ff, $40
	.word @song3ref311
	.byte $ff, $40
	.word @song3ref311
	.byte $ff, $40
	.word @song3ref311
	.byte $fd
	.word @song3ch3loop
@song3ch4:
	.byte $f9, $f9, $f9, $91, $f9, $f9, $f9, $91
@song3ch4loop:
	.byte $f9, $f9, $f9, $91, $f9, $f9, $f9, $91, $f9, $f9, $f9, $91, $f9, $f9, $f9, $91, $f9, $f9, $f9, $91, $f9, $f9, $f9, $91
	.byte $f9, $f9, $f9, $91, $f9, $f9, $f9, $91, $fd
	.word @song3ch4loop
@song4ch0:
	.byte $fb, $01, $88, $54, $f9, $8d, $00, $50, $f9, $b5, $00, $4a, $f9, $f1, $00, $cf
@song4ch0loop:
	.byte $f9, $f9, $cb, $fd
	.word @song4ch0loop
@song4ch1:
	.byte $8e, $3c, $f9, $8d, $00, $38, $f9, $b5, $00, $32, $f9, $f1, $00, $cf
@song4ch1loop:
	.byte $f9, $f9, $cb, $fd
	.word @song4ch1loop
@song4ch2:
	.byte $80, $1a, $b7, $00, $93, $1a, $b7, $00, $89, $1a, $87, $00, $85, $1a, $af, $00, $93, $20, $b7, $00, $89, $1e, $87, $00
	.byte $85, $1e, $af, $00, $8b, $1a, $85, $00, $85, $1a, $af, $00, $8b, $18, $85, $00, $85, $1a, $c3, $00, $cf
@song4ch2loop:
	.byte $f9, $f9, $cb, $fd
	.word @song4ch2loop
@song4ch3:
	.byte $94, $4e, $a5, $96, $4e, $a5, $4e, $91, $4e, $a5, $94, $4e, $a5, $96, $4e, $a5, $48, $91, $4e, $a5, $4e, $a5, $94, $4e
	.byte $a5, $96, $4e, $91, $48, $91, $4e, $91, $48, $a5, $4e, $df, $00, $cf
@song4ch3loop:
	.byte $f9, $f9, $cb, $fd
	.word @song4ch3loop
@song4ch4:
	.byte $f9, $f9, $cb, $f9, $f9, $cb
@song4ch4loop:
	.byte $f9, $f9, $cb, $fd
	.word @song4ch4loop
@song5ch0:
	.byte $fb, $01, $8c, $10, $a5, $14, $a5, $10, $a5, $16, $f9, $a1, $00, $a7
@song5ch0loop:
	.byte $f9, $f9, $cb, $fd
	.word @song5ch0loop
@song5ch1:
	.byte $8e, $40, $a5, $44, $a5, $40, $a5, $46, $f9, $a1, $00, $a7
@song5ch1loop:
	.byte $f9, $f9, $cb, $fd
	.word @song5ch1loop
@song5ch2:
	.byte $80, $10, $f9, $f9, $a1, $00, $a5
@song5ch2loop:
	.byte $f9, $f9, $cb, $fd
	.word @song5ch2loop
@song5ch3:
	.byte $96, $70, $a5, $94, $72, $a5, $72, $a5, $96, $70, $f9, $a3, $00, $a5
@song5ch3loop:
	.byte $f9, $f9, $cb, $fd
	.word @song5ch3loop
@song5ch4:
	.byte $f9, $f9, $cb
@song5ch4loop:
	.byte $f9, $f9, $cb, $fd
	.word @song5ch4loop
@song6ch0:
	.byte $fb, $01
@song6ch0loop:
@song6ref4:
	.byte $f9, $f9, $f9, $91, $f9, $f9, $f9, $91, $f9, $f9, $f9, $91, $f9, $f9, $f9, $91, $f9, $f9, $f9, $91, $f9, $f9, $f9, $91
	.byte $f9, $f9, $f9, $91, $f9, $f9, $f9, $91, $fd
	.word @song6ch0loop
@song6ch1:
@song6ch1loop:
	.byte $f9, $f9, $f9, $91, $f9, $f9, $f9, $91, $f9, $f9, $f9, $91, $f9, $f9, $f9, $91, $88
@song6ref57:
	.byte $4a, $93, $00, $32, $93, $00, $32, $93, $00, $4a, $93, $00, $32, $93, $00, $32, $93, $00, $4a, $93, $00, $32
@song6ref79:
	.byte $93, $00, $4e, $93, $00, $36, $93, $00, $36, $93, $00, $4e, $93, $00, $36, $93, $00, $36, $93, $00, $4e, $93, $00, $36
	.byte $93, $00, $52, $93, $00, $3a, $93, $00, $3a, $93, $00, $52, $93, $00, $3a, $93, $00, $3a, $93, $00, $52, $93, $00, $3a
	.byte $ff, $1a
	.word @song6ref79
	.byte $ff, $46
	.word @song6ref57
	.byte $93, $00, $54, $93, $00, $3c, $93, $00, $3c, $93, $00, $54, $93, $00, $3c, $93, $00, $3c, $93, $00, $54, $93, $00, $3c
	.byte $93, $00, $fd
	.word @song6ch1loop
@song6ch2:
@song6ch2loop:
	.byte $80
@song6ref164:
	.byte $1a, $95, $1e, $8f, $00, $83, $1e, $8f, $00, $83, $1a, $95, $1e, $8f, $00, $83, $1e, $8f, $00, $b3, $1a, $95, $1e, $8f
	.byte $00, $83, $1e, $8f, $00, $83, $1a, $95, $1e, $8f, $00, $83, $1e, $8f, $00, $83, $24, $a7, $00, $83
	.byte $ff, $28
	.word @song6ref164
	.byte $28, $a7, $00, $83
	.byte $ff, $2c
	.word @song6ref164
	.byte $ff, $28
	.word @song6ref164
	.byte $28, $a7, $00, $83
	.byte $ff, $2c
	.word @song6ref164
	.byte $ff, $28
	.word @song6ref164
	.byte $28, $a7, $00, $83
	.byte $ff, $2c
	.word @song6ref164
	.byte $ff, $28
	.word @song6ref164
	.byte $28, $a7, $00, $83, $fd
	.word @song6ch2loop
@song6ch3:
@song6ch3loop:
	.byte $f9, $f9, $f9, $91, $f9, $f9, $f9, $91
@song6ref257:
	.byte $86, $54, $ad, $90, $48, $95, $48, $95, $00, $95, $86, $54, $95, $90, $48, $95, $48, $95, $86, $54, $ad, $90, $48, $95
	.byte $48, $95, $00, $95, $86, $54, $95, $90, $48, $95, $48, $95
	.byte $ff, $1c
	.word @song6ref257
	.byte $ff, $1c
	.word @song6ref257
	.byte $ff, $1c
	.word @song6ref257
	.byte $ff, $1c
	.word @song6ref257
	.byte $ff, $1c
	.word @song6ref257
	.byte $fd
	.word @song6ch3loop
@song6ch4:
@song6ch4loop:
	.byte $ff, $20
	.word @song6ref4
	.byte $fd
	.word @song6ch4loop
