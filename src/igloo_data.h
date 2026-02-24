// Do not try to use pointers from another bank while that bank isn't 
// swapped in the correct place in RAM.
// Otherwise, you'll get garbage data.

#pragma rodata-name(push, "BANK1")

const char igloo_all_items_bonus_phrase[] = "Got all items!";
const char igloo_all_items_bonus_prefix[] = "Bonus: ";
const char igloo_starting_next_level_phrase[] = "Level ";
const char fake_time[] = " 30";

const char igloo_piano_troll_message_1[] = "Piano catching";
const char igloo_piano_troll_message_2[] = "is a bad idea!";

const char igloo_itemdrop_troll_message_1[] = "Cleanup on";
const char igloo_itemdrop_troll_message_2[] = "aisle you!";

const char igloo_game_over_message[] = "Game Over";

const unsigned char const carassa_idle[] = {
  0, 0, 0x81, 0|OAM_FLIP_H,
  8, 0, 0x80, 0,
 16, 0, 0x81, 0,
  0, 8, 0x91, 0|OAM_FLIP_H,
  8, 8, 0x90, 0,
 16, 8, 0x91, 0,
  0,16, 0xA1, 0|OAM_FLIP_H,
  8,16, 0xA0, 0,
 16,16, 0xA1, 0,
  0,24, 0xB1, 0|OAM_FLIP_H,
  8,24, 0xB0, 0,
 16,24, 0xB1, 0,
 128
};

const unsigned char const carassa_idle_shocked[] = {
  0, 0, 0x81, 0|OAM_FLIP_H,
  8, 0, 0x80, 0,
 16, 0, 0x81, 0,
  0, 8, 0xA9, 0|OAM_FLIP_H,
  8, 8, 0xA8, 0,
 16, 8, 0xA9, 0,
  0,16, 0xA1, 0|OAM_FLIP_H,
  8,16, 0xA0, 0,
 16,16, 0xA1, 0,
  0,24, 0xB1, 0|OAM_FLIP_H,
  8,24, 0xB0, 0,
 16,24, 0xB1, 0,
 128
};

const unsigned char const carassa_walk1[] = {
  0, 0, 0x81, 0|OAM_FLIP_H,
  8, 0, 0x80, 0,
 16, 0, 0x81, 0,
  0, 8, 0x91, 0|OAM_FLIP_H,
  8, 8, 0x90, 0,
 16, 8, 0x91, 0,
  0,16, 0xA1, 0|OAM_FLIP_H,
  8,16, 0xA0, 0,
 16,16, 0xA1, 0,
  0,24, 0xC1, 0|OAM_FLIP_H,
  8,24, 0xD0, 0,
 16,24, 0xD1, 0,
 128
};

const unsigned char const carassa_walk2[] = {
  0, 0, 0x81, 0|OAM_FLIP_H,
  8, 0, 0x80, 0,
 16, 0, 0x81, 0,
  0, 8, 0x91, 0|OAM_FLIP_H,
  8, 8, 0x90, 0,
 16, 8, 0x91, 0,
  0,16, 0xA1, 0|OAM_FLIP_H,
  8,16, 0xA0, 0,
 16,16, 0xA1, 0,
 16,24, 0xC1, 0,
  8,24, 0xC0, 0,
  0,24, 0xD1, 0|OAM_FLIP_H,
 128
};

const unsigned char const carassa_shocked_walk1[] = {
  0, 0, 0x81, 0|OAM_FLIP_H,
  8, 0, 0x80, 0,
 16, 0, 0x81, 0,
  0, 8, 0xA9, 0|OAM_FLIP_H,
  8, 8, 0xA8, 0,
 16, 8, 0xA9, 0,
  0,16, 0xA1, 0|OAM_FLIP_H,
  8,16, 0xA0, 0,
 16,16, 0xA1, 0,
  0,24, 0xC1, 0|OAM_FLIP_H,
  8,24, 0xD0, 0,
 16,24, 0xD1, 0,
 128
};

const unsigned char const carassa_shocked_walk2[] = {
  0, 0, 0x81, 0|OAM_FLIP_H,
  8, 0, 0x80, 0,
 16, 0, 0x81, 0,
  0, 8, 0xA9, 0|OAM_FLIP_H,
  8, 8, 0xA8, 0,
 16, 8, 0xA9, 0,
  0,16, 0xA1, 0|OAM_FLIP_H,
  8,16, 0xA0, 0,
 16,16, 0xA1, 0,
 16,24, 0xC1, 0,
  8,24, 0xC0, 0,
  0,24, 0xD1, 0|OAM_FLIP_H,
 128
};

