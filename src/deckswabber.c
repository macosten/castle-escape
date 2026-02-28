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
#define enemy_depthmap (enemymap + 64)

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

ZEROPAGE_EXTERN(unsigned char, player_sword_timer);
#define hostile_entity_timer player_sword_timer

ZEROPAGE_EXTERN(unsigned char, player_death_timer);
#define passive_entity_timer player_death_timer

ZEROPAGE_EXTERN(unsigned char, player_walking_timer);
#define game_frame_timer player_walking_timer

extern unsigned int deckswabber_1p_high_score;
extern unsigned int previous_score;
extern unsigned char chrbuffer[32];

const unsigned char * const * deckswabber_active_level_pack_levels;

// Importing only the enemies_whatever arrays we need to store Doughnutfruit data...
extern unsigned char enemies_x[MAX_ENEMIES]; // Entity X coords (tile X)
extern unsigned char enemies_y[MAX_ENEMIES]; // Entity Y coords (tile Y)
extern unsigned char enemies_timer[MAX_ENEMIES]; // internal timer
extern unsigned char enemies_extra[MAX_ENEMIES]; // animation timer
extern unsigned char enemies_type[MAX_ENEMIES];
extern unsigned char enemies_flags[MAX_ENEMIES]; // animation timer

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
extern void empty_function(void);

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
void deckswabber_update_health_bar(void);
void deckswabber_draw_goal_hud(void);
void deckswabber_draw_goal_hud_sub_write_ending_backarrow(void);
void deckswabber_draw_goal_hud_sub_write_mt(void);

void deckswabber_tile_increment_fn_original_round1(void);
void deckswabber_tile_increment_fn_original_round2(void);
void deckswabber_tile_increment_fn_original_round3(void);
void deckswabber_tile_increment_fn_original_round4(void);
void deckswabber_tile_increment_fn_original_round5(void);
void deckswabber_tile_increment_fn_original_round6(void);
void deckswabber_tile_increment_fn_original_round7(void);
void deckswabber_tile_increment_fn_original_round8(void);
void deckswabber_tile_increment_fn_bonus_round9(void);
void deckswabber_tile_increment_fn_bonus_round10(void);
void deckswabber_tile_increment_fn_bonus_round11(void);
void deckswabber_tile_increment_fn_bonus_round12(void);
void deckswabber_tile_increment_fn_bonus_round13(void);
void deckswabber_tile_increment_fn_bonus_round14(void);

void deckswabber_tile_increment_fn_sub_upperlimit_rollover(void);

void deckswabber_entity_movement(void);
void deckswabber_entity_ai_curtain(void);
void deckswabber_entity_ai_generic_meander(void);
void deckswabber_entity_ai_explosion(void);

void deckswabber_entity_ai_sub_meander(void);

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
    deckswabber_tile_increment_fn_bonus_round9,
    deckswabber_tile_increment_fn_bonus_round10,
    deckswabber_tile_increment_fn_bonus_round11,
    deckswabber_tile_increment_fn_bonus_round12,
    deckswabber_tile_increment_fn_bonus_round13,
    deckswabber_tile_increment_fn_bonus_round14,
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

    pal_bright(4);

    // Begin first level...
    begin_deckswabber_level();    
}

