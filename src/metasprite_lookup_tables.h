//
// metasprite_lookup_tables.h

#include "metasprites.h"

#pragma rodata-name(push, "BANK5")

const unsigned char * const valrigard_idle_sprite_lookup_table[] = {
    valrigard_idle_left, valrigard_idle_right
};

const unsigned char * const valrigard_sword_swing_sprite_lookup_table[] = {
    // Animation for swinging (reversed): (Left, Right)
    valrigard_swing_left_followthrough, valrigard_swing_right_followthrough,              
    valrigard_swing_left_followthrough, valrigard_swing_right_followthrough,
    valrigard_swing_left_followthrough, valrigard_swing_right_followthrough,
    valrigard_idle_left,                valrigard_idle_right,               
    valrigard_swing_left_high,          valrigard_swing_right_high,
    valrigard_swing_left_mid,           valrigard_swing_right_mid,           
    valrigard_swing_left_low,           valrigard_swing_right_low,
    valrigard_swing_left_low,           valrigard_swing_right_low,
};

const unsigned char * const valrigard_dead_sprite_lookup_table[] = {
    valrigard_dead_left, valrigard_dead_right
};

const unsigned char * const valrigard_flying_sprite_lookup_table[] = {
    // Animation for flying (reversed): (Left, Right)
    valrigard_flying_left1, valrigard_flying_right1,
    valrigard_flying_left1, valrigard_flying_right1,
    valrigard_flying_left2, valrigard_flying_right2,
    valrigard_flying_left2, valrigard_flying_right2,
    valrigard_flying_left1, valrigard_flying_right1,
    valrigard_idle_left,    valrigard_idle_right,
    valrigard_flying_left0, valrigard_flying_right0,
    valrigard_flying_left0, valrigard_flying_right0,
};

const unsigned char * const valrigard_walking_sprite_lookup_table[] = {
    // Animation for walking (reversed): (Left, Right)
    valrigard_walking_left0, valrigard_walking_right0,// These should each last twice as long. 
    valrigard_walking_left1, valrigard_walking_right1,
    valrigard_idle_left,     valrigard_idle_right,
    valrigard_walking_left2, valrigard_walking_right2,
    valrigard_walking_left3, valrigard_walking_right3,
    valrigard_idle_left,     valrigard_idle_right,
};

// Lookup tables for enemy sprites (not yet animated).
const unsigned char * const korbat_sprite_lookup_table[] = {
    korbat_left, korbat_right,
    korbat_flap1_left, korbat_flap1_right,
    korbat_flap2_left, korbat_flap2_right,
    korbat_flap3_left, korbat_flap3_right,
    korbat_flap4_left, korbat_flap4_right,
    korbat_flap5_left, korbat_flap5_right,
    korbat_flap6_left, korbat_flap6_right,
    korbat_flap6_left, korbat_flap6_right,
    korbat_flap6_left, korbat_flap6_right,
    korbat_flap5_left, korbat_flap5_right,
    korbat_flap4_left, korbat_flap4_right,
    korbat_flap3_left, korbat_flap3_right,
    korbat_flap2_left, korbat_flap2_right,
    korbat_flap1_left, korbat_flap1_right,
    korbat_left, korbat_right,
    korbat_left, korbat_right,
};


const unsigned char * const grarrl_sprite_lookup_table[] = {
    grarrl_left, grarrl_right,
    grarrl_left, grarrl_right,
    grarrl_backfoot_step0_left, grarrl_backfoot_step0_right,
    grarrl_backfoot_step0_left, grarrl_backfoot_step0_right,
    grarrl_backfoot_step0_left, grarrl_backfoot_step0_right,
    grarrl_backfoot_step0_left, grarrl_backfoot_step0_right,
    grarrl_backfoot_step1_left, grarrl_backfoot_step1_right,
    grarrl_backfoot_step1_left, grarrl_backfoot_step1_right,
    grarrl_left, grarrl_right,
    grarrl_left, grarrl_right,
    grarrl_frontfoot_step0_left, grarrl_frontfoot_step0_right,
    grarrl_frontfoot_step0_left, grarrl_frontfoot_step0_right,
    grarrl_frontfoot_step0_left, grarrl_frontfoot_step0_right,
    grarrl_frontfoot_step0_left, grarrl_frontfoot_step0_right,
    grarrl_frontfoot_step1_left, grarrl_frontfoot_step1_right,
    grarrl_frontfoot_step1_left, grarrl_frontfoot_step1_right,
};

