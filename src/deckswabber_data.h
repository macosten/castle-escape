// Do not try to use pointers from another bank while that bank isn't 
// swapped in the correct place in RAM.
// Otherwise, you'll get garbage data.

#pragma rodata-name(push, "BANK1")

const char deckswabber_tiles_remaining[] = "Tiles Left";
const char deckswabber_level_finished[] = "Finished!";
const char deckswabber_bonus[] = "Bonus:";
const char deckswabber_goal[] = "Goal";

const unsigned char blumaroo_idle[] = {
    0, 0, 0x00, 0,
    8, 0, 0x01, 0,
    0, 8, 0x3E, 0,
    8, 8, 0x11, 0,
    0,16, 0x20, 0,
    8,16, 0x21, 0,
    128
};

const unsigned char blumaroo_jump1[] = {
    0, 0, 0x02, 0,
    8, 0, 0x03, 0,
    0, 8, 0x12, 0,
    8, 8, 0x13, 0,
    0,16, 0x22, 0,
    8,16, 0x23, 0,
    128
};

const unsigned char blumaroo_jump2[] = {
    0, 8, 0x04, 0,
    8, 8, 0x05, 0,
    0,16, 0x14, 0,
    8,16, 0x15, 0,
    128
};

const unsigned char techo_idle[] = {
    0, 0, 0x06, 3,
    8, 0, 0x07, 3,
    0, 8, 0x16, 3,
    8, 8, 0x17, 3,
    0,16, 0x26, 3,
    8,16, 0x27, 3,
    128
};

const unsigned char techo_jump1[] = {
    0, 0, 0x08, 3,
    8, 0, 0x09, 3,
    0, 8, 0x18, 3,
    8, 8, 0x19, 3,
    0,16, 0x28, 3,
    8,16, 0x29, 3,
    128
};

const unsigned char techo_jump2[] = {
    0, 0, 0x0A, 3,
    8, 0, 0x0B, 3,
    0, 8, 0x1A, 3,
    8, 8, 0x1B, 3,
    0,16, 0x2A, 3,
    8,16, 0x2B, 3,
    128
};

const unsigned char sword_idle[] = {
    0, 0, 0x0C, 1,
    0, 8, 0x1C, 1,
    0,16, 0x2C, 1,
    128
};

const unsigned char sword_jump1[] = {
    0, 0, 0x0D, 1,
    0, 8, 0x1D, 1,
    0,16, 0x2D, 1,
    128
};

const unsigned char sword_jump2[] = {
    0, 0, 0x0E, 1,
    0, 8, 0x1E, 1,
    0,16, 0x2E, 1,
    128
};

const unsigned char entity_intro_curtain1[] = {
    0, 0, 0x1F, 1,
    8, 0, 0x1F, 1,
    128
};

const unsigned char entity_intro_curtain2[] = {
    0, 0, 0x0F, 1,
    8, 0, 0x0F, 1,
    128
};

const unsigned char entity_intro_curtain3[] = {
    0, 0, 0x0F, 1,
    8, 0, 0x0F, 1,
    0, 8, 0x1F, 1,
    8, 8, 0x1F, 1,
    128
};

const unsigned char entity_intro_curtain4[] = {
    0, 0, 0x0F, 1,
    8, 0, 0x0F, 1,
    0, 8, 0x0F, 1,
    8, 8, 0x0F, 1,
    128
};

const unsigned char captain_dread_idle[] = {
    0, 8, 0x30, 2,
    8, 8, 0x31, 2,
    0,16, 0x40, 2,
    8,16, 0x41, 2,
    128
};

const unsigned char captain_dread_jump1[] = {
    0, 0, 0x32, 2,
    8, 0, 0x33, 2,
    0, 8, 0x42, 2,
    8, 8, 0x43, 2,
    0,16, 0x52, 2,
    8,16, 0x53, 2,
    128
};

const unsigned char captain_dread_jump2[] = {
    0, 0, 0x24, 2,
    8, 0, 0x25, 2,
    0, 8, 0x34, 2,
    8, 8, 0x35, 2,
    0,16, 0x44, 2,
    8,16, 0x45, 2,
    128
};

