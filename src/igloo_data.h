// Do not try to use pointers from another bank while that bank isn't 
// swapped in the correct place in RAM.
// Otherwise, you'll get garbage data.

#pragma rodata-name(push, "BANK1")

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
  8,24, 0xC0, 0|OAM_FLIP_H,
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
  0,24, 0xC1, 0,
  8,24, 0xC0, 0,
 16,24, 0xD1, 0|OAM_FLIP_H,
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

const unsigned char item_piano[] = {
    8, 0, 0x85, 2,
   16, 0, 0x86, 2,
    0, 8, 0x94, 2,
    8, 8, 0x95, 2,
   16, 8, 0x96, 2,
    0,16, 0xA4, 2,
    8,16, 0xA5, 2,
   16,16, 0xA4, 2|OAM_FLIP_H,
   128 
};

const unsigned char item_chiapop[] = {
    0, 0, 0x5E, 2,
    8, 0, 0x5F, 2,
    128
};

const unsigned char item_coin[] = {
    0, 0, 0x5C, 0,
    8, 0, 0x5D, 0,
    128
};

const unsigned char item_umbrella[] = {
    0, 0, 0x4C, 0,
    8, 0, 0x4D, 0,
   16, 0, 0x4E, 0,
   128
};

const unsigned char item_bomb[] = {
    0, 0, 0xE0, 2,
    8, 0, 0xE1, 2,
    0, 8, 0xF0, 2,
    8, 8, 0xF1, 2,
    128
};

const unsigned char item_thingy[] = {
    0, 0, 0xE2, 3,
    0, 0, 0xF2, 3,
    128
};

const unsigned char item_bag[] = {
    0, 0, 0xE3, 3,
    8, 0, 0xE3, 3|OAM_FLIP_H,
    0, 8, 0xE4, 3,
    8, 8, 0xE4, 3|OAM_FLIP_H,
    128
};

#pragma rodata-name(pop)