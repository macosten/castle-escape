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
	.export		_show_title_screen
	.import		_get_pad_new
	.import		_get_frame_count
	.import		_pal_fade_to
	.import		_pal_bg
	.import		_pal_spr
	.import		_ppu_wait_nmi
	.import		_ppu_off
	.import		_ppu_on_all
	.import		_oam_clear
	.importzp	_TEMP
	.import		_oam_meta_spr_fast_sub
	.import		_pad_poll
	.import		_vram_write
	.importzp	_temp0
	.importzp	_pad1
	.importzp	_pad1_new
	.import		_clear_screen
	.import		_set_chr_bank_0
	.import		_set_chr_bank_1
	.export		_titlescreen
	.export		_title_palette_bg
	.export		_title_palette_spr
	.export		_grarrl_eye_spr
	.export		_grarrl_mouth_spr
	.export		_valrigard_face_spr
	.export		_t_bottom_spr
	.export		_button_press_spr0
	.export		_button_press_spr1

.segment	"RODATA"

.segment	"BANK4"
_titlescreen:
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$D9
	.byte	$8E
	.byte	$D9
	.byte	$E0
	.byte	$D9
	.byte	$8E
	.byte	$F7
	.byte	$F9
	.byte	$F3
	.byte	$FF
	.byte	$D9
	.byte	$8E
	.byte	$FF
	.byte	$FF
	.byte	$D9
	.byte	$8E
	.byte	$D9
	.byte	$8E
	.byte	$D9
	.byte	$8E
	.byte	$D9
	.byte	$E0
	.byte	$D9
	.byte	$E0
	.byte	$D9
	.byte	$8E
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$53
	.byte	$FF
	.byte	$F5
	.byte	$F8
	.byte	$E5
	.byte	$E0
	.byte	$FF
	.byte	$FF
	.byte	$53
	.byte	$FF
	.byte	$F5
	.byte	$8E
	.byte	$FF
	.byte	$FF
	.byte	$F5
	.byte	$8E
	.byte	$E5
	.byte	$E0
	.byte	$53
	.byte	$FF
	.byte	$F5
	.byte	$F8
	.byte	$F5
	.byte	$F4
	.byte	$F5
	.byte	$8E
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$E5
	.byte	$8E
	.byte	$5C
	.byte	$5C
	.byte	$F2
	.byte	$F4
	.byte	$FF
	.byte	$FF
	.byte	$E5
	.byte	$8E
	.byte	$E5
	.byte	$8E
	.byte	$FF
	.byte	$FF
	.byte	$E5
	.byte	$8E
	.byte	$F2
	.byte	$F4
	.byte	$E5
	.byte	$8E
	.byte	$5C
	.byte	$5C
	.byte	$5C
	.byte	$FF
	.byte	$E5
	.byte	$8E
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$00
	.byte	$00
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$01
	.byte	$02
	.byte	$02
	.byte	$03
	.byte	$04
	.byte	$05
	.byte	$06
	.byte	$07
	.byte	$08
	.byte	$09
	.byte	$0A
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$0B
	.byte	$0C
	.byte	$0D
	.byte	$02
	.byte	$0E
	.byte	$02
	.byte	$0F
	.byte	$10
	.byte	$11
	.byte	$12
	.byte	$13
	.byte	$14
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$15
	.byte	$16
	.byte	$17
	.byte	$18
	.byte	$FF
	.byte	$19
	.byte	$1A
	.byte	$1B
	.byte	$1C
	.byte	$1D
	.byte	$1E
	.byte	$1F
	.byte	$20
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$21
	.byte	$22
	.byte	$23
	.byte	$24
	.byte	$FF
	.byte	$FF
	.byte	$25
	.byte	$26
	.byte	$27
	.byte	$02
	.byte	$28
	.byte	$29
	.byte	$2A
	.byte	$2B
	.byte	$2C
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$2D
	.byte	$2E
	.byte	$2F
	.byte	$30
	.byte	$31
	.byte	$32
	.byte	$33
	.byte	$34
	.byte	$DE
	.byte	$36
	.byte	$37
	.byte	$38
	.byte	$39
	.byte	$3A
	.byte	$3B
	.byte	$3C
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$3D
	.byte	$3E
	.byte	$02
	.byte	$3F
	.byte	$40
	.byte	$41
	.byte	$42
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$43
	.byte	$44
	.byte	$45
	.byte	$46
	.byte	$47
	.byte	$48
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$49
	.byte	$4A
	.byte	$02
	.byte	$4B
	.byte	$4C
	.byte	$02
	.byte	$4D
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$4E
	.byte	$4F
	.byte	$50
	.byte	$51
	.byte	$52
	.byte	$2C
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$54
	.byte	$55
	.byte	$56
	.byte	$02
	.byte	$4B
	.byte	$57
	.byte	$58
	.byte	$59
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$5A
	.byte	$5B
	.byte	$35
	.byte	$5D
	.byte	$5E
	.byte	$5F
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$60
	.byte	$61
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$62
	.byte	$63
	.byte	$02
	.byte	$02
	.byte	$64
	.byte	$02
	.byte	$65
	.byte	$66
	.byte	$D4
	.byte	$35
	.byte	$35
	.byte	$DF
	.byte	$67
	.byte	$35
	.byte	$68
	.byte	$69
	.byte	$6A
	.byte	$6B
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$6C
	.byte	$6D
	.byte	$6E
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$6F
	.byte	$63
	.byte	$02
	.byte	$02
	.byte	$70
	.byte	$71
	.byte	$72
	.byte	$73
	.byte	$E1
	.byte	$D5
	.byte	$9C
	.byte	$35
	.byte	$74
	.byte	$75
	.byte	$76
	.byte	$77
	.byte	$78
	.byte	$79
	.byte	$FF
	.byte	$7A
	.byte	$7B
	.byte	$7C
	.byte	$7D
	.byte	$7E
	.byte	$7F
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$80
	.byte	$81
	.byte	$02
	.byte	$02
	.byte	$82
	.byte	$83
	.byte	$84
	.byte	$85
	.byte	$CD
	.byte	$35
	.byte	$86
	.byte	$87
	.byte	$88
	.byte	$FF
	.byte	$FF
	.byte	$89
	.byte	$8A
	.byte	$8B
	.byte	$8C
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$8F
	.byte	$90
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$91
	.byte	$92
	.byte	$02
	.byte	$93
	.byte	$94
	.byte	$95
	.byte	$35
	.byte	$96
	.byte	$97
	.byte	$98
	.byte	$99
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$9A
	.byte	$9B
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$9D
	.byte	$9E
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$9F
	.byte	$A0
	.byte	$02
	.byte	$A1
	.byte	$A2
	.byte	$35
	.byte	$35
	.byte	$A3
	.byte	$A4
	.byte	$A5
	.byte	$A6
	.byte	$A7
	.byte	$FF
	.byte	$A8
	.byte	$A9
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$AA
	.byte	$AB
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$AD
	.byte	$AC
	.byte	$AE
	.byte	$AF
	.byte	$35
	.byte	$35
	.byte	$B0
	.byte	$B1
	.byte	$35
	.byte	$35
	.byte	$B2
	.byte	$FF
	.byte	$B3
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$B4
	.byte	$B5
	.byte	$B6
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$8D
	.byte	$B7
	.byte	$B8
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$B9
	.byte	$BA
	.byte	$35
	.byte	$35
	.byte	$BB
	.byte	$BC
	.byte	$BD
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$8D
	.byte	$BE
	.byte	$35
	.byte	$BF
	.byte	$C0
	.byte	$C1
	.byte	$C2
	.byte	$C3
	.byte	$C4
	.byte	$C5
	.byte	$02
	.byte	$C6
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$C7
	.byte	$C8
	.byte	$C9
	.byte	$CA
	.byte	$FF
	.byte	$CB
	.byte	$35
	.byte	$CC
	.byte	$FF
	.byte	$CE
	.byte	$CF
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$D0
	.byte	$FD
	.byte	$D1
	.byte	$FF
	.byte	$D2
	.byte	$FE
	.byte	$D3
	.byte	$FF
	.byte	$FF
	.byte	$15
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$D6
	.byte	$D7
	.byte	$D8
	.byte	$FF
	.byte	$FF
	.byte	$DA
	.byte	$DB
	.byte	$DC
	.byte	$DD
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$E2
	.byte	$E3
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$6C
	.byte	$02
	.byte	$E6
	.byte	$E7
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$E8
	.byte	$E9
	.byte	$EA
	.byte	$EB
	.byte	$35
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$EC
	.byte	$ED
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$EE
	.byte	$EF
	.byte	$F0
	.byte	$F1
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$15
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$A6
	.byte	$A7
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$21
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$F6
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$B2
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$FF
	.byte	$49
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$35
	.byte	$B2
	.byte	$FA
	.byte	$FB
	.byte	$35
	.byte	$35
	.byte	$FC
	.byte	$FF
	.byte	$EA
	.byte	$FA
	.byte	$FA
	.byte	$FA
	.byte	$FA
	.byte	$FA
	.byte	$FA
	.byte	$BA
	.byte	$AE
	.byte	$2F
	.byte	$0F
	.byte	$0F
	.byte	$0F
	.byte	$AF
	.byte	$AF
	.byte	$AB
	.byte	$AA
	.byte	$02
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$88
	.byte	$AA
	.byte	$AA
	.byte	$AA
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$88
	.byte	$AA
	.byte	$AA
	.byte	$AA
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$88
	.byte	$AA
	.byte	$AA
	.byte	$AA
	.byte	$AA
	.byte	$22
	.byte	$00
	.byte	$00
	.byte	$A9
	.byte	$AA
	.byte	$AA
	.byte	$AA
	.byte	$AA
	.byte	$A2
	.byte	$A0
	.byte	$A9
	.byte	$AA
	.byte	$AA
	.byte	$AE
	.byte	$EB
	.byte	$0A
	.byte	$0A
	.byte	$0A
	.byte	$0A
	.byte	$0A
	.byte	$0A
	.byte	$0A
	.byte	$0A
