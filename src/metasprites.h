//
//  metasprites.h
//  Castle Escape
//
//  Created by Zaccari Silverman on 6/11/20.
//

// (0, 8), but since Valrigard's hitbox is 11 wide and 13 tall,
// we'll center it by shifting his sprite left by two pixels and up by one.

// Each line corresponds to one hardware sprite, and the following information is in each line:
// X offset, Y offset, sprite/tile ID, palette (| OAM flip)
// 128 terminates.

#define METASPRITE_BANK 5

#pragma rodata-name(push, "BANK5")

const unsigned char const valrigard_idle_left[] = {
      0xfe, 0xfd,   0xe0,   1,
      6,    0xfd,   0xe1,   1,
      0xfe,    5,   0xf0,   1,
      6,       5,   0xf1,   1,
    128
};

const unsigned char const valrigard_idle_right[] = {
         6,  0xfd,  0xe0,1|OAM_FLIP_H,
      0xfe,  0xfd,  0xe1,1|OAM_FLIP_H,
         6,     5,  0xf0,1|OAM_FLIP_H,
      0xfe,     5,  0xf1,1|OAM_FLIP_H,
    128
};

const unsigned char const valrigard_swing_left_high[] = {
      0xfe, 0xfd,   0xe2,   1,
      6,    0xfd,   0xe1,   1,
      0xfe,    5,   0xf2,   1,
      6,       5,   0xf3,   1,
    128
};

const unsigned char const valrigard_swing_right_high[] = {
         6, 0xfd,   0xe2,   1|OAM_FLIP_H,
      0xfe, 0xfd,   0xe1,   1|OAM_FLIP_H,
         6,    5,   0xf2,   1|OAM_FLIP_H,
      0xfe,    5,   0xf3,   1|OAM_FLIP_H,
    128
};

const unsigned char const valrigard_swing_left_mid[] = {
      0xfe, 0xfd,   0xe4,   1,
      6,    0xfd,   0xe1,   1,
      0xfe,    5,   0xf4,   1,
      6,       5,   0xf3,   1,
    128
};

const unsigned char const valrigard_swing_right_mid[] = {
      6,    0xfd,   0xe4,   1|OAM_FLIP_H,
      0xfe, 0xfd,   0xe1,   1|OAM_FLIP_H,
      6,       5,   0xf4,   1|OAM_FLIP_H,
      0xfe,    5,   0xf3,   1|OAM_FLIP_H,
    128
};

const unsigned char const valrigard_swing_left_low[] = {
      0xfe, 0xfd,   0xe5,   1,
      6,    0xfd,   0xe1,   1,
      0xfe,    5,   0xf5,   1,
      6,       5,   0xf6,   1,
    128
};

const unsigned char const valrigard_swing_right_low[] = {
      6,    0xfd,   0xe5,   1|OAM_FLIP_H,
      0xfe, 0xfd,   0xe1,   1|OAM_FLIP_H,
      6,       5,   0xf5,   1|OAM_FLIP_H,
      0xfe,    5,   0xf6,   1|OAM_FLIP_H,
    128
};

const unsigned char const valrigard_swing_left_followthrough[] = {
      0xfe, 0xfd,   0xe0,   1,
      6,    0xfd,   0xe8,   1,
      0xfe,    5,   0xf7,   1,
      6,       5,   0xf8,   1,
    128
};

const unsigned char const valrigard_swing_right_followthrough[] = {
      6,    0xfd,   0xe0,   1|OAM_FLIP_H,
      0xfe, 0xfd,   0xe8,   1|OAM_FLIP_H,
      6,       5,   0xf7,   1|OAM_FLIP_H,
      0xfe,    5,   0xf8,   1|OAM_FLIP_H,
    128
};

const unsigned char const valrigard_dead_left[] = {
      6,    5,  0xd1,  1,
      0xfe, 5,  0xd0,  1,
    128
};

const unsigned char const valrigard_dead_right[] = {
      0xfe, 5, 0xd1, 1|OAM_FLIP_H,
      6,    5, 0xd0, 1|OAM_FLIP_H,
    128
};

const unsigned char const valrigard_flying_left0[] = {
  0xfe, 0xfd, 0xe0, 1,
  0x06, 0xfd, 0x90, 1,
  0xfe, 0x05, 0xf0, 1,
  0x06, 0x05, 0xf1, 1,  
  128
};