const unsigned char chest_bronze_idle[] = {
    0, 7, 0x36, 1,
    8, 7, 0x37, 1,
    0,15, 0x46, 1,
    8,15, 0x47, 1,
    4,16, 0x8A, 1,
    128
};

const unsigned char chest_bronze_jump1[] = {
    0, 4, 0x38, 1,
    8, 4, 0x39, 1,
    0,12, 0x48, 1,
    8,12, 0x49, 1,
    4,16, 0x8A, 1,
    128
};

const unsigned char chest_bronze_jump2[] = {
    0, 6, 0x38, 1,
    8, 6, 0x39, 1,
    0,14, 0x48, 1,
    8,14, 0x49, 1,
    4,16, 0x8A, 1,
    128
};

const unsigned char chest_silver_idle[] = {
    0, 7, 0x3A, 1,
    8, 7, 0x3B, 1,
    0,15, 0x4A, 1,
    8,15, 0x4B, 1,
    4,16, 0x8A, 1,
    128
};

const unsigned char chest_silver_jump1[] = {
    0, 4, 0x3C, 1,
    8, 4, 0x3D, 1,
    0,12, 0x4C, 1,
    8,12, 0x4D, 1,
    4,16, 0x8A, 1,
    128
};

const unsigned char chest_silver_jump2[] = {
    0, 6, 0x3C, 1,
    8, 6, 0x3D, 1,
    0,14, 0x4C, 1,
    8,14, 0x4D, 1,
    4,16, 0x8A, 1,
    128
};

const unsigned char chest_gold_idle[] = {
    0, 7, 0x36, 2,
    8, 7, 0x37, 2,
    0,15, 0x46, 2,
    8,15, 0x47, 2,
    4,16, 0x8A, 2,
    128
};

const unsigned char chest_gold_jump1[] = {
    0, 4, 0x38, 2,
    8, 4, 0x39, 2,
    0,12, 0x48, 2,
    8,12, 0x49, 2,
    4,16, 0x8A, 2,
    128
};

const unsigned char chest_gold_jump2[] = {
    0, 6, 0x38, 2,
    8, 6, 0x39, 2,
    0,14, 0x48, 2,
    8,14, 0x49, 2,
    4,16, 0x8A, 2,
    128
};

const unsigned char coin_bronze_idle[] = {
    0, 7, 0x5A, 1,
    8, 7, 0x5B, 1,
    0,15, 0x6A, 1,
    8,15, 0x6B, 1,
    4,16, 0x8A, 1,
    128
};

const unsigned char coin_bronze_jump1[] = {
    0, 4, 0x5C, 1,
    8, 4, 0x5D, 1,
    0,12, 0x6C, 1,
    8,12, 0x6D, 1,
    4,16, 0x8A, 1,
    128
};

const unsigned char coin_bronze_jump2[] = {
    0, 6, 0x5E, 1,
    8, 6, 0x5F, 1,
    0,14, 0x6E, 1,
    8,14, 0x6F, 1,
    4,16, 0x8A, 1,
    128
};

const unsigned char coin_silver_idle[] = {
    0, 7, 0x54, 1,
    8, 7, 0x55, 1,
    0,15, 0x64, 1,
    8,15, 0x65, 1,
    4,16, 0x8A, 1,
    128
};

const unsigned char coin_silver_jump1[] = {
    0, 4, 0x56, 1,
    8, 4, 0x57, 1,
    0,12, 0x66, 1,
    8,12, 0x67, 1,
    4,16, 0x8A, 1,
    128
};

const unsigned char coin_silver_jump2[] = {
    0, 6, 0x58, 1,
    8, 6, 0x59, 1,
    0,14, 0x68, 1,
    8,14, 0x69, 1,
    4,16, 0x8A, 1,
    128
};

const unsigned char coin_gold_idle[] = {
    0, 7, 0x5A, 2,
    8, 7, 0x5B, 2,
    0,15, 0x6A, 2,
    8,15, 0x6B, 2,
    4,16, 0x8A, 2,
    128
};

const unsigned char coin_gold_jump1[] = {
    0, 4, 0x5C, 2,
    8, 4, 0x5D, 2,
    0,12, 0x6C, 2,
    8,12, 0x6D, 2,
    4,16, 0x8A, 2,
    128
};

