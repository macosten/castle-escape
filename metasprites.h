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
// 128 terminates (I think).
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
      0xfe, 0xfd,   0xe7,   1,
      6,    0xfd,   0xe8,   1,
      0xfe,    5,   0xf7,   1,
      6,       5,   0xf8,   1,
    128
};

const unsigned char const valrigard_swing_right_followthrough[] = {
      6,    0xfd,   0xe7,   1|OAM_FLIP_H,
      0xfe, 0xfd,   0xe8,   1|OAM_FLIP_H,
      6,       5,   0xf7,   1|OAM_FLIP_H,
      0xfe,    5,   0xf8,   1|OAM_FLIP_H,
    128
};

const unsigned char const spikeball[] = {
      0xff,  0xff,  0x12,3,
         7,  0xff,  0x12,3|OAM_FLIP_H,
      0xff,     7,  0x12,3|OAM_FLIP_V,
         7,     7,  0x12,3|OAM_FLIP_H|OAM_FLIP_V,
    128     
};

const unsigned char const korbat_left[] = {
      0xff, 0xff, 0x20,2,
      0x07, 0xff, 0x21,2,
      0xff, 0x07, 0x30,2,
      0x07, 0x07, 0x31,2,
  128
};

const unsigned char const korbat_right[] = {
      0x07, 0xff, 0x20,2|OAM_FLIP_H,
      0xff, 0xff, 0x21,2|OAM_FLIP_H,
      0x07, 0x07, 0x30,2|OAM_FLIP_H,
      0xff, 0x07, 0x31,2|OAM_FLIP_H,
  128
};

const unsigned char const sun0[] = {
      0xff, 0xff, 0x13,0,
      0x07, 0xff, 0x13,0|OAM_FLIP_H,
      0xff, 0x07, 0x14,0,
      0x07, 0x07, 0x14,0|OAM_FLIP_H,
  128
};

const unsigned char const sun1[] = {
      0xff, 0xff, 0x15,0,
      0x07, 0xff, 0x15,0|OAM_FLIP_H,
      0xff, 0x07, 0x16,0,
      0x07, 0x07, 0x16,0|OAM_FLIP_H,
  128
};

// These are single sprites, so these are just their tile IDs in bank 1.
#define ACIDDROP_SPRITE_OFFSET 0x17
#define CANNONBALL_SPRITE_OFFSET 0x18
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
      0xff, 0xff, 0x1A,0|OAM_FLIP_V,
      0x07, 0xff, 0x1A,0|OAM_FLIP_H|OAM_FLIP_V,
      0xff, 0x07, 0x19,0|OAM_FLIP_V,
      0x07, 0x07, 0x19,0|OAM_FLIP_V|OAM_FLIP_H,
    128
};

const unsigned char const cannon_up_left[] = {
    0xff, 0xff, 0x1E,0|OAM_FLIP_H|OAM_FLIP_V,
    0xff, 0x07, 0x1D,0|OAM_FLIP_H|OAM_FLIP_V,
    0x07, 0xff, 0x1C,0|OAM_FLIP_H|OAM_FLIP_V,
    0x07, 0x07, 0x1B,0|OAM_FLIP_H|OAM_FLIP_V,
  128
};

const unsigned char const cannon_left[] = {
      0xff, 0xff, 0x2F,0|OAM_FLIP_H,
      0x07, 0xff, 0x1F,0|OAM_FLIP_H,
      0xff, 0x07, 0x2F,0|OAM_FLIP_H|OAM_FLIP_V,
      0x07, 0x07, 0x1F,0|OAM_FLIP_H|OAM_FLIP_V,
    128
};

const unsigned char const cannon_down_left[] = {
      0xff, 0xff, 0x1D,0|OAM_FLIP_H,
      0x07, 0xff, 0x1B,0|OAM_FLIP_H,
      0xff, 0x07, 0x1E,0|OAM_FLIP_H,
      0x07, 0x07, 0x1C,0|OAM_FLIP_H,
    128
};

const unsigned char const cannon_down[] = {
      0xff, 0xff, 0x19,0,
      0x07, 0xff, 0x19,0|OAM_FLIP_H,
      0xff, 0x07, 0x1A,0,
      0x07, 0x07, 0x1A,0|OAM_FLIP_H,
    128
};

const unsigned char const cannon_down_right[] = {
      0xff, 0xff, 0x1B,0,
      0x07, 0xff, 0x1D,0,
      0xff, 0x07, 0x1C,0,
      0x07, 0x07, 0x1E,0,
    128
};

const unsigned char const cannon_right[] = {
      0xff, 0xff, 0x1F,0,
      0x07, 0xff, 0x2F,0,
      0xff, 0x07, 0x1F,0|OAM_FLIP_V,
      0x07, 0x07, 0x2F,0|OAM_FLIP_V,
    128
};

const unsigned char const cannon_up_right[] = {
      0xff, 0xff, 0x1C,0|OAM_FLIP_V,
      0x07, 0xff, 0x1E,0|OAM_FLIP_V,
      0xff, 0x07, 0x1B,0|OAM_FLIP_V,
      0x07, 0x07, 0x1D,0|OAM_FLIP_V,
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

const unsigned char const grarrl_right[] = {
      0x07, 0xff, 0x40,2|OAM_FLIP_H,
      0xff, 0xff, 0x41,2|OAM_FLIP_H,
      0x07, 0x07, 0x50,2|OAM_FLIP_H,
      0xff, 0x07, 0x51,2|OAM_FLIP_H,
  128
};

// Acid Blobs
const unsigned char const acidblob0[] = {
      0xff, 0xff, 0x28,3,
      0x07, 0xff, 0x28,3|OAM_FLIP_H,
  128
};

const unsigned char const acidblob1[] = {
      0xff, 0xff, 0x29,3,
      0x07, 0xff, 0x29,3|OAM_FLIP_H,
  128
};

const unsigned char const acidblob2[] = {
      0xff, 0xff, 0x2A,3,
      0x07, 0xff, 0x2A,3|OAM_FLIP_H,
  128
};

const unsigned char const acidblob3[] = {
      0xff, 0xff, 0x2B,3,
      0x07, 0xff, 0x2B,3|OAM_FLIP_H,
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