_title_palette_bg:
	.byte	$10
	.byte	$0F
	.byte	$01
	.byte	$32
	.byte	$10
	.byte	$0F
	.byte	$01
	.byte	$27
	.byte	$10
	.byte	$0F
	.byte	$14
	.byte	$27
	.byte	$10
	.byte	$0F
	.byte	$14
	.byte	$30
_title_palette_spr:
	.byte	$10
	.byte	$0F
	.byte	$16
	.byte	$30
	.byte	$10
	.byte	$0F
	.byte	$16
	.byte	$26
	.byte	$10
	.byte	$0F
	.byte	$16
	.byte	$38
	.byte	$10
	.byte	$0F
	.byte	$00
	.byte	$30
_grarrl_eye_spr:
	.byte	$00
	.byte	$00
	.byte	$08
	.byte	$01
	.byte	$08
	.byte	$00
	.byte	$09
	.byte	$01
	.byte	$10
	.byte	$00
	.byte	$0A
	.byte	$01
	.byte	$00
	.byte	$08
	.byte	$18
	.byte	$01
	.byte	$08
	.byte	$08
	.byte	$19
	.byte	$01
	.byte	$10
	.byte	$08
	.byte	$1A
	.byte	$01
	.byte	$00
	.byte	$10
	.byte	$28
	.byte	$01
	.byte	$08
	.byte	$10
	.byte	$29
	.byte	$01
	.byte	$10
	.byte	$10
	.byte	$2A
	.byte	$01
	.byte	$00
	.byte	$18
	.byte	$38
	.byte	$01
	.byte	$08
	.byte	$18
	.byte	$39
	.byte	$01
	.byte	$10
	.byte	$18
	.byte	$3A
	.byte	$01
	.byte	$01
	.byte	$20
	.byte	$48
	.byte	$01
	.byte	$09
	.byte	$20
	.byte	$49
	.byte	$01
	.byte	$11
	.byte	$20
	.byte	$4A
	.byte	$01
	.byte	$80
