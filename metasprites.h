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
const unsigned char const valrigardIdleLeft[] = {
      0xfe, 0xfd,   0xe0,   1,
      6,    0xfd,   0xe1,   1,
      0xfe,    5,   0xf0,   1,
      6,       5,   0xf1,   1,
    128
};

const unsigned char const valrigardIdleRight[] = {
         6,  0xfd,  0xe0,1|OAM_FLIP_H,
      0xfe,  0xfd,  0xe1,1|OAM_FLIP_H,
         6,     5,  0xf0,1|OAM_FLIP_H,
      0xfe,     5,  0xf1,1|OAM_FLIP_H,
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
