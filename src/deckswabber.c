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

ZEROPAGE_EXTERN(unsigned char, player_frame_timer);

extern unsigned char cmap[];
#define tile_typemap (cmap)
#define tile_colormap (cmap + 64)
#define enemymap (cmap + 128)
#define chrbuffer (cmap + 192)

ZEROPAGE_EXTERN(unsigned char, temp_x);
ZEROPAGE_EXTERN(unsigned char, temp_y);
#define player_tile_x (temp_x)
#define player_tile_y (temp_y)

ZEROPAGE_EXTERN(unsigned char, eject_L);
#define tile_color_increment_type eject_L

extern unsigned int deckswabber_1p_high_score;
extern unsigned int previous_score;

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
    0x37, 0x12, 0x2B, 0x39, // Blue + Green Tiles
    0x37, 0x15, 0x24, 0x33, // Red + Pink + Periwinkle (Red/Pink board)
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

void begin_deckswabber_level(void);

void deckswabber_player_movement(void);

void deckswabber_draw_sprites(void);
void deckswabber_draw_player(void);
void deckswabber_redraw_player_tile(void);

void deckswabber_tile_increment_fn_original_round1(void);
void deckswabber_tile_increment_fn_original_round2(void);
void deckswabber_tile_increment_fn_original_round3(void);
void deckswabber_tile_increment_fn_original_round4(void);
void deckswabber_tile_increment_fn_original_round5(void);
void deckswabber_tile_increment_fn_original_round6(void);
void deckswabber_tile_increment_fn_original_round7(void);
void deckswabber_tile_increment_fn_original_round8(void);

// Lookup table for behavior when landing on a tile. Assume that player_tile_x and player_tile_y is correctly set when called.
const void (* const tile_increment_functions[])(void) = {
    deckswabber_tile_increment_fn_original_round1,
    deckswabber_tile_increment_fn_original_round2,
    deckswabber_tile_increment_fn_original_round3,
    deckswabber_tile_increment_fn_original_round4,
    deckswabber_tile_increment_fn_original_round5,
    deckswabber_tile_increment_fn_original_round6,
    deckswabber_tile_increment_fn_original_round7,
    deckswabber_tile_increment_fn_original_round8,
};

void begin_deckswabber(void) {
    // Change the menu screen so that it becomes the game screen...
    pal_fade_to(4, 0);
    prepare_screen_transition();

    // Do stuff that can't be done in PRG1...
    set_prg_bank(5);
    temppointer = deckswabber_game_screen;
    LZG_decode(temppointer, cmap);
    vram_write(cmap, 32*32);

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

    set_mt_pointer(deckswabber_metatiles);

    // Begin first level...
    begin_deckswabber_level();

    ppu_on_all();
    pal_bright(4);
}

void begin_deckswabber_level(void) {
    // Reset tile typemap, tile colormap, tile enemymap, and chrbuffer
    // TODO - make it so that we only reset colormap and later, we can just write over typemap once on load
    memfill(cmap, 0, 64 + 64 + 64 + 32); // 64 tile types + 64 tile colors + 64 game board enemy presences + 32 characters for text buffering

    // Reset all PPU palette entries on the board
    // ...

    // (Re)draw the "goal" indicator, if applicable?
    // ...

    // Increment the starting tile by 1 and update it

    // X and Y coordinates for this game will be in tiles; the drawing routines will figure out where they belong on-screen...
    player_tile_x = 0;
    player_tile_y = 0;

    // This should become some kind of lookup to the update function
    tile_color_increment_type = 0; // Set based on some level info
    AsmCallFunctionAtPtrOffsetByIndexVar(tile_increment_functions, eject_L);
    deckswabber_redraw_player_tile();
}

