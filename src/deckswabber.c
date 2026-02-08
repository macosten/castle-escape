#include "lib/nesdoug.h"
#include "lib/neslib.h"

#include "asm/macros.h"
#include "asm/score.h"
#include "enemy_macros.h"
#include "other_macros.h"
#include "player_macros.h"
#include "structs.h"

#include "constants.h"
#include "deckswabber_constants.h"
#include "deckswabber_data.h"
#include "menu_screens_constants.h"

ZEROPAGE_EXTERN(unsigned char, temp0);
ZEROPAGE_EXTERN(unsigned char, temp1);
ZEROPAGE_EXTERN(unsigned char, temp2);
ZEROPAGE_EXTERN(unsigned char, temp3);
ZEROPAGE_EXTERN(unsigned char, temp4);
ZEROPAGE_EXTERN(unsigned int, temp5);
ZEROPAGE_EXTERN(unsigned int, temp6);
ZEROPAGE_EXTERN(unsigned char, pad1);
ZEROPAGE_EXTERN(unsigned char, pad1_new);
ZEROPAGE_EXTERN(unsigned char, pad2);
ZEROPAGE_EXTERN(unsigned char, pad2_new);
ZEROPAGE_EXTERN(int, address);
ZEROPAGE_EXTERN(unsigned char, index);
ZEROPAGE_EXTERN(unsigned char, x);
ZEROPAGE_EXTERN(unsigned char, y);
ZEROPAGE_EXTERN(Player, player2);
ZEROPAGE_EXTERN(Hitbox, hitbox);
ZEROPAGE_EXTERN(Hitbox, hitbox2);
ZEROPAGE_EXTERN(const unsigned char *, temppointer);
ZEROPAGE_EXTERN(unsigned int, score);

#pragma rodata-name(push, "BANK1")
#pragma code-name(push, "BANK1")

unsigned char deckswabber_palette_sp[] = {
    0x21, 0x0f, 0x38, 0x29, // ???
    0x21, 0x0f, 0x23, 0x26, // ???
    0x21, 0x0f, 0x00, 0x00, // ???
    0x21, 0x0f, 0x00, 0x00, // ???
};

unsigned char const deckswabber_palette_bg[] = {
    0x37, 0x17, 0x27, 0x0f, // Wood / Brown tiles / Paper / text
    0x37, 0x01, 0x2B, 0x39, // Blue + Green Tiles
    0x37, 0x11, 0x21, 0x0f, // Water tiles + black (for parchment HUD)
    0x37, 0x37, 0x30, 0x0f, // ?????
};