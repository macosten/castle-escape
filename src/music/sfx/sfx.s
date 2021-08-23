;this file has been assembled from famistudio_sfx and tui_sfx (which was exported to NSF, then nsf2data'd)

sounds:
	.word @ntsc
	.word @ntsc
@ntsc:
	.word @sfx_ntsc_00_sword_slash
	.word @sfx_ntsc_01_star_collect
	.word @sfx_ntsc_02_enemy_kill
	.word @sfx_ntsc_03_cannon_fire
	.word @sfx_ntsc_04_menu_beep
	.word @sfx_ntsc_05_bump
	.word @sfx_ntsc_06_acid_drop
	.word @sfx_ntsc_07_smack

@sfx_ntsc_00_sword_slash:
	.byte $8a,$0f,$89,$3f,$8a,$0e,$01,$8a,$0d,$01,$8a,$0c,$01,$8a,$0b,$01
	.byte $8a,$0a,$01,$8a,$09,$01,$8a,$08,$01,$8a,$07,$01,$8a,$06,$00
@sfx_ntsc_01_star_collect:
	.byte $82,$00,$81,$bd,$80,$3f,$89,$f0,$81,$7e,$01,$81,$54,$80,$36,$01
	.byte $80,$3d,$01,$80,$39,$01,$80,$37,$01,$80,$35,$01,$80,$33,$01,$80
	.byte $32,$01,$80,$31,$03,$00
@sfx_ntsc_02_enemy_kill:
	.byte $8a,$07,$89,$3a,$8a,$06,$89,$3d,$01,$8a,$05,$89,$3c,$01,$8a,$04
	.byte $01,$8a,$05,$89,$3b,$03,$89,$3a,$02,$89,$39,$01,$89,$38,$02,$89
	.byte $37,$02,$89,$36,$03,$89,$35,$02,$89,$34,$03,$89,$33,$05,$89,$32
	.byte $14,$89,$31,$20,$00
@sfx_ntsc_03_cannon_fire:
	.byte $8a,$0f,$89,$3a,$8a,$0e,$89,$3d,$01,$8a,$0d,$89,$3c,$01,$8a,$0c
	.byte $01,$8a,$0d,$89,$3b,$03,$89,$3a,$02,$89,$39,$01,$89,$38,$02,$89
	.byte $37,$02,$89,$36,$03,$89,$35,$02,$89,$34,$03,$89,$33,$05,$89,$32
	.byte $14,$89,$31,$20,$00
@sfx_ntsc_04_menu_beep:
	.byte $80,$b5,$81,$c0,$82,$00,$01,$81,$91,$01,$80,$30,$83,$b1,$84,$c0
	.byte $85,$00,$01,$84,$91,$01,$00
@sfx_ntsc_05_bump: ; bump
	.byte $83,$ba,$84,$1a,$85,$02,$01,$84,$39,$01,$84,$58,$01,$84,$77,$01
	.byte $00
@sfx_ntsc_06_acid_drop:
	.byte $83,$b6,$84,$e9,$85,$03,$01,$83,$b7,$84,$da,$01,$84,$cb,$01,$83
	.byte $b8,$84,$bc,$01,$83,$b9,$84,$ad,$01,$83,$30,$01,$83,$b6,$84,$92
	.byte $85,$02,$01,$83,$b7,$84,$7e,$01,$84,$6a,$01,$83,$b8,$84,$56,$01
	.byte $83,$b9,$84,$42,$01,$84,$2e,$01,$00
@sfx_ntsc_07_smack:
	.byte $8a,$0f,$89,$3f,$01,$8a,$08,$89,$36,$01,$89,$3d,$01,$89,$39,$01
	.byte $89,$37,$01,$89,$35,$01,$89,$33,$01,$89,$32,$01,$89,$31,$03,$00
