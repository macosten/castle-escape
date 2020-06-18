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
	.forceimport	__STARTUP__
	.import		_pal_bg
	.import		_pal_spr
	.import		_pal_col
	.import		_ppu_wait_nmi
	.import		_ppu_off
	.import		_ppu_on_all
	.import		_oam_clear
	.import		_oam_meta_spr
	.import		_pad_poll
	.import		_bank_spr
	.import		_memcpy
	.import		_set_vram_buffer
	.import		_clear_vram_buffer
	.import		_set_scroll_y
	.import		_sub_scroll_y
	.import		_get_ppu_addr
	.import		_set_data_pointer
	.import		_set_mt_pointer
	.import		_buffer_4_mt
	.import		_flush_vram_update_nmi
	.export		_valrigardIdleLeft
	.export		_valrigardIdleRight
	.export		_valrigardIdleLeftAlternate
	.export		_metatiles
	.export		_level10_0
	.export		_level10_1
	.export		_level10_2
	.export		_level_starting_nt
	.export		_valrigard_starting_nt
	.export		_valrigard_inital_coords
	.export		_level_nametables
	.export		_pad1
	.export		_collision
	.export		_collision_L
	.export		_collision_R
	.export		_collision_U
	.export		_collision_D
	.export		_coordinates
	.export		_temp1
	.export		_temp2
	.export		_temp3
	.export		_temp4
	.export		_temp5
	.export		_temp6
	.export		_eject_L
	.export		_eject_R
	.export		_eject_D
	.export		_eject_U
	.export		_direction
	.export		_address
	.export		_temppointer
	.export		_x
	.export		_y
	.export		_index
	.export		_map
	.export		_pseudo_scroll_y
	.export		_scroll_y
	.export		_L_R_switch
	.export		_old_x
	.export		_old_y
	.export		_level_index
	.export		_song
	.export		_nt_max
	.export		_nt_current
	.export		_c_map
	.export		_c_map2
	.export		_palette_bg
	.export		_palette_sp
	.export		_valrigard
	.export		_hitbox
	.export		_star
	.export		_draw_sprites
	.export		_movement
	.export		_load_level
	.export		_load_room
	.export		_bg_collision
	.export		_bg_collision_sub
	.export		_draw_screen_U
	.export		_new_cmap
	.export		_main

.segment	"DATA"

_level_nametables:
	.addr	_level10_0
	.addr	_level10_1
	.addr	_level10_2
_valrigard:
	.word	$0014
	.word	$0028
	.res	4,$00
_star:
	.byte	$46
	.byte	$28
	.byte	$0F
	.byte	$0F

.segment	"RODATA"

_valrigardIdleLeft:
	.byte	$FE
	.byte	$FD
	.byte	$E0
	.byte	$01
	.byte	$06
	.byte	$FD
	.byte	$E1
	.byte	$01
	.byte	$FE
	.byte	$05
	.byte	$F0
	.byte	$01
	.byte	$06
	.byte	$05
	.byte	$F1
	.byte	$01
	.byte	$80
_valrigardIdleRight:
	.byte	$06
	.byte	$FD
	.byte	$E0
	.byte	$41
	.byte	$FE
	.byte	$FD
	.byte	$E1
	.byte	$41
	.byte	$06
	.byte	$05
	.byte	$F0
	.byte	$41
	.byte	$FE
	.byte	$05
	.byte	$F1
	.byte	$41
	.byte	$80
_valrigardIdleLeftAlternate:
	.byte	$00
	.byte	$00
	.byte	$E0
	.byte	$00
	.byte	$08
	.byte	$00
	.byte	$E1
	.byte	$00
	.byte	$00
	.byte	$08
	.byte	$F0
	.byte	$00
	.byte	$08
	.byte	$08
	.byte	$F1
	.byte	$00
	.byte	$80
