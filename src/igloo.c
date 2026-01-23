#include <stdlib.h>

#include "lib/nesdoug.h"
#include "lib/neslib.h"

#include "asm/macros.h"
#include "asm/score.h"
#include "enemy_macros.h"
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
ZEROPAGE_EXTERN(unsigned char, x);
ZEROPAGE_EXTERN(unsigned char, y);
ZEROPAGE_EXTERN(Hitbox, hitbox);
ZEROPAGE_EXTERN(Hitbox, hitbox2);
ZEROPAGE_EXTERN(const unsigned char *, temppointer);
ZEROPAGE_EXTERN(unsigned int, score);
ZEROPAGE_EXTERN(unsigned char, game_mode);
ZEROPAGE_EXTERN(unsigned char, player_flags);
ZEROPAGE_EXTERN(unsigned char, player_frame_timer);
ZEROPAGE_EXTERN(unsigned char, menu);
ZEROPAGE_EXTERN(unsigned char, shuffle_offset);
ZEROPAGE_EXTERN(unsigned char, shuffle_maximum);
ZEROPAGE_EXTERN(unsigned char, temp_x);
ZEROPAGE_EXTERN(unsigned char, temp_y);
ZEROPAGE_ARRAY_EXTERN(unsigned char, debug_values, 4);

ZEROPAGE_EXTERN(unsigned char, eject_L);
#define items_dropped_this_round eject_L

ZEROPAGE_EXTERN(unsigned char, eject_R);
#define round_begin_timer eject_R

ZEROPAGE_EXTERN(unsigned char, eject_D);
#define threshold_coin eject_D

ZEROPAGE_EXTERN(unsigned char, eject_U);
#define threshold_bomb eject_U

ZEROPAGE_EXTERN(unsigned char, nt);
#define threshold_piano nt

ZEROPAGE_EXTERN(Player, valrigard);
#define mika valrigard

ZEROPAGE_EXTERN(Player, player2);
#define carassa player2

ZEROPAGE_EXTERN(unsigned char, level_index);
#define round_number level_index

ZEROPAGE_EXTERN(unsigned char, player_death_timer);
#define player_stun_timer player_death_timer

ZEROPAGE_EXTERN(unsigned char, player_sword_timer);
#define carassa_walking_frame_timer player_sword_timer

ZEROPAGE_EXTERN(unsigned char, index);
#define next_item_timer index

ZEROPAGE_EXTERN(unsigned char, enemy_limit);
#define game_seconds_timer enemy_limit

ZEROPAGE_EXTERN(unsigned char, lowest_enemy_index);
#define game_frame_timer lowest_enemy_index

ZEROPAGE_EXTERN(unsigned int, pseudo_scroll_y);
#define score_this_round pseudo_scroll_y

ZEROPAGE_EXTERN(unsigned char *, temp_mutablepointer);
#define igloo_item_drop_speeds ((unsigned int *)(temp_mutablepointer))

ZEROPAGE_EXTERN(unsigned int, old_y);
#define carassa_item_walk_index_queue ((unsigned char *)(old_y))

ZEROPAGE_EXTERN(unsigned char, tile_clear_front);
#define carassa_item_walk_queue_front tile_clear_front

ZEROPAGE_EXTERN(unsigned char, tile_clear_back);
#define carassa_item_walk_queue_back tile_clear_back

ZEROPAGE_EXTERN(unsigned int, min_scroll_y);
#define carassa_speed min_scroll_y

ZEROPAGE_EXTERN(unsigned char, did_headbonk);
#define item_spawn_frame_delay_index_base did_headbonk

extern unsigned char shuffle_array[];
extern unsigned char shuffle_leg_size;

// Importing only the enemies_whatever arrays we need to store Doughnutfruit data...
extern unsigned char enemies_x[MAX_ENEMIES]; // Object X coords
extern unsigned char enemies_y[MAX_ENEMIES]; // Object Y coords

extern unsigned char enemies_actual_y[MAX_ENEMIES];
#define enemies_subpixel_y enemies_actual_y

extern unsigned char enemies_extra[MAX_ENEMIES];
#define enemies_y_velocity_pixels enemies_extra

