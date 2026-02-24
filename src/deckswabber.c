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
ZEROPAGE_EXTERN(const unsigned char *, temppointer1);
ZEROPAGE_EXTERN(unsigned int, score);
ZEROPAGE_EXTERN(unsigned int, old_x);
ZEROPAGE_EXTERN(unsigned int, old_y);
ZEROPAGE_EXTERN(unsigned char, game_mode);
ZEROPAGE_EXTERN(unsigned char, player_flags);
ZEROPAGE_EXTERN(unsigned char, player_flags2);
ZEROPAGE_EXTERN(unsigned char, menu);
ZEROPAGE_EXTERN(unsigned char, shuffle_offset);
ZEROPAGE_EXTERN(unsigned char, shuffle_maximum);
ZEROPAGE_EXTERN(unsigned char, energy);
ZEROPAGE_EXTERN(unsigned char, player_frame_timer);

extern unsigned char cmap[];
#define tile_typemap (cmap)
#define tile_colormap (cmap + 64)
#define enemymap (cmap + 128)
#define chrbuffer (cmap + 192)

ZEROPAGE_EXTERN(unsigned char, temp_x);
ZEROPAGE_EXTERN(unsigned char, temp_y);
#define player_tile_x (old_x)
#define player_tile_y (old_y)

ZEROPAGE_EXTERN(unsigned char, eject_L);
#define tile_color_increment_type eject_L

ZEROPAGE_EXTERN(unsigned char, eject_R);
#define tiles_remaining eject_R

ZEROPAGE_EXTERN(unsigned char, eject_U);
#define round eject_U

ZEROPAGE_EXTERN(unsigned char, level_index);
#define level level_index

ZEROPAGE_EXTERN(unsigned char, enemy_is_using_bg_collision);
#define transition_timer enemy_is_using_bg_collision

ZEROPAGE_EXTERN(unsigned char, coordinates);
#define level_pack_index coordinates

extern unsigned int deckswabber_1p_high_score;
extern unsigned int previous_score;

const unsigned char * const * deckswabber_active_level_pack_levels;
const unsigned char * deckswabber_active_level_pack_header;

#if (DECKSWABBER_TILE_WIDTH != 8 || DECKSWABBER_TILE_HEIGHT != 8)
    #warning "Careful (deckswabber): Odd width and height, the code probably doesn't support this yet."
#endif

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

extern void igloo_write_decimal_in_message_sub(void);

void game_deckswabber(void);
void begin_deckswabber(void);
void begin_deckswabber_sub(void);
void end_deckswabber(void);

void begin_deckswabber_level(void);

void deckswabber_player_movement(void);

void deckswabber_draw_sprites(void);
void deckswabber_draw_player(void);
void deckswabber_redraw_player_tile(void);
// Note: temp_x and temp_y should be set to the values to update the byte for, in deckswabber tile coordinates.
void deckswabber_update_attribute_byte(void);
void deckswabber_update_score(void);
void deckswabber_update_tiles_remaining(void);
void deckswabber_write_finished_message(void);

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

    level = 0;
    round = 0;

    // Temporary/Debug: Hardcode selected level pack
    level_pack_index = 0;

    deckswabber_active_level_pack_levels = deckswabber_level_data_db[level_pack_index];

    pal_bright(4);

    // Begin first level...
    begin_deckswabber_level();    
}