const unsigned char const valrigard_flying_right0[] = {
  0xfe, 0xfd, 0x90, 1|OAM_FLIP_H,
  0x06, 0xfd, 0xe0, 1|OAM_FLIP_H,
  0xfe, 0x05, 0xf1, 1|OAM_FLIP_H,
  0x06, 0x05, 0xf0, 1|OAM_FLIP_H,  
  128
};

const unsigned char const valrigard_flying_left1[] = {
  0xfe, 0xfd, 0xe0, 1,
  0x06, 0xfd, 0x91, 1,
  0xfe, 0x05, 0xf0, 1,
  0x06, 0x05, 0xa1, 1,  
  128
};

const unsigned char const valrigard_flying_right1[] = {
  0xfe, 0xfd, 0x91, 1|OAM_FLIP_H,
  0x06, 0xfd, 0xe0, 1|OAM_FLIP_H,
  0xfe, 0x05, 0xa1, 1|OAM_FLIP_H,
  0x06, 0x05, 0xf0, 1|OAM_FLIP_H,  
  128
};

const unsigned char const valrigard_flying_left2[] = {
  0xfe, 0xfd, 0x92, 1,
  0x06, 0xfd, 0x93, 1,
  0xfe, 0x05, 0xa2, 1,
  0x06, 0x05, 0xa3, 1,
  128
};

const unsigned char const valrigard_flying_right2[] = {
  0xfe, 0xfd, 0x93, 1|OAM_FLIP_H,
  0x06, 0xfd, 0x92, 1|OAM_FLIP_H,
  0xfe, 0x05, 0xa3, 1|OAM_FLIP_H,
  0x06, 0x05, 0xa2, 1|OAM_FLIP_H,
  128
};

const unsigned char const valrigard_walking_left0[] = {
  0xfe, 0xfd, 0xe0, 1,
  0x06, 0xfd, 0xe1, 1,
  0xfe, 0x05, 0xb0, 1,
  0x06, 0x05, 0xb1, 1,
  128
};

const unsigned char const valrigard_walking_left1[] = {
  0xfe, 0xfd, 0xe0, 1,
  0x06, 0xfd, 0xe1, 1,
  0xfe, 0x05, 0xc0, 1,
  0x06, 0x05, 0xc1, 1,
  128
};

const unsigned char const valrigard_walking_left2[] = {
  0xfe, 0xfd, 0xe0, 1,
  0x06, 0xfd, 0xe1, 1,
  0xfe, 0x05, 0xb2, 1,
  0x06, 0x05, 0xb3, 1,
  128
};

const unsigned char const valrigard_walking_left3[] = {
  0xfe, 0xfd, 0xe0, 1,
  0x06, 0xfd, 0xe1, 1,
  0xfe, 0x05, 0xc2, 1,
  0x06, 0x05, 0xc3, 1,
  128
};

const unsigned char const valrigard_walking_right0[] = {
  0xfe, 0xfd, 0xe1, 1|OAM_FLIP_H,
  0x06, 0xfd, 0xe0, 1|OAM_FLIP_H,
  0xfe, 0x05, 0xb1, 1|OAM_FLIP_H,
  0x06, 0x05, 0xb0, 1|OAM_FLIP_H,
  128
};

const unsigned char const valrigard_walking_right1[] = {
  0xfe, 0xfd, 0xe1, 1|OAM_FLIP_H,
  0x06, 0xfd, 0xe0, 1|OAM_FLIP_H,
  0xfe, 0x05, 0xc1, 1|OAM_FLIP_H,
  0x06, 0x05, 0xc0, 1|OAM_FLIP_H,
  128
};

const unsigned char const valrigard_walking_right2[] = {
  0xfe, 0xfd, 0xe1, 1|OAM_FLIP_H,
  0x06, 0xfd, 0xe0, 1|OAM_FLIP_H,
  0xfe, 0x05, 0xb3, 1|OAM_FLIP_H,
  0x06, 0x05, 0xb2, 1|OAM_FLIP_H,
  128
};

