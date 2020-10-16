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
    0x4d // x,y; 0x4d -> (4 [0x4], 13 [0xd] - starts at that metatile)
};

const unsigned char * const level_nametables[] = {
    level10_0, level10_1, level10_2,
 };

const unsigned char * const level_names[] = {
    "Mockup Level 10",
    "Null"
};

// Max of 32 enemies.
// Be aware that cannons and acid blobs functionally take up 2 slots
// because they also have a projectile loaded directly after them in RAM.
// This also means you should prevent either of these things from being 
// loaded into the MAX_ENEMIES-th slot, since the projectile will be
// loaded into RAM that overlaps with other RAM.
const unsigned char * const level_enemy_data[] = {
    level10_enemy
};