const unsigned char coin_gold_jump2[] = {
    0, 6, 0x5E, 2,
    8, 6, 0x5F, 2,
    0,14, 0x6E, 2,
    8,14, 0x6F, 2,
    4,16, 0x8A, 2,
    128
};

const unsigned char flag_half_idle[] = {
    0, 0, 0x50, 1,
    0, 8, 0x60, 1,
    0,16, 0x70, 1,
    1, 8, 0x2F, 2,
    9, 8, 0x2F, 2|OAM_FLIP_H|OAM_FLIP_V,
    4,16, 0x8A, 1,
    128
};

const unsigned char flag_half_jump1[] = {
    0,-3, 0x51, 1,
    0, 5, 0x61, 1,
    0,13, 0x71, 1,
    3, 5, 0x62, 2,
   11, 5, 0x63, 2,
    4,16, 0x8A, 1,
    128
};

const unsigned char flag_half_jump2[] = {
    0,-1, 0x51, 1,
    0, 7, 0x61, 1,
    0,15, 0x71, 1,
    3, 7, 0x62, 2,
   11, 7, 0x63, 2,
    4,16, 0x8A, 1,
    128
};

const unsigned char flag_full_idle[] = {
    0, 0, 0x50, 1,
    0, 8, 0x60, 1,
    0,16, 0x70, 1,
    1, 0, 0x2F, 2,
    9, 0, 0x2F, 2|OAM_FLIP_H|OAM_FLIP_V,
    4,16, 0x8A, 1,
    128
};

const unsigned char flag_full_jump1[] = {
    0,-3, 0x51, 1,
    0, 5, 0x61, 1,
    0,13, 0x71, 1,
    1,-3, 0x62, 2,
    9,-3, 0x63, 2,
    4,16, 0x8A, 1,
    128
};

const unsigned char flag_full_jump2[] = {
    0,-1, 0x51, 1,
    0, 7, 0x61, 1,
    0,15, 0x71, 1,
    1, 7, 0x62, 2,
    9, 7, 0x63, 2,
    4,16, 0x8A, 1,
    128
};

const unsigned char dirt_bomb_plus_idle[] = {
    0, 7, 0x72, 1,
    8, 7, 0x73, 1,
    0,15, 0x82, 1,
    8,15, 0x83, 1,
    4,16, 0x8A, 1,
    128
};

const unsigned char dirt_bomb_plus_jump1[] = {
    0, 4, 0x74, 1,
    8, 4, 0x75, 1,
    0,12, 0x84, 1,
    8,12, 0x85, 1,
    4,16, 0x8A, 1,
    128
};

const unsigned char dirt_bomb_plus_jump2[] = {
    0, 6, 0x76, 1,
    8, 6, 0x77, 1,
    0,14, 0x86, 1,
    8,14, 0x87, 1,
    4,16, 0x8A, 1,
    128
};

const unsigned char dirt_bomb_square_idle[] = {
    0, 7, 0x78, 1,
    8, 7, 0x79, 1,
    0,15, 0x88, 1,
    8,15, 0x89, 1,
    4,16, 0x8A, 1,
    128
};

const unsigned char dirt_bomb_square_jump1[] = {
   -3, 4, 0x7A, 1,
    5, 4, 0x7B, 1,
    5,12, 0x8B, 1,
    4,16, 0x8A, 1,
    128
};

const unsigned char dirt_bomb_square_jump2[] = {
    0, 6, 0x7C, 1,
    0,14, 0x8C, 1,
    8,14, 0x8D, 1,
    4,16, 0x8A, 1,
    128
};

const unsigned char mynci_idle[] = {
    0, 0, 0x80, 3,
    8, 0, 0x81, 3,
    0, 8, 0x90, 3,
    8, 8, 0x91, 3,
    0,16, 0xA0, 3,
    8,16, 0xA1, 3,
    128
};

const unsigned char mynci_jump1[] = {
    0, 0, 0x92, 3,
    8, 0, 0x93, 3,
    0, 8, 0xA2, 3,
    8, 8, 0xA3, 3,
    0,16, 0xB2, 3,
    8,16, 0xB3, 3,
    128
};

const unsigned char mynci_jump2[] = {
    0, 0, 0x94, 3,
    8, 0, 0x95, 3,
    0, 8, 0xA4, 3,
    8, 8, 0xA5, 3,
    0,16, 0xB4, 3,
    8,16, 0xB4, 3,
    128
};