const unsigned char const valrigard_walking_right3[] = {
  0xfe, 0xfd, 0xe1, 1|OAM_FLIP_H,
  0x06, 0xfd, 0xe0, 1|OAM_FLIP_H,
  0xfe, 0x05, 0xc3, 1|OAM_FLIP_H,
  0x06, 0x05, 0xc2, 1|OAM_FLIP_H,
  128
};


// -- Enemies

// Spikeball 

const unsigned char const spikeball[] = {
      0xff,  0xff,  0x12,3,
         7,  0xff,  0x12,3|OAM_FLIP_H,
      0xff,     7,  0x12,3|OAM_FLIP_V,
         7,     7,  0x12,3|OAM_FLIP_H|OAM_FLIP_V,
    128     
};

// Korbats 

const unsigned char const korbat_left[] = {
      0xff, 0xff, 0x20,2,
      0x07, 0xff, 0x21,2,
      0xff, 0x07, 0x30,2,
      0x07, 0x07, 0x31,2,
  128
};

const unsigned char const korbat_flap1_left[] = {
      0xff, 0xff, 0x20,2,
      0x07, 0xff, 0x22,2,
      0xff, 0x07, 0x30,2,
      0x07, 0x07, 0x31,2,
  128
};

const unsigned char const korbat_flap2_left[] = {
      0xff, 0xff, 0x20,2,
      0x07, 0xff, 0x32,2,
      0xff, 0x07, 0x30,2,
      0x07, 0x07, 0x31,2,
  128
};

const unsigned char const korbat_flap3_left[] = {
      0xff, 0xff, 0x20,2,
      0x07, 0xff, 0x23,2,
      0xff, 0x07, 0x30,2,
      0x07, 0x07, 0x33,2,
  128
};

const unsigned char const korbat_flap4_left[] = {
      0xff, 0xff, 0x20,2,
      0xff, 0x07, 0x30,2,
      0x07, 0x07, 0x24,2,
  128
};

const unsigned char const korbat_flap5_left[] = {
      0xff, 0xff, 0x20,2,
      0xff, 0x07, 0x30,2,
      0x07, 0x07, 0x34,2,
  128
};

const unsigned char const korbat_flap6_left[] = {
      0xff, 0xff, 0x20,2,
      0xff, 0x07, 0x30,2,
      0x07, 0x07, 0x25,2,
  128
};

const unsigned char const korbat_right[] = {
      0x07, 0xff, 0x20,2|OAM_FLIP_H,
      0xff, 0xff, 0x21,2|OAM_FLIP_H,
      0x07, 0x07, 0x30,2|OAM_FLIP_H,
      0xff, 0x07, 0x31,2|OAM_FLIP_H,
  128
};


const unsigned char const korbat_flap1_right[] = {
      0xff, 0xff, 0x22,2|OAM_FLIP_H,
      0x07, 0xff, 0x20,2|OAM_FLIP_H,
      0xff, 0x07, 0x31,2|OAM_FLIP_H,
      0x07, 0x07, 0x30,2|OAM_FLIP_H,
  128
};

const unsigned char const korbat_flap2_right[] = {
      0xff, 0xff, 0x32,2|OAM_FLIP_H,
      0x07, 0xff, 0x20,2|OAM_FLIP_H,
      0xff, 0x07, 0x31,2|OAM_FLIP_H,
      0x07, 0x07, 0x30,2|OAM_FLIP_H,
  128
};

const unsigned char const korbat_flap3_right[] = {
      0xff, 0xff, 0x23,2|OAM_FLIP_H,
      0x07, 0xff, 0x20,2|OAM_FLIP_H,
      0xff, 0x07, 0x33,2|OAM_FLIP_H,
      0x07, 0x07, 0x30,2|OAM_FLIP_H,
  128
};

const unsigned char const korbat_flap4_right[] = {
      0x07, 0xff, 0x20,2|OAM_FLIP_H,
      0x07, 0x07, 0x30,2|OAM_FLIP_H,
      0xff, 0x07, 0x24,2|OAM_FLIP_H,
  128
};

const unsigned char const korbat_flap5_right[] = {
      0x07, 0xff, 0x20,2|OAM_FLIP_H,
      0x07, 0x07, 0x30,2|OAM_FLIP_H,
      0xff, 0x07, 0x34,2|OAM_FLIP_H,
  128
};