_metatiles:
	.byte	$A0
	.byte	$A0
	.byte	$B0
	.byte	$B0
	.byte	$00
	.byte	$A1
	.byte	$A1
	.byte	$B1
	.byte	$B1
	.byte	$00
	.byte	$A2
	.byte	$A3
	.byte	$A2
	.byte	$A3
	.byte	$00
	.byte	$B2
	.byte	$B3
	.byte	$B2
	.byte	$B3
	.byte	$00
	.byte	$E6
	.byte	$E7
	.byte	$F6
	.byte	$F7
	.byte	$00
	.byte	$84
	.byte	$E5
	.byte	$F6
	.byte	$F7
	.byte	$00
	.byte	$E6
	.byte	$E3
	.byte	$F6
	.byte	$F3
	.byte	$00
	.byte	$E6
	.byte	$E7
	.byte	$94
	.byte	$F5
	.byte	$00
	.byte	$E2
	.byte	$E7
	.byte	$F2
	.byte	$F7
	.byte	$00
	.byte	$84
	.byte	$8D
	.byte	$F6
	.byte	$F3
	.byte	$00
	.byte	$E6
	.byte	$E3
	.byte	$94
	.byte	$9D
	.byte	$00
	.byte	$E2
	.byte	$E7
	.byte	$F0
	.byte	$F5
	.byte	$00
	.byte	$E0
	.byte	$E5
	.byte	$F2
	.byte	$F7
	.byte	$00
	.byte	$84
	.byte	$E5
	.byte	$94
	.byte	$F5
	.byte	$00
	.byte	$E2
	.byte	$E3
	.byte	$F2
	.byte	$F3
	.byte	$00
	.byte	$E0
	.byte	$8D
	.byte	$F0
	.byte	$9D
	.byte	$00
	.byte	$80
	.byte	$81
	.byte	$90
	.byte	$91
	.byte	$00
	.byte	$84
	.byte	$85
	.byte	$94
	.byte	$95
	.byte	$00
	.byte	$86
	.byte	$81
	.byte	$96
	.byte	$91
	.byte	$00
	.byte	$84
	.byte	$89
	.byte	$94
	.byte	$99
	.byte	$00
	.byte	$8A
	.byte	$81
	.byte	$9A
	.byte	$91
	.byte	$00
	.byte	$84
	.byte	$8D
	.byte	$94
	.byte	$9D
	.byte	$00
	.byte	$EC
	.byte	$ED
	.byte	$FC
	.byte	$FD
	.byte	$03
	.byte	$A4
	.byte	$A5
	.byte	$B4
	.byte	$B5
	.byte	$03
	.byte	$A6
	.byte	$A5
	.byte	$B6
	.byte	$B5
	.byte	$03
	.byte	$A6
	.byte	$A9
	.byte	$B6
	.byte	$B9
	.byte	$03
	.byte	$AA
	.byte	$AB
	.byte	$BA
	.byte	$BB
	.byte	$02
	.byte	$8E
	.byte	$8F
	.byte	$9E
	.byte	$9F
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$8E
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$8F
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$9E
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$9F
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$AC
	.byte	$AD
	.byte	$BC
	.byte	$BD
	.byte	$00
	.byte	$AE
	.byte	$AF
	.byte	$BE
	.byte	$BF
	.byte	$00
	.byte	$BC
	.byte	$CD
	.byte	$BC
	.byte	$DD
	.byte	$00
	.byte	$CE
	.byte	$BF
	.byte	$DE
	.byte	$BF
	.byte	$00
	.byte	$C4
	.byte	$C5
	.byte	$D4
	.byte	$D5
	.byte	$00
	.byte	$C6
	.byte	$C7
	.byte	$D6
	.byte	$D7
	.byte	$00
	.byte	$C8
	.byte	$C9
	.byte	$D8
	.byte	$D9
	.byte	$00
	.byte	$CA
	.byte	$CB
	.byte	$DA
	.byte	$DB
	.byte	$00
	.byte	$C2
	.byte	$C3
	.byte	$D2
	.byte	$D3
	.byte	$03
	.byte	$C0
	.byte	$C1
	.byte	$D0
	.byte	$D1
	.byte	$00
	.byte	$E8
	.byte	$E9
	.byte	$F8
	.byte	$F9
	.byte	$03
	.byte	$EA
	.byte	$EB
	.byte	$FA
	.byte	$FB
	.byte	$01
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
_level10_0:
	.byte	$06
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$0E
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$06
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$0E
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$06
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$0E
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$06
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$0E
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$06
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$0E
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$06
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$0E
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$06
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$0E
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$06
	.byte	$2A
	.byte	$2A
	.byte	$2A
	.byte	$2A
	.byte	$2A
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$0E
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$06
	.byte	$17
	.byte	$19
	.byte	$2A
	.byte	$2A
	.byte	$2A
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$0E
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$06
	.byte	$18
	.byte	$18
	.byte	$2A
	.byte	$2A
	.byte	$2A
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$0E
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$06
	.byte	$0D
	.byte	$0D
	.byte	$0D
	.byte	$0D
	.byte	$0D
	.byte	$0D
	.byte	$0D
	.byte	$15
	.byte	$2C
	.byte	$2C
	.byte	$0E
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$06
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$01
	.byte	$01
	.byte	$2C
	.byte	$2C
	.byte	$0E
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$06
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$0E
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$06
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$0F
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$0E
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$06
	.byte	$2C
	.byte	$2C
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$0E
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
_level10_1:
	.byte	$06
	.byte	$2C
	.byte	$2C
	.byte	$0C
	.byte	$05
	.byte	$05
	.byte	$05
	.byte	$05
	.byte	$05
	.byte	$05
	.byte	$05
	.byte	$06
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$06
	.byte	$2C
	.byte	$2C
	.byte	$0B
	.byte	$07
	.byte	$07
	.byte	$07
	.byte	$07
	.byte	$07
	.byte	$07
	.byte	$07
	.byte	$06
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$06
	.byte	$2C
	.byte	$28
	.byte	$2C
	.byte	$28
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$0E
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$06
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$0E
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$04
	.byte	$0D
	.byte	$11
	.byte	$2C
	.byte	$10
	.byte	$11
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$0E
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$06
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$0E
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$06
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$0E
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$06
	.byte	$2C
	.byte	$16
	.byte	$2C
	.byte	$2C
	.byte	$28
	.byte	$1C
	.byte	$1D
	.byte	$28
	.byte	$2C
	.byte	$2C
	.byte	$0E
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$06
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$1E
	.byte	$1F
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$0E
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$06
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$0E
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$06
	.byte	$29
	.byte	$2C
	.byte	$29
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$0E
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$04
	.byte	$0D
	.byte	$0D
	.byte	$11
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$0E
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$06
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2A
	.byte	$2A
	.byte	$2A
	.byte	$2C
	.byte	$0E
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$06
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2A
	.byte	$16
	.byte	$2A
	.byte	$2C
	.byte	$0E
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$06
	.byte	$2C
	.byte	$2C
	.byte	$2A
	.byte	$2A
	.byte	$2A
	.byte	$2C
	.byte	$2A
	.byte	$2A
	.byte	$2A
	.byte	$2C
	.byte	$0E
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
_level10_2:
	.byte	$06
	.byte	$2C
	.byte	$2C
	.byte	$2A
	.byte	$16
	.byte	$2A
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$0E
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$06
	.byte	$2C
	.byte	$2C
	.byte	$2A
	.byte	$2A
	.byte	$2A
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$0E
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$06
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$10
	.byte	$0D
	.byte	$0D
	.byte	$0D
	.byte	$0D
	.byte	$0D
	.byte	$06
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$06
	.byte	$28
	.byte	$1C
	.byte	$1D
	.byte	$28
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$0E
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$06
	.byte	$2C
	.byte	$1E
	.byte	$1F
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$0E
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$04
	.byte	$05
	.byte	$05
	.byte	$05
	.byte	$05
	.byte	$05
	.byte	$09
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$0E
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$04
	.byte	$07
	.byte	$07
	.byte	$07
	.byte	$07
	.byte	$07
	.byte	$07
	.byte	$15
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$0E
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$06
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$0E
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$06
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$28
	.byte	$2C
	.byte	$2C
	.byte	$0E
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$06
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$0C
	.byte	$06
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$06
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$28
	.byte	$2C
	.byte	$2C
	.byte	$0C
	.byte	$04
	.byte	$06
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$06
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$0C
	.byte	$07
	.byte	$07
	.byte	$0A
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$06
	.byte	$2C
	.byte	$28
	.byte	$2C
	.byte	$28
	.byte	$2C
	.byte	$2C
	.byte	$0C
	.byte	$06
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$06
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$0C
	.byte	$04
	.byte	$06
	.byte	$2C
	.byte	$1A
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$04
	.byte	$05
	.byte	$05
	.byte	$05
	.byte	$05
	.byte	$05
	.byte	$04
	.byte	$04
	.byte	$04
	.byte	$05
	.byte	$05
	.byte	$09
	.byte	$2C
	.byte	$2C
	.byte	$2C
	.byte	$2C