void begin_deckswabber_level(void) {
    ppu_off();
    // Reset tile typemap, tile colormap, tile enemymap, and chrbuffer
    set_prg_bank(DECKSWABBER_CODE_BANK);
    // Figure out what the tilemap should be (from the current options...)
    temppointer = deckswabber_active_level_pack_levels[level];//deckswabber_active_level_pack_levels[level];
    
    tiles_remaining = DECKSWABBER_TILE_HEIGHT * DECKSWABBER_TILE_WIDTH;
    
    x = 8;
    y = 8;
    for (index = 0; index < (DECKSWABBER_TILE_HEIGHT * DECKSWABBER_TILE_WIDTH); index += 2) {
        clear_vram_buffer();
        // Bitpacked: 2 tiles per byte
        temp4 = index >> 1;
        temp0 = temppointer[temp4] >> 4;
        temp1 = temppointer[temp4] & 0x0F;

        temp0 = deckswabber_nibble_to_tile_id_map[temp0];
        temp1 = deckswabber_nibble_to_tile_id_map[temp1];

        if (temp0 == DECKSWABBER_WATER_HOLE_ID || temp0 == DECKSWABBER_EMPTY_HOLE_ID) {
            --tiles_remaining;
        }

        if (temp1 == DECKSWABBER_WATER_HOLE_ID || temp1 == DECKSWABBER_EMPTY_HOLE_ID) {
            --tiles_remaining;
        }

        tile_typemap[index] = temp0;
        temp2 = deckswabber_metatile_palettes[temp0];
        tile_colormap[index] = temp0;
        enemymap[index] = 0;
        
        address = NTADR_A(x, y);
        buffer_1_mt(address, temp0);

        //address += 2; // Equivalent to x += 2 and then recalc'ing address, since we know it'll never skip to the next row here (x would be odd)
        x += 2;

        tile_typemap[index + 1] = temp1;
        temp2 = deckswabber_metatile_palettes[temp1];
        tile_colormap[index + 1] = temp1;
        enemymap[index + 1] = 0;
        
        address = NTADR_A(x, y);
        buffer_1_mt(address, temp1);

        x += 2;
        if (x >= 24) {
            x = 8;
            y += 2;
        }
        flush_vram_update_nmi();
    }

    index = 0;
    // Draw attribute table bytes
    
    for (y = 0; y < DECKSWABBER_TILE_HEIGHT; y += 1) {
        for (x = 0; x < DECKSWABBER_TILE_WIDTH; x += 1) {
            clear_vram_buffer();
            temp_x = x;
            temp_y = y;
            DeckswabberGetTileIndex(temp0, temp_x, temp_y);
            deckswabber_update_attribute_byte();
            flush_vram_update_nmi();
        }
    }

    transition_timer = 120;
    
    // Clear the rest of relevant memory
    memfill(tile_colormap, 0, 64 + 64 + 32);

    // (Re)draw the "goal" indicator, if applicable?
    // ...

    energy = 192; // 6 HUD tiles that take up the visual HP bar * 32 "health" per tile (16 per "tick")
    // Redraw the energy meter

    // X and Y coordinates for this game will be in tiles; the drawing routines will figure out where they belong on-screen...
    player_tile_x = 0;
    player_tile_y = 0;

    // Increment the starting tile by 1 and update it
    tile_color_increment_type = round; // Set based on some level info
    
    temp_x = player_tile_x;
    temp_y = player_tile_y;
    AsmCallFunctionAtPtrOffsetByIndexVar(tile_increment_functions, eject_L);
    deckswabber_redraw_player_tile();

    multi_vram_buffer_horz(deckswabber_tiles_remaining, 10, NTADR_A(17, 4));
    deckswabber_update_tiles_remaining();

    ppu_on_all();
}

void game_deckswabber(void) {
    DECKSWABBER_RESET_PLAYER_FLAGS_START_FRAME();

    pad1 = pad_poll(0); // read the first controller
    pad1_new = get_pad_new(0);

    ppu_wait_nmi(); // wait till beginning of the frame

    clear_vram_buffer();

    // If not paused:
    set_prg_bank(DECKSWABBER_CODE_BANK);

    if (tiles_remaining > 0) {
        deckswabber_player_movement();
    } else {
        if (transition_timer == 120) {
            deckswabber_write_finished_message();
        }
        if (transition_timer) {
            --transition_timer;
        } else if (pad1_new) {
            ++level;
            temp0 = round << 1;   
            temppointer1 = deckswabber_round_bounds_db[level_pack_index];
            temp2 = temppointer1[temp0 + 1]; // Maximum level in round
            if (level >= temp2) {
                // Increment round
                level = temppointer1[temp0]; // Minimum level in round
                ++round;
                if (round >= deckswabber_maximum_round_db[level_pack_index]) {
                    // Todo - write some kind of congrats message for winning
                    end_deckswabber();
                    return;
                }
            }
            begin_deckswabber_level(); // Next Level (todo)
            return;
        }
    }

    deckswabber_draw_sprites();

    if (DECKSWABBER_SCORE_CHANGED_THIS_FRAME) { deckswabber_update_score(); }

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
        // Update new tile if necessary (if this is a non-hazard tile)
        DeckswabberGetTileIndex(temp0, old_x, old_y);
        temp1 = tile_typemap[temp0];
        if (temp1 < DECKSWABBER_WATER_HOLE_ID) {
            temp_x = player_tile_x;
            temp_y = player_tile_y;
            AsmCallFunctionAtPtrOffsetByIndexVar(tile_increment_functions, eject_L);
            // Update the tile under the player
            deckswabber_redraw_player_tile();
            deckswabber_update_tiles_remaining();
        }
    }
}