const unsigned char const korbat_flap6_right[] = {
      0x07, 0xff, 0x20,2|OAM_FLIP_H,
      0x07, 0x07, 0x30,2|OAM_FLIP_H,
      0xff, 0x07, 0x25,2|OAM_FLIP_H,
  128
};

// Suns

const unsigned char const sun0[] = {
      0x00, 0xff, 0x13,0,
      0x08, 0xff, 0x13,0|OAM_FLIP_H,
      0x00, 0x07, 0x14,0,
      0x08, 0x07, 0x14,0|OAM_FLIP_H,
  128
};

const unsigned char const sun1[] = {
      0x00, 0xff, 0x15,0,
      0x08, 0xff, 0x15,0|OAM_FLIP_H,
      0x00, 0x07, 0x16,0,
      0x08, 0x07, 0x16,0|OAM_FLIP_H,
  128
};

// These are single sprites, so these are just their tile IDs in bank 1.
#define ACIDDROP_SPRITE_OFFSET 0x17
#define CANNONBALL_SPRITE_OFFSET 0x18
#define BOSS_MAGIC_SPRITE_OFFSET 0x80
/*const unsigned char const acid_drop[]= {
      0xff, 0xff, 0x17,3,
  128
};

const unsigned char const cannonball[]= {
      0xff, 0xff, 0x18,2,
  128
};*/

// Cannons

const unsigned char const cannon_up[] = {
      0x00, 0xff, 0x1A,0|OAM_FLIP_V,
      0x08, 0xff, 0x1A,0|OAM_FLIP_H|OAM_FLIP_V,
      0x00, 0x07, 0x19,0|OAM_FLIP_V,
      0x08, 0x07, 0x19,0|OAM_FLIP_V|OAM_FLIP_H,
    128
};

const unsigned char const cannon_up_left[] = {
    0x00, 0xff, 0x1E,0|OAM_FLIP_H|OAM_FLIP_V,
    0x00, 0x07, 0x1D,0|OAM_FLIP_H|OAM_FLIP_V,
    0x08, 0xff, 0x1C,0|OAM_FLIP_H|OAM_FLIP_V,
    0x08, 0x07, 0x1B,0|OAM_FLIP_H|OAM_FLIP_V,
  128
};

const unsigned char const cannon_left[] = {
      0x00, 0xff, 0x2F,0|OAM_FLIP_H,
      0x08, 0xff, 0x1F,0|OAM_FLIP_H,
      0x00, 0x07, 0x2F,0|OAM_FLIP_H|OAM_FLIP_V,
      0x08, 0x07, 0x1F,0|OAM_FLIP_H|OAM_FLIP_V,
    128
};

const unsigned char const cannon_down_left[] = {
      0x00, 0xff, 0x1D,0|OAM_FLIP_H,
      0x08, 0xff, 0x1B,0|OAM_FLIP_H,
      0x00, 0x07, 0x1E,0|OAM_FLIP_H,
      0x08, 0x07, 0x1C,0|OAM_FLIP_H,
    128
};

const unsigned char const cannon_down[] = {
      0x00, 0xff, 0x19,0,
      0x08, 0xff, 0x19,0|OAM_FLIP_H,
      0x00, 0x07, 0x1A,0,
      0x08, 0x07, 0x1A,0|OAM_FLIP_H,
    128
};

const unsigned char const cannon_down_right[] = {
      0x00, 0xff, 0x1B,0,
      0x08, 0xff, 0x1D,0,
      0x00, 0x07, 0x1C,0,
      0x08, 0x07, 0x1E,0,
    128
};

const unsigned char const cannon_right[] = {
      0x00, 0xff, 0x1F,0,
      0x08, 0xff, 0x2F,0,
      0x00, 0x07, 0x1F,0|OAM_FLIP_V,
      0x08, 0x07, 0x2F,0|OAM_FLIP_V,
    128
};

const unsigned char const cannon_up_right[] = {
      0x00, 0xff, 0x1C,0|OAM_FLIP_V,
      0x08, 0xff, 0x1E,0|OAM_FLIP_V,
      0x00, 0x07, 0x1B,0|OAM_FLIP_V,
      0x08, 0x07, 0x1D,0|OAM_FLIP_V,
    128
};

// Grarrls

