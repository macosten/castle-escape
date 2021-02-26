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
	.export		_boss_ai_idle
	.export		_boss_ai_ascending
	.export		_boss_ai_descending
	.importzp	_TEMP
	.import		_rand8
	.import		_add_scroll_y_fast_sub
	.import		_metatile_property_lookup_table
	.importzp	_temp0
	.importzp	_temp1
	.importzp	_temp2
	.importzp	_temp3
	.importzp	_temp4
	.importzp	_x
	.importzp	_temp_x
	.importzp	_coordinates
	.importzp	_collision
	.importzp	_collision_U
	.importzp	_collision_D
	.importzp	_collision_L
	.importzp	_collision_R
	.importzp	_player_flags
	.importzp	_enemy_is_using_bg_collision
	.importzp	_temp5
	.importzp	_temp6
	.importzp	_temppointer
	.importzp	_temp_mutablepointer
	.importzp	_hitbox
	.import		_cmaps
	.import		_boss_dialog
	.import		_active_dboxdata
	.import		_enemies
	.import		_boss_state
	.import		_boss_memory
	.import		_trigger_dialog_box
	.import		_fire_at_target
	.import		_cannonball_ai_sub
	.import		_bg_collision
	.export		_boss_ai_intro
	.export		_boss_collide_sub

; ---------------------------------------------------------------
; void __near__ boss_ai_idle (void)
; ---------------------------------------------------------------

.segment	"CODE"

.proc	_boss_ai_idle: near

.segment	"CODE"

;
; if (temp0 == enemies.extra[x]) {
;
	ldy     _x
	lda     _enemies+384,y
	cmp     _temp0
	bne     L002B
;
; boss_state = BOSS_STATE_ASCENDING;
;
	lda     #$02
	sta     _boss_state
;
; temp0 = enemies.x[x];
;
	ldy     _x
	lda     _enemies,y
	sta     _temp0
;
; temp0 += rand8() & 0b00111111; // x -- MSB is sign
;
	jsr     _rand8
	and     #$3F
	clc
	adc     _temp0
	sta     _temp0
;
; temp1 = enemies.y[x];
;
	ldy     _x
	lda     _enemies+64,y
	sta     _temp1
;
; temp1 += rand8() & 0b00111111; // y -- MSB is sign
;
	jsr     _rand8
	and     #$3F
	clc
	adc     _temp1
	sta     _temp1
;
; temp2 = enemies.x[x];
;
	ldy     _x
	lda     _enemies,y
	sta     _temp2
;
; temp3 = enemies.y[x];
;
	ldy     _x
	lda     _enemies+64,y
	sta     _temp3
;
; temp_x = x;
;
	lda     _x
	sta     _temp_x
;
; fire_at_target();
;
	jsr     _fire_at_target
;
; BOSS_BRADS_TARGET = temp0;
;
	lda     _temp0
	sta     _boss_memory
;
; }
;
L002B:	rts

.endproc

; ---------------------------------------------------------------
; void __near__ boss_ai_ascending (void)
; ---------------------------------------------------------------

.segment	"CODE"

.proc	_boss_ai_ascending: near

.segment	"CODE"

;
; temp1 = BOSS_BRADS_TARGET;
;
	lda     _boss_memory
	sta     _temp1
;
; cannonball_ai_sub();
;
	jsr     _cannonball_ai_sub
;
; enemy_is_using_bg_collision = 1;
;
	lda     #$01
	sta     _enemy_is_using_bg_collision
;
; hitbox.x = enemies.x[x];
;
	ldy     _x
	lda     _enemies,y
	sta     _hitbox
;
; hitbox.y = enemies.y[x];
;
	ldy     _x
	lda     _enemies+64,y
	sta     _hitbox+1
;
; hitbox.width = 16;
;
	lda     #$10
	sta     _hitbox+2
;
; hitbox.height = 16;
;
	sta     _hitbox+3
;
; bg_collision();
;
	jsr     _bg_collision
;
; if (collision_U >= 2 && !CANNONBALL_Y_DIRECTION(x)) { CANNONBALL_SET_POS_Y(x); } 
;
	lda     _collision_U
	cmp     #$02
	bcc     L012B
	ldy     _x
	lda     _enemies+256,y
	and     #$20
	bne     L012B
	ldy     _x
	lda     _enemies+256,y
	ora     #$20
;
; else if (collision_D >= 2 && CANNONBALL_Y_DIRECTION(x)) { CANNONBALL_SET_NEG_Y(x); }
;
	jmp     L013D
L012B:	lda     _collision_D
	cmp     #$02
	bcc     L012F
	ldy     _x
	lda     _enemies+256,y
	and     #$20
	beq     L012F
	ldy     _x
	lda     _enemies+256,y
	and     #$DF
L013D:	sta     _TEMP
	ldy     _x
	lda     _TEMP
	sta     _enemies+256,y
;
; if (collision_L >= 2 && !CANNONBALL_X_DIRECTION(x)) { CANNONBALL_SET_POS_X(x); }
;
L012F:	lda     _collision_L
	cmp     #$02
	bcc     L0133
	ldy     _x
	lda     _enemies+256,y
	and     #$40
	bne     L0133
	ldy     _x
	lda     _enemies+256,y
	ora     #$40
;
; else if (collision_R >= 2 && CANNONBALL_X_DIRECTION(x)) { CANNONBALL_SET_NEG_X(x); }
;
	jmp     L013E
L0133:	lda     _collision_R
	cmp     #$02
	lda     #$00
	bcc     L0138
	ldy     _x
	lda     _enemies+256,y
	and     #$40
	beq     L0138
	ldy     _x
	lda     _enemies+256,y
	and     #$BF
