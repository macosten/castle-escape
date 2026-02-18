#include <stdlib.h>

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
ZEROPAGE_EXTERN(unsigned int, old_x);
ZEROPAGE_EXTERN(unsigned int, old_y);
ZEROPAGE_EXTERN(unsigned char, game_mode);
ZEROPAGE_EXTERN(unsigned char, player_flags);
ZEROPAGE_EXTERN(unsigned char, player_flags2);
ZEROPAGE_EXTERN(unsigned char, menu);
ZEROPAGE_EXTERN(unsigned char, shuffle_offset);
ZEROPAGE_EXTERN(unsigned char, shuffle_maximum);
ZEROPAGE_EXTERN(unsigned char, temp_x);
ZEROPAGE_EXTERN(unsigned char, temp_y);

extern unsigned char cmap[];

extern unsigned int deckswabber_1p_high_score;

#pragma rodata-name(push, "BANK1")

unsigned char deckswabber_palette_sp[] = {
    0x37, 0x1c, 0x38, 0x0f, // Player Character (Teal/Yellow/Black)
    0x21, 0x3d, 0x16, 0x0f, // Grey + Brown (Sword, Boxes, Cannon, Coins/Chests etc)
    0x21, 0x30, 0x27, 0x0f, // White + Gold (Kiko Skeleton, Gold Coins/Chests, flags)
    0x21, 0x0f, 0x00, 0x00, // Reserved for Mynci/Techo color palette
};

// Techo Pirate - 0x2A, 0x26, 0x0f
// Mynci Pirate - 0x14, 0x36, 0x0f

unsigned char const deckswabber_palette_bg[] = {
    0x37, 0x17, 0x27, 0x0f, // Wood / Brown tiles / Paper / text
    0x37, 0x11, 0x21, 0x0f, // Water tiles + black (for parchment HUD)
    0x37, 0x01, 0x2B, 0x39, // Blue + Green Tiles
    0x37, 0x37, 0x30, 0x0f, // ?????
};

extern void clear_screen(void);
extern void put_str_sub(void);
extern void set_prg_bank(unsigned char bank);
extern void calculate_shuffle_array(void);
extern void prepare_screen_transition(void);
extern void switch_menu(void);

extern void LZG_decode(const unsigned char *src, unsigned char *dest);
extern void prepare_score_string(void);
extern void update_checksum(void);

void game_deckswabber(void);
void begin_deckswabber(void);
void begin_deckswabber_sub(void);
void end_deckswabber(void);

void begin_deckswabber(void) {
    // Change the menu screen so that it becomes the game screen...
    pal_fade_to(4, 0);
    prepare_screen_transition();

    // Do stuff that can't be done in PRG1...
    set_prg_bank(5);
    temppointer = deckswabber_game_screen;
    LZG_decode(temppointer, cmap);
    vram_write(cmap, 32*32);
    memfill(cmap, 0, 32); // For character buffering later on when we want spaces

    set_prg_bank(DECKSWABBER_CODE_BANK);
    begin_deckswabber_sub();
}

#pragma code-name(push, "BANK1")

void begin_deckswabber_sub(void) {
    music_play(ICE_CREAM_MACHINE_SONG);
    game_mode = MODE_GAME;
    // ...

    seed_rng();
    srand(rand8());

    ppu_on_all();
    pal_bright(4);
}

void game_deckswabber(void) {
    pad1 = pad_poll(0); // read the first controller
    pad1_new = get_pad_new(0);

    ppu_wait_nmi(); // wait till beginning of the frame

    clear_vram_buffer();

    // If not paused:
    set_prg_bank(DECKSWABBER_CODE_BANK);

    if (pad1 & PAD_B) {
        end_deckswabber();
        return;
    }
    // gray_line();
}

void end_deckswabber(void) {
    pal_fade_to(4, 0);
    menu = MENU_DECKSWABBER;
    switch_menu();
    music_play(MENU_SONG);
    pal_bright(4);
}

#pragma code-name(pop)
#pragma rodata-name(pop)
