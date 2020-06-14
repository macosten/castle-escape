//
//  metasprites.h
//  Castle Escape
//
//  Created by Zaccari Silverman on 6/11/20.
//

const unsigned char valrigardIdleLeft[]={
      0,  0,0xe0,1,
      8,  0,0xe1,1,
      0,  8,0xf0,1,
      8,  8,0xf1,1,
    128
};

const unsigned char valrigardIdleRight[]={
      8,  0,0xe0,1|OAM_FLIP_H,
      0,  0,0xe1,1|OAM_FLIP_H,
      8,  8,0xf0,1|OAM_FLIP_H,
      0,  8,0xf1,1|OAM_FLIP_H,
    128
};



const unsigned char valrigardIdleLeftAlternate[]={
        0,  0,0xe0,0,
        8,  0,0xe1,0,
        0,  8,0xf0,0,
        8,  8,0xf1,0,
    128
};
