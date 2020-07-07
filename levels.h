//
//  levels.h
//  Castle Escape
//
//  Created by Zaccari Silverman on 6/17/20.
//

#include "tilemaps/level10.c"

const unsigned char const level_starting_nt[] = {
    0, 3 // Test level starts a nt 0 and is 3 nt long (incidentally, the next level would start at nt 3... etc)
};

const unsigned char const valrigard_starting_nt[] = {
    2 // The index of the nametable Valrigard starts in.
};

// It's awfully convenient that nametables are no more than 16 metatiles in dimension
// This means we can just use nibbles to encode the coordinates at which valrigard starts on a level.

const unsigned char const valrigard_inital_coords[] = {
    0x4d // x,y; 0x4d -> (4 [0x4], 13 [0xd])
};

const unsigned char * const level_nametables[] = {
    level10_0, level10_1, level10_2,
    level10_0 // a buffer -- this prevents graphical glitching when at the bottom of a level
};

const unsigned char * const level_names[] = {
    "Mockup Level 10",
    "Null"
};

/* Notes related to storing initial state of objects:
 There is OBJECT_BITFIELD which will allow up to 255 destructible objects (stars, killable enemies, etc) to have their states kept track of.
 If need be, perhaps we can expand this to allow for a separate STAR_BITFIELD and ENEMY_BITFIELD, but I'm not sure we'll need more than 255 things in a (non user-designed) level.
 
 What if we store objects like this:
    0xAABC 0xXY
 
 Where:
    A is a nametable ID - 1 nt - the nametable in which
*/
