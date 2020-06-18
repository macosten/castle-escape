//
//  metasprites.h
//  Castle Escape
//
//  Created by Zaccari Silverman on 6/11/20.
//

// (0, 8), but since Valrigard's hitbox is 11 wide and 13 tall,
// we'll center it by shifting his sprite left by two pixels and up by one.

// Each line corresponds to one hardware sprite, and the following information is in each line:
// X offset, Y offset, sprite/tile ID, palette
// 128 terminates (I think).
const unsigned char valrigardIdleLeft[]={
      0xfe, 0xfd,   0xe0,   1,
      6,    0xfd,   0xe1,   1,
      0xfe,    5,   0xf0,   1,
      6,       5,   0xf1,   1,
    128
};

const unsigned char valrigardIdleRight[]={
         6,  0xfd,  0xe0,1|OAM_FLIP_H,
      0xfe,  0xfd,  0xe1,1|OAM_FLIP_H,
         6,     5,  0xf0,1|OAM_FLIP_H,
      0xfe,     5,  0xf1,1|OAM_FLIP_H,
    128
};
