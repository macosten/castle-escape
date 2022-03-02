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
#include "tilemaps/level09.h"
#include "tilemaps/level10.h"
#include "tilemaps/level11.h"
#include "tilemaps/level12.h"
#include "tilemaps/level13.h"
#include "tilemaps/level14.h"
#include "tilemaps/level15.h"
#include "tilemaps/level16.h"
#include "tilemaps/level17.h"
#include "tilemaps/level18.h"
#include "tilemaps/level19.h"
#include "tilemaps/level20.h"

#include "tilemaps/level_user_jack_marek.h"
#include "tilemaps/level_user_neoman17777.h"
#include "tilemaps/level_user_thomas2070.h"

#include "tilemaps/level_debug_tiledump.h"
#include "tilemaps/level_star_test.h"
#include "tilemaps/level_enemies_test.h"
#include "tilemaps/level_question_block_test.h"
#include "tilemaps/level_downloader_test.h"
#include "tilemaps/level_screen_edge_test.h"

#include "tilemaps/level_bonus01.h"
#include "tilemaps/level_bonus02.h"

// Some of this stuff might be able to live in another PRG bank if bank 7 gets too full.
#define LEVEL_METADATA_BANK 0
#define BONUS_LEVEL_DATA_BANK 0

#pragma rodata-name(push, "BANK0")

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
    6,
    3, // Level 10
    6, // Level 11
    6,
    3, // Level 13
    2,
    6, // 15
    1, // Level 16
    3,
    1, // Level 18
    6,
    1, // Level 20
    // User Levels
    6,
    6,
    6,

    // Debug Levels
    1, // tiledump
    3, // star test
    3, // enemies test
    1,
    6, // downloader test
    1,
    
    // Bonus Levels
    3,
    3,
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
    5,
    2, // Level 10
    5, 
    5,
    2,
    1, // Level 14
    5, // 15
    0, // Level 16
    2,
    0, // Level 18
    0,
    0,
    // User Levels
    5,
    5,
    5,

    // Debug Levels
    0, // Tiledump
    0, // Star Test
    2,
    0,
    5,
    0,
    
    // Bonus Levels
    0,
    0,
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
    0x4d,
    0x4d, // 10
    0x4d, // 11
    0x4d, // 12
    0x4d,
    0x4d, // 14
    0x4d, // 15
    0x4a, // 16
    0x4d,
    0x4d,
    0x64,
    0x4c, // 20
    // User Levels
    0x4d,
    0x4d,
    0x4d,

    // Debug Levels
    0x4d,
    0x30,
    0x4d,
    0x4d,
    0x4d,
    0x4d,
    
    // Bonus Levels
    0x21,
    0x21,
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
    level09,
    level10, 
    level11,
    level12,
    level13,
    level14,
    level15,
    level16,
    level17,
    level18,
    level19,
    level20,

    level_jack_marek,
    level_neoman17777,
    level_thomas2070,

    level_debug_tiledump,
    level_star_test,
    level_enemies_test,
    level_question_block_test,
    level_macosten,
    level_screen_edge_test,
    
    level_bonus01,
    level_bonus02,
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
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    // User Levels
    0,
    0,
    0,

    // Debug Levels
    0,
    0,
    0,
    0,
    0,
    0,
    
    // Bonus Levels
    0,
    0,
};

const char * const level_names[] = {
    "Level 1",
    "Level 2",
    "Level 3",
    "Level 4",
    "Level 5",
    "Level 6",
    "Level 7",
    "Level 8",
    "Level 9",
    "Level 10",
    "Level 11",
    "Level 12",
    "Level 13",
    "Level 14",
    "Level 15",
    "Level 16",
    "Level 17",
    "Level 18",
    "Level 19",
    "Level 20",

    "User: jack_marek",
    "User: neoman17777",
    "User: thomas2070",

    "Tile Dump",
    "Star Test",
    "Enemy Test",
    "? Block Test",
    "Downloader Test",
    "Screen Edge Test",
    
    "Bonus Level 1",
    "Bonus Level 2",
};

// Convenient ROM value that shows the number of levels implemented.
#define NUMBER_OF_LEVELS 31 // Includes bonus levels
#define NUMBER_OF_BONUS_LEVELS 2 // The last X levels will be considered bonus levels.

// Max of MAX_ENEMIES enemies.
// Be aware that cannons and acid blobs functionally take up 2 slots
// because they also have a projectile loaded directly after them in RAM.
// This also means you should prevent either of these things from being 
// loaded into the MAX_ENEMIES-th slot, since the projectile will be
// loaded into RAM that overlaps with other RAM.

const unsigned char const empty_enemy[]={ 0xFF };

const unsigned char * const level_enemy_data[] = {
    level01_enemy,
    level02_enemy,
    level03_enemy,
    level04_enemy,
    level05_enemy,
    level06_enemy,
    level07_enemy,
    level08_enemy,
    level09_enemy,
    level10_enemy,
    level11_enemy,
    level12_enemy,
    level13_enemy,
    level14_enemy,
    level15_enemy,
    level16_enemy,
    level17_enemy,
    level18_enemy,
    empty_enemy, // 19
    level20_enemy,

    level_jack_marek_enemy,
    level_neoman17777_enemy,
    level_thomas2070_enemy,

    level_debug_tiledump_enemy,
    empty_enemy,
    level_enemies_test_enemy,
    empty_enemy,
    level_macosten_enemy,
    level_screen_edge_test_enemy,
    
    empty_enemy,
    empty_enemy,
};

#pragma rodata-name(pop)
