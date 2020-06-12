//
//  metasprites.c
//  Castle Escape
//
//  Created by Zaccari Silverman on 6/11/20.
//

const unsigned char valrigardIdleLeft[]={
    - 8,-16,0xe0,1,
      0,-16,0xe1,1,
    - 8,- 8,0xf0,1,
      0,- 8,0xf1,1,
    128
};


// NESDoug's sample sprites(stars)
const unsigned char metasprite[]={
      0,  0,0x01,0,
      0,  8,0x11,0,
      8,  0,0x01,0|OAM_FLIP_H,
      8,  8,0x11,0|OAM_FLIP_H,
    128
};

const unsigned char metasprite2[]={
      8,  0,0x03,0,
      0,  8,0x12,0,
      8,  8,0x13,0,
     16,  8,0x12,0|OAM_FLIP_H,
      0, 16,0x22,0,
      8, 16,0x23,0,
     16, 16,0x22,0|OAM_FLIP_H,
    128
};