_grarrl_mouth_spr:
	.byte	$05
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$0D
	.byte	$00
	.byte	$01
	.byte	$00
	.byte	$15
	.byte	$00
	.byte	$02
	.byte	$00
	.byte	$00
	.byte	$08
	.byte	$03
	.byte	$00
	.byte	$08
	.byte	$08
	.byte	$04
	.byte	$00
	.byte	$10
	.byte	$08
	.byte	$05
	.byte	$00
	.byte	$18
	.byte	$08
	.byte	$06
	.byte	$00
	.byte	$20
	.byte	$08
	.byte	$07
	.byte	$00
	.byte	$0B
	.byte	$10
	.byte	$10
	.byte	$00
	.byte	$13
	.byte	$10
	.byte	$11
	.byte	$00
	.byte	$1B
	.byte	$10
	.byte	$12
	.byte	$00
	.byte	$23
	.byte	$10
	.byte	$13
	.byte	$00
	.byte	$2B
	.byte	$10
	.byte	$14
	.byte	$00
	.byte	$1C
	.byte	$18
	.byte	$15
	.byte	$00
	.byte	$24
	.byte	$18
	.byte	$16
	.byte	$00
	.byte	$2C
	.byte	$18
	.byte	$17
	.byte	$00
	.byte	$2F
	.byte	$20
	.byte	$20
	.byte	$00
	.byte	$80