const unsigned char const grarrl_left[] = {
      0xff, 0xff, 0x40,2,
      0x07, 0xff, 0x41,2,
      0xff, 0x07, 0x50,2,
      0x07, 0x07, 0x51,2,
  128
};

const unsigned char const grarrl_backfoot_step0_left[] = {
      0xff, 0xff, 0x40,2,
      0x07, 0xff, 0x41,2,
      0xff, 0x07, 0x42,2,
      0x07, 0x07, 0x44,2,
  128
};

const unsigned char const grarrl_backfoot_step1_left[] = {
      0xff, 0xff, 0x40,2,
      0x07, 0xff, 0x41,2,
      0xff, 0x07, 0x43,2,
      0x07, 0x07, 0x44,2,
  128
};

const unsigned char const grarrl_frontfoot_step0_left[] = {
      0xff, 0xff, 0x40,2,
      0x07, 0xff, 0x41,2,
      0xff, 0x07, 0x52,2,
      0x07, 0x07, 0x54,2,
  128
};

const unsigned char const grarrl_frontfoot_step1_left[] = {
      0xff, 0xff, 0x40,2,
      0x07, 0xff, 0x41,2,
      0xff, 0x07, 0x53,2,   
      0x07, 0x07, 0x54,2,
  128   
};

const unsigned char const grarrl_right[] = {
      0x07, 0xff, 0x40,2|OAM_FLIP_H,
      0xff, 0xff, 0x41,2|OAM_FLIP_H,
      0x07, 0x07, 0x50,2|OAM_FLIP_H,
      0xff, 0x07, 0x51,2|OAM_FLIP_H,
  128
};

const unsigned char const grarrl_backfoot_step0_right[] = {
      0xff, 0xff, 0x41,2|OAM_FLIP_H,
      0x07, 0xff, 0x40,2|OAM_FLIP_H,
      0xff, 0x07, 0x44,2|OAM_FLIP_H,
      0x07, 0x07, 0x42,2|OAM_FLIP_H,
  128
};

const unsigned char const grarrl_backfoot_step1_right[] = {
      0xff, 0xff, 0x41,2|OAM_FLIP_H,
      0x07, 0xff, 0x40,2|OAM_FLIP_H,
      0xff, 0x07, 0x44,2|OAM_FLIP_H,
      0x07, 0x07, 0x43,2|OAM_FLIP_H,
  128
};

const unsigned char const grarrl_frontfoot_step0_right[] = {
      0xff, 0xff, 0x41,2|OAM_FLIP_H,
      0x07, 0xff, 0x40,2|OAM_FLIP_H,
      0xff, 0x07, 0x54,2|OAM_FLIP_H,
      0x07, 0x07, 0x52,2|OAM_FLIP_H,
  128
};

const unsigned char const grarrl_frontfoot_step1_right[] = {
      0xff, 0xff, 0x41,2|OAM_FLIP_H,
      0x07, 0xff, 0x40,2|OAM_FLIP_H,
      0xff, 0x07, 0x54,2|OAM_FLIP_H,   
      0x07, 0x07, 0x53,2|OAM_FLIP_H,
  128   
};


// Acid Blobs
const unsigned char const acidblob0[] = {
      0x00, 0xff, 0x28,3,
      0x08, 0xff, 0x28,3|OAM_FLIP_H,
  128
};

const unsigned char const acidblob1[] = {
      0x00, 0xff, 0x29,3,
      0x08, 0xff, 0x29,3|OAM_FLIP_H,
  128
};

const unsigned char const acidblob2[] = {
      0x00, 0xff, 0x2A,3,
      0x08, 0xff, 0x2A,3|OAM_FLIP_H,
  128
};

const unsigned char const acidblob3[] = {
      0x00, 0xff, 0x2B,3,
      0x08, 0xff, 0x2B,3|OAM_FLIP_H,
  128
};

// Splykes

const unsigned char const splyke_tornado0[] = {
      0xff, 0xff, 0x60,0,
      0x07, 0xff, 0x61,0,
      0xff, 0x07, 0x70,0,
      0x07, 0x07, 0x71,0,
  128
};

const unsigned char const splyke_tornado1[] = {
      0xff, 0xff, 0x62,0,
      0x07, 0xff, 0x63,0,
      0xff, 0x07, 0x72,0,
      0x07, 0x07, 0x73,0,
  128
};