extern unsigned char enemies_extra2[MAX_ENEMIES]; // Object speed index (actual speed saved in LUT)
#define enemies_y_velocity_subpixels enemies_extra2

extern unsigned char enemies_type[MAX_ENEMIES];
extern unsigned char enemies_flags[MAX_ENEMIES];
extern unsigned char enemies_timer[MAX_ENEMIES]; // Timer???
extern unsigned char enemies_count;

extern unsigned char cmap[];

extern unsigned int igloo_1p_high_score;
extern unsigned int igloo_2p_high_score;

#if ONSCREEN_JUNK_MAXIMUM>15
    #warning "Careful (igloo): ONSCREEN_JUNK_MAXIMUM is high; may have to change logic for carassa walk queue(s)."
#endif

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
void igloo_carassa_movement(void);

void igloo_sprite_collisions(void);
void igloo_draw_sprites(void);
void igloo_update_score(void);
void igloo_update_time(void);
void igloo_write_fake_time(void);
void igloo_clear_top_message(void);
void igloo_clear_bottom_message(void);
void igloo_write_decimal_in_message_sub(void);
void igloo_write_allitems_message(void);
void igloo_write_nextlevel_message(void);
void igloo_write_general_gameover_message(void);
void igloo_write_piano_gameover_message(void);
void igloo_write_itemdrop_gameover_message(void);

void igloo_begin_new_round(void);
void calculate_next_igloo_item(void);

void igloo_item_ai_downwards_sub(void);
void igloo_default_item_ai(void);
void igloo_bomb_item_ai(void);
void igloo_piano_item_ai(void);
void igloo_explosion_ai(void);
void igloo_piano_explosion_ai(void);

void igloo_collision_empty_function(void);
void igloo_collision_with_points(void);
void igloo_collision_with_bomb(void);
void igloo_collision_with_bomb_explosion(void);
void igloo_collision_with_piano(void);

void igloo_draw_mika(void);
void igloo_draw_carassa(void);
void igloo_draw_items(void);
void igloo_default_draw(void);
void igloo_bomb_draw(void);
void igloo_bomb_explosion_draw(void);
void igloo_piano_explosion_draw(void);

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
    enemies_count = ONSCREEN_JUNK_MAXIMUM; // Used by extern'd functions
    calculate_shuffle_array();

    // A bit of shared code, but with a bit of upkeep:
    score = 0;
    round_number = 0;
    score_this_round = 0; // don't buffer a message...
    player_frame_timer = 0;

    // Item drop speeds
    temp_mutablepointer = ((void *)(cmap + 128)); // Pointer crimes ("it's memory, just do what I want please")
    
    // Carassa walking queue pointers
    old_y = ((unsigned int)(temp_mutablepointer)) + 16;

    igloo_begin_new_round();
    // igloo_begin_new_round will be called again later but this puts
    // the Chias in the right place without needing to copy+paste the
    // code to do so.

    hitbox.width = MIKA_WIDTH;
    hitbox.height = MIKA_HEIGHT;
    carassa_speed = 0x0100;

    seed_rng();
    srand(rand8());

    igloo_update_score();

    ppu_on_all();
    pal_bright(4);
}