const unsigned char const carassa_drop[] = {
  0, 0, 0x81, 0|OAM_FLIP_H,
  8, 0, 0x80, 0,
 16, 0, 0x81, 0,
  0, 8, 0x91, 0|OAM_FLIP_H,
  8, 8, 0x90, 0,
 16, 8, 0x91, 0,
  0,16, 0xE7, 0|OAM_FLIP_H,
  8,16, 0xD2, 0,
 16,16, 0xE7, 0,
  0,24, 0xB1, 0|OAM_FLIP_H,
  8,24, 0xB0, 0,
 16,24, 0xB1, 0,
 128
};

const unsigned char * const carassa_walk_animation[] = {
    carassa_walk1,
    carassa_walk2,
    carassa_shocked_walk1,
    carassa_shocked_walk2,
};

const unsigned char const mika_idle[] = {
  0, 0, 0x83, 1|OAM_FLIP_H,
  8, 0, 0x82, 1,
 16, 0, 0x83, 1,
  0, 8, 0x93, 1|OAM_FLIP_H,
  8, 8, 0x92, 1,
 16, 8, 0x93, 1,
  0,16, 0xA3, 1|OAM_FLIP_H,
  8,16, 0xA2, 1,
 16,16, 0xA3, 1,
  0,24, 0xB3, 1|OAM_FLIP_H,
  8,24, 0xB2, 1,
 16,24, 0xB3, 1,
 128
};

const unsigned char const mika_walk1[] = {
  0, 0, 0x83, 1|OAM_FLIP_H,
  8, 0, 0x82, 1,
 16, 0, 0x83, 1,
  0, 8, 0x93, 1|OAM_FLIP_H,
  8, 8, 0x92, 1,
 16, 8, 0x93, 1,
  0,16, 0xA3, 1|OAM_FLIP_H,
  8,16, 0xA2, 1,
 16,16, 0xA3, 1,
  0,24, 0xC3, 1|OAM_FLIP_H,
  8,24, 0xC2, 1|OAM_FLIP_H,
 16,24, 0xD3, 1,
 128
};

const unsigned char const mika_walk2[] = {
  0, 0, 0x83, 1|OAM_FLIP_H,
  8, 0, 0x82, 1,
 16, 0, 0x83, 1,
  0, 8, 0x93, 1|OAM_FLIP_H,
  8, 8, 0x92, 1,
 16, 8, 0x93, 1,
  0,16, 0xA3, 1|OAM_FLIP_H,
  8,16, 0xA2, 1,
 16,16, 0xA3, 1,
 16,24, 0xC3, 1,
  8,24, 0xC2, 1,
  0,24, 0xD3, 1|OAM_FLIP_H,
 128
};

const unsigned char * const mika_walk_animation[] = {
    mika_walk1,
    mika_walk2,
};

const unsigned char const mika_stunned[] = {
  0, 0, 0x83, 2|OAM_FLIP_H,
  8, 0, 0x82, 2,
 16, 0, 0x83, 2,
  0, 8, 0x93, 2|OAM_FLIP_H,
  8, 8, 0x92, 2,
 16, 8, 0x93, 2,
  0,16, 0xA3, 2|OAM_FLIP_H,
  8,16, 0xA2, 2,
 16,16, 0xA3, 2,
  0,24, 0xB3, 2|OAM_FLIP_H,
  8,24, 0xB2, 2,
 16,24, 0xB3, 2,
 128
};

// Items

const unsigned char item_potion[] = {
    0, 0, 0x87, 1,
    8, 0, 0x87, 1|OAM_FLIP_H,
    0, 8, 0x97, 1,
    8, 8, 0x97, 1|OAM_FLIP_H,
    128
};

const unsigned char item_potion_broken[] = {
    0, 0, 0xA7, 1,
    8, 0, 0xA7, 1|OAM_FLIP_H,
    128
};

const unsigned char item_piano[] = {
    8, 0, 0x85, 2,
   16, 0, 0x86, 2,
    0, 8, 0x94, 2,
    8, 8, 0x95, 2,
   16, 8, 0x96, 2,
    0,16, 0xA4, 2,
    8,16, 0xA5, 2,
   17,16, 0xA4, 2|OAM_FLIP_H,
   128 
};

const unsigned char item_chiapop[] = {
    0, 0, 0x5E, 2,
    8, 0, 0x5F, 2,
    128
};