const unsigned char cannon_idle[] = {
    0, 7, 0x96, 1,
    8, 7, 0x97, 1,
    0,15, 0xA6, 1,
    8,15, 0xA7, 1,
    4,16, 0x8A, 1,
    128
};

const unsigned char cannon_jump1[] = {
    0, 4, 0x98, 1,
    8, 4, 0x99, 1,
    0,12, 0xA8, 1,
    8,12, 0xA9, 1,
    4,16, 0x8A, 1,
    128
};

const unsigned char cannon_jump2[] = {
    0, 6, 0x98, 1,
    8, 6, 0x99, 1,
    0,14, 0xA8, 1,
    8,14, 0xA9, 1,
    4,16, 0x8A, 1,
    128
};

// Reverse order animations; they'll start with some kind of counter that will decrement
const unsigned char * const blumaroo_jump_animation[] = {
    blumaroo_idle,
    blumaroo_jump2,
    blumaroo_jump1,
    // blumaroo_jump1,
};

const unsigned char * const techo_jump_animation[] = {
    techo_idle,
    techo_jump2,
    techo_jump1
};

const unsigned char * const sword_jump_animation[] = {
    sword_idle,
    sword_jump2,
    sword_jump1
};

const unsigned char * const captain_dread_jump_animation[] = {
    captain_dread_idle,
    captain_dread_jump2,
    captain_dread_jump1,
};

const unsigned char * const entity_intro_curtain_animation[] = {
    entity_intro_curtain4,
    entity_intro_curtain3,
    entity_intro_curtain2,
    entity_intro_curtain1,
};

const unsigned char * const chest_bronze_jump_animation[] = {
    chest_bronze_idle,
    chest_bronze_jump2,
    chest_bronze_jump1,
};

const unsigned char * const chest_silver_jump_animation[] = {
    chest_silver_idle,
    chest_silver_jump2,
    chest_silver_jump1,
};

const unsigned char * const chest_gold_jump_animation[] = {
    chest_gold_idle,
    chest_gold_jump2,
    chest_gold_jump1,
};

const unsigned char * const coin_bronze_jump_animation[] = {
    coin_bronze_idle,
    coin_bronze_jump2,
    coin_bronze_jump1,
};

const unsigned char * const coin_silver_jump_animation[] = {
    coin_silver_idle,
    coin_silver_jump2,
    coin_silver_jump1,
};

const unsigned char * const coin_gold_jump_animation[] = {
    coin_gold_idle,
    coin_gold_jump2,
    coin_gold_jump1,
};

const unsigned char * const dirt_bomb_plus_jump_animation[] = {
    dirt_bomb_plus_idle,
    dirt_bomb_plus_jump2,
    dirt_bomb_plus_jump1,
};

const unsigned char * const dirt_bomb_square_jump_animation[] = {
    dirt_bomb_square_idle,
    dirt_bomb_square_jump2,
    dirt_bomb_square_jump1,
};

const unsigned char * const mynci_jump_animation[] = {
    mynci_idle,
    mynci_jump2,
    mynci_jump1,
};

const unsigned char * const cannon_jump_animation[] = {
    cannon_idle,
    cannon_jump2,
    cannon_jump1,
};

const unsigned char * const flag_half_jump_animation[] = {
    flag_half_idle,
    flag_half_jump2,
    flag_half_jump1,
};

const unsigned char * const flag_full_jump_animation[] = {
    flag_full_idle,
    flag_full_jump2,
    flag_full_jump1,
};

extern const unsigned char * const item_explosion_animation[];

// Levels. Each hex digit corresponds to a tile type.
// 0 is regular, 1 is water; 2-F will be extra types for extra levels.

const unsigned char deckswabber_original_map1[] = {
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
};

const unsigned char deckswabber_original_map2[] = {
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x01, 0x10, 0x00,
    0x00, 0x01, 0x10, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
};

const unsigned char deckswabber_original_map3[] = {
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x10, 0x01, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x10, 0x01, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
};