void game_igloo(void) {
    IGLOO_RESET_PLAYER_FLAGS_START_FRAME();

    pad1 = pad_poll(0); // read the first controller
    pad1_new = get_pad_new(0);

    ppu_wait_nmi(); // wait till beginning of the frame

    clear_vram_buffer();

    // If not paused:
    set_prg_bank(IGLOO_CODE_BANK);

    igloo_player_movement();
    igloo_carassa_movement();
    igloo_item_movement();
    igloo_sprite_collisions();
    igloo_draw_sprites();

    if (IGLOO_SCORE_CHANGED_THIS_FRAME) { igloo_update_score(); }

    if (game_seconds_timer > 0 || game_frame_timer > 0) {
        --game_frame_timer;
        if (game_frame_timer == 0) {
            if (game_seconds_timer == 0) {
                igloo_begin_new_round();
                return; // Don't rarely drop an item right as the round ends, that's mean...
            } else {
                game_frame_timer = 60;
                --game_seconds_timer;
                igloo_update_time();
            }
        }

        --next_item_timer;
        if (next_item_timer == 0) {
            temp0 = (rand8() & 0b111);
            temp0 += item_spawn_frame_delay_index_base;
            next_item_timer = item_spawn_frame_delays[temp0];
            calculate_next_igloo_item();

            for (x = 0; x < ONSCREEN_JUNK_MAXIMUM; ++x) {
                if (!IS_ENEMY_ACTIVE(x)) {
                    // insert here!
                    // temp5 still = rand() from calculate_next_igloo_item;
                    enemies_type[x] = temp4;
                    enemies_flags[x] = 0;
                    ACTIVATE_ENEMY(x);
                    temp0 = high_byte(temp5) + 64; // since rand() is 0...0x7FFF, high_byte is 0...127
                    enemies_x[x] = temp0;
                    enemies_y[x] = IGLOO_BRIDGE_Y_CHAR; // minus whatever for the item's height? Maybe use a LUT for that
                    enemies_subpixel_y[x] = 0;
                    enemies_timer[x] = 0;

                    //carassa_item_walk_index_queue[carassa_item_walk_queue_back] = x;
                    AsmSet1ByteAtMutPtrWithOffset(old_y, tile_clear_back, x);

                    ++carassa_item_walk_queue_back;
                    carassa_item_walk_queue_back &= 0b1111;

                    temp0 = rand8() & 0b111;
                    //temp5 = igloo_item_drop_speeds[temp0];
                    AsmSet2ByteFromMutPtrWithOffset(temp5, temp_mutablepointer, temp0);
                    enemies_y_velocity_pixels[x] = high_byte(temp5);
                    enemies_y_velocity_subpixels[x] = low_byte(temp5);
                    break;
                }
            } // And as before if the screen is full it will just fail silently. No biggie.
        }
    } else if (round_begin_timer > 0) {
        // Start next round
        --round_begin_timer;
        if (round_begin_timer == 0) {
            game_seconds_timer = 30;
            game_frame_timer = 60;
            igloo_clear_top_message();
            igloo_clear_bottom_message();
        }
    }

    if (pad1 & PAD_B) {
        end_igloo();
        return;
    }
    //gray_line();
}

void end_igloo(void) {
    game_frame_timer = 0;
    game_seconds_timer = 0;
    pal_fade_to(4, 0);
    menu = MENU_IGLOO;
    round_number = 0;
    switch_menu();
    music_play(MENU_SONG);
    pal_bright(4);
}


void igloo_begin_new_round(void) {
    if (round_number < 255) { ++round_number; }
    round_begin_timer = 180; // 1 per frame of waiting
    player_stun_timer = 0;

    mika.velocity_x = 0;
    mika.velocity_y = 0;
    carassa.velocity_x = 0;
    carassa.velocity_y = 0;

    mika.x = CHIA_STARTING_X;
    mika.y = MIKA_STARTING_Y;
    carassa.x = CHIA_STARTING_X;
    carassa.y = CARASSA_STARTING_Y;
    IGLOO_SET_NOT_JUMPING();

    temp0 = MIN(round_number, 5);
    threshold_coin = IGLOOT_BASE_THRESHOLD + temp0;
    threshold_bomb = threshold_coin + temp0;
    threshold_piano = threshold_bomb + temp0; // Should top out at 248?

    // Clear all enemy active flags
    for (x = 0; x < ONSCREEN_JUNK_MAXIMUM; ++x) {
        enemies_flags[x] = 0;
    }

    if (items_dropped_this_round == 0 && score_this_round > 0) {
        score += score_this_round;
        igloo_write_allitems_message();
        igloo_update_score();
    }
    items_dropped_this_round = 0;
    score_this_round = 0;

    // Calculate falling speed table
    temp0 = MIN(round_number, 8);
    temp1 = igloot_speed_increments[temp0];
    temp5 = IGLOOT_MIN_SPEED;
    for (x = 0; x < 8; ++x) {
        temp5 += temp1;
        //igloo_item_drop_speeds[x] = temp5;
        AsmSet2ByteAtMutPtrWithOffset(temp_mutablepointer, x, temp5);
    }

    temp0 = MIN(round_number, 15);
    //carassa_speed = carassa_speeds[temp0];
    AsmSet2ByteFromPtrAtIndexVar(min_scroll_y, carassa_speeds, temp0)

    carassa_item_walk_queue_back = 0;
    carassa_item_walk_queue_front = 0;

    item_spawn_frame_delay_index_base = MIN(round_number, 12) << 3;

    next_item_timer = 1 + rand8() & 0b11111;
    igloo_write_nextlevel_message();
    igloo_write_fake_time();
}