const unsigned char item_chiapop_broken[] = {
    0, 0, 0xC4, 2,
    8, 0, 0xC5, 2,
    128
};

const unsigned char item_coin[] = {
    0, 0, 0x5C, 0,
    8, 0, 0x5D, 0,
    128
};

const unsigned char item_coin_broken[] = {
    0, 0, 0xD6, 0,
    8, 0, 0xD7, 0,
    128
};

const unsigned char item_umbrella[] = {
    0, 0, 0x4C, 0,
    8, 0, 0x4D, 0,
   16, 0, 0x4E, 0,
   128
};

const unsigned char item_umbrella_broken[] = {
    4, 0, 0xC6, 0,
   12, 0, 0xC7, 0,
   128
};

const unsigned char item_bomb0[] = {
    0,-8, 0xE0, 0,
    0, 0, 0xF0, 2,
    128
};

const unsigned char item_bomb1[] = {
    0,-8, 0xE1, 0,
    0, 0, 0xF0, 2,
    128
};

const unsigned char item_bomb2[] = {
    0,-8, 0xF1, 0,
    0, 0, 0xF0, 2,
    128
};

const unsigned char item_bomb3[] = {
    0,-8, 0xE4, 0,
    0, 0, 0xF0, 2,
    128
};

const unsigned char item_bomb4[] = {
    0,-8, 0xF4, 0,
    0, 0, 0xF0, 2,
    128
};

const unsigned char item_bomb5[] = {
   -1,-8, 0xE5, 0,
    0, 0, 0xF0, 2,
    128
};

const unsigned char item_bomb6[] = {
   -1,-8, 0xF5, 0,
    0, 0, 0xF0, 2,
    128
};

const unsigned char item_bomb7[] = {
   -1,-8, 0xE6, 0,
    0, 0, 0xF0, 2,
    128
};

const unsigned char item_bomb8[] = {
   -1,-8, 0xF6, 0,
    0, 0, 0xF0, 2,
    128
};

const unsigned char * const item_bomb_fuse_animation[] = {
    item_bomb1,
    item_bomb2,
    item_bomb3,
    item_bomb4,
    item_bomb5,
    item_bomb6,
    item_bomb7,
    item_bomb8,
};

const unsigned char item_thingy[] = {
    0, 0, 0xE2, 3,
    0, 8, 0xF2, 3,
    128
};

const unsigned char item_thingy_broken[] = {
   -4, 0, 0xD4, 3,
    4, 0, 0xD5, 3,
    128
};

const unsigned char item_bag[] = {
    0, 0, 0xE3, 3,
    8, 0, 0xE3, 3|OAM_FLIP_H,
    0, 8, 0xF3, 3,
    8, 8, 0xF3, 3|OAM_FLIP_H,
    128
};

const unsigned char item_bag_broken[] = {
    0, 0, 0xB7, 3,
    8, 0, 0xB7, 3|OAM_FLIP_H,
    128
};

const unsigned char item_explosion1[] = {
    0, 0, 0xFD, 0,
    8, 0, 0xFD, 0|OAM_FLIP_H,
    0, 8, 0xFD, 0|OAM_FLIP_V,
    8, 8, 0xFD, 0|OAM_FLIP_H|OAM_FLIP_V,
    -3, 14, 0xFF, 0,
    10, -8, 0xFF, 0,
    4, -10, 0xFE, 0,
    12, 10, 0xFE, 0,
    128
};

const unsigned char item_explosion2[] = {
    -1, 0, 0xFD, 0,
    7, 0, 0xFD, 0|OAM_FLIP_H,
    -1, 8, 0xFD, 0|OAM_FLIP_V,
    7, 8, 0xFD, 0|OAM_FLIP_H|OAM_FLIP_V,
    4, -10, 0xFF, 0,
    12, 10, 0xFF, 0,
    -7, 3, 0xFE, 0,
    15, -4, 0xFE, 0,
    128
};

const unsigned char item_explosion3[] = {
    -1, -1, 0xFD, 0,
    7, -1, 0xFD, 0|OAM_FLIP_H,
    -1, 7, 0xFD, 0|OAM_FLIP_V,
    7, 7, 0xFD, 0|OAM_FLIP_H|OAM_FLIP_V,
    -7, 3, 0xFF, 0,
    15, -4, 0xFF, 0,
    -8, -8, 0xFE, 0,
    6, 17, 0xFE, 0,
    128
};

