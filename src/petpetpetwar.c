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
#include "petpetpetwar_constants.h"
#include "petpetpetwar_data.h"
#include "menu_screens_constants.h"

// === Extern'd zero page symbols, defined in zeropage.h.
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
ZEROPAGE_EXTERN(unsigned char, menu_selection);

ZEROPAGE_EXTERN(Player, valrigard);
#define player1 valrigard

extern unsigned char shuffle_array[];
extern unsigned char shuffle_leg_size;

extern unsigned char enemies_x[MAX_ENEMIES];
extern unsigned char enemies_y[MAX_ENEMIES];
extern unsigned char enemies_extra[MAX_ENEMIES];
extern unsigned char enemies_type[MAX_ENEMIES];
extern unsigned char enemies_extra2[MAX_ENEMIES];
extern unsigned char enemies_flags[MAX_ENEMIES];
extern unsigned char enemies_timer[MAX_ENEMIES];
extern unsigned char enemies_count;

extern unsigned char cmap[];

extern unsigned int petpetpetwar_1p_high_score;

#pragma rodata-name(push, "BANK1")

// TODO - I think it may eventually be wise to make sure these map to the same spaces in RAM as the other mutable palettes?
unsigned char petpetpetwar_palette_sp[] = {
    0x21, 0x0f, 0x29, 0x38, // Mootix Player
    0x21, 0x0f, 0x00, 0x00, // ??
    0x21, 0x0f, 0x00, 0x00, // ??
    0x21, 0x0f, 0x00, 0x00, // ??
};

unsigned char petpetpetwar_palette_bg[] = {
    0x21, 0x0f, 0x00, 0x00, // ??
    0x21, 0x0f, 0x00, 0x00, // ??
    0x21, 0x0f, 0x00, 0x00, // ??
    0x21, 0x0f, 0x00, 0x00, // ??
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

void game_petpetpetwar(void);
void begin_petpetpetwar(void);
void begin_petpetpetwar_sub(void);
void end_petpetpetwar(void);

void begin_petpetpetwar(void) {
    pal_fade_to(4, 0);
    prepare_screen_transition();

    set_prg_bank(5);
    //temppointer = petpetpetwar_game_screen;
    //LZG_decode(temppointer, cmap);
    vram_write(cmap, 32*32);

    set_prg_bank(PETPETPETWAR_CODE_BANK);
    begin_petpetpetwar_sub();
}

void begin_petpetpetwar_sub(void) {
    // ...
}

void game_petpetpetwar(void) {
    // PETPETPETWAR_RESET_PLAYER_FLAGS_START_FRAME();

    pad1 = pad_poll(0);
    pad1_new = get_pad_new(0);

    ppu_wait_nmi();
    clear_vram_buffer();

    set_prg_bank(PETPETPETWAR_CODE_BANK);

    // ...gameplay here

    //gray_line();
}

void end_petpetpetwar(void) {
    pal_fade_to(4, 0);
    menu = MENU_PETPETPETWAR;
    switch_menu();
    music_play(MENU_SONG);
    pal_bright(4);
}