void igloo_write_decimal_in_message_sub(void) {
    prepare_score_string();
    // Count non-space chars...
    temp0 = 0; // Digit count
    temp1 = 64; // Offset to cmap to write to as temporary buffer
    for (x = 0; x < 5; ++x) { // Assuming at least one digit every time
        if (score_string[x] >= '0') {
            ++temp0;
            // cmap[temp1] = score_string[x];
            temp2 = score_string[x];
            cmap[temp1] = temp2;
            ++temp1;
        }
    }
}

void igloo_write_allitems_message(void) {
    multi_vram_buffer_horz(igloo_all_items_bonus_phrase, 15, NTADR_A(10, 5));
    multi_vram_buffer_horz(igloo_all_items_bonus_prefix, 7, NTADR_A(12, 6));
    convert_to_decimal(score_this_round);
    igloo_write_decimal_in_message_sub();
    multi_vram_buffer_horz(cmap+64, temp0, NTADR_A(19, 6));
}

void igloo_write_nextlevel_message(void) {
    multi_vram_buffer_horz(igloo_starting_next_level_phrase, 6, NTADR_A(13, 7));
    convert_to_decimal(round_number);
    igloo_write_decimal_in_message_sub();
    multi_vram_buffer_horz(cmap+64, temp0, NTADR_A(19, 7));
}

void igloo_clear_top_message(void) {
    multi_vram_buffer_horz(cmap, 15, NTADR_A(10, 5));
    multi_vram_buffer_horz(cmap, 12, NTADR_A(11, 6));
}

void igloo_clear_bottom_message(void) {
    multi_vram_buffer_horz(cmap, 15, NTADR_A(10, 7));
}

void igloo_update_score(void) {
    convert_to_decimal(score);
    prepare_score_string();
    multi_vram_buffer_horz(score_string, 5, NTADR_A(10, 2)); // Change, or if it doesn't change, just call hasee_update_score for code reuse as this is in the same bank
}

void igloo_update_time(void) {
    convert_to_decimal(game_seconds_timer);
    prepare_score_string();
    multi_vram_buffer_horz(score_string, 5, NTADR_A(17, 2));
}

void igloo_write_fake_time(void) {
    multi_vram_buffer_horz(fake_time, 3, NTADR_A(19, 2));
}

void igloo_write_general_gameover_message(void) {
    multi_vram_buffer_horz(igloo_game_over_message, 9, NTADR_A(19, 7));
}

void igloo_write_piano_gameover_message(void) {
    multi_vram_buffer_horz(igloo_piano_troll_message_1, 15, NTADR_A(10, 5));
    multi_vram_buffer_horz(igloo_piano_troll_message_2, 12, NTADR_A(11, 6));
}

void igloo_write_itemdrop_gameover_message(void) {
    multi_vram_buffer_horz(igloo_itemdrop_troll_message_1, 13, NTADR_A(10, 5));
    multi_vram_buffer_horz(igloo_itemdrop_troll_message_2, 12, NTADR_A(11, 6));
}