const unsigned char item_explosion4[] = {
    0, -1, 0xFD, 0,
    8, -1, 0xFD, 0|OAM_FLIP_H,
    0, 7, 0xFD, 0|OAM_FLIP_V,
    8, 7, 0xFD, 0|OAM_FLIP_H|OAM_FLIP_V,
    -8, -8, 0xFF, 0,
    6, 17, 0xFF, 0,
    -3, 14, 0xFE, 0,
    10, -8, 0xFE, 0,
    128
};

const unsigned char * const item_explosion_animation[] = {
    item_explosion1,
    item_explosion2,
    item_explosion3,
    item_explosion4,
};

const unsigned char item_piano_explosion1[] = {
    4, 4, 0xFE, 2,
    4, 12, 0xFE, 2,
    12, 4, 0xFE, 2,
    12, 12, 0xFE, 2,
    128
};

const unsigned char item_piano_explosion2[] = {
    4, 0, 0xFE, 2,
    4, 8, 0xFE, 2,
    12, 0, 0xFE, 2,
    12, 8, 0xFE, 2,
    128
};

const unsigned char item_piano_explosion3[] = {
    4, -4, 0xFE, 2,
    4, 4, 0xFE, 2,
    12, -4, 0xFE, 2,
    12, 4, 0xFE, 2,
    128
};

const unsigned char item_piano_explosion4[] = {
    4, -8, 0xFE, 2,
    4, 0, 0xFE, 2,
    12, -8, 0xFE, 2,
    12, 0, 0xFE, 2,
    128
};

const unsigned char item_piano_explosion5[] = {
    4, -10, 0xFE, 2,
    4, -2, 0xFE, 2,
    12, -10, 0xFE, 2,
    12, -2, 0xFE, 2,
    128
};

const unsigned char item_piano_explosion6[] = {
    2, -8, 0xFE, 2,
    2, 0, 0xFE, 2,
    14, -8, 0xFE, 2,
    14, 0, 0xFE, 2,
    128
};

const unsigned char item_piano_explosion7[] = {
    0, -6, 0xFF, 2,
    0, 2, 0xFF, 2,
    16, -6, 0xFF, 2,
    16, 2, 0xFF, 2,
    128
};

const unsigned char item_piano_explosion8[] = {
    -2, -4, 0xFF, 2,
    -2, 4, 0xFF, 2,
    18, -4, 0xFF, 2,
    18, 4, 0xFF, 2,
    128
};

const unsigned char * const item_piano_explosion_animation[] = {
    item_piano_explosion1,
    item_piano_explosion2,
    item_piano_explosion3,
    item_piano_explosion4,
    item_piano_explosion4,
    item_piano_explosion5,
    item_piano_explosion5,
    item_piano_explosion6,
    item_piano_explosion6,
    item_piano_explosion7,
    item_piano_explosion7,
    item_piano_explosion8,
    item_piano_explosion8,
    item_piano_explosion8,
};

const unsigned char * const igloot_metasprite_defaultdraw_lut[] = {
    item_bag, item_bag_broken,
    item_chiapop, item_chiapop_broken,
    item_umbrella, item_umbrella_broken,
    item_thingy, item_thingy_broken,
    item_potion, item_potion_broken,
    item_coin, item_coin_broken,
    item_bomb0, item_bomb0,
    item_piano, item_piano
};

const unsigned char igloot_hitbox_width_lookup_table[] = {
    16, // Bag
    16, // Chia Pop
    24, // Umbrella
    8,  // Thingy
    16, // Potion
    16, // Coin
    6, // Bomb
    20, // Piano
    16, // Bomb Explosion
    0,  // Piano Explosion
};

const unsigned char igloot_hitbox_height_lookup_table[] = {
    16, // Bag
    8,  // Chia Pop
    8,  // Umbrella
    16, // Thingy
    16, // Potion
    8,  // Coin
    6, // Bomb
    20, // Piano
    16, // Bomb Explosion
    0,  // Piano Explosion
};

const unsigned char igloot_hitbox_x_offset_lookup_table[] = {
    0, // Bag
    0, // Chia Pop
    0, // Umbrella
    0, // Thingy
    0, // Potion
    0,  // Coin
    1, // Bomb
    4, // Piano
    0, // Bomb Explosion
    0, // Piano Explosion
};

const unsigned char igloot_hitbox_y_offset_lookup_table[] = {
    0, // Bag
    0, // Chia Pop
    0, // Umbrella
    0, // Thingy
    0, // Potion
    0, // Coin
    1, // Bomb
    4, // Piano
    0, // Bomb Explosion
    0, // Piano Explosion
};

