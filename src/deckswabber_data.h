// Do not try to use pointers from another bank while that bank isn't 
// swapped in the correct place in RAM.
// Otherwise, you'll get garbage data.

#pragma rodata-name(push, "BANK1")

const unsigned char blumaroo_idle[] = {
    0, 0, 0x00, 0,
    8, 0, 0x01, 0,
    0, 8, 0x10, 0,
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

extern const unsigned char * const item_explosion_animation[];

#pragma rodata-name(pop);

#pragma rodata-name(push, "BANK5")

// LibLZG'd data either need to be in Bank 5 or be copied to RAM because the LZG decode function is in bank 5
const unsigned char const deckswabber_game_screen[] = {
	76,90,71,0,0,4,0,0,0,1,51,129,116,98,120,1,
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
	3,69,120,7,21,3,66,120,152,153,150,209,3,15,150,7,
	2,197,209,3,66,120,85,7,5,81,20,5,5,0,0,64,
	85,85,1,3,2,81,4,85,85,3,4,90,7,247,6,37,
	7,2,5,
};

#pragma rodata-name(pop);