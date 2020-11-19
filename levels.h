//
//  levels.h
//  Castle Escape
//
//  Created by Zaccari Silverman on 6/17/20.
//

#include "tilemaps/level10.h"
#include "tilemaps/level_debug_tiledump.h"

// Some of this stuff might be able to live in another PRG bank if bank 7 gets too full.

// Length of a level in nametables.
const unsigned char const level_nt_length[] = {
    3,
    1,
};

const unsigned char const valrigard_starting_nt[] = {
    2, // The index of the nametable Valrigard starts in.
    0,
};

// It's awfully convenient that nametables are no more than 16 metatiles in dimension
// This means we can just use nibbles to encode the coordinates at which valrigard starts on a level.

const unsigned char const valrigard_inital_coords[] = {
    0x4d, // x,y; 0x4d -> (4 [0x4], 13 [0xd] - starts at that metatile)
    0x4d,
};

// Pointers to compressed level data.
const unsigned char * const level_compressed_nametable_pointers[] = {
    level10_compressed, 
    level_debug_tiledump
};

// The banks in which each level's data is actually located.
const unsigned char const level_nametable_banks[] = {
    0, 
    0,
};

const char * const level_names[] = {
    "Fake Level 10   ",
    "Debug: Tile Dump",
};

// Convenient ROM value that shows the number of levels implemented.
#define NUMBER_OF_LEVELS 2

// Max of 32 enemies.
// Be aware that cannons and acid blobs functionally take up 2 slots
// because they also have a projectile loaded directly after them in RAM.
// This also means you should prevent either of these things from being 
// loaded into the MAX_ENEMIES-th slot, since the projectile will be
// loaded into RAM that overlaps with other RAM.
const unsigned char * const level_enemy_data[] = {
    level10_enemy,
    level_debug_tiledump_enemy,
};