void deckswabber_update_score(void) {
    convert_to_decimal(score);
    prepare_score_string();
    multi_vram_buffer_horz(score_string, 5, NTADR_A(4, 3));
}

void deckswabber_update_tiles_remaining(void) {
    convert_to_decimal(tiles_remaining);
    prepare_score_string();
    multi_vram_buffer_horz(score_string+3, 2, NTADR_A(21, 5));
}

void deckswabber_write_finished_message(void) {
    multi_vram_buffer_horz(deckswabber_level_finished, 10, NTADR_A(17, 4));
    multi_vram_buffer_horz(deckswabber_bonus, 6, NTADR_A(17, 5));

    // ...calculate bonus here
    // ...temporary
    convert_to_decimal(energy);
    igloo_write_decimal_in_message_sub(); // Note: this writes to cmap+64. It shouldn't matter by the time we call this, though.
    multi_vram_buffer_horz(cmap+64, temp0, NTADR_A(24, 5));
}

void deckswabber_draw_sprites(void) {
    set_prg_bank(DECKSWABBER_METASPRITE_BANK);
    oam_clear();

    // Idea:
    // Have an array of 8 bytes that serves as a 1-d version of enemymap
    // each bit is 1 if that enemy slot is in that row, or 0 if not

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
// Assume temp0 is set with DeckswabberGetTileIndex
// - Modify the color of the tile
// - set temp0 to the tile's index in the tilemaps (DeckswabberGetTileIndex(temp0, ..., ...))
// - set temp1 to the just-set color of the tile, in case something else wants to know
// - set temp2 to the previous color of the tile
void deckswabber_tile_increment_fn_original_round1(void) {
    DeckswabberGetTileIndex(temp0, temp_x, temp_y);
    temp2 = tile_colormap[temp0];
    tile_colormap[temp0] = 1;
    if (temp2 != 1) {
        --tiles_remaining;
    }
    temp1 = 1;
}

void deckswabber_tile_increment_fn_original_round2(void) {
    DeckswabberGetTileIndex(temp0, temp_x, temp_y);
    temp2 = tile_colormap[temp0];
    temp1 = temp2;
    temp1 ^= 1;
    tile_colormap[temp0] = temp1;
    if (temp1 == 1) {
        --tiles_remaining;
    } else if (temp2 == 1) {
        ++tiles_remaining;
    }
}

void deckswabber_tile_increment_fn_original_round3(void) {
    DeckswabberGetTileIndex(temp0, temp_x, temp_y);
    temp2 = tile_colormap[temp0];
    temp1 = temp2 + 1;
    if (temp1 > 2) { temp1 = 1; }
    tile_colormap[temp0] = temp1;
    if (temp1 == 2) {
        --tiles_remaining;
    } else if (temp2 == 2) {
        ++tiles_remaining;
    }
}

void deckswabber_tile_increment_fn_original_round4(void) {
    DeckswabberGetTileIndex(temp0, temp_x, temp_y);
    temp2 = tile_colormap[temp0];
    temp1 = temp2 + 1;
    if (temp1 > 2) { temp1 = 0; }
    tile_colormap[temp0] = temp1;
    if (temp1 == 2) {
        --tiles_remaining;
    } else if (temp2 == 2) {
        ++tiles_remaining;
    }
}

void deckswabber_tile_increment_fn_original_round5(void) {
    DeckswabberGetTileIndex(temp0, temp_x, temp_y);
    temp2 = tile_colormap[temp0];
    temp1 = temp2 + 1;
    if (temp1 > 3) { temp1 = 2; }
    tile_colormap[temp0] = temp1;
    if (temp1 == 3) {
        --tiles_remaining;
    } else if (temp2 == 3) {
        ++tiles_remaining;
    }
}

void deckswabber_tile_increment_fn_original_round6(void) {
    DeckswabberGetTileIndex(temp0, temp_x, temp_y);
    temp2 = tile_colormap[temp0];
    temp1 = temp2 + 1;
    if (temp1 > 3) { temp1 = 0; }
    tile_colormap[temp0] = temp1;
    if (temp1 == 3) {
        --tiles_remaining;
    } else if (temp2 == 3) {
        ++tiles_remaining;
    }
}

void deckswabber_tile_increment_fn_original_round7(void) {
    DeckswabberGetTileIndex(temp0, temp_x, temp_y);
    temp2 = tile_colormap[temp0];
    temp1 = temp2 + 1;
    if (temp1 > 4) { temp1 = 3; }
    tile_colormap[temp0] = temp1;
    if (temp1 == 4) {
        --tiles_remaining;
    } else if (temp2 == 4) {
        ++tiles_remaining;
    }
}

void deckswabber_tile_increment_fn_original_round8(void) {
    DeckswabberGetTileIndex(temp0, temp_x, temp_y);
    temp2 = tile_colormap[temp0];
    temp1 = temp2 + 1;
    if (temp1 > 4) { temp1 = 0; }
    tile_colormap[temp0] = temp1;
    if (temp1 == 4) {
        --tiles_remaining;
    } else if (temp2 == 4) {
        ++tiles_remaining;
    }
}

void deckswabber_redraw_player_tile(void) {
    temp2 = 8 + (player_tile_x << 1);
    temp3 = 8 + (player_tile_y << 1);
    address = NTADR_A(temp2, temp3);
    buffer_1_mt(address, temp1);
    temp_x = player_tile_x;
    temp_y = player_tile_y;
    deckswabber_update_attribute_byte();
}

void deckswabber_update_attribute_byte(void) {
    // Figure out the attribute table update address
    address = ATTRIBTABLE_A;
    
    temp2 = 2 + (temp_x >> 1);
    temp3 = 2 + (temp_y >> 1);

    temp0 = (temp3 << 3) + temp2;
    address += temp0;

    // Figure out what the new attribute byte *should* be
    temp2 = temp_x | 1; // Ensure we start on the bottom-right metatile for this attribute table byte
    temp3 = temp_y | 1;

    temp4 = 0; // Destination byte buffer

    // Work from MSB to LSB
    // Bottom-right (bits 7 and 6)
    DeckswabberGetTileIndex(temp0, temp2, temp3);
    temp0 = tile_colormap[temp0];
    temp4 = deckswabber_metatile_palettes[temp0];

    // bottom-left (bits 5 and 4)
    --temp2;
    DeckswabberGetTileIndex(temp0, temp2, temp3);
    temp0 = tile_colormap[temp0];
    temp4 <<= 2;
    temp4 |= deckswabber_metatile_palettes[temp0];

    // Top-right (bits 3 and 2)
    ++temp2;
    --temp3;
    DeckswabberGetTileIndex(temp0, temp2, temp3);
    temp0 = tile_colormap[temp0];
    temp4 <<= 2;
    temp4 |= deckswabber_metatile_palettes[temp0];

    // Bottom-left (bits 1 and 0)
    --temp2;
    DeckswabberGetTileIndex(temp0, temp2, temp3);
    temp0 = tile_colormap[temp0];
    temp4 <<= 2;
    temp4 |= deckswabber_metatile_palettes[temp0];

    // Write it!
    one_vram_buffer(temp4, address);
}

#pragma code-name(pop)
#pragma rodata-name(pop)