const unsigned char deckswabber_original_map4[] = {
    0x00, 0x00, 0x00, 0x00,
    0x01, 0x00, 0x00, 0x10,
    0x01, 0x01, 0x10, 0x10,
    0x01, 0x01, 0x10, 0x10,
    0x01, 0x00, 0x00, 0x10,
    0x01, 0x00, 0x00, 0x10,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
};

const unsigned char deckswabber_original_map5[] = {
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x10, 0x01, 0x00,
    0x01, 0x10, 0x01, 0x10,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x01, 0x10, 0x01, 0x10,
    0x00, 0x10, 0x01, 0x00,
    0x00, 0x00, 0x00, 0x00,
};

const unsigned char deckswabber_original_map6[] = {
    0x01, 0x00, 0x00, 0x10,
    0x00, 0x00, 0x00, 0x00,
    0x01, 0x00, 0x00, 0x10,
    0x01, 0x00, 0x00, 0x10,
    0x01, 0x00, 0x00, 0x10,
    0x01, 0x00, 0x00, 0x10,
    0x00, 0x00, 0x00, 0x00,
    0x01, 0x00, 0x00, 0x10,
};

const unsigned char deckswabber_original_map7[] = {
    0x00, 0x00, 0x00, 0x00,
    0x01, 0x00, 0x00, 0x10,
    0x01, 0x00, 0x00, 0x10,
    0x01, 0x00, 0x00, 0x10,
    0x01, 0x00, 0x00, 0x10,
    0x01, 0x00, 0x00, 0x10,
    0x01, 0x00, 0x00, 0x10,
    0x00, 0x00, 0x00, 0x00,
};

const unsigned char deckswabber_original_map8[] = {
    0x00, 0x01, 0x10, 0x00,
    0x00, 0x01, 0x10, 0x00,
    0x00, 0x01, 0x10, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x01, 0x10, 0x00,
    0x00, 0x01, 0x10, 0x00,
    0x00, 0x01, 0x10, 0x00,
};

const unsigned char deckswabber_original_map9[] = {
    0x00, 0x00, 0x00, 0x00,
    0x01, 0x10, 0x01, 0x10,
    0x01, 0x00, 0x00, 0x10,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x01, 0x00, 0x00, 0x10,
    0x01, 0x10, 0x01, 0x10,
    0x00, 0x00, 0x00, 0x00,
};

const unsigned char deckswabber_original_map10[] = {
    0x00, 0x00, 0x00, 0x00,
    0x01, 0x01, 0x11, 0x10,
    0x01, 0x00, 0x00, 0x10,
    0x01, 0x00, 0x00, 0x10,
    0x01, 0x00, 0x00, 0x10,
    0x01, 0x00, 0x00, 0x10,
    0x01, 0x11, 0x10, 0x10,
    0x00, 0x00, 0x00, 0x00,
};

// Bitpacked tile x,y
const unsigned char deckswabber_original_starting_coords[] = {
    0x00,
    0x00,
    0x00,
    0x77,
    0x33,
    0x00,
    0x33,
    0x00,
    0x33,
    0x77,
};

const unsigned char deckswabber_metatiles[] = {
    0x80, 0x81, 0x90, 0x91, 0, // Brown board, plain
    0x82, 0x83, 0x92, 0x93, 0, // Yellow board, plain
    0xA0, 0xA1, 0xB0, 0xB1, 2, // Blue board, wavy
    0xA2, 0xA3, 0xB2, 0xB3, 2, // Green board, wavy
    0x84, 0x85, 0x94, 0x95, 3, // Red board, chevron
    0xA4, 0xA5, 0xB4, 0xB5, 3, // Pink board, chevron
    0x82, 0x83, 0x92, 0x93, 2, // Light Blue Board, plain
    0xE8, 0xE9, 0xF8, 0xF9, 0, // Black board, wavy 
    // Others go below...

    // These two holes should stay the last two in this order
    0xA8, 0xA9, 0xB8, 0xB9, 1, // Water hole
    0xC8, 0xC9, 0xD8, 0xD9, 0, // Empty hole
};

#define DECKSWABBER_WATER_HOLE_ID (sizeof(deckswabber_metatiles)/5 - 2)
#define DECKSWABBER_EMPTY_HOLE_ID (sizeof(deckswabber_metatiles)/5 - 1)