const unsigned char const splyke_tornado2[] = {
      0xff, 0xff, 0x61,0|OAM_FLIP_H,
      0x07, 0xff, 0x60,0|OAM_FLIP_H,
      0xff, 0x07, 0x71,0|OAM_FLIP_H,
      0x07, 0x07, 0x70,0|OAM_FLIP_H,
  128 
};

const unsigned char const splyke_idle_left0[] = {
      0xff, 0xff, 0x64,0,
      0x07, 0xff, 0x65,0,
      0xff, 0x07, 0x74,0,
      0x07, 0x07, 0x75,0,
  128
};

const unsigned char const splyke_idle_left1[] = {
      0xff, 0xff, 0x64,0,
      0x07, 0xff, 0x65,0,
      0xff, 0x07, 0x76,0,
      0x07, 0x07, 0x75,0,
  128 
};

const unsigned char const splyke_idle_right0[] = {
      0xff, 0xff, 0x65,0|OAM_FLIP_H,
      0x07, 0xff, 0x64,0|OAM_FLIP_H,
      0xff, 0x07, 0x75,0|OAM_FLIP_H,
      0x07, 0x07, 0x74,0|OAM_FLIP_H,
  128
};

const unsigned char const splyke_idle_right1[] = {
      0xff, 0xff, 0x65,0|OAM_FLIP_H,
      0x07, 0xff, 0x64,0|OAM_FLIP_H,
      0xff, 0x07, 0x75,0|OAM_FLIP_H,
      0x07, 0x07, 0x76,0|OAM_FLIP_H,
  128 
};


// Boss

const unsigned char const boss_head_eye_open_mouth_closed_left[] = {
  0xfc, 0xfe, 0x3B,0, // Head
  0x04, 0xfe, 0x3C,0,
  0xff, 0x06, 0x4D,0,
  128
};

const unsigned char const boss_head_eye_open_mouth_closed_right[] = {
  0x0c, 0xfe, 0x3B,0|OAM_FLIP_H, // Head
  0x04, 0xfe, 0x3C,0|OAM_FLIP_H,
  0x09, 0x06, 0x4D,0|OAM_FLIP_H,
  128
};

const unsigned char const boss_head_eye_closed_mouth_closed_left[] = {
  0xfc, 0xfe, 0x3B,0, // Head
  0x04, 0xfe, 0x3D,0,
  0xff, 0x06, 0x4D,0,
  128
};

const unsigned char const boss_head_eye_closed_mouth_closed_right[] = {
  0x0c, 0xfe, 0x3B,0|OAM_FLIP_H, // Head
  0x04, 0xfe, 0x3D,0|OAM_FLIP_H,
  0x09, 0x06, 0x4D,0|OAM_FLIP_H,
  128
};

const unsigned char const boss_body_idle_left0[] = {
  0x00, 0x00, 0x3E,2, // Body
  0x08, 0x00, 0x3F,2,
  0x00, 0x08, 0x4E,2,
  0x08, 0x08, 0x4F,2,
  128
};

const unsigned char const boss_body_idle_right0[] = {
  0x08, 0x00, 0x3E,2|OAM_FLIP_H, // Body
  0x00, 0x00, 0x3F,2|OAM_FLIP_H,
  0x08, 0x08, 0x4E,2|OAM_FLIP_H,
  0x00, 0x08, 0x4F,2|OAM_FLIP_H,
  128
};

const unsigned char const boss_body_flying_left0[] = {
  0x00, 0x00, 0x5E,2,
  0x08, 0x00, 0x5F,2,
  0x00, 0x08, 0x6E,2,
  0x08, 0x08, 0x6E,2|OAM_FLIP_H,
  128
};

const unsigned char const boss_body_flying_right0[] = {
  0x08, 0x00, 0x5E,2|OAM_FLIP_H,
  0x00, 0x00, 0x5F,2|OAM_FLIP_H,
  0x08, 0x08, 0x6E,2|OAM_FLIP_H,
  0x00, 0x08, 0x6E,2,
  128
};

const unsigned char const boss_body_flying_left1[] = {
  0x00, 0x00, 0x5E,2,
  0x08, 0x00, 0x5F,2,
  0x00, 0x08, 0x6D,2,
  0x08, 0x08, 0x6F,2|OAM_FLIP_H,
  128
};