void igloo_player_movement(void) {
    if (player_stun_timer > 0) { --player_stun_timer; }
    if (game_seconds_timer > 0 || game_frame_timer > 0 /* && items_dropped_this_round < 5 */ /*i.e game isn't over; change if I add a flag for this*/) {
        if (IGLOO_IS_JUMPING) { // Jump (if jumping)
            if (high_byte(mika.y) >= 0xB0) {
                mika.velocity_y = 0;
                mika.y = MIKA_STARTING_Y;
                IGLOO_SET_NOT_JUMPING();
                sfx_play(SFX_SMACK, 1);
            } else {
                mika.velocity_y += IGLOO_GRAVITY;
            }
        } else if (!(pad1 & (PAD_A | PAD_LEFT | PAD_RIGHT))) { // Ground friction (if not pressing )
            if (mika.velocity_x > 0) {
                if (mika.velocity_x < IGLOO_FRICTION) {
                    mika.velocity_x = 0;
                } else {
                    mika.velocity_x -= IGLOO_FRICTION;
                }
            } else if (mika.velocity_x < 0) {
                if (mika.velocity_x > -IGLOO_FRICTION) {
                    mika.velocity_x = 0;
                } else {
                    mika.velocity_x += IGLOO_FRICTION;
                }
            }
        } else if (player_stun_timer == 0) { // Process inputs if not stunned
            if (pad1 & PAD_A) {
                IGLOO_SET_JUMPING();
                mika.velocity_y = -MIKA_JUMP_STRENGTH;
                sfx_play(SFX_JUMP, 1);
            } else if (pad1 & PAD_LEFT) {
                mika.velocity_x -= IGLOO_ACCEL;
                --player_frame_timer;
            } else if (pad1 & PAD_RIGHT) {
                mika.velocity_x += IGLOO_ACCEL;
                ++player_frame_timer;
            }

            if (mika.velocity_x > IGLOO_MAX_SPEED) {
                mika.velocity_x = IGLOO_MAX_SPEED;
            } else if (mika.velocity_x < -IGLOO_MAX_SPEED) {
                mika.velocity_x = -IGLOO_MAX_SPEED;
            }
        }

        mika.x += mika.velocity_x;
        mika.y += mika.velocity_y;

        if (high_byte(mika.x) < IGLOO_MIN_X) {
            mika.x = IGLOO_MIN_X << 8;
        } else if (high_byte(mika.x) > IGLOO_MAX_X) {
            mika.x = IGLOO_MAX_X << 8;
        }
    }
}

void igloo_carassa_movement(void) {
    if (game_seconds_timer == 0 && game_frame_timer == 0 /* || items_dropped_this_round < 5 */) {
        carassa_walking_frame_timer = 0;
        IGLOO_SET_CARASSA_NOT_WALKING();
        return;
    } // Game or Round over?
    if (carassa_item_walk_queue_back == carassa_item_walk_queue_front) {
        carassa_walking_frame_timer = 0;
        IGLOO_SET_CARASSA_NOT_WALKING();
        return;
    } // Empty queue, we're caught up
    
    IGLOO_SET_CARASSA_WALKING();
    ++carassa_walking_frame_timer;
    // Target an item:
    //x = carassa_item_walk_index_queue[carassa_item_walk_queue_front];
    AsmSet1ByteFromZpPtrAtIndexVar(x, old_y, tile_clear_front);

    if (!IS_ENEMY_ACTIVE(x)) { // Somehow trying to walk to an inactive item, don't do that?
        IGLOO_SET_CARASSA_NOT_WALKING();
        return;
    }

    temp1 = enemies_x[x];
    temp4 = enemies_type[x];
    temp1 += (igloot_hitbox_width_lookup_table[temp4] >> 1) + igloot_hitbox_x_offset_lookup_table[temp4]; // Half of the visible sprite width
    temp1 -= 12; // Half the visual size of three 8-by-3 sprites (Carassa's width)

    // There was an odd bug where Carassa will sometimes walk all the way to the left, presumably
    // because an invalid/inactive target item whose x coordinate was cleared was being walked to.
    // It's not clear if moving the x coordinate calculation here fixes it fully as intended
    // but I haven't seen it happen since doing so yet. If it's still messed up:

    // if (temp1 < 64 || temp1 > 127+64) { /* Invalid target X, figure something else out */ }

    if (high_byte(carassa.x) < temp1) {
        carassa.x += carassa_speed;
        if (high_byte(carassa.x) > temp1) {
            high_byte(carassa.x) = temp1;
            low_byte(carassa.x) = 0;
        }
    } else if (high_byte(carassa.x) > temp1) {
        carassa.x -= carassa_speed;
        if (high_byte(carassa.x) < temp1) {
            high_byte(carassa.x) = temp1;
            low_byte(carassa.x) = 0;
        }
    } else {
        // Let the item drop!
        // temp0 = carassa_item_walk_index_queue[carassa_item_walk_queue_front];
        AsmSet1ByteFromZpPtrAtIndexVar(temp0, old_y, tile_clear_front);
        IGLOO_START_MOVING_ITEM(temp0);
        ++carassa_item_walk_queue_front;
        carassa_item_walk_queue_front &= 0b1111;
    }
}

