#include <stdlib.h>

#include "lib/nesdoug.h"
#include "lib/neslib.h"

#include "asm/macros.h"
#include "asm/score.h"
#include "other_macros.h"
#include "structs.h"

#include "constants.h"
#include "igloo_constants.h"
#include "igloo_data.h"
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


// ZEROPAGE_EXTERN(int, address);
// ZEROPAGE_EXTERN(unsigned char, index);
// ZEROPAGE_EXTERN(unsigned char, x);
// ZEROPAGE_EXTERN(unsigned char, y);
ZEROPAGE_EXTERN(Hitbox, hitbox);
ZEROPAGE_EXTERN(Hitbox, hitbox2);
ZEROPAGE_EXTERN(const unsigned char *, temppointer);
ZEROPAGE_EXTERN(unsigned int, score);
ZEROPAGE_EXTERN(unsigned int, old_x);
ZEROPAGE_EXTERN(unsigned int, old_y);
ZEROPAGE_EXTERN(unsigned char, game_mode);
ZEROPAGE_EXTERN(unsigned char, player_flags);
ZEROPAGE_EXTERN(unsigned char, menu);

ZEROPAGE_EXTERN(Player, valrigard);
#define mika valrigard

ZEROPAGE_EXTERN(Player, player2);
#define carassa player2

ZEROPAGE_EXTERN(unsigned char, level_index);
#define level level_index

ZEROPAGE_EXTERN(unsigned char, player_death_timer);
#define player_stun_timer player_death_timer

extern unsigned char shuffle_array[];
extern unsigned char shuffle_leg_size;

// Importing only the enemies_whatever arrays we need to store Doughnutfruit data...
extern unsigned char enemies_x[MAX_ENEMIES]; // Object X coords
extern unsigned char enemies_y[MAX_ENEMIES]; // Object Y coords
extern unsigned char enemies_extra[MAX_ENEMIES]; // Object subpixel Y?
extern unsigned char enemies_type[MAX_ENEMIES];
extern unsigned char enemies_extra2[MAX_ENEMIES]; // Object speed?
extern unsigned char enemies_flags[MAX_ENEMIES];
extern unsigned char enemies_timer[MAX_ENEMIES]; // Timer???
extern unsigned char enemies_count;

extern unsigned char cmap[];

extern unsigned int igloo_1p_high_score;
extern unsigned int igloo_2p_high_score;

#pragma rodata-name(push, "BANK1")

unsigned char const igloo_palette_sp[] = {
    0x21, 0x06, 0x29, 0x38, // Carassa, Umbrella
    0x21, 0x01, 0x13, 0x21, // Mika, Potion
    0x21, 0x0f, 0x00, 0x30, // Greyscale - stunned Mika, Piano, Chia Pop
    0x21, 0x05, 0x35, 0x38, // Pink - bag, thingy, etc
};

unsigned char igloo_palette_bg[] = {
    // Still TBD
    0x31, 0x3c, 0x30, 0x15, // Ice + related
    0x31, 0x07, 0x17, 0x27, // Wood
    0x31, 0x00, 0x10, 0x0f, // Text + ???
    0x31, 0x11, 0x2c, 0x30, // Logo + Secondary snow
};

extern void clear_screen(void);
extern void put_str_sub(void);
extern void set_prg_bank(unsigned char bank);
extern void calculate_shuffle_array(void);
extern void switch_menu(void);

extern void LZG_decode(const unsigned char *src, unsigned char *dest);
extern void prepare_score_string(void);
extern void update_checksum(void);

void game_igloo(void);
void begin_igloo(void);
void begin_igloo_sub(void);
void end_igloo(void);
void igloo_item_movement(void);
void igloo_player_movement(void);
void igloo_sprite_collisions(void);
void igloo_draw_sprites(void);
void igloo_update_score(void);

// Relevant menu function here

void begin_igloo(void) {
    // Change the menu screen so that it becomes the game screen...
    pal_fade_to(4, 0);
    ppu_off();
    clear_screen();

    // Devensive programming: clear these buffers.
    clear_vram_buffer();
    oam_clear();

    // Do stuff that can't be done in PRG1...
    set_prg_bank(5);
    temppointer = igloo_game_screen;
    LZG_decode(temppointer, cmap);
    vram_write(cmap, 32*32);
    memfill(cmap, 0, 32); // For character buffering later on when we want spaces

    set_prg_bank(IGLOO_CODE_BANK);
    begin_igloo_sub();   
}

#pragma code-name(push, "BANK1")

void begin_igloo_sub(void) {
    music_play(ICE_CREAM_MACHINE_SONG);

    game_mode = MODE_GAME;

    mika.velocity_x = 0;
    mika.velocity_y = 0;
    carassa.velocity_x = 0;
    carassa.velocity_y = 0;

    mika.x = 0x8000;
    mika.y = 0xB000;
    carassa.x = 0x8000;
    carassa.y = 0x2000;

    level = 0;
    //level_timer = LEVEL_FRAME_LENGTH;
    player_stun_timer = 0;

    seed_rng();
    srand(rand8());

    igloo_update_score();

    ppu_on_all();
    pal_bright(4);
}

void game_igloo(void) {
    //IGLOO_RESET_PLAYER_FLAGS_START_FRAME();

    pad1 = pad_poll(0); // read the first controller
    pad1_new = get_pad_new(0);

    ppu_wait_nmi(); // wait till beginning of the frame

    clear_vram_buffer();

    // If not paused:
    set_prg_bank(IGLOO_CODE_BANK);

    igloo_player_movement();
    igloo_item_movement();
    igloo_sprite_collisions();
    igloo_draw_sprites();

    if (IGLOO_SCORE_CHANGED_THIS_FRAME) { igloo_update_score(); }

    if (pad1 & PAD_B) {
        end_igloo();
        return;
    }
    //gray_line();
}

void end_igloo(void) {
    pal_fade_to(4, 0);
    menu = MENU_IGLOO;
    switch_menu();
    music_play(MENU_SONG);
    pal_bright(4);
}


void igloo_item_movement(void) {

}

void igloo_update_score(void) {
    convert_to_decimal(score);
    prepare_score_string();
    multi_vram_buffer_horz(score_string, 5, NTADR_A(10, 2)); // Change, or if it doesn't change, just call hasee_update_score for code reuse as this is in the same bank
}

void igloo_player_movement(void) {

}

void igloo_sprite_collisions(void) {

}

void igloo_draw_sprites(void) {
    // clear all sprites from sprite buffer
    oam_clear();

    // Mika
    oam_meta_spr(high_byte(mika.x), high_byte(mika.y), mika_idle);

    // Carassa
    oam_meta_spr(high_byte(carassa.x), high_byte(carassa.y), carassa_idle);


    // Items

}

#pragma code-name(pop)

#pragma rodata-name(pop)