const unsigned char const boss_body_flying_right1[] = {
  0x08, 0x00, 0x5E,2|OAM_FLIP_H,
  0x00, 0x00, 0x5F,2|OAM_FLIP_H,
  0x08, 0x08, 0x6D,2|OAM_FLIP_H,
  0x00, 0x08, 0x6F,2,
  128
};

const unsigned char const boss_body_flying_left2[] = {
  0x00, 0x00, 0x5E,2,
  0x08, 0x00, 0x5F,2,
  0x00, 0x08, 0x6F,2,
  0x08, 0x08, 0x6D,2|OAM_FLIP_H,
  128
};

const unsigned char const boss_body_flying_right2[] = {
  0x08, 0x00, 0x5E,2|OAM_FLIP_H,
  0x00, 0x00, 0x5F,2|OAM_FLIP_H,
  0x08, 0x08, 0x6F,2|OAM_FLIP_H,
  0x00, 0x08, 0x6D,2,
  128
};

const unsigned char const boss_body_flying_left3[] = {
  0x00, 0x00, 0x5E,2,
  0x08, 0x00, 0x5F,2,
  0x00, 0x08, 0x7F,2,
  0x08, 0x08, 0x7F,2|OAM_FLIP_H,
  128
};

const unsigned char const boss_body_flying_right3[] = {
  0x08, 0x00, 0x5E,2|OAM_FLIP_H,
  0x00, 0x00, 0x5F,2|OAM_FLIP_H,
  0x08, 0x08, 0x7F,2|OAM_FLIP_H,
  0x00, 0x08, 0x7F,2,
  128
};

// Enemy death effects

const unsigned char const purple_death_effect0[] = {
      0xff, 0xff, 0x2C,2,
      0x07, 0xff, 0x2C,2|OAM_FLIP_H,
      0xff, 0x07, 0x2C,2|OAM_FLIP_V,
      0x07, 0x07, 0x2C,2|OAM_FLIP_H|OAM_FLIP_V,
  128
};

const unsigned char const purple_death_effect1[] = {
      0xff, 0xff, 0x2D,2,
      0x07, 0xff, 0x2D,2|OAM_FLIP_H,
      0xff, 0x07, 0x2D,2|OAM_FLIP_V,
      0x07, 0x07, 0x2D,2|OAM_FLIP_H|OAM_FLIP_V,
  128
};

// only used in the boss's death animation
const unsigned char const purple_death_effect2[] = {
      0xff, 0xff, 0x2E,2,
      0x07, 0xff, 0x2E,2|OAM_FLIP_H,
      0xff, 0x07, 0x2E,2|OAM_FLIP_V,
      0x07, 0x07, 0x2E,2|OAM_FLIP_H|OAM_FLIP_V,
  128
};

const unsigned char const splyke_death_effect0[] = {
      0xff, 0xff, 0x2C,0,
      0x07, 0xff, 0x2C,0|OAM_FLIP_H,
      0xff, 0x07, 0x2C,0|OAM_FLIP_V,
      0x07, 0x07, 0x2C,0|OAM_FLIP_H|OAM_FLIP_V,
  128
};

const unsigned char const splyke_death_effect1[] = {
      0xff, 0xff, 0x2E,0,
      0x07, 0xff, 0x2E,0|OAM_FLIP_H,
      0xff, 0x07, 0x2E,0|OAM_FLIP_V,
      0x07, 0x07, 0x2E,0|OAM_FLIP_H|OAM_FLIP_V,
  128
};

// Energy bars.
const unsigned char const energy_bar_0[] = {
    0, 3, 0x0A, 1|OAM_FLIP_V,
    0, 8, 0x0A, 1,
    0, 16, 0x0A, 1,
    0, 24, 0x0A, 1, 
  128
};

const unsigned char const energy_bar_1[] = {
    0, 3, 0x0A, 1|OAM_FLIP_V,
    0, 8, 0x0A, 1,
    0, 16, 0x0A, 1,
    0, 24, 0x0B, 1, 
  128
};

const unsigned char const energy_bar_2[] = {
    0, 3, 0x0A, 1|OAM_FLIP_V,
    0, 8, 0x0A, 1,
    0, 16, 0x0A, 1,
    0, 24, 0x0C, 1, 
  128
};

