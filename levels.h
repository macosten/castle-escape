//
//  levels.h
//  Castle Escape
//
//  Created by Zaccari Silverman on 6/17/20.
//

#include "tilemaps/level01.h"
#include "tilemaps/level02.h"
#include "tilemaps/level03.h"
#include "tilemaps/level04.h"
#include "tilemaps/level05.h"
#include "tilemaps/level06.h"
#include "tilemaps/level07.h"
#include "tilemaps/level08.h"
#include "tilemaps/level10.h"
#include "tilemaps/level_debug_tiledump.h"
#include "tilemaps/level_star_test.h"
#include "tilemaps/level_enemies_test.h"

// Some of this stuff might be able to live in another PRG bank if bank 7 gets too full.

// Length of a level in nametables.
const unsigned char const level_nt_length[] = {
    3, 
    3,
    3,
    3, 
    3, // Level 05
    3,
    3,
    3, // Level 08, 9 will be longer
    3, // Level 10
    1, // tiledump
    3, // star test
    3, // enemies test
};

const unsigned char const valrigard_starting_nt[] = {
    2,
    2, // The index of the nametable Valrigard starts in.
    2,
    2, 
    2, // Level 05
    2,
    2,
    2, // Level 08
    2, // Level 10
    0, // Tiledump
    0, // Star Test
    2,
};

// It's awfully convenient that nametables are no more than 16 metatiles in dimension
// This means we can just use nibbles to encode the coordinates at which valrigard starts on a level.

const unsigned char const valrigard_inital_coords[] = {
    0x4d, // x,y; 0x4d -> (4 [0x4], 13 [0xd] - starts at that metatile)
    0x4d, 
    0x4d,
    0x4d,
    0x4d,
    0x4d,
    0x4d,
    0x4d, // 8
    0x4d, // 10
    0x4d,
    0x30,
    0x4d,
};

// Pointers to compressed level data.
const unsigned char * const level_compressed_nametable_pointers[] = {
    level01,
    level02,
    level03,
    level04,
    level05,
    level06,
    level07,
    level08,
    level10, 
    level_debug_tiledump,
    level_star_test,
    level_enemies_test,
};

// The banks in which each level's data is actually located.
const unsigned char const level_nametable_banks[] = {
    0,
    0, 
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
};

const char * const level_names[] = {
    "Level  1 ",
    "Level  2 ",
    "Level  3 ",
    "Level  4 ",
    "Level  5 ",
    "Level  6 ",
    "Level  7 ",
    "Level  8 ",
    "Level 10 ",
    "Tile Dump",
    "Star Test",
    "EnemyTest",
};

// Convenient ROM value that shows the number of levels implemented.
#define NUMBER_OF_LEVELS 12

// Max of 32 enemies.
// Be aware that cannons and acid blobs functionally take up 2 slots
// because they also have a projectile loaded directly after them in RAM.
// This also means you should prevent either of these things from being 
// loaded into the MAX_ENEMIES-th slot, since the projectile will be
// loaded into RAM that overlaps with other RAM.
const unsigned char * const level_enemy_data[] = {
    level01_enemy,
    level02_enemy,
    level03_enemy,
    level04_enemy,
    level05_enemy,
    level06_enemy,
    level07_enemy,
    level08_enemy,
    level10_enemy,
    level_debug_tiledump_enemy,
    level_star_test_enemy,
    level_enemies_test_enemy,
};