const void (* const igloo_collision_functions[])(void) = {
    igloo_collision_with_points,
    igloo_collision_with_points,
    igloo_collision_with_points,
    igloo_collision_with_points,
    igloo_collision_with_points,
    igloo_collision_with_points,
    igloo_collision_with_bomb,
    igloo_collision_with_piano,
    igloo_collision_with_bomb_explosion,
    igloo_collision_empty_function    
};

void igloo_sprite_collisions(void) {
    hitbox.x = high_byte(mika.x);
    hitbox.y = high_byte(mika.y);

    for (x = 0; x < ONSCREEN_JUNK_MAXIMUM; ++x) {
        if (IS_ENEMY_ACTIVE(x) && !IGLOO_IS_ITEM_BROKEN(x)) {
            temp1 = GET_ENEMY_TYPE(x);
            
            hitbox2.width = igloot_hitbox_width_lookup_table[temp1];
            hitbox2.height = igloot_hitbox_height_lookup_table[temp1];

            hitbox2.x = enemies_x[x];
            hitbox2.x += igloot_hitbox_x_offset_lookup_table[temp1];

            hitbox2.y = enemies_y[x];
            hitbox2.y += igloot_hitbox_y_offset_lookup_table[temp1];

            check_collision(temp0, hitbox, hitbox2);
            if (temp0) {
                AsmCallFunctionAtPtrOffsetByIndexVar(igloo_collision_functions, temp1);
            }
        }
    }
}

void igloo_collision_empty_function(void) { }

void igloo_collision_with_points(void) {
    // Turn this into a points effect, if we want to do that:
    // enemies_type[x] = whatever_lut_we_keep_this_in[x];
    // but for now:
    DEACTIVATE_ENEMY(x);
    IGLOO_SET_SCORE_CHANGED_THIS_FRAME();
    temp0 = enemies_type[x];
    score += igloot_point_values[temp0];
    score_this_round += igloot_point_values[temp0];
    sfx_play(SFX_STAR_COLLECT, 0);
}

void igloo_collision_with_bomb(void) {
    // Immediately turn into an explosion and stun the player
    enemies_type[x] = IGLOOT_BOMB_EXPLOSION;
    enemies_timer[x] = 0;
    player_stun_timer = 180;
    sfx_play(SFX_CANNON_FIRE, 0);
    sfx_play(SFX_ENEMY_KILL, 1);
}

void igloo_collision_with_bomb_explosion(void) {
    if (!IGLOO_IS_JUMPING && player_stun_timer == 0 && enemies_timer[x] <= 1) { // Don't stun if in the air
        player_stun_timer = 180;
        sfx_play(SFX_CANNON_FIRE, 0);
        sfx_play(SFX_ENEMY_KILL, 1);
    }
}

void igloo_collision_with_piano(void) {
    // Die immediately...
    if (items_dropped_this_round < 5) {
        sfx_play(SFX_CANNON_FIRE, 0);
        sfx_play(SFX_ENEMY_KILL, 1);
        // Unless we decide to add a new flag for the game being over specifically:
        items_dropped_this_round += 5;
        player_stun_timer = 0xFF;
    }
}

void igloo_draw_sprites(void) {
    // clear all sprites from sprite buffer
    oam_clear();
    igloo_draw_carassa();
    igloo_draw_items();
    igloo_draw_mika();
}