// Code using these values assumes all will be 0...3
const unsigned char deckswabber_metatile_palettes[] = {
    0,
    0,
    2,
    2,
    3,
    3,
    1,
    0,
    //
    1,
    0,
};

const unsigned char deckswabber_nibble_to_tile_id_map[] = {
    0, // 0x0 -> Plain Tile
    DECKSWABBER_WATER_HOLE_ID, // 0x1 -> Water hole
    DECKSWABBER_EMPTY_HOLE_ID, // 0x2 -> Empty hole
    // The rest of these could correspond to tiles that block 1 or more directions
};

// Each row is one "level" and each row is of the format:
// (which index level to use), (which index tile color increment function to use)


const unsigned char * const deckswabber_original_level_pack_levels[] = {
    deckswabber_original_map1,
    deckswabber_original_map2,
    deckswabber_original_map3,
    deckswabber_original_map4,
    deckswabber_original_map5,
    deckswabber_original_map6,
    deckswabber_original_map7,
    deckswabber_original_map8,
    deckswabber_original_map9,
    deckswabber_original_map10,
};


const unsigned char deckswabber_original_level_pack_round_bounds[] = {
    0, 5, // Round 1: Start at level 0 (1-indexed 1), round up at level 5 (after beating 1-indexed level 5)
    0, 5, // etc
    1, 6,
    2, 7,
    3, 8,
    4, 9,
    5, 10,
    5, 10, // 8 rounds originally
};

const unsigned char * const * const deckswabber_level_data_db[] = {
    deckswabber_original_level_pack_levels,
    // ..., ..., "Stowaway Sting Levels",
};

const char * const deckswabber_level_name_db[] = {
    "Original Levels",
    // "Stowaway Sting",
};

const unsigned char deckswabber_maximum_round_db[] = {
    sizeof(deckswabber_original_level_pack_round_bounds)/2, // Original levels have 8 rounds
};

const unsigned char * const deckswabber_round_bounds_db[] = {
    deckswabber_original_level_pack_round_bounds
};

// Expected to be bitpacked tile x,y
const unsigned char * const deckswabber_starting_coords_db[] = {
    deckswabber_original_starting_coords,
};

const unsigned char deckswabber_goal_hud_tiles_toprow[] = {
    0x80, 0x81, 0x08, 0x82, 0x83, 0x08, 0xA0, 0xA1, 0x08, 0xA2, 0xA3, 0x08, 0x84, 0x85, 0x08, 0xA4, 0xA5, 0x08, 0x82, 0x83, 0x08, 0xE8, 0xE9, 0x08
};

const unsigned char deckswabber_goal_hud_tiles_bottomrow[] = {
    0x90, 0x91, 0x00, 0x92, 0x93, 0x00, 0xB0, 0xB1, 0x00, 0xB2, 0xB3, 0x00, 0x94, 0x95, 0x00, 0xB4, 0xB5, 0x00, 0x92, 0x93, 0x00, 0xF8, 0xF9, 0x00
};

// 8x8 tile x, not 16 by 16 tile x
const unsigned char deckswabber_goal_hud_starting_tile_x_from_increment_type[] = {
    12,
    12,
    12,
    12,
    9,
    9,
    9,
    9,
    6,
    6,
    6,
    6,
    3,
    3,
};

// How much of each row should be written out to the hud (in tiles/bytes) 
const unsigned char deckswabber_goal_hud_byte_write_length_from_increment_type[] = {
    5,
    5,
    8,
    8,
    11,
    11,
    14,
    14,
    17,
    17,
    20,
    20,
    23,
    23,
};

/*
Do something based on the lower nibble: 
   - 0: Nothing
   - 1: Add a backwards arrow on the bottom row between the final two tiles
   - 2: Write out a (meta)tile after this one (the upper nibble determines which one, if I bother implementing that)
*/ 
const unsigned char deckswabber_goal_hud_touchup_procedure_from_increment_type[] = {
    0x00,
    0x01,
    0x01,
    0x02,
    0x01,
    0x02,
    0x01,
    0x02,
    0x01,
    0x02,
    0x01,
    0x02,
    0x01,
    0x02, // Theoretically we could want a future increment type to loop back to a different tile than the first
};