// Cannon lookup tables.
const unsigned char * const cannon_sprite_lookup_table[] = {cannon_up, cannon_up_left, cannon_left, cannon_down_left, cannon_down, cannon_down_right, cannon_right, cannon_up_right};

// const unsigned char const enemy_contact_behavior_lookup_table[] = {0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1}

// Frames here are in reverse order so that they can be array-indexed and then have the index decremented.
const unsigned char * const acidblob_sprite_lookup_table[] = {acidblob0, acidblob3, acidblob0, acidblob1, acidblob2, acidblob1, acidblob0};

const unsigned char * const sun_sprite_lookup_table[] = {sun0, sun1};

// Indexed by a 4-bit number with bits arranged as follows:
// [Moving (idle/tornado)][Frame Number MSB][Frame Number LSB][Direction (left/right)]
const unsigned char * const splyke_sprite_lookup_table[] = {
    splyke_idle_left0,  splyke_idle_right0,     splyke_idle_left0,  splyke_idle_right0,
    splyke_idle_left1,  splyke_idle_right1,     splyke_idle_left1,  splyke_idle_right1,
    splyke_tornado0,    splyke_tornado0,        splyke_tornado1,    splyke_tornado1,
    splyke_tornado2,    splyke_tornado2,        splyke_tornado1,    splyke_tornado1
};

const unsigned char * const purple_death_effect_sprite_lookup_table[] = {
    purple_death_effect1, purple_death_effect1, purple_death_effect0
};

const unsigned char * const splyke_death_effect_sprite_lookup_table[] = {
    splyke_death_effect1, splyke_death_effect1, splyke_death_effect0
};

const unsigned char * const energy_bar_lookup_table[] = {
    energy_bar_0, energy_bar_1, energy_bar_2, energy_bar_3, energy_bar_4, energy_bar_5, energy_bar_6, energy_bar_7,
    energy_bar_8, energy_bar_9, energy_bar_a, energy_bar_b, energy_bar_c, energy_bar_d, energy_bar_e, energy_bar_f
};

const unsigned char * const boss_body_sprite_idle_lookup_table[] = {
    boss_body_idle_left0, boss_body_idle_right0,
};

const unsigned char * const boss_head_sprite_lookup_table[] = {
    boss_head_eye_open_mouth_closed_left, boss_head_eye_open_mouth_closed_right,
    boss_head_eye_closed_mouth_closed_left, boss_head_eye_closed_mouth_closed_right,
};

const unsigned char * const boss_body_sprite_flying_lookup_table[] = {
    boss_body_flying_left0, boss_body_flying_right0,
    boss_body_flying_left1, boss_body_flying_right1,
    boss_body_flying_left2, boss_body_flying_right2,
    boss_body_flying_left3, boss_body_flying_right3,
};

const unsigned char * const boss_dying_sprite_lookup_table[] = {
    purple_death_effect0, purple_death_effect1, purple_death_effect0, purple_death_effect2
};

const unsigned char const boss_magic_offset_table[] = { 0x80, 0x81, 0x82 };

const unsigned char * const floating_numbers_sprite_lookup_table[] = {
    points_1, points_2, points_3, points_4, points_100
};

const unsigned char const floating_numbers_y_offset_lookup_table[] = {
    0, -5, -8, -9, -11, -12, -12, -13, -14, -14, -15, -15, -15, -16, -16, -16,
    -16, -16, -16, -15, -15, -15, -14, -14, -13, -12, -12, -11, -9, -8, -5, 0
};

#pragma rodata-name(pop)