void calculate_next_igloo_item(void) {
    temp5 = rand(); // 0...RAND_MAX (0x7FFF)
    if (low_byte(temp5) < 73) {
        temp4 = IGLOOT_BAG;
    } else if (low_byte(temp5) < 131) {
        temp4 = IGLOOT_CHIAPOP;
    } else if (low_byte(temp5) < 174) {
        temp4 = IGLOOT_UMBRELLA;
    } else if (low_byte(temp5) < 203) {
        temp4 = IGLOOT_THINGY;
    } else if (low_byte(temp5) < IGLOOT_BASE_THRESHOLD) {
        temp4 = IGLOOT_POTION;
    } else if (low_byte(temp5) < threshold_coin) {
        temp4 = IGLOOT_COIN;
    } else if (low_byte(temp5) < threshold_bomb) {
        temp4 = IGLOOT_BOMB;
    } else if (low_byte(temp5) < threshold_piano) {
        temp4 = IGLOOT_PIANO;
    } else {
        temp4 = IGLOOT_BAG;
    }
}

const void (* const igloo_ai_pointers[])(void) = {
    igloo_default_item_ai,
    igloo_default_item_ai,
    igloo_default_item_ai,
    igloo_default_item_ai,
    igloo_default_item_ai,
    igloo_default_item_ai,
    igloo_bomb_item_ai,
    igloo_piano_item_ai,
    igloo_explosion_ai,
    igloo_piano_explosion_ai,
};

void igloo_item_movement(void) {
    for (x = 0; x < ONSCREEN_JUNK_MAXIMUM; ++x) {
        if (IS_ENEMY_ACTIVE(x)) {
            temp0 = enemies_type[x];
            AsmCallFunctionAtPtrOffsetByIndexVar(igloo_ai_pointers, temp0);
        }
    }
}

void igloo_item_ai_downwards_sub(void) {
    temp4 = 0; // temp4 = did this item touch the floor this frame
    if (IGLOO_IS_ITEM_MOVING(x)) {
        high_byte(temp5) = enemies_y[x];
        low_byte(temp5) = enemies_subpixel_y[x];

        high_byte(temp6) = enemies_y_velocity_pixels[x];
        low_byte(temp6) = enemies_y_velocity_subpixels[x];

        temp5 += temp6;
        if (temp5 >= IGLOO_FLOOR_Y) {
            temp5 = IGLOO_FLOOR_Y;
            IGLOO_STOP_ITEM(x);
            IGLOO_SET_ITEM_BROKEN(x);
            temp4 = 1;
            sfx_play(SFX_SMACK, 0);
        }

        enemies_y[x] = high_byte(temp5);
        enemies_subpixel_y[x] = low_byte(temp5);
    }
}

void igloo_default_item_ai(void) {
    igloo_item_ai_downwards_sub();
    if (temp4) {
        items_dropped_this_round += 1;
    }
}

void igloo_bomb_item_ai(void) {
    igloo_item_ai_downwards_sub();
    if (IGLOO_IS_ITEM_BROKEN(x)) { // TODO - is this the right condition?
        if (enemies_timer[x] > 120) {
            // Turn this into an explosion
            enemies_timer[x] = 0;
            enemies_type[x] = IGLOOT_BOMB_EXPLOSION;
            IGLOO_SET_ITEM_UNBROKEN(x);
            sfx_play(SFX_CANNON_FIRE, 0);
        } else {
            //++enemies_timer[x];
            __asm__("ldy %v", x);
            __asm__("lda #$01");
            __asm__("clc");
            __asm__("adc %v,y", enemies_timer);
            __asm__("sta %v,y", enemies_timer);
        }
    } else if (enemies_y[x] >= 0xC8) { // Bombs should stop a bit sooner
        enemies_y[x] = 0xC8;
        IGLOO_STOP_ITEM(x);
        IGLOO_SET_ITEM_BROKEN(x);
    }
}

void igloo_piano_item_ai(void) {
    igloo_item_ai_downwards_sub();
    if (high_byte(temp5) >= 0xC4) {
        enemies_timer[x] = 0;
        enemies_type[x] = IGLOOT_PIANO_EXPLOSION;
        sfx_play(SFX_CANNON_FIRE, 0);
    }
}

void igloo_explosion_ai(void) {
    // Just exist for a bit and then go away.
    //++enemies_timer[x];
    AsmSet1ByteFromPtrAtIndexVar(temp0, enemies_timer, x);
    if (temp0 > 30) {
        enemies_flags[x] = 0;
        enemies_timer[x] = 0;
    } else {
        ++temp0;
        AsmSet1ByteAtPtrWithOffset(enemies_timer, x, temp0);
    }
}