_valrigard_face_spr:
	.byte	$0A
	.byte	$02
	.byte	$0B
	.byte	$02
	.byte	$00
	.byte	$08
	.byte	$0C
	.byte	$02
	.byte	$08
	.byte	$08
	.byte	$0D
	.byte	$02
	.byte	$00
	.byte	$10
	.byte	$1C
	.byte	$02
	.byte	$08
	.byte	$10
	.byte	$1D
	.byte	$02
	.byte	$0F
	.byte	$00
	.byte	$0E
	.byte	$02
	.byte	$14
	.byte	$06
	.byte	$1E
	.byte	$02
	.byte	$0D
	.byte	$12
	.byte	$2D
	.byte	$00
	.byte	$15
	.byte	$12
	.byte	$2E
	.byte	$00
	.byte	$0D
	.byte	$1A
	.byte	$2C
	.byte	$00
	.byte	$80
_t_bottom_spr:
	.byte	$00
	.byte	$00
	.byte	$0F
	.byte	$00
	.byte	$00
	.byte	$08
	.byte	$1F
	.byte	$00
	.byte	$80
_button_press_spr0:
	.byte	$00
	.byte	$00
	.byte	$3E
	.byte	$03
	.byte	$08
	.byte	$00
	.byte	$3E
	.byte	$43
	.byte	$00
	.byte	$08
	.byte	$4E
	.byte	$03
	.byte	$08
	.byte	$08
	.byte	$4E
	.byte	$43
	.byte	$80
_button_press_spr1:
	.byte	$00
	.byte	$00
	.byte	$3F
	.byte	$03
	.byte	$08
	.byte	$00
	.byte	$3F
	.byte	$43
	.byte	$00
	.byte	$08
	.byte	$4F
	.byte	$03
	.byte	$08
	.byte	$08
	.byte	$4F
	.byte	$43
	.byte	$80
.segment	"RODATA"

; ---------------------------------------------------------------
; void __near__ show_title_screen (void)
; ---------------------------------------------------------------

.segment	"BANK4"

.proc	_show_title_screen: near

.segment	"BANK4"

;
; temp0 = 0; // Show the animated button press if temp0 != 0
;
	lda     #$00
	sta     _temp0
;
; ppu_off();
;
	jsr     _ppu_off
;
; clear_screen();
;
	jsr     _clear_screen
;
; pal_bg(title_palette_bg);
;
	lda     #<(_title_palette_bg)
	ldx     #>(_title_palette_bg)
	jsr     _pal_bg