const unsigned char const energy_bar_3[] = {
    0, 3, 0x0A, 1|OAM_FLIP_V,
    0, 8, 0x0A, 1,
    0, 16, 0x0A, 1,
    0, 24, 0x0D, 1, 
  128
};

const unsigned char const energy_bar_4[] = {
    0, 3, 0x0A, 1|OAM_FLIP_V,
    0, 8, 0x0A, 1,
    0, 16, 0x0F, 1,
    0, 24, 0x0E, 1, 
  128
};

const unsigned char const energy_bar_5[] = {
    0, 3, 0x0A, 1|OAM_FLIP_V,
    0, 8, 0x0A, 1,
    0, 16, 0x0B, 1,
    0, 24, 0x0E, 1, 
  128
};

const unsigned char const energy_bar_6[] = {
    0, 3, 0x0A, 1|OAM_FLIP_V,
    0, 8, 0x0A, 1,
    0, 16, 0x0C, 1,
    0, 24, 0x0E, 1, 
  128
};

const unsigned char const energy_bar_7[] = {
    0, 3, 0x0A, 1|OAM_FLIP_V,
    0, 8, 0x0A, 1,
    0, 16, 0x0D, 1,
    0, 24, 0x0E, 1, 
  128
};

const unsigned char const energy_bar_8[] = {
    0, 3, 0x0A, 1|OAM_FLIP_V,
    0, 8, 0x0F, 1,
    0, 16, 0x0E, 1,
    0, 24, 0x0E, 1, 
  128
};

const unsigned char const energy_bar_9[] = {
    0, 3, 0x0A, 1|OAM_FLIP_V,
    0, 8, 0x0B, 1,
    0, 16, 0x0E, 1,
    0, 24, 0x0E, 1, 
  128
};

const unsigned char const energy_bar_a[] = {
    0, 3, 0x0A, 1|OAM_FLIP_V,
    0, 8, 0x0C, 1,
    0, 16, 0x0E, 1,
    0, 24, 0x0E, 1, 
  128
};

const unsigned char const energy_bar_b[] = {
    0, 3,  0xFF, 1,
    0, 4,  0x0B, 1,
    0, 8,  0x0D, 1,
    0, 16, 0x0E, 1,
    0, 24, 0x0E, 1, 
  128
};

const unsigned char const energy_bar_c[] = {
    0, 3,  0xFF, 1,
    0, 4,  0x0C, 1,
    0, 8,  0x0E, 1,
    0, 16, 0x0E, 1,
    0, 24, 0x0E, 1, 
  128
};

const unsigned char const energy_bar_d[] = {
    0, 3,  0xFF, 1,
    0, 4,  0x0D, 1,
    0, 8,  0x0E, 1,
    0, 16, 0x0E, 1,
    0, 24, 0x0E, 1, 
  128
};

const unsigned char const energy_bar_e[] = {
    0, 3,  0x0E, 1|OAM_FLIP_V,
    0, 8,  0x0E, 1,
    0, 16, 0x0E, 1,
    0, 24, 0x0E, 1, 
  128
};

const unsigned char const energy_bar_f[] = {
    0, 1,  0x0E, 1|OAM_FLIP_V,
    0, 8,  0x0E, 1,
    0, 16, 0x0E, 1,
    0, 24, 0x0E, 1, 
  128
};

const unsigned char const points_1[] = {
     0,-24, 0x01, 3,
  128
};

const unsigned char const points_2[] = {
     0,-24, 0x02, 3,
  128
};

const unsigned char const points_3[] = {
     0,-24, 0x03, 3,
  128
};

const unsigned char const points_4[] = {
     0,-24, 0x04, 3,
  128
};

const unsigned char const points_100[] = {
  0xf8,-24, 0x01, 3,
     0,-24, 0x00, 3,
     8,-24, 0x00, 3,
  128
};

const unsigned char const paused_text[] = {
    0,  0, 0xF9, 1,
    8,  0, 0xFA, 1,
   16,  0, 0xFB, 1,
   24,  0, 0xFC, 1,
   32,  0, 0xFD, 1,
   40,  0, 0xFE, 1,
  128
};

#pragma rodata-name(pop)