_level_starting_nt:
	.byte	$00
	.byte	$03
_valrigard_starting_nt:
	.byte	$02
_valrigard_inital_coords:
	.byte	$4D
_palette_bg:
	.byte	$0F
	.byte	$00
	.byte	$10
	.byte	$30
	.byte	$0F
	.byte	$01
	.byte	$11
	.byte	$31
	.byte	$0F
	.byte	$06
	.byte	$16
	.byte	$26
	.byte	$0F
	.byte	$17
	.byte	$27
	.byte	$38
_palette_sp:
	.byte	$0F
	.byte	$17
	.byte	$27
	.byte	$07
	.byte	$0F
	.byte	$01
	.byte	$0F
	.byte	$32
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00

.segment	"BSS"

.segment	"ZEROPAGE"
_pad1:
	.res	1,$00
_collision:
	.res	1,$00
_collision_L:
	.res	1,$00
_collision_R:
	.res	1,$00
_collision_U:
	.res	1,$00
_collision_D:
	.res	1,$00
_coordinates:
	.res	1,$00
_temp1:
	.res	1,$00
_temp2:
	.res	1,$00
_temp3:
	.res	1,$00
_temp4:
	.res	1,$00
_temp5:
	.res	2,$00
_temp6:
	.res	2,$00
_eject_L:
	.res	1,$00
_eject_R:
	.res	1,$00
_eject_D:
	.res	1,$00
_eject_U:
	.res	1,$00
_direction:
	.res	1,$00
_address:
	.res	2,$00
_temppointer:
	.res	2,$00
_x:
	.res	1,$00
_y:
	.res	1,$00
_index:
	.res	1,$00
_map:
	.res	1,$00
_pseudo_scroll_y:
	.res	2,$00
_scroll_y:
	.res	2,$00
_L_R_switch:
	.res	1,$00
_old_x:
	.res	2,$00
_old_y:
	.res	2,$00
_level_index:
	.res	1,$00
_song:
	.res	1,$00
_nt_max:
	.res	1,$00
_nt_current:
	.res	1,$00
.segment	"BSS"
_c_map:
	.res	240,$00
_c_map2:
	.res	240,$00
_hitbox:
	.res	4,$00

; ---------------------------------------------------------------
; void __near__ draw_sprites (void)
; ---------------------------------------------------------------

.segment	"CODE"

.proc	_draw_sprites: near

.segment	"CODE"

;
; oam_clear();
;
	jsr     _oam_clear
;
; temp1 = valrigard.x >> 8;
;
	lda     _valrigard+1
	sta     _temp1
;
; temp2 = valrigard.y >> 8;
;
	lda     _valrigard+3
	sta     _temp2
;
; if (direction == LEFT) {
;
	lda     _direction
	bne     L049C
;
; oam_meta_spr(temp1, temp2, valrigardIdleLeft);
;
	jsr     decsp2
	lda     _temp1
	ldy     #$01
	sta     (sp),y
	lda     _temp2
	dey
	sta     (sp),y
	lda     #<(_valrigardIdleLeft)
	ldx     #>(_valrigardIdleLeft)
;
; } else {
;
	jmp     L05E4
;
; oam_meta_spr(temp1, temp2, valrigardIdleRight);
;
L049C:	jsr     decsp2
	lda     _temp1
	ldy     #$01
	sta     (sp),y
	lda     _temp2
	dey
	sta     (sp),y
	lda     #<(_valrigardIdleRight)
	ldx     #>(_valrigardIdleRight)
L05E4:	jsr     _oam_meta_spr
;
; oam_meta_spr(star.x, star.y, valrigardIdleLeftAlternate);
;
	jsr     decsp2
	lda     _star
	ldy     #$01
	sta     (sp),y
	lda     _star+1
	dey
	sta     (sp),y
	lda     #<(_valrigardIdleLeftAlternate)
	ldx     #>(_valrigardIdleLeftAlternate)
	jmp     _oam_meta_spr

.endproc

; ---------------------------------------------------------------
; void __near__ movement (void)
; ---------------------------------------------------------------

.segment	"CODE"

.proc	_movement: near

.segment	"CODE"

;
; old_x = valrigard.x;
;
	lda     _valrigard+1
	sta     _old_x+1
	lda     _valrigard
	sta     _old_x
;
; if (pad1 & PAD_LEFT) {
;
	lda     _pad1
	and     #$02
	beq     L05EA
;
; direction = LEFT;
;
	lda     #$00
	sta     _direction
;
; if (valrigard.x <= 2) { // Changed by 1 from nesdoug's example because Valrigard's hitbox is narrower by 1 pixel on both sides
;
	lda     _valrigard+1
	cmp     #$00
	bne     L04B4
	lda     _valrigard
	cmp     #$03
L04B4:	bcs     L04B2
;
; valrigard.velocity_x = 0;
;
	lda     #$00
	sta     _valrigard+4
	sta     _valrigard+4+1
;
; valrigard.x = 0x200;
;
	ldx     #$02
	sta     _valrigard
	stx     _valrigard+1
;
; } else if (valrigard.x < 0x0600) { // Don't allow us to wrap to the other side
;
	jmp     L04D4
L04B2:	ldx     _valrigard+1
	cpx     #$06
	bcs     L04BA
;
; valrigard.velocity_x = -0x100;
;
	ldx     #$FF
	lda     #$00
;
; } else {
;
	jmp     L05EC
;
; valrigard.velocity_x = -SPEED;
;
L04BA:	ldx     #$FE
;
; else if (pad1 & PAD_RIGHT){
;
	jmp     L05F5
L05EA:	lda     _pad1
	ldx     #$00
	and     #$01
	beq     L05EC
;
; direction = RIGHT;
;
	lda     #$01
	sta     _direction
;
; if (valrigard.x >= 0xf000) {  // Also changed by 1 from nesdoug's example because Valrigard's hitbox is narrower by 1 pixel on both sides
;
	lda     _valrigard
	cmp     #$00
	lda     _valrigard+1
	sbc     #$F0
	bcc     L04C6
;
; valrigard.velocity_x = 0;
;
	txa
	sta     _valrigard+4
	sta     _valrigard+4+1
;
; valrigard.x = 0xf000; // ...and we'd add 0x100 to this constant here (and the 0x200 in the PAD_LEFT block would be a 0x100)
;
	ldx     #$F0
	sta     _valrigard
	stx     _valrigard+1
;
; } else if (valrigard.x > 0xec00) { // Don't allow us to wrap to the other side
;
	jmp     L04D4
L04C6:	lda     _valrigard
	cmp     #$01
	lda     _valrigard+1
	sbc     #$EC
	bcc     L04CD
;
; valrigard.velocity_x = 0x100;
;
	inx
	lda     #$00
;
; } else {
;
	jmp     L05EC
;
; valrigard.velocity_x = SPEED;
;
L04CD:	inx
L05F5:	lda     #$80
;
; valrigard.velocity_x = 0;
;
L05EC:	sta     _valrigard+4
	stx     _valrigard+4+1
;
; valrigard.x += valrigard.velocity_x;
;
L04D4:	lda     _valrigard+4
	clc
	adc     _valrigard
	sta     _valrigard
	lda     _valrigard+4+1
	adc     _valrigard+1
	sta     _valrigard+1
;
; L_R_switch = 1; // Shrinks the Y values in bg_coll. This makes head/foot collisions less problematic (examine this)
;
	lda     #$01
	sta     _L_R_switch
;
; hitbox.x = high_byte(valrigard.x);
;
	lda     _valrigard+1
	sta     _hitbox
;
; hitbox.y = high_byte(valrigard.y);
;
	lda     _valrigard+3
	sta     _hitbox+1
;
; hitbox.width = VALRIGARD_WIDTH;
;
	lda     #$0B
	sta     _hitbox+2
;
; hitbox.height = VALRIGARD_HEIGHT;
;
	lda     #$0D
	sta     _hitbox+3
;
; bg_collision();
;
	jsr     _bg_collision
;
; if (collision_L && collision_R) { // Half-stuck in a wall, I'm guessing?
;
	lda     _collision_L
	beq     L04E6
	lda     _collision_R
	beq     L04E6
;
; valrigard.x = old_x;
;
	lda     _old_x+1
	sta     _valrigard+1
	lda     _old_x
	sta     _valrigard
;
; else if (collision_L) {
;
	jmp     L04F3
L04E6:	lda     _collision_L
	beq     L04ED
;
; valrigard.x -= (eject_L << 8);
;
	lda     #$FF
	sec
	adc     _valrigard
	sta     _valrigard
	lda     _eject_L
;
; else if (collision_R) {
;
	jmp     L05FA
L04ED:	lda     _collision_R
	beq     L04F3
;
; valrigard.x -= (eject_R << 8);
;
	lda     #$FF
	sec
	adc     _valrigard
	sta     _valrigard
	lda     _eject_R
L05FA:	eor     #$FF
	adc     _valrigard+1
	sta     _valrigard+1
;
; old_y = valrigard.y;
;
L04F3:	lda     _valrigard+2+1
	sta     _old_y+1
	lda     _valrigard+2
	sta     _old_y
;
; if (pad1 & PAD_UP) {
;
	lda     _pad1
	and     #$08
	beq     L05ED
;
; if (valrigard.y < 0x0200) {
;
	ldx     _valrigard+2+1
	cpx     #$02
	bcs     L04FC
;
; valrigard.velocity_y = 0;
;
	lda     #$00
	sta     _valrigard+6
	sta     _valrigard+6+1
;
; valrigard.y = 0x100;
;
	ldx     #$01
	sta     _valrigard+2
	stx     _valrigard+2+1
;
; else if (valrigard.y < 0x0400) { // Stop sprite-wrapping
;
	jmp     L051B
L04FC:	ldx     _valrigard+2+1
	cpx     #$04
	bcs     L0503
;
; valrigard.velocity_y = -0x100;
;
	ldx     #$FF
	lda     #$00
;
; else {
;
	jmp     L05EF
;
; valrigard.velocity_y = -SPEED;
;
L0503:	ldx     #$FE
;
; else if (pad1 & PAD_DOWN) {
;
	jmp     L05F7
L05ED:	lda     _pad1
	ldx     #$00
	and     #$04
	beq     L05EF
;
; if (valrigard.y > 0xdf00) {
;
	lda     _valrigard+2
	cmp     #$01
	lda     _valrigard+2+1
	sbc     #$DF
	bcc     L050D
;
; valrigard.velocity_y = 0;
;
	txa
	sta     _valrigard+6
	sta     _valrigard+6+1
;
; valrigard.y = 0xdf00;
;
	ldx     #$DF
	sta     _valrigard+2
	stx     _valrigard+2+1
;
; else if (valrigard.y > 0xdc00) { // If you could be half in the floor and half in the cieling, would you?
;
	jmp     L051B
L050D:	lda     _valrigard+2
	cmp     #$01
	lda     _valrigard+2+1
	sbc     #$DC
	bcc     L0514
;
; valrigard.velocity_y = 0x100;
;
	inx
	lda     #$00
;
; else {
;
	jmp     L05EF
;
; valrigard.velocity_y = SPEED;
;
L0514:	inx
L05F7:	lda     #$80
;
; valrigard.velocity_y = 0;
;
L05EF:	sta     _valrigard+6
	stx     _valrigard+6+1
;
; valrigard.y += valrigard.velocity_y;
;
L051B:	lda     _valrigard+6
	clc
	adc     _valrigard+2
	sta     _valrigard+2
	lda     _valrigard+6+1
	adc     _valrigard+2+1
	sta     _valrigard+2+1
;
; L_R_switch = 0;
;
	lda     #$00
	sta     _L_R_switch
;
; hitbox.x = high_byte(valrigard.x);
;
	lda     _valrigard+1
	sta     _hitbox
;
; hitbox.y = high_byte(valrigard.y);
;
	lda     _valrigard+3
	sta     _hitbox+1
;
; bg_collision();
;
	jsr     _bg_collision
;
; if (collision_U && collision_D) {
;
	lda     _collision_U
	beq     L0529
	lda     _collision_D
	beq     L0529
;
; valrigard.y = old_y;
;
	lda     _old_y+1
	sta     _valrigard+2+1
	lda     _old_y
	sta     _valrigard+2
;
; else if(collision_U) {
;
	jmp     L0536
L0529:	lda     _collision_U
	beq     L0530
;
; valrigard.y -= (eject_U << 8);
;
	lda     #$FF
	sec
	adc     _valrigard+2
	sta     _valrigard+2
	lda     _eject_U
;
; else if (collision_D) {
;
	jmp     L05FB
L0530:	lda     _collision_D
	beq     L0536
;
; valrigard.y -= (eject_D << 8);
;
	lda     #$FF
	sec
	adc     _valrigard+2
	sta     _valrigard+2
	lda     _eject_D
L05FB:	eor     #$FF
	adc     _valrigard+2+1
	sta     _valrigard+2+1
;
; temp5 = valrigard.y;
;
L0536:	lda     _valrigard+2+1
	sta     _temp5+1
	lda     _valrigard+2
	sta     _temp5
;
; if (valrigard.y < MAX_UP) {
;
	ldx     _valrigard+2+1
	cpx     #$40
	bcs     L053D
;
; temp1 = (MAX_UP - valrigard.y + 0x80) >> 8; // "the numbers work better with +80 (like 0.5)". I'll take his word for it.
;
	lda     #$00
	sec
	sbc     _valrigard+2
	pha
	lda     #$40
	sbc     _valrigard+2+1
	tax
	pla
	clc
	adc     #$80
	bcc     L0542
	inx
L0542:	txa
	sta     _temp1
;
; scroll_y = sub_scroll_y(temp1, scroll_y);
;
	jsr     pusha
	lda     _scroll_y
	ldx     _scroll_y+1
	jsr     _sub_scroll_y
	sta     _scroll_y
	stx     _scroll_y+1
;
; valrigard.y += (temp1 << 8);
;
	lda     #$00
	clc
	adc     _valrigard+2
	sta     _valrigard+2
	lda     _temp1
	adc     _valrigard+2+1
	sta     _valrigard+2+1
;
; if((high_byte(scroll_y) >= 0x80) || (scroll_y <= MIN_SCROLL)) { // 0x80 = negative
;
L053D:	lda     _scroll_y+1
	cmp     #$80
	bcs     L05F0
	cmp     #$00
	bne     L054F
	lda     _scroll_y
	cmp     #$03
L054F:	bcs     L055B
;
; scroll_y = MIN_SCROLL;
;
L05F0:	ldx     #$00
	lda     #$02
	sta     _scroll_y
	stx     _scroll_y+1
;
; valrigard.y = temp5;
;
	lda     _temp5+1
	sta     _valrigard+2+1
	lda     _temp5
	sta     _valrigard+2
;
; if (high_byte(valrigard.y) < 2) valrigard.y = 0x0200;
;
	lda     _valrigard+3
	cmp     #$02
;
; else if (high_byte(valrigard.y) > 0xf0) valrigard.y = 0x0200; // > 0xf0 wrapped to the bottom.
;
	bcc     L05FC
	cmp     #$F1
	bcc     L055B
L05FC:	ldx     #$02
	lda     #$00
	sta     _valrigard+2
	stx     _valrigard+2+1
;
; if ((scroll_y & 0xff) >= 0xec) {
;
L055B:	lda     _scroll_y
	cmp     #$EC
;
; new_cmap();
;
	jcs     _new_cmap
;
; }
;
	rts

.endproc

; ---------------------------------------------------------------
; void __near__ load_level (void)
; ---------------------------------------------------------------

.segment	"CODE"

.proc	_load_level: near

.segment	"CODE"

;
; nt_max = level_starting_nt[level_index+1];
;
	ldx     #$00
	lda     _level_index
	clc
	adc     #$01
	bcc     L0436
	inx
L0436:	sta     ptr1
	txa
	clc
	adc     #>(_level_starting_nt)
	sta     ptr1+1
	ldy     #<(_level_starting_nt)
	lda     (ptr1),y
	sta     _nt_max
;
; nt_current = valrigard_starting_nt[level_index];
;
	ldy     _level_index
	lda     _valrigard_starting_nt,y
	sta     _nt_current
;
; high_byte(scroll_y) = nt_current;
;
	sta     _scroll_y+1
;
; low_byte(scroll_y) = 0;
;
	lda     #$00
	sta     _scroll_y
;
; }
;
	rts

.endproc

; ---------------------------------------------------------------
; void __near__ load_room (void)
; ---------------------------------------------------------------

.segment	"CODE"

.proc	_load_room: near

.segment	"CODE"

;
; temp1 = nt_current;
;
	lda     _nt_current
	sta     _temp1
;
; temppointer = level_nametables[temp1];
;
	ldx     #$00
	lda     _temp1
	asl     a
	bcc     L0603
	inx
	clc
L0603:	adc     #<(_level_nametables)
	sta     ptr1
	txa
	adc     #>(_level_nametables)
	sta     ptr1+1
	ldy     #$01
	lda     (ptr1),y
	sta     _temppointer+1
	dey
	lda     (ptr1),y
	sta     _temppointer
;
; temp1 = valrigard_inital_coords[level_index];
;
	ldy     _level_index
	lda     _valrigard_inital_coords,y
	sta     _temp1
;
; valrigard.x = ((temp1 >> 4) * 16) << 8;
;
	lsr     a
	lsr     a
	lsr     a
	lsr     a
	asl     a
	asl     a
	asl     a
	asl     a
	sta     _valrigard+1
	lda     #$00
	sta     _valrigard
;
; valrigard.y = ((temp1 & 0x0f) * 16) << 8;
;
	lda     _temp1
	and     #$0F
	asl     a
	asl     a
	asl     a
	asl     a
	sta     _valrigard+2+1
	lda     #$00
	sta     _valrigard+2
;
; set_data_pointer(temppointer);
;
	lda     _temppointer
	ldx     _temppointer+1
	jsr     _set_data_pointer
;
; set_mt_pointer(metatiles);
;
	lda     #<(_metatiles)
	ldx     #>(_metatiles)
	jsr     _set_mt_pointer
;
; temp2 = nt_current - nt_max; // temp2 is now the total length of this level
;
	lda     _nt_current
	sec
	sbc     _nt_max
	sta     _temp2
;
; temp1 = (((temp2 * 0x100) - 0x11) >> 8) + 1; // MAX_SCROLL in scroll_up.h, but different since we're keeping track of nt_max.
;
	tax
	lda     #$00
	sec
	sbc     #$11
	bcs     L045E
	dex
L045E:	txa
	clc
	adc     #$01
	sta     _temp1
;
; temp1 = (temp1 & 1) << 1; // Not sure if the temp1 stuff here is necessary.
;
	and     #$01
	asl     a
	sta     _temp1
;
; for (y = 0; /*We'll break manually*/; y += 0x20) {
;
	lda     #$00
L0601:	sta     _y
;
; for (x = 0; ; x += 0x20) {
;
	lda     #$00
L0600:	sta     _x
;
; clear_vram_buffer(); // do this each frame, and before putting anything in the buffer.
;
	jsr     _clear_vram_buffer
;
; address = get_ppu_addr(temp1, x, y);
;
	jsr     decsp2
	lda     _temp1
	ldy     #$01
	sta     (sp),y
	lda     _x
	dey
	sta     (sp),y
	lda     _y
	jsr     _get_ppu_addr
	sta     _address
	stx     _address+1
;
; index = (y & 0xf0) + (x >> 4);
;
	lda     _y
	and     #$F0
	sta     ptr1
	lda     _x
	lsr     a
	lsr     a
	lsr     a
	lsr     a
	clc
	adc     ptr1
	sta     _index
;
; buffer_4_mt(address, index); // ppu_address, index to the data
;
	lda     _address
	ldx     _address+1
	jsr     pushax
	lda     _index
	jsr     _buffer_4_mt
;
; flush_vram_update_nmi();
;
	jsr     _flush_vram_update_nmi
;
; if (x == 0xe0) break;
;
	lda     _x
	cmp     #$E0
	beq     L0604
;
; for (x = 0; ; x += 0x20) {
;
	lda     #$20
	clc
	adc     _x
	jmp     L0600
;
; if (y == 0xe0) break;
;
L0604:	lda     _y
	cmp     #$E0
	beq     L0605
;
; for (y = 0; /*We'll break manually*/; y += 0x20) {
;
	lda     #$20
	clc
	adc     _y
	jmp     L0601
;
; temp1 = temp1 ^ 2;
;
L0605:	lda     _temp1
	eor     #$02
	sta     _temp1
;
; new_cmap();
;
	jsr     _new_cmap
;
; if (nt_current == nt_max - 1){ // If we're at the top, there's not much of a reason to load what's above us.
;
	ldx     #$00
	lda     _nt_max
	sec
	sbc     #$01
	bcs     L048A
	dex
L048A:	cpx     #$00
	bne     L0488
	cmp     _nt_current
	bne     L0488
;
; y = 0;
;
	stx     _y
;
; temp2 = nt_current + 1;
;
	lda     _nt_current
	clc
	adc     #$01
;
; } else { // Usually we'll be going up, so we'll pre-load part of the nt above us.
;
	jmp     L0602
;
; y = 0xe0;
;
L0488:	lda     #$E0
	sta     _y
;
; temp2 = nt_current - 1;
;
	lda     _nt_current
	sec
	sbc     #$01
L0602:	sta     _temp2
;
; }
;
	rts

.endproc

; ---------------------------------------------------------------
; void __near__ bg_collision (void)
; ---------------------------------------------------------------

.segment	"CODE"

.proc	_bg_collision: near

.segment	"CODE"

;
; collision_L = 0;
;
	lda     #$00
	sta     _collision_L
;
; collision_R = 0;
;
	sta     _collision_R
;
; collision_U = 0;
;
	sta     _collision_U
;
; collision_D = 0;
;
	sta     _collision_D
;
; if(hitbox.y >= 0xf0) return;
;
	lda     _hitbox+1
	cmp     #$F0
	bcc     L0608
;
; }
;
	rts
;
; temp6 = temp5 = hitbox.x /*+ scroll_x*/; // upper left (temp6 = save for reuse)
;
L0608:	lda     _hitbox
	ldx     #$00
	sta     _temp5
	stx     _temp5+1
	sta     _temp6
	stx     _temp6+1
;
; temp1 = temp5 & 0xff; // low byte x
;
	lda     _temp5
	sta     _temp1
;
; temp2 = temp5 >> 8; // high byte x
;
	lda     _temp5+1
	sta     _temp2
;
; eject_L = temp1 | 0xf0;
;
	lda     _temp1
	ora     #$F0
	sta     _eject_L
;
; temp3 = hitbox.y; // y top
;
	lda     _hitbox+1
	sta     _temp3
;
; eject_U = temp3 | 0xf0;
;
	ora     #$F0
	sta     _eject_U
;
; if(L_R_switch) temp3 += 2; // fix bug, walking through walls
;
	lda     _L_R_switch
	beq     L057C
	lda     #$02
	clc
	adc     _temp3
	sta     _temp3
;
; bg_collision_sub();
;
L057C:	jsr     _bg_collision_sub
;
; if(collision){ // find a corner in the collision map
;
	lda     _collision
	beq     L0581
;
; ++collision_L;
;
	inc     _collision_L
;
; ++collision_U;
;
	inc     _collision_U
;
; temp5 += hitbox.width;
;
L0581:	lda     _hitbox+2
	clc
	adc     _temp5
	sta     _temp5
	lda     #$00
	adc     _temp5+1
	sta     _temp5+1
;
; temp1 = temp5 & 0xff; // low byte x
;
	lda     _temp5
	sta     _temp1
;
; temp2 = temp5 >> 8; // high byte x
;
	lda     _temp5+1
	sta     _temp2
;
; eject_R = (temp1 + 1) & 0x0f;
;
	lda     _temp1
	clc
	adc     #$01
	and     #$0F
	sta     _eject_R
;
; bg_collision_sub();
;
	jsr     _bg_collision_sub
;
; if(collision){ // find a corner in the collision map
;
	lda     _collision
	beq     L0590
;
; ++collision_R;
;
	inc     _collision_R
;
; ++collision_U;
;
	inc     _collision_U
;
; temp3 = hitbox.y + hitbox.height; //y bottom
;
L0590:	lda     _hitbox+1
	clc
	adc     _hitbox+3
	sta     _temp3
;
; if(L_R_switch) temp3 -= 2; // fix bug, walking through walls
;
	lda     _L_R_switch
	beq     L0607
	lda     _temp3
	sec
	sbc     #$02
	sta     _temp3
;
; eject_D = (temp3 + 1) & 0x0f;
;
L0607:	lda     _temp3
	clc
	adc     #$01
	and     #$0F
	sta     _eject_D
;
; if(temp3 >= 0xf0) return;
;
	lda     _temp3
	cmp     #$F0
	bcs     L05AA
;
; bg_collision_sub();
;
	jsr     _bg_collision_sub
;
; if(collision){ // find a corner in the collision map
;
	lda     _collision
	beq     L05A1
;
; ++collision_R;
;
	inc     _collision_R
;
; ++collision_D;
;
	inc     _collision_D
;
; temp1 = temp6 & 0xff; // low byte x
;
L05A1:	lda     _temp6
	sta     _temp1
;
; temp2 = temp6 >> 8; // high byte x
;
	lda     _temp6+1
	sta     _temp2
;
; bg_collision_sub();
;
	jsr     _bg_collision_sub
;
; if(collision){ // find a corner in the collision map
;
	lda     _collision
	beq     L05AA
;
; ++collision_L;
;
	inc     _collision_L
;
; ++collision_D;
;
	inc     _collision_D
;
; }
;
L05AA:	rts

.endproc

; ---------------------------------------------------------------
; void __near__ bg_collision_sub (void)
; ---------------------------------------------------------------

.segment	"CODE"

.proc	_bg_collision_sub: near

.segment	"CODE"

;
; coordinates = (temp1 >> 4) + (temp3 & 0xf0); // upper left
;
	lda     _temp1
	lsr     a
	lsr     a
	lsr     a
	lsr     a
	sta     ptr1
	lda     _temp3
	and     #$F0
	clc
	adc     ptr1
	sta     _coordinates
;
; map = temp2&1; // high byte
;
	lda     _temp2
	and     #$01
	sta     _map
;
; if (!map) {
;
	lda     _map
	bne     L05B5
;
; pal_col(0,0x10);
;
	jsr     pusha
	lda     #$10
	jsr     _pal_col
;
; collision = (c_map[coordinates] < 0x17 && c_map[coordinates] > 0x03); // 0x17 is the first non-solid tile, so if the tile is less than that, it's a collision
;
	ldy     _coordinates
	lda     _c_map,y
	cmp     #$17
	bcs     L060A
	ldy     _coordinates
	lda     _c_map,y
	cmp     #$04
	bcs     L060E
L060A:	lda     #$00
	jmp     L0609
;
; pal_col(0,0x20);
;
L05B5:	lda     #$00
	jsr     pusha
	lda     #$20
	jsr     _pal_col
;
; collision = (c_map2[coordinates] < 0x17 && c_map[coordinates] > 0x03);
;
	ldy     _coordinates
	lda     _c_map2,y
	cmp     #$17
	bcs     L060D
	ldy     _coordinates
	lda     _c_map,y
	cmp     #$04
	bcs     L060E
L060D:	lda     #$00
	jmp     L0609
L060E:	lda     #$01
L0609:	sta     _collision
;
; }
;
	rts

.endproc

; ---------------------------------------------------------------
; void __near__ draw_screen_U (void)
; ---------------------------------------------------------------

.segment	"CODE"

.proc	_draw_screen_U: near

.segment	"CODE"

;
; }
;
	rts

.endproc

; ---------------------------------------------------------------
; void __near__ new_cmap (void)
; ---------------------------------------------------------------

.segment	"CODE"

.proc	_new_cmap: near

.segment	"CODE"

;
; nt_current = high_byte(scroll_y); //high byte = the index of the nametable we're in?, one to the right
;
	lda     _scroll_y+1
	sta     _nt_current
;
; map = nt_current & 1; //even or odd?
;
	and     #$01
	sta     _map
;
; if (!map) {
;
	lda     _map
	bne     L05D7
;
; memcpy (c_map, level_nametables[nt_current], 240);
;
	lda     #<(_c_map)
	ldx     #>(_c_map)
	jsr     pushax
	ldx     #$00
	lda     _nt_current
	asl     a
	bcc     L0612
	inx
	clc
L0612:	adc     #<(_level_nametables)
	sta     ptr1
	txa
	adc     #>(_level_nametables)
	sta     ptr1+1
	ldy     #$01
	lda     (ptr1),y
	tax
	dey
	lda     (ptr1),y
	jsr     pushax
	ldx     #$00
	lda     #$F0
	jmp     _memcpy
;
; memcpy (c_map2, level_nametables[nt_current], 240);
;
L05D7:	lda     #<(_c_map2)
	ldx     #>(_c_map2)
	jsr     pushax
	ldx     #$00
	lda     _nt_current
	asl     a
	bcc     L0613
	inx
	clc
L0613:	adc     #<(_level_nametables)
	sta     ptr1
	txa
	adc     #>(_level_nametables)
	sta     ptr1+1
	ldy     #$01
	lda     (ptr1),y
	tax
	dey
	lda     (ptr1),y
	jsr     pushax
	ldx     #$00
	lda     #$F0
	jmp     _memcpy

.endproc

; ---------------------------------------------------------------
; void __near__ main (void)
; ---------------------------------------------------------------

.segment	"CODE"

.proc	_main: near

.segment	"CODE"

;
; ppu_off(); // screen off
;
	jsr     _ppu_off
;
; pal_bg(palette_bg);
;
	lda     #<(_palette_bg)
	ldx     #>(_palette_bg)
	jsr     _pal_bg
;
; pal_spr(palette_sp);
;
	lda     #<(_palette_sp)
	ldx     #>(_palette_sp)
	jsr     _pal_spr
;
; bank_spr(1);
;
	lda     #$01
	jsr     _bank_spr
;
; set_vram_buffer(); // do at least once, sets a pointer to a buffer
;
	jsr     _set_vram_buffer
;
; clear_vram_buffer();
;
	jsr     _clear_vram_buffer
;
; load_level();
;
	jsr     _load_level
;
; load_room();
;
	jsr     _load_room
;
; ppu_on_all(); // turn on screen
;
	jsr     _ppu_on_all
;
; level_index = 0;
;
	lda     #$00
	sta     _level_index
;
; ppu_wait_nmi(); // wait till beginning of the frame
;
L0425:	jsr     _ppu_wait_nmi
;
; pad1 = pad_poll(0); // read the first controller
;
	lda     #$00
	jsr     _pad_poll
	sta     _pad1
;
; movement();
;
	jsr     _movement
;
; set_scroll_y(scroll_y);
;
	lda     _scroll_y
	ldx     _scroll_y+1
	jsr     _set_scroll_y
;
; draw_screen_U();
;
	jsr     _draw_screen_U
;
; draw_sprites();
;
	jsr     _draw_sprites
;
; while (1){
;
	jmp     L0425

.endproc