void game_deckswabber(void) {
    pad1 = pad_poll(0); // read the first controller
    pad1_new = get_pad_new(0);

    ppu_wait_nmi(); // wait till beginning of the frame

    clear_vram_buffer();

    // If not paused:
    set_prg_bank(DECKSWABBER_CODE_BANK);

    deckswabber_player_movement();

    deckswabber_draw_sprites();

    if (pad1 & PAD_B) {
        end_deckswabber();
        return;
    }

    if (pad1_new & PAD_START) {
        ++tile_color_increment_type; // Cycle through for debug
        tile_color_increment_type &= 0b111; // 0-7
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

void deckswabber_player_movement(void) {
    temp0 = 0;
    if (pad1_new & PAD_UP && player_tile_y > 0) {
        player_tile_y -= 1;
        temp0 = 1;
    } else if (pad1_new & PAD_DOWN && player_tile_y < DECKSWABBER_TILE_HEIGHT-1) {
        player_tile_y += 1;
        temp0 = 1;
    } else if (pad1_new & PAD_LEFT && player_tile_x > 0) {
        player_tile_x -= 1;
        temp0 = 1;
    } else if (pad1_new & PAD_RIGHT && player_tile_x < DECKSWABBER_TILE_WIDTH-1) {
        player_tile_x += 1;
        temp0 = 1;
    }
    if (temp0) {
        sfx_play(SFX_JUMP, 0);
        player_frame_timer = 12;
        // Update new tile if necessary
        AsmCallFunctionAtPtrOffsetByIndexVar(tile_increment_functions, eject_L);
        // Update the tile under the player
        deckswabber_redraw_player_tile();
    }
}

void deckswabber_draw_sprites(void) {
    set_prg_bank(DECKSWABBER_METASPRITE_BANK);
    oam_clear();

    // This will need to be made to draw things from "front" to "back", but for now:
    deckswabber_draw_player();
}

void deckswabber_draw_player(void) {
    if (player_frame_timer) {
        --player_frame_timer;
    }
    temp0 = player_frame_timer >> 2;
    AsmSet2ByteFromPtrAtIndexVar(temppointer, blumaroo_jump_animation, temp0);
    temp0 = 64 + (player_tile_x << 4);
    temp1 = 54 + (player_tile_y << 4);
    oam_meta_spr(temp0, temp1, temppointer);
}

// Tile increment functions:
// - Modify the color of the tile
// - set temp0 to the tile's index in the tilemaps (DeckswabberGetTileIndex(temp0, ..., ...))
// - set temp1 to the just-set color of the tile, in case something else wants to know
void deckswabber_tile_increment_fn_original_round1(void) {
    DeckswabberGetTileIndex(temp0, temp_x, temp_y);
    tile_colormap[temp0] = 1;
    temp1 = 1;
}

void deckswabber_tile_increment_fn_original_round2(void) {
    DeckswabberGetTileIndex(temp0, temp_x, temp_y);
    temp1 = tile_colormap[temp0];
    temp1 ^= 1;
    tile_colormap[temp0] = temp1;
}

void deckswabber_tile_increment_fn_original_round3(void) {
    DeckswabberGetTileIndex(temp0, temp_x, temp_y);
    temp1 = tile_colormap[temp0] + 1;
    if (temp1 > 2) { temp1 = 1; }
    tile_colormap[temp0] = temp1;
}

void deckswabber_tile_increment_fn_original_round4(void) {
    DeckswabberGetTileIndex(temp0, temp_x, temp_y);
    temp1 = tile_colormap[temp0] + 1;
    if (temp1 > 2) { temp1 = 0; }
    tile_colormap[temp0] = temp1;
}

void deckswabber_tile_increment_fn_original_round5(void) {
    DeckswabberGetTileIndex(temp0, temp_x, temp_y);
    temp1 = tile_colormap[temp0] + 1;
    if (temp1 > 3) { temp1 = 2; }
    tile_colormap[temp0] = temp1;
}

void deckswabber_tile_increment_fn_original_round6(void) {
    DeckswabberGetTileIndex(temp0, temp_x, temp_y);
    temp1 = tile_colormap[temp0] + 1;
    if (temp1 > 3) { temp1 = 0; }
    tile_colormap[temp0] = temp1;
}

void deckswabber_tile_increment_fn_original_round7(void) {
    DeckswabberGetTileIndex(temp0, temp_x, temp_y);
    temp1 = tile_colormap[temp0] + 1;
    if (temp1 > 4) { temp1 = 3; }
    tile_colormap[temp0] = temp1;
}

void deckswabber_tile_increment_fn_original_round8(void) {
    DeckswabberGetTileIndex(temp0, temp_x, temp_y);
    temp1 = tile_colormap[temp0] + 1;
    if (temp1 > 4) { temp1 = 0; }
    tile_colormap[temp0] = temp1;
}

void deckswabber_redraw_player_tile(void) {
    temp2 = 8 + (player_tile_x << 1);
    temp3 = 8 + (player_tile_y << 1);
    address = NTADR_A(temp2, temp3);
    buffer_1_mt(address, temp1);
}

#pragma code-name(pop)
#pragma rodata-name(pop)