L013E:	sta     _TEMP
	ldy     _x
	lda     _TEMP
	sta     _enemies+256,y
;
; enemy_is_using_bg_collision = 0;
;
	lda     #$00
L0138:	sta     _enemy_is_using_bg_collision
;
; }
;
	rts

.endproc

; ---------------------------------------------------------------
; void __near__ boss_ai_descending (void)
; ---------------------------------------------------------------

.segment	"CODE"

.proc	_boss_ai_descending: near

.segment	"CODE"

;
; high_byte(temp5) = enemies.nt[x];
;
	ldy     _x
	lda     _enemies+192,y
	sta     _temp5+1
;
; low_byte(temp5) = enemies.actual_y[x];
;
	ldy     _x
	lda     _enemies+128,y
	sta     _temp5
;
; temp1 = enemies.x[x] + 8; 
;
	ldy     _x
	lda     _enemies,y
	clc
	adc     #$08
	sta     _temp1
;
; add_scroll_y(temp5, 1, temp5);
;
	lda     _temp5
	sta     _TEMP
	lda     _temp5+1
	sta     _TEMP+1
	lda     #$01
	jsr     _add_scroll_y_fast_sub
	sta     _temp5
	stx     _temp5+1
;
; add_scroll_y(temp6, 16, temp5); 
;
	lda     _temp5
	sta     _TEMP
	lda     _temp5+1
	sta     _TEMP+1
	lda     #$10
	jsr     _add_scroll_y_fast_sub
	sta     _temp6
	stx     _temp6+1
;
; temp2 = low_byte(temp6); // Y of tile of interest
;
	lda     _temp6
	sta     _temp2
;
; temp4 = high_byte(temp6); // NT of tile of interest
;
	lda     _temp6+1
	sta     _temp4
;
; coordinates = (temp1 >> 4) + (temp2 & 0xf0);
;
	lda     _temp1
	lsr     a
	lsr     a
	lsr     a
	lsr     a
	sta     ptr1
	lda     _temp2
	and     #$F0
	clc
	adc     ptr1
	sta     _coordinates
;
; AsmSet2ByteFromPtrAtIndexVar(temppointer, cmaps, temp4);
;
	lda     _temp4
	asl     a
	tay
	lda     _cmaps,y
	sta     _temppointer
	lda     _cmaps+1,y
	sta     _temppointer+1
;
; AsmSet1ByteFromZpPtrAtIndexVar(collision, temppointer, coordinates);
;
	ldy     _coordinates
	lda     (_temppointer),y
	sta     _collision
;
; if (METATILE_IS_SOLID(collision)) {
;
	ldy     _collision
	lda     _metatile_property_lookup_table,y
	and     #$01
	beq     L0110
;
; enemies.extra[x] = rand8();
;
	lda     #<(_enemies+384)
	ldx     #>(_enemies+384)
	clc
	adc     _x
	bcc     L0117
	inx
L0117:	jsr     pushax
	jsr     _rand8
	ldy     #$00
	jsr     staspidx
;
; boss_state = BOSS_STATE_IDLE;
;
	lda     #$01
	sta     _boss_state
;
; enemies.nt[x] = high_byte(temp5);
;
L0110:	ldy     _x
	lda     _temp5+1
	sta     _enemies+192,y
;
; enemies.actual_y[x] = low_byte(temp5);
;
	ldy     _x
	lda     _temp5
	sta     _enemies+128,y
;
; }
;
	rts

.endproc

; ---------------------------------------------------------------
; void __near__ boss_ai_intro (void)
; ---------------------------------------------------------------

.segment	"CODE"

.proc	_boss_ai_intro: near

.segment	"CODE"

;
; SET_DIRECTION_RIGHT();
;
	lda     _player_flags
	ora     #$01
	sta     _player_flags
;
; active_dboxdata = boss_dialog;
;
	lda     #<(_active_dboxdata)
	ldx     #>(_active_dboxdata)
	jsr     pushax
	lda     #<(_boss_dialog)
	ldx     #>(_boss_dialog)
	jsr     pushax
	ldx     #$00
	lda     #$05
	jsr     _memcpy
;
; trigger_dialog_box();
;
	jsr     _trigger_dialog_box
;
; boss_state = BOSS_STATE_DESCENDING;
;
	lda     #$03
	sta     _boss_state
;
; }
;
	rts

.endproc

; ---------------------------------------------------------------
; void __near__ boss_collide_sub (void)
; ---------------------------------------------------------------

.segment	"CODE"

.proc	_boss_collide_sub: near

.segment	"CODE"

;
; coordinates = (temp1 >> 4) + (temp3 & 0xf0); 
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
; temp0 = high_byte(temp5);
;
	lda     _temp5+1
	sta     _temp0
;
; AsmSet2ByteFromPtrAtIndexVar(temp_mutablepointer, cmaps, temp0);
;
	asl     a
	tay
	lda     _cmaps,y
	sta     _temp_mutablepointer
	lda     _cmaps+1,y
	sta     _temp_mutablepointer+1
;
; AsmSet1ByteFromZpPtrAtIndexVar(temp4, temp_mutablepointer, coordinates);
;
	ldy     _coordinates
	lda     (_temp_mutablepointer),y
	sta     _temp4
;
; temp0 = metatile_property_lookup_table[temp4];
;
	ldy     _temp4
	lda     _metatile_property_lookup_table,y
	sta     _temp0
;
; collision = METATILE_IS_SOLID(temp4);
;
	ldy     _temp4
	lda     _metatile_property_lookup_table,y
	and     #$01
	sta     _collision
;
; }
;
	rts

.endproc