// Writes starting at X3F0 (figure these out by looking in NES Screen Tool)
// *in case* I want two consecutive ones to not be identical, I'm not doubling up
const unsigned char deckswabber_goal_hud_attribute_bytes_from_increment_type[] = {
    0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x44, // 0
    0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x44, // 1
    0x11, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x44, // 2
    0x11, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x44, // 3
    0x11, 0x00, 0x00, 0x80, 0xA0, 0x00, 0x00, 0x44, // 4
    0x11, 0x00, 0x00, 0x80, 0xA0, 0x00, 0x00, 0x44, // 5
    0x11, 0x00, 0x00, 0x80, 0xA0, 0xF0, 0x00, 0x44, // 6
    0x11, 0x00, 0x00, 0x80, 0xA0, 0xF0, 0x00, 0x44, // 7
    0x11, 0x00, 0x00, 0xA0, 0xE0, 0xF0, 0x00, 0x44, // 8
    0x11, 0x00, 0x00, 0xA0, 0xE0, 0xF0, 0x00, 0x44, // 9
    0x11, 0x00, 0x00, 0xA0, 0xE0, 0xF0, 0x10, 0x44, // 10
    0x11, 0x00, 0x00, 0xA0, 0xE0, 0xF0, 0x10, 0x44, // 11
    0x11, 0x00, 0xA0, 0xE0, 0xF0, 0x50, 0x00, 0x44, // 12
    0x11, 0x00, 0xA0, 0xE0, 0xF0, 0x50, 0x00, 0x44, // 13
};

const unsigned char deckswabber_entity_index_to_depthmask[] = {
    0b1,
    0b10,
    0b100,
    0b1000,
    0b10000,
    0b100000,
    0b1000000,
    // MSB reserved for player
};

const unsigned char * const * const deckswabber_entity_id_to_animation_ptr[] = {
    entity_intro_curtain_animation,
    item_explosion_animation,
    coin_bronze_jump_animation,
    coin_silver_jump_animation,
    coin_gold_jump_animation,
    chest_bronze_jump_animation,
    chest_silver_jump_animation,
    chest_gold_jump_animation,
    sword_jump_animation,
    flag_half_jump_animation,
    flag_full_jump_animation,
    dirt_bomb_plus_jump_animation,
    dirt_bomb_square_jump_animation,
    cannon_jump_animation,
    mynci_jump_animation,
    techo_jump_animation,
    captain_dread_jump_animation,
};

#pragma rodata-name(pop);

#pragma rodata-name(push, "BANK5")

// LibLZG'd data either need to be in Bank 5 or be copied to RAM because the LZG decode function is in bank 5
const unsigned char const deckswabber_game_screen[] = {
	76,90,71,0,0,4,0,0,0,1,41,158,174,97,9,1,
	2,3,6,7,135,224,225,134,135,7,54,6,20,151,230,229,
	231,7,23,240,241,150,135,230,244,0,83,99,111,114,101,0,
	82,111,117,110,100,0,0,76,101,118,101,108,0,0,84,105,
	109,101,0,228,241,6,56,244,0,48,7,2,0,7,194,0,
	7,1,3,5,4,58,7,193,228,6,248,0,0,72,101,97,
	108,116,104,6,86,6,49,108,101,115,6,62,102,116,0,3,
	6,56,0,0,246,7,3,6,152,6,251,7,228,3,7,56,
	192,212,7,15,194,6,152,3,5,56,195,197,211,7,13,197,
	196,3,5,56,136,137,213,213,213,195,196,128,129,7,44,195,
	196,6,15,226,227,134,151,242,243,150,151,150,195,196,144,145,
	7,44,195,196,151,150,6,84,3,34,32,192,193,3,18,56,
	193,194,6,84,6,30,150,209,197,3,18,56,197,209,6,84,
	6,92,6,88,3,16,56,197,209,3,31,56,3,31,56,3,
	28,56,150,3,66,156,3,23,56,3,71,184,3,18,56,3,
	72,184,3,66,24,3,78,55,3,69,24,3,69,56,208,3,
	78,55,211,211,210,3,74,120,7,21,3,7,56,3,23,24,
	3,27,56,6,184,3,67,152,7,21,226,227,150,85,7,6,
	21,5,7,1,69,85,85,1,6,113,4,85,85,6,250,7,
	240,80,7,3,85,6,102,7,2,
};

#pragma rodata-name(pop);