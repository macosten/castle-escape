#include <stdlib.h>

#include "lib/nesdoug.h"
#include "lib/neslib.h"

#include "asm/macros.h"
#include "asm/score.h"
#include "structs.h"

#include "constants.h"
#include "igloo_constants.h"
#include "igloo_data.h"
#include "menu_screens_constants.h"

// === Extern'd zero page symbols, defined in zeropage.h.
// ZEROPAGE_EXTERN(unsigned char, temp0);
// ZEROPAGE_EXTERN(unsigned char, temp1);
// ZEROPAGE_EXTERN(unsigned char, temp2);
// ZEROPAGE_EXTERN(unsigned char, temp3);
// ZEROPAGE_EXTERN(unsigned char, temp4);
// ZEROPAGE_EXTERN(unsigned int, temp5);
// ZEROPAGE_EXTERN(unsigned int, temp6);
// ZEROPAGE_EXTERN(unsigned char, pad1);
// ZEROPAGE_EXTERN(unsigned char, pad1_new);

// ZEROPAGE_EXTERN(int, address);
// ZEROPAGE_EXTERN(unsigned char, index);
// ZEROPAGE_EXTERN(unsigned char, x);
// ZEROPAGE_EXTERN(unsigned char, y);
// ZEROPAGE_EXTERN(Hitbox, hitbox);
// ZEROPAGE_EXTERN(Hitbox, hitbox2);
// ZEROPAGE_EXTERN(const unsigned char *, temppointer);
// ZEROPAGE_EXTERN(unsigned int, score);
// ZEROPAGE_EXTERN(unsigned int, old_x);
// ZEROPAGE_EXTERN(unsigned int, old_y);

#pragma rodata-name(push, "BANK1")

unsigned char const igloo_palette_sp[] = {
    0x21, 0x06, 0x29, 0x38, // Carassa, Umbrella
    0x21, 0x01, 0x13, 0x21, // Mika, Potion
    0x21, 0x0f, 0x00, 0x30, // Greyscale - stunned Mika, Piano, Chia Pop
    0x21, 0x0f, 0x19, 0x30, // Reserved?
};

unsigned char igloo_palette_bg[] = {
    // Still TBD
    0x00,
};

// Relevant menu function here

#pragma code-name(push, "BANK1")

#pragma code-name(pop)

#pragma rodata-name(pop)