;
; pal_spr(title_palette_spr);
;
	lda     #<(_title_palette_spr)
	ldx     #>(_title_palette_spr)
	jsr     _pal_spr
;
; set_chr_bank_0(2);
;
	lda     #$02
	jsr     _set_chr_bank_0
;
; set_chr_bank_1(3);
;
	lda     #$03
	jsr     _set_chr_bank_1
;
; vram_write(titlescreen, 1024);
;
	lda     #<(_titlescreen)
	ldx     #>(_titlescreen)
	jsr     pushax
	ldx     #$04
	lda     #$00
	jsr     _vram_write
;
; ppu_on_all();
;
	jsr     _ppu_on_all
;
; ppu_wait_nmi();
;
L050E:	jsr     _ppu_wait_nmi
;
; oam_clear();
;
	jsr     _oam_clear
;
; oam_meta_spr(181, 140, grarrl_eye_spr);
;
	lda     #$B5
	sta     _TEMP+5
	lda     #$8C
	sta     _TEMP+6
	lda     #<(_grarrl_eye_spr)
	ldx     #>(_grarrl_eye_spr)
	jsr     _oam_meta_spr_fast_sub
;
; oam_meta_spr(176, 187, grarrl_mouth_spr);
;
	lda     #$B0
	sta     _TEMP+5
	lda     #$BB
	sta     _TEMP+6
	lda     #<(_grarrl_mouth_spr)
	ldx     #>(_grarrl_mouth_spr)
	jsr     _oam_meta_spr_fast_sub
;
; oam_meta_spr(100, 89, valrigard_face_spr);
;
	lda     #$64
	sta     _TEMP+5
	lda     #$59
	sta     _TEMP+6
	lda     #<(_valrigard_face_spr)
	ldx     #>(_valrigard_face_spr)
	jsr     _oam_meta_spr_fast_sub
;
; oam_meta_spr(76, 23, t_bottom_spr);
;
	lda     #$4C
	sta     _TEMP+5
	lda     #$17
	sta     _TEMP+6
	lda     #<(_t_bottom_spr)
	ldx     #>(_t_bottom_spr)
	jsr     _oam_meta_spr_fast_sub
;
; if (!get_frame_count()) { // At frame #256...
;
	jsr     _get_frame_count
	tax
	bne     L053C
;
; temp0 = 1;
;
	lda     #$01
	sta     _temp0
;
; if (temp0) {
;
L053C:	lda     _temp0
	beq     L056D
;
; if (get_frame_count() & 64) {
;
	jsr     _get_frame_count
	and     #$40
	beq     L056C
;
; oam_meta_spr(192, 64, button_press_spr1); // Pressed button
;
	lda     #$C0
	sta     _TEMP+5
	lda     #$40
	sta     _TEMP+6
	lda     #<(_button_press_spr1)
	ldx     #>(_button_press_spr1)
;
; } else {
;
	jmp     L056B
;
; oam_meta_spr(192, 64, button_press_spr0); // Depressed button
;
L056C:	lda     #$C0
	sta     _TEMP+5
	lda     #$40
	sta     _TEMP+6
	lda     #<(_button_press_spr0)
	ldx     #>(_button_press_spr0)
L056B:	jsr     _oam_meta_spr_fast_sub
;
; pad1 = pad_poll(0);
;
	lda     #$00
L056D:	jsr     _pad_poll
	sta     _pad1
;
; pad1_new = get_pad_new(0);
;
	lda     #$00
	jsr     _get_pad_new
	sta     _pad1_new
;
; if (pad1_new) {
;
	lda     _pad1_new
	jeq     L050E
;
; pal_fade_to(4, 0);
;
	lda     #$04
	jsr     pusha
	lda     #$00
	jsr     _pal_fade_to
;
; oam_clear();
;
	jsr     _oam_clear
;
; set_chr_bank_0(0); // Back to the "default" banks
;
	lda     #$00
	jsr     _set_chr_bank_0
;
; set_chr_bank_1(1);
;
	lda     #$01
	jmp     _set_chr_bank_1

.endproc