const unsigned char igloot_point_values[] = {
    1,  // Bag
    2,  // Chia Pop
    3,  // Umbrella
    5,  // Thingy
    10, // Potion
    50  // Coin
};

// Min speed is 0x0080; add this increment N times to igloot_speeds[N]
const unsigned char igloot_speed_increments[] = {
    0x05, // There is no level 0 so this shouldn't actually ever happen
    0x10,
    0x15,
    0x20,
    0x30,
    0x40,
    0x50,
    0x60,
    0x70, // Max speed increment
};

const unsigned int carassa_speeds[] = {
    0x0100,
    0x0100, // 400
    0x0110, // 425
    0x0120, // 450
    0x0130, // 475
    0x0140, // 500
    0x0150, // 525
    0x0160, // 550
    0x0180, // 600,
    0x01A0, // 650
    0x01C0, // 700,
    0x0200, // 800,
    0x0240, // 900,
    0x0280, // 1000,
    0x02C0, // 1100,
    0x0300, // 1200,
};

// 1 row (of 8) per round
const unsigned char item_spawn_frame_delays[] = {
    // min 120, max 204...
    120, 132, 144, 156, 168, 180, 192, 204, // 0
    120, 132, 144, 156, 168, 180, 192, 204, // 1
    // Min 102, max 192...
    102, 115, 128, 141, 153, 166, 179, 192, // 2
    // Min 84, max 180
    84, 98, 111, 125, 139, 153, 166, 180, // 3
    // Min 66, max 168
    66, 81, 95, 110, 124, 139, 153, 168, // 4
    // Min 48, max 156
    48, 63, 79, 94, 110, 125, 141, 156, // 5
    // Min 30, max 144
    30, 46, 63, 79, 95, 111, 128, 114, // 6
    // Min 12, max 126
    12, 28, 45, 61, 77, 93, 110, 126, // 7
    // Min 12, max 108
    12, 16, 39, 53, 67, 81, 94, 108, // 8
    // Min 12, max 90 (except I'm making it 89 so the pattern is pretty)
    12, 23, 34, 45, 56, 67, 78, 89, // 9,
    // Min 12, max 72
    12, 21, 29, 38, 46, 55, 63, 72, // 10
    // Min 12, max 54
    12, 18, 24, 30, 36, 42, 48, 54, // 11
    // Min 12, max 36
    12, 15, 19, 22, 26, 29, 33, 36, // 12 and later
};

#pragma rodata-name(pop)

#pragma rodata-name(push, "BANK5")

// LibLZG'd data either need to be in Bank 5 or be copied to RAM because the LZG decode function is in bank 5
const unsigned char const igloo_game_screen[] = {
	76,90,71,0,0,4,0,0,0,1,85,39,224,114,121,1,
	1,2,3,4,0,0,4,30,0,0,48,48,48,4,201,4,
	1,214,215,4,162,248,249,3,198,0,6,7,8,4,194,22,
	23,24,2,5,15,230,231,3,65,246,247,2,19,70,246,247,
	214,215,3,121,248,244,245,249,228,229,2,16,24,244,245,230,
	231,230,231,214,215,246,247,246,247,227,2,5,99,4,9,3,
	117,4,33,230,231,244,245,244,245,213,2,5,99,4,9,3,
	117,4,33,241,241,242,241,4,6,2,8,2,3,67,3,198,
	2,5,1,2,9,19,2,8,5,4,226,224,224,240,240,182,
	208,208,183,226,240,3,2,224,226,4,1,4,130,240,3,65,
	4,193,3,21,3,3,198,209,209,199,3,69,3,207,2,5,
	20,3,217,2,9,4,2,10,5,182,208,183,182,208,208,226,
	183,2,10,23,4,10,198,209,199,198,209,2,3,78,4,8,
	182,226,2,2,127,2,7,5,3,117,2,3,136,2,3,150,
	3,172,2,9,41,2,5,139,2,7,3,3,93,182,183,3,
	59,2,15,107,182,208,183,2,5,187,198,2,2,108,2,11,
	107,4,1,183,198,2,5,11,2,9,106,3,197,2,4,69,
	199,240,3,57,2,13,106,3,197,2,6,69,2,6,57,2,
	10,248,240,224,2,6,174,4,21,240,2,17,131,4,3,2,
	27,48,4,1,2,22,48,4,22,210,211,212,4,65,4,99,
	4,134,2,5,11,3,200,225,4,30,4,29,4,5,0,0,
	136,170,170,170,0,80,16,0,152,4,225,80,85,5,4,5,
	2,115,75,4,17,
};

#pragma rodata-name(pop);