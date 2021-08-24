;this file for FamiTone2 library generated by FamiStudio

castle_escape_music_data:
	.byte 3
	.word @instruments
	.word @samples-3
	.word @song0ch0,@song0ch1,@song0ch2,@song0ch3,@song0ch4,307,256
	.word @song1ch0,@song1ch1,@song1ch2,@song1ch3,@song1ch4,307,256
	.word @song2ch0,@song2ch1,@song2ch2,@song2ch3,@song2ch4,307,256

.export castle_escape_music_data
.global FAMISTUDIO_DPCM_PTR

@instruments:
	.byte $b0 ;instrument 00 (Bass)
	.word @env6, @env1, @env1
	.byte $00
	.byte $30 ;instrument 01 (GBA Snare (8-#))
	.word @env4, @env9, @env1
	.byte $00
	.byte $b0 ;instrument 02 (Harpsichord-Alt1)
	.word @env11, @env1, @env1
	.byte $00
	.byte $30 ;instrument 03 (LeadVerse2)
	.word @env12, @env1, @env0
	.byte $00
	.byte $30 ;instrument 04 (NoiseBassDrum)
	.word @env5, @env3, @env1
	.byte $00
	.byte $30 ;instrument 05 (NoiseCymbal)
	.word @env2, @env8, @env1
	.byte $00
	.byte $30 ;instrument 06 (NoiseHiHat)
	.word @env7, @env10, @env1
	.byte $00

@samples:
@env0:
	.byte $c0,$1d,$c0,$c1,$c2,$c2,$c1,$c0,$bf,$be,$bd,$be,$bf,$c0,$00,$02
@env1:
	.byte $c0,$7f,$00,$00
@env2:
	.byte $ca,$c6,$c3,$c0,$00,$03
@env3:
	.byte $c0,$bf,$c1,$00,$02
@env4:
	.byte $cf,$c6,$cd,$c9,$c7,$c5,$c3,$c2,$c1,$02,$c0,$00,$0a
@env5:
	.byte $cf,$cd,$c6,$c4,$c3,$c0,$00,$05
@env6:
	.byte $cf,$7f,$00,$00
@env7:
	.byte $ca,$ca,$c7,$c6,$c6,$c5,$04,$c4,$c3,$c2,$c1,$05,$c0,$00,$0c
@env8:
	.byte $c0,$c1,$c2,$00,$02
@env9:
	.byte $b9,$c0,$00,$01
@env10:
	.byte $c0,$c1,$03,$c2,$c1,$c2,$c1,$c2,$c1,$c2,$00,$09
@env11:
	.byte $c7,$c6,$c5,$c5,$c4,$c4,$c3,$00,$06
@env12:
	.byte $c6,$c6,$c7,$02,$c6,$03,$c5,$00,$06
@song0ch0:
	.byte $fb, $01
@song0ch0loop:
@song0ref4:
	.byte $f9, $f9, $f9, $91, $f9, $f9, $f9, $91, $f9, $f9, $f9, $91, $f9, $f9, $f9, $91, $f9, $f9, $f9, $91, $f9, $f9, $f9, $91
	.byte $f9, $f9, $f9, $91, $f9, $f9, $f9, $91, $fd
	.word @song0ch0loop
@song0ch1:
@song0ch1loop:
	.byte $f9, $f9, $f9, $91, $f9, $f9, $f9, $91, $f9, $f9, $f9, $91, $f9, $f9, $f9, $91, $84
@song0ref57:
	.byte $4a, $93, $00, $32, $93, $00, $32, $93, $00, $4a, $93, $00, $32, $93, $00, $32, $93, $00, $4a, $93, $00, $32
@song0ref79:
	.byte $93, $00, $4e, $93, $00, $36, $93, $00, $36, $93, $00, $4e, $93, $00, $36, $93, $00, $36, $93, $00, $4e, $93, $00, $36
	.byte $93, $00, $52, $93, $00, $3a, $93, $00, $3a, $93, $00, $52, $93, $00, $3a, $93, $00, $3a, $93, $00, $52, $93, $00, $3a
	.byte $ff, $1a
	.word @song0ref79
	.byte $ff, $46
	.word @song0ref57
	.byte $93, $00, $54, $93, $00, $3c, $93, $00, $3c, $93, $00, $54, $93, $00, $3c, $93, $00, $3c, $93, $00, $54, $93, $00, $3c
	.byte $93, $00, $fd
	.word @song0ch1loop
@song0ch2:
@song0ch2loop:
	.byte $80
@song0ref164:
	.byte $1a, $95, $1e, $8f, $00, $83, $1e, $8f, $00, $83, $1a, $95, $1e, $8f, $00, $83, $1e, $8f, $00, $b3, $1a, $95, $1e, $8f
	.byte $00, $83, $1e, $8f, $00, $83, $1a, $95, $1e, $8f, $00, $83, $1e, $8f, $00, $83, $24, $a7, $00, $83
	.byte $ff, $28
	.word @song0ref164
	.byte $28, $a7, $00, $83
	.byte $ff, $2c
	.word @song0ref164
	.byte $ff, $28
	.word @song0ref164
	.byte $28, $a7, $00, $83
	.byte $ff, $2c
	.word @song0ref164
	.byte $ff, $28
	.word @song0ref164
	.byte $28, $a7, $00, $83
	.byte $ff, $2c
	.word @song0ref164
	.byte $ff, $28
	.word @song0ref164
	.byte $28, $a7, $00, $83, $fd
	.word @song0ch2loop
@song0ch3:
@song0ch3loop:
	.byte $f9, $f9, $f9, $91, $f9, $f9, $f9, $91
@song0ref257:
	.byte $82, $54, $ad, $88, $48, $95, $48, $95, $00, $95, $82, $54, $95, $88, $48, $95, $48, $95, $82, $54, $ad, $88, $48, $95
	.byte $48, $95, $00, $95, $82, $54, $95, $88, $48, $95, $48, $95
	.byte $ff, $1c
	.word @song0ref257
	.byte $ff, $1c
	.word @song0ref257
	.byte $ff, $1c
	.word @song0ref257
	.byte $ff, $1c
	.word @song0ref257
	.byte $ff, $1c
	.word @song0ref257
	.byte $fd
	.word @song0ch3loop
@song0ch4:
@song0ch4loop:
	.byte $ff, $20
	.word @song0ref4
	.byte $fd
	.word @song0ch4loop
@song1ch0:
	.byte $fb, $01, $f9, $f9, $cb, $f9, $f9, $cb, $f9, $f9, $cb, $f9, $f9, $cb, $f9, $f9, $cb
@song1ch0loop:
	.byte $f9, $f9, $cb, $f9, $f9, $cb, $f9, $f9, $cb, $86, $1a, $f9, $f1, $00, $28, $cb, $00, $24, $f9, $cb, $24, $a3, $00, $18
	.byte $cb, $00, $1a, $f9, $f1, $00, $28, $cb, $00, $24, $f9, $f1, $00, $2e, $a5, $2e, $a3, $00, $28, $f9, $f1, $00, $1a, $cb
	.byte $00, $24, $f9, $f3, $24, $cb, $00, $93
@song1ref75:
	.byte $24, $8f, $00, $20, $8f, $00, $1a, $8f, $00, $24, $8f, $00, $20, $8f, $00, $1a, $8f, $00, $24, $a3, $00
	.byte $ff, $13
	.word @song1ref75
	.byte $91, $91, $00, $28, $8f, $00, $24, $8f, $00, $1e, $8f, $00, $28, $8f, $00, $24, $8f, $00, $1e, $8f, $00, $28, $a3, $00
	.byte $2e, $8f, $00, $2c, $8f, $00, $24, $8f, $00, $2e, $8f, $00, $2c, $8f, $00, $24, $8f, $00, $2e, $91, $a7, $2e, $f9, $dd
	.byte $00, $bb, $fd
	.word @song1ch0loop
@song1ch1:
	.byte $f9, $f9, $cb, $f9, $f9, $cb, $f9, $f9, $cb, $f9, $f9, $cb, $84
@song1ref165:
	.byte $1a, $8f, $00, $1a, $8f, $00, $32, $8f, $00, $32, $8f, $00, $1a, $8f, $00, $1a, $8f, $00, $32, $91, $32, $8f, $00
	.byte $ff, $17
	.word @song1ref165
@song1ch1loop:
	.byte $84
@song1ref193:
	.byte $1e, $8f, $00, $1e, $8f, $00, $36, $8f, $00, $36, $8f, $00, $1e, $8f, $00, $1e, $8f, $00, $36, $91, $36, $8f, $00
	.byte $ff, $17
	.word @song1ref193
@song1ref219:
	.byte $20, $8f, $00, $20, $8f, $00, $38, $8f, $00, $38, $8f, $00, $20, $8f, $00, $20, $8f, $00, $38, $91, $38, $8f, $00
	.byte $ff, $17
	.word @song1ref219
@song1ref245:
	.byte $24, $8f, $00, $24, $8f, $00, $3c, $8f, $00, $3c, $8f, $00, $24, $8f, $00, $24, $8f, $00, $3c, $91, $3c, $8f, $00, $28
	.byte $8f, $00, $28, $8f, $00, $40, $8f, $00, $40, $8f, $00, $16, $8f, $00, $16, $8f, $00, $2e, $91, $2e, $8f, $00
	.byte $ff, $17
	.word @song1ref165
	.byte $ff, $17
	.word @song1ref165
	.byte $ff, $17
	.word @song1ref193
	.byte $ff, $17
	.word @song1ref193
	.byte $ff, $17
	.word @song1ref219
	.byte $ff, $17
	.word @song1ref219
	.byte $ff, $2e
	.word @song1ref245
	.byte $ff, $17
	.word @song1ref165
	.byte $ff, $17
	.word @song1ref165
	.byte $ff, $17
	.word @song1ref193
	.byte $ff, $17
	.word @song1ref193
	.byte $ff, $17
	.word @song1ref219
	.byte $ff, $17
	.word @song1ref219
	.byte $ff, $2e
	.word @song1ref245
	.byte $ff, $17
	.word @song1ref165
	.byte $ff, $17
	.word @song1ref165
	.byte $fd
	.word @song1ch1loop
@song1ch2:
	.byte $80, $4a, $f9, $f9, $c9
@song1ref347:
	.byte $4e, $f9, $f9, $c9, $50, $f9, $f9, $c9, $54, $f9, $a3, $58, $cd, $46, $cd, $4a, $f9, $f9, $c9
@song1ch2loop:
	.byte $80
	.byte $ff, $13
	.word @song1ref347
	.byte $ff, $13
	.word @song1ref347
	.byte $ff, $13
	.word @song1ref347
	.byte $fd
	.word @song1ch2loop
@song1ch3:
@song1ref380:
	.byte $88, $5a, $91, $5a, $91, $82, $50, $91, $50, $91, $88, $5a, $91, $5a, $91, $82, $50, $91, $00, $91, $88, $5a, $91, $5a
	.byte $91, $82, $50, $91, $50, $91, $88, $5a, $91, $5a, $91, $82, $50, $91, $00, $91
	.byte $ff, $20
	.word @song1ref380
	.byte $ff, $20
	.word @song1ref380
	.byte $ff, $20
	.word @song1ref380
	.byte $ff, $20
	.word @song1ref380
@song1ch3loop:
	.byte $ff, $20
	.word @song1ref380
	.byte $ff, $20
	.word @song1ref380
	.byte $ff, $20
	.word @song1ref380
	.byte $ff, $20
	.word @song1ref380
	.byte $ff, $20
	.word @song1ref380
	.byte $ff, $20
	.word @song1ref380
	.byte $ff, $20
	.word @song1ref380
	.byte $ff, $20
	.word @song1ref380
	.byte $ff, $20
	.word @song1ref380
	.byte $ff, $20
	.word @song1ref380
	.byte $ff, $20
	.word @song1ref380
	.byte $ff, $20
	.word @song1ref380
	.byte $fd
	.word @song1ch3loop
@song1ch4:
	.byte $f9, $f9, $cb, $f9, $f9, $cb, $f9, $f9, $cb, $f9, $f9, $cb, $f9, $f9, $cb
@song1ch4loop:
@song1ref488:
	.byte $f9, $f9, $cb, $f9, $f9, $cb, $f9, $f9, $cb, $f9, $f9, $cb, $f9, $f9, $cb, $f9, $f9, $cb
	.byte $ff, $12
	.word @song1ref488
	.byte $fd
	.word @song1ch4loop
@song2ch0:
	.byte $fb, $01
@song2ch0loop:
	.byte $f9, $f9, $f9, $91, $fd
	.word @song2ch0loop
@song2ch1:
@song2ch1loop:
	.byte $f9, $f9, $f9, $91, $fd
	.word @song2ch1loop
@song2ch2:
@song2ch2loop:
	.byte $f9, $f9, $f9, $91, $fd
	.word @song2ch2loop
@song2ch3:
@song2ch3loop:
@song2ref28:
	.byte $8a, $5a, $89, $82, $5a, $89, $5a, $89, $8a, $5a, $89, $82, $5a, $89, $5a, $89, $8a, $5a, $89, $82, $5a, $89
	.byte $ff, $10
	.word @song2ref28
	.byte $ff, $10
	.word @song2ref28
	.byte $8c, $58, $89, $82, $5a, $89, $5a, $89, $8c, $58, $89, $82, $5a, $89, $5a, $89, $8c, $5a, $89, $56, $89, $fd
	.word @song2ch3loop
@song2ch4:
@song2ch4loop:
	.byte $f9, $f9, $f9, $91, $fd
	.word @song2ch4loop