void begin_deckswabber_level(void) {
    ppu_off();
    // Reset tile typemap, tile colormap, tile enemymap, and chrbuffer
    set_prg_bank(DECKSWABBER_CODE_BANK);
    // Figure out what the tilemap should be (from the current options...)
    //address = (int)(deckswabber_level_data_db[level_pack_index]);
    AsmSet2ByteFromPtrAtIndexVar(address, deckswabber_level_data_db, coordinates);
    //temppointer = (const unsigned char *)((const unsigned char * const *)address)[level];
    AsmSet2ByteFromMutPtrWithOffset(temppointer, address, level_index);
    
    tiles_remaining = DECKSWABBER_TILE_HEIGHT * DECKSWABBER_TILE_WIDTH;
    
    x = 8;
    y = 8;
    for (index = 0; index < (DECKSWABBER_TILE_HEIGHT * DECKSWABBER_TILE_WIDTH); index += 1) {
        clear_vram_buffer();
        // Bitpacked: 2 tiles per byte
        temp4 = index >> 1;

        // temp2 = temppointer[temp4];
        AsmSet1ByteFromZpPtrAtIndexVar(temp2, temppointer, temp4);
        temp0 = temp2 >> 4;
        temp1 = temp2 & 0x0F;

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

        ++index;

        tile_typemap[index] = temp1;
        temp2 = deckswabber_metatile_palettes[temp1];
        tile_colormap[index] = temp1;
        enemymap[index] = 0;
        
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
    memfill(enemymap, 0, 64 + 8); // enemymap and enemy_depthmap
    memfill(chrbuffer, 0, 32);

    // Update HUD for level/round
    clear_vram_buffer();
    convert_to_decimal(round + 1);
    prepare_score_string();
    multi_vram_buffer_horz(score_string, 5, NTADR_A(9, 3));
    convert_to_decimal(level + 1);
    prepare_score_string();
    multi_vram_buffer_horz(score_string, 5, NTADR_A(16, 3));
    flush_vram_update_nmi();

    // Increment the starting tile by 1 and update it
    tile_color_increment_type = round; // Set based on some level info

    // (Re)draw the "goal" indicator
    deckswabber_draw_goal_hud();

    clear_vram_buffer();
    energy = 192; // 6 HUD tiles that take up the visual HP bar * 32 "health" per tile (16 per "tick")
    // Redraw the energy meter
    deckswabber_update_health_bar();

    // X and Y coordinates for this game will be in tiles; the drawing routines will figure out where they belong on-screen...
    //temppointer1 = deckswabber_starting_coords_db[level_pack_index];
    AsmSet2ByteFromPtrAtIndexVar(temppointer1, deckswabber_starting_coords_db, coordinates);
    temp0 = temppointer1[level];
    
    player_tile_x = temp0 >> 4;
    player_tile_y = temp0 & 0x0F;
    
    temp_x = player_tile_x;
    temp_y = player_tile_y;
    AsmCallFunctionAtPtrOffsetByIndexVar(tile_increment_functions, eject_L);
    deckswabber_redraw_player_tile();

    multi_vram_buffer_horz(deckswabber_tiles_remaining, 10, NTADR_A(17, 4));
    multi_vram_buffer_horz(chrbuffer, 12, NTADR_A(17, 5));
    deckswabber_update_tiles_remaining();

    hostile_entity_timer = 5;
    passive_entity_timer = 6;
    game_frame_timer = 60;
    player_flags = 0;

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

        --game_frame_timer;
        if (game_frame_timer == 0) {
            // Increment timer
            --hostile_entity_timer;
            --passive_entity_timer;
            // ...
            game_frame_timer = 60;
        }

        // Entity creation:
        if (hostile_entity_timer == 0) {
            if (DECKSWABBER_CAN_MAKE_HOSTILE_ENTITY) {
                DECKSWABBER_RESET_CAN_MAKE_HOSTILE_ENTITY();
                if ((rand8() & 0b1) == 0) { // 50%
                    // Create a hostile entity
                }
            } else {
                DECKSWABBER_SET_CAN_MAKE_HOSTILE_ENTITY();
            }
            hostile_entity_timer = 5;
        }

        // Every 6th second, 25% chance to spawn a friendly entity
        if (passive_entity_timer == 0) {
            if (DECKSWABBER_CAN_MAKE_PASSIVE_ENTITY) {
                DECKSWABBER_RESET_CAN_MAKE_PASSIVE_ENTITY();
                if ((rand8() & 0b11) == 0) { // 25%
                    // Create a hostile entity
                }
            } else {
                DECKSWABBER_SET_CAN_MAKE_PASSIVE_ENTITY();
            }
            passive_entity_timer = 6;
        }

    } else {
        if (transition_timer == 120) {
            deckswabber_write_finished_message();
        }
        if (transition_timer) {
            --transition_timer;
        } else if (pad1_new) {
            ++level;
            temp0 = round << 1;
            //temppointer1 = deckswabber_round_bounds_db[level_pack_index];
            AsmSet2ByteFromPtrAtIndexVar(temppointer1, deckswabber_round_bounds_db, coordinates);
            temp1 = temp0 + 1;
            // temp2 = temppointer1[temp1]; // Maximum level in round
            AsmSet1ByteFromZpPtrAtIndexVar(temp2, temppointer1, temp1); // Maximum level in round
            if (level >= temp2) {
                // Increment round
                //level = temppointer1[temp0]; // Minimum level in round
                AsmSet1ByteFromZpPtrAtIndexVar(level_index, temppointer1, temp0);
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
        ++round;
        if (round >= sizeof(tile_increment_functions)/2) {
            round = 0;
        }
        begin_deckswabber_level();
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

const unsigned char * const score_string_last2 = score_string + 3;
void deckswabber_update_tiles_remaining(void) {
    convert_to_decimal(tiles_remaining);
    prepare_score_string();
    //score_string + 3;
    multi_vram_buffer_horz_indirect_ptr(score_string_last2, 2, NTADR_A(21, 5));
}

void deckswabber_write_finished_message(void) {
    multi_vram_buffer_horz(deckswabber_level_finished, 10, NTADR_A(17, 4));
    multi_vram_buffer_horz(deckswabber_bonus, 6, NTADR_A(17, 5));

    // ...calculate bonus here
    convert_to_decimal(0); // Todo - proper bonus calculation
    igloo_write_decimal_in_message_sub(); // Note: this writes to cmap+64. It shouldn't matter by the time we call this, though.
    multi_vram_buffer_horz(chrbuffer, temp0, NTADR_A(24, 5));
}

const unsigned char deckswabber_full_hp_bar[] = { 0xF6, 0xF6, 0xF6, 0xF6, 0xF6, 0xF6 };
void deckswabber_update_health_bar(void) {
    // Todo - implement proper bar rendering (and delete the above Full HP Bar)
    multi_vram_buffer_horz(deckswabber_full_hp_bar, 6, NTADR_A(6, 5));
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

// We can make these a bit more generic at some point if we need to reclaim the ROM space...
void deckswabber_tile_increment_fn_original_round2(void) {
    temp3 = 1;
    temp4 = 0;
    deckswabber_tile_increment_fn_sub_upperlimit_rollover();
}

void deckswabber_tile_increment_fn_original_round3(void) {
    temp3 = 2;
    temp4 = 1;
    deckswabber_tile_increment_fn_sub_upperlimit_rollover();
}

void deckswabber_tile_increment_fn_original_round4(void) {
    temp3 = 2;
    temp4 = 0;
    deckswabber_tile_increment_fn_sub_upperlimit_rollover();
}

void deckswabber_tile_increment_fn_original_round5(void) {
    temp3 = 3;
    temp4 = 2;
    deckswabber_tile_increment_fn_sub_upperlimit_rollover();
}

void deckswabber_tile_increment_fn_original_round6(void) {
    temp3 = 3;
    temp4 = 0;
    deckswabber_tile_increment_fn_sub_upperlimit_rollover();
}

void deckswabber_tile_increment_fn_original_round7(void) {
    temp3 = 4;
    temp4 = 3;
    deckswabber_tile_increment_fn_sub_upperlimit_rollover();
}

void deckswabber_tile_increment_fn_original_round8(void) {
    temp3 = 4;
    temp4 = 0;
    deckswabber_tile_increment_fn_sub_upperlimit_rollover();
}

void deckswabber_tile_increment_fn_bonus_round9(void) {
    temp3 = 5;
    temp4 = 4;
    deckswabber_tile_increment_fn_sub_upperlimit_rollover();
}

void deckswabber_tile_increment_fn_bonus_round10(void) {
    temp3 = 5;
    temp4 = 0;
    deckswabber_tile_increment_fn_sub_upperlimit_rollover();
}

void deckswabber_tile_increment_fn_bonus_round11(void) {
    temp3 = 6;
    temp4 = 5;
    deckswabber_tile_increment_fn_sub_upperlimit_rollover();
}

void deckswabber_tile_increment_fn_bonus_round12(void) {
    temp3 = 6;
    temp4 = 0;
    deckswabber_tile_increment_fn_sub_upperlimit_rollover();
}

void deckswabber_tile_increment_fn_bonus_round13(void) {
    temp3 = 7;
    temp4 = 6;
    deckswabber_tile_increment_fn_sub_upperlimit_rollover();
}

void deckswabber_tile_increment_fn_bonus_round14(void) {
    temp3 = 7;
    temp4 = 0;
    deckswabber_tile_increment_fn_sub_upperlimit_rollover();
}

void deckswabber_tile_increment_fn_sub_upperlimit_rollover(void) {
    DeckswabberGetTileIndex(temp0, temp_x, temp_y);
    temp2 = tile_colormap[temp0];
    temp1 = temp2 + 1;
    if (temp1 > temp3) { temp1 = temp4; }
    tile_colormap[temp0] = temp1;
    if (temp1 == temp3) {
        --tiles_remaining;
    } else if (temp2 == temp3) {
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

const void (* const deckswabber_goal_hud_subfns[])(void) = {
    empty_function,
    deckswabber_draw_goal_hud_sub_write_ending_backarrow,
    deckswabber_draw_goal_hud_sub_write_mt,
};

void deckswabber_draw_goal_hud(void) {
    // Assuming the PPU is off...

    // Clear the HUD area first
    clear_vram_buffer();
    multi_vram_buffer_horz(chrbuffer, 26, NTADR_A(3, 26));
    multi_vram_buffer_horz(chrbuffer, 26, NTADR_A(3, 27));
    multi_vram_buffer_horz(chrbuffer, 26, NTADR_A(3, 28));
    flush_vram_update_nmi();

    // Draw the tiles themselves...
    clear_vram_buffer();
    temp_x = deckswabber_goal_hud_starting_tile_x_from_increment_type[tile_color_increment_type];
    temp4 = deckswabber_goal_hud_byte_write_length_from_increment_type[tile_color_increment_type];
    multi_vram_buffer_horz(deckswabber_goal_hud_tiles_toprow, temp4, NTADR_A(temp_x, 26));
    multi_vram_buffer_horz(deckswabber_goal_hud_tiles_bottomrow, temp4, NTADR_A(temp_x, 27));
    flush_vram_update_nmi();


    // Draw the attribute bytes
    clear_vram_buffer();
    // Using precomputed arrays...
    address = NAMETABLE_A + 0x03F0;
    temppointer = deckswabber_goal_hud_attribute_bytes_from_increment_type + (tile_color_increment_type << 3);
    multi_vram_buffer_horz_indirect_ptr(temppointer, 8, address);
    flush_vram_update_nmi();

    // Draw the word "goal"
    clear_vram_buffer();
    temp0 = temp_x + temp4 - 3;
    multi_vram_buffer_horz(deckswabber_goal, 4, NTADR_A(temp0, 28));
    flush_vram_update_nmi();

    // Perform touch-ups
    clear_vram_buffer();
    temp2 = deckswabber_goal_hud_touchup_procedure_from_increment_type[tile_color_increment_type];
    temp3 = temp2 & 0x0F;
    AsmCallFunctionAtPtrOffsetByIndexVar(deckswabber_goal_hud_subfns, temp3);
    flush_vram_update_nmi();    
}

void deckswabber_draw_goal_hud_sub_write_ending_backarrow(void) {
    one_vram_buffer(0x18, NTADR_A(temp0, 27));
}

void deckswabber_draw_goal_hud_sub_write_mt(void) {
    // Draw arrow
    temp0 += 3;
    address = NTADR_A(temp0, 26);
    one_vram_buffer(0x08, address);
    // Draw tile
    address += 1;
    temppointer = deckswabber_metatiles;
    multi_vram_buffer_horz_indirect_ptr(temppointer, 2, address);
    address += 0x20;
    temppointer += 2;
    multi_vram_buffer_horz_indirect_ptr(temppointer, 2, address);
}

const void (* const deckswabber_ai_pointers[])(void) = {
    deckswabber_entity_ai_curtain,                        // 0 - DECKSWABBER_ENTITY_CURTAIN
    deckswabber_entity_ai_generic_meander, // 1 - Bronze Coin
    deckswabber_entity_ai_generic_meander, // 2 - Silver Coin
    deckswabber_entity_ai_generic_meander, // 3 - Gold Coin
    deckswabber_entity_ai_generic_meander, // 4 - Bronze Chest
    deckswabber_entity_ai_generic_meander, // 5 - Silver Chest
    deckswabber_entity_ai_generic_meander, // 6 - Gold Chest
    deckswabber_entity_ai_generic_meander, // 7 - Sword
    deckswabber_entity_ai_generic_meander, // 8 - Half Flag
    deckswabber_entity_ai_generic_meander, // 9 - Full Flag
    deckswabber_entity_ai_generic_meander, // 10 - Dirt Bomb 1 (+)
    deckswabber_entity_ai_generic_meander, // 11 - Dirt Bomb 2 (#)
    deckswabber_entity_ai_generic_meander, // 12 - Cannon (?)
    deckswabber_entity_ai_explosion, // 13 - Explosion effect
    // ? - Mynci
    // ? - Techo
    // ? - Captain Dread
};

void deckswabber_entity_movement(void) {
    // Only up to 7 entities at a time (MSB reserved for the player in enemy_depthmap).
    for (x = 0; x < DECKSWABBER_MAX_ONSCREEN_ENTITIES; ++x) {
        if (IS_ENEMY_ACTIVE(x)) {
            temp0 = GET_ENEMY_TYPE(x);
            AsmCallFunctionAtPtrOffsetByIndexVar(deckswabber_ai_pointers, temp0);
        }
    }
}

void deckswabber_entity_ai_generic_meander(void) {
    // Jump around aimlessly.

    // 1/1000 chance to blow up every frame.
    // ...proc here

    temp0 = enemies_timer[x];
    --temp0;
    if (temp0 == 0) {
        // Take a movement opportunity: 1 in X chance to move. (maybe a 75% chance?)
        deckswabber_entity_ai_sub_meander();
        enemies_timer[x] = 100;
    }

}

void deckswabber_entity_ai_curtain(void) {
    temp0 = enemies_timer[x];
    --temp0;
    if (temp0 == 0) {
        // What's behind the curtain??
        //enemies_type[x] = enemies_extra[x];
        __asm__("ldy %v", x);
        __asm__("lda %v,y", enemies_extra);
        __asm__("sta %v,y", enemies_type);
    } else {
        enemies_timer[x] = temp0;
    }
}

void deckswabber_entity_ai_explosion(void) {
    temp0 = enemies_timer[x];
    --temp0;
    if (temp0 == 0) {
        // Proc despawn
        DEACTIVATE_ENEMY(x);
    } else {
        enemies_timer[x] = temp0;
    }
}

void deckswabber_entity_ai_sub_meander(void) {
    temp1 = rand8() & 0b11;
    switch (temp1)
    {
        case 0: { // Left
            temp2 = enemies_x[x];
            if (temp2 == 0) {
                goto deckswabber_meander_right;
            }
        deckswabber_meander_left:
            --temp2;
            enemies_x[x] = temp2;
            break;
        }
        case 1: { // Right
            temp2 = enemies_x[x];
            if (temp2 >= (DECKSWABBER_TILE_WIDTH - 1)) {
                goto deckswabber_meander_left;
            }
        deckswabber_meander_right:
            ++temp2;
            enemies_x[x] = temp2;
            break;
        }
        case 2: { // Up
            temp2 = enemies_y[x];
            if (temp2 == 0) {
                goto deckswabber_meander_down;
            }
        deckswabber_meander_up:
            --temp2;
            enemies_y[x] = temp2;
            break;
        }
        default: { // Down
            temp2 = enemies_y[x];
            if (enemies_y[x] >= (DECKSWABBER_TILE_HEIGHT - 1)) {
                goto deckswabber_meander_up;
            }
        deckswabber_meander_down:
            ++temp2;
            enemies_y[x] = temp2;
            break;
        }
    }
}

#pragma code-name(pop)
#pragma rodata-name(pop)