void igloo_piano_explosion_ai(void) {
    // Just exist for a bit and then go away.
    //++enemies_timer[x]
    AsmSet1ByteFromPtrAtIndexVar(temp0, enemies_timer, x);
    if (enemies_timer[x] > 26) {
        enemies_flags[x] = 0;
        enemies_timer[x] = 0;
    } else {
        ++temp0;
        AsmSet1ByteAtPtrWithOffset(enemies_timer, x, temp0);
    }
}

const void (* const igloo_draw_func_pointers[])(void) = {
    igloo_default_draw,
    igloo_default_draw,
    igloo_default_draw,
    igloo_default_draw,
    igloo_default_draw,
    igloo_default_draw,
    igloo_bomb_draw,
    igloo_default_draw,
    igloo_bomb_explosion_draw,
    igloo_piano_explosion_draw,
};

void igloo_draw_mika(void) {
    if (player_stun_timer > 0) {
        temppointer = mika_stunned;
    } else if (!IGLOO_IS_JUMPING) {
        if (high_byte(mika.velocity_x) > 0) {
            temp0 = (player_frame_timer >> 4) & 0b1;
            AsmSet2ByteFromPtrAtIndexVar(temppointer, mika_walk_animation, temp0);
        } else {
            temppointer = mika_idle;
        }
    } else {
        temppointer = mika_idle;
    }
    oam_meta_spr(high_byte(mika.x), high_byte(mika.y), temppointer);
}

void igloo_draw_carassa(void) {
    if (round_begin_timer != 0) { return; }
    if (!IGLOO_IS_CARASSA_WALKING) {
        temppointer = carassa_idle;
    } else {
        temp0 = (carassa_walking_frame_timer >> 4) & 0b1;
        //temppointer = carassa_walk_animation[temp0];
        AsmSet2ByteFromPtrAtIndexVar(temppointer, carassa_walk_animation, temp0);
    }
    oam_meta_spr(high_byte(carassa.x), high_byte(carassa.y), temppointer);
}

void igloo_draw_items(void) {
    for (y = 0; y < shuffle_leg_size; ++y) {
        temp1 = y + shuffle_offset;
        AsmSet1ByteFromPtrAtIndexVar(x, shuffle_array, temp1);
        if (IS_ENEMY_ACTIVE(x)) {
            temp_x = enemies_x[x];
            temp_y = enemies_y[x];
            temp0 = enemies_type[x];
            AsmCallFunctionAtPtrOffsetByIndexVar(igloo_draw_func_pointers, temp0);
            oam_meta_spr(temp_x, temp_y, temppointer);
        }
    }
    shuffle_offset += shuffle_leg_size;
    if (shuffle_offset == shuffle_maximum) { shuffle_offset = 0; }
}

void igloo_default_draw(void) {
    temp0 <<= 1;
    temp0 |= IGLOO_IS_ITEM_BROKEN(x);
    AsmSet2ByteFromPtrAtIndexVar(temppointer, igloot_metasprite_defaultdraw_lut, temp0);
}

void igloo_bomb_draw(void) {
    if (!IGLOO_IS_ITEM_BROKEN(x)) {
        temppointer = item_bomb0;
    } else {
        temp0 = enemies_timer[x] >> 4;
        AsmSet2ByteFromPtrAtIndexVar(temppointer, item_bomb_fuse_animation, temp0);
    }
}

void igloo_bomb_explosion_draw(void) {
    temp0 = (enemies_timer[x] >> 1) & 0b11;
    // temppointer = item_explosion_animation[temp0];
    AsmSet2ByteFromPtrAtIndexVar(temppointer, item_explosion_animation, temp0);
}

void igloo_piano_explosion_draw(void) {
    temp0 = enemies_timer[x] >> 1;
    // temppointer = item_piano_explosion_animation[temp0];
    AsmSet2ByteFromPtrAtIndexVar(temppointer, item_piano_explosion_animation, temp0);
}

#pragma code-name(pop)

#pragma rodata-name(pop)
