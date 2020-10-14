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
    level10_0 // a buffer -- this prevents graphical glitching when at the bottom of a level
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

//
const unsigned char const bitpacked_coord_lookup_table[] = {
    0,1,2,3,4,5,6,7,8,9,10,11,0xff,0xff,0xff,0xff,
    12,13,14,15,16,17,18,19,20,21,22,23,0xff,0xff,0xff,0xff,
    24,25,26,27,28,29,30,31,32,33,34,35,0xff,0xff,0xff,0xff,
    36,37,38,39,40,41,42,43,44,45,46,47,0xff,0xff,0xff,0xff,
    48,49,50,51,52,53,54,55,56,57,58,59,0xff,0xff,0xff,0xff,
    60,61,62,63,64,65,66,67,68,69,70,71,0xff,0xff,0xff,0xff,
    72,73,74,75,76,77,78,79,80,81,82,83,0xff,0xff,0xff,0xff,
    84,85,86,87,88,89,90,91,92,93,94,95,0xff,0xff,0xff,0xff,
    96,97,98,99,100,101,102,103,104,105,106,107,0xff,0xff,0xff,0xff,
    108,109,110,111,112,113,114,115,116,117,118,119,0xff,0xff,0xff,0xff,
    120,121,122,123,124,125,126,127,128,129,130,131,0xff,0xff,0xff,0xff,
    132,133,134,135,136,137,138,139,140,141,142,143,0xff,0xff,0xff,0xff,
    144,145,146,147,148,149,150,151,152,153,154,155,0xff,0xff,0xff,0xff,
    156,157,158,159,160,161,162,163,164,165,166,167,0xff,0xff,0xff,0xff,
    168,169,170,171,172,173,174,175,176,177,178,179,0xff,0xff,0xff,0xff,
};

/* Notes related to storing initial state of objects:
 There is OBJECT_BITFIELD which will allow up to 255 destructible objects (stars, killable enemies, etc) to have their states kept track of.
 If need be, perhaps we can expand this to allow for a separate STAR_BITFIELD and ENEMY_BITFIELD, but I'm not sure we'll need more than 255 things in a (non user-designed) level.
 
 We shouldn't really need to store what the object is in the data since there will be a source tile at that area (a star, an Enemy Source Tile, etc)
 
 What if we store objects like this:
    0xAAXY
 
 Where (1 letter = 1 hex digit/4 bits):
    AA is a nametable ID - 8 bits - the nametable in which this object lives.
    X is its X coordinate (metatile)
    Y is its Y coordinate (metatile)
 
 Or perhaps we could store objects in two tables:
    Table 1 could be the number of objects in each nametable, and
    Table 2 could be the coordinates of each object (0xXY).
 
 When copying cmaps to memory, we will check to see if an object should be loaded or if it should be replaced with an empty tile.
 
 We can store compacted coordinates of objects somewhere in RAM in a hash map of some sort...
 
 This also means that an enemy in RAM should keep track of what bit in the OBJECT_BITFIELD it's supposed to modify.
*/
