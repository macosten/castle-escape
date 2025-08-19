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
#include "hasee_constants.h"
#include "hasee_data.h"
#include "menu_screens_constants.h"

// === Extern'd zero page symbols, defined in zeropage.h.
ZEROPAGE_EXTERN(unsigned char, temp0);
ZEROPAGE_EXTERN(unsigned char, temp1);
ZEROPAGE_EXTERN(unsigned char, temp2);
ZEROPAGE_EXTERN(unsigned char, temp3);
ZEROPAGE_EXTERN(unsigned char, temp4);
ZEROPAGE_EXTERN(unsigned int, temp5);
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
ZEROPAGE_EXTERN(unsigned char, tile_clear_back);
ZEROPAGE_EXTERN(unsigned char, shuffle_offset);
ZEROPAGE_EXTERN(unsigned char, temp_x);
ZEROPAGE_EXTERN(unsigned char, temp_y);
ZEROPAGE_ARRAY_EXTERN(unsigned int, tile_clear_queue, 4);
ZEROPAGE_ARRAY_EXTERN(unsigned char, tile_clear_to_type_queue, 4);
ZEROPAGE_ARRAY_EXTERN(unsigned char, debug_values, 4);
// Aliased values... I can use the same addresses with different names this way, with only a medium jank factor
ZEROPAGE_EXTERN(unsigned char, eject_L); // Just don't use this original name...
#define previously_collected_treats_this_jump eject_L

ZEROPAGE_EXTERN(Player, valrigard);
#define player1 valrigard

ZEROPAGE_EXTERN(unsigned char, player_frame_timer);
#define player1_frame_timer player_frame_timer

ZEROPAGE_EXTERN(unsigned char, player_sword_timer);
#define player2_frame_timer player_sword_timer

ZEROPAGE_EXTERN(unsigned char, player_death_timer);
#define player1_stun_timer player_death_timer

ZEROPAGE_EXTERN(unsigned char, player_walking_timer);
#define player2_stun_timer player_walking_timer

ZEROPAGE_EXTERN(unsigned int, pseudo_scroll_y);
#define game_timer pseudo_scroll_y

ZEROPAGE_EXTERN(signed int, scroll_y); // Letting me treat an unsigned int as a signed int, very kind
#define old_velocity_y scroll_y

ZEROPAGE_EXTERN(unsigned char, level_index);
#define level level_index

ZEROPAGE_EXTERN(unsigned int, temp6);
#define level_timer temp6

ZEROPAGE_EXTERN(unsigned char, did_headbonk);
#define treat_timer did_headbonk

// ZEROPAGE_EXTERN(unsigned char, enemy_is_using_bg_collision);

// extern unsigned char eject_R;
// #pragma zpsym("eject_R")
// #define ...

extern unsigned char shuffle_array[];
extern unsigned char shuffle_leg_size;

// Importing only the enemies_whatever arrays we need to store Doughnutfruit data...
extern unsigned char enemies_x[MAX_ENEMIES]; // Object X coords
extern unsigned char enemies_y[MAX_ENEMIES]; // Object Y coords
extern unsigned char enemies_extra[MAX_ENEMIES]; // Doughnutfruit subpixel X?
extern unsigned char enemies_type[MAX_ENEMIES];
extern unsigned char enemies_extra2[MAX_ENEMIES]; // doughnutfruit speed?
extern unsigned char enemies_flags[MAX_ENEMIES]; // doughnutfruit speed?
extern unsigned char enemies_timer[MAX_ENEMIES]; // Timer?
extern unsigned char enemies_count;

extern unsigned char cmap[];
extern unsigned char boss_memory[8];
#define letter_status boss_memory

// extern const unsigned char const leftright_movement_moving_lookup_table[2];

// Since EfMC is in the non-swapping PRG segment, we can access all of its rodata
// Hasee Bounce ROdata bank will be Bank 1

#pragma rodata-name(push, "BANK0")

unsigned char hasee_palette_sp[] = {
    0x21, 0x0f, 0x38, 0x29, // sky, black, yellow, green - regular doughnutfruit, stars
    0x21, 0x0f, 0x23, 0x26, // sky, black, purple, orange - Hasees and letters
    0x21, 0x0f, 0x00, 0x00, // Reserved for common special doughnutfruit (blue or green)
    0x21, 0x0f, 0x00, 0x00, // Reserved for rare special doughnutfruit (others)
};

unsigned char const hasee_palette_bg[] = {
    0x21, 0x07, 0x17, 0x27, // Wood + Trees
    0x21, 0x07, 0x19, 0x29, // Grass + Bushes
    0x21, 0x0f, 0x23, 0x26, // sky, black, purple, orange - Hasees and letters,
    0x21, 0x0f, 0x19, 0x30, // Text-on-sky and seesaw-on-background
};

extern void clear_screen(void);
extern void put_str_sub(void);
extern void set_prg_bank(unsigned char bank);
extern void calculate_shuffle_array(void);
extern void switch_menu(void);
extern void handle_tile_clear_queue(void);

// Make sure prg bank is 5 before calling:
extern void LZG_decode(const unsigned char *src, unsigned char *dest);
extern unsigned char __fastcall__ divide_by_3(unsigned char input);
extern unsigned char __fastcall__ divide_by_26(unsigned char input);

void game_hasee_bounce(void);
void begin_hasee_bounce(void);
void calculate_next_treat(void);
// void spawn_next_treat(void);
void hasee_sprite_collisions(void);
void hasee_singleplayer_movement(void);
void hasee_multiplayer_movement(void);
void hasee_draw_sprites(void);
void hasee_treat_movement(void);
void hasee_update_score(void);
void orange_hasee_lr_movement(void);
void purple_hasee_lr_movement(void);

extern const char const about_screen[];

void begin_hasee_bounce(void) {
    // Change the menu screen so that it becomes the game screen...
    pal_fade_to(4, 0);
    ppu_off();
    clear_screen();

    // Devensive programming: clear these buffers.
    clear_vram_buffer();
    oam_clear();

    game_timer = 6300; // Frames per game (105 seconds * 60 fps)
    game_mode = MODE_GAME;

    player1.velocity_x = 0;
    player1.velocity_y = 0;
    player2.velocity_x = 0;
    player2.velocity_y = 0;

    player2.x = 0x4000;
    player2.y = ON_BRANCH_STARTING_Y_VALUE;
    player1.x = 0xC000;
    player1.y = ON_GROUND_STARTING_Y_VALUE;

    player_flags = ORANGE_HASEE_ACTIVE | ACTIVE_PLAYER_BRANCH_STATUS;
    player_flags2 = 0;
    score = 0;
    level = 0;
    set_mt_pointer(hasee_metatiles);

    set_prg_bank(5);
    temppointer = hasee_game_screen;
    LZG_decode(temppointer, cmap);
    vram_write(cmap, 32*32);

    letter_status[LETTER_H_INDEX] = LETTER_UNCOLLECTED;
    letter_status[LETTER_A_INDEX] = LETTER_UNCOLLECTED;
    letter_status[LETTER_S_INDEX] = LETTER_UNCOLLECTED;
    letter_status[LETTER_E_INDEX] = LETTER_UNCOLLECTED;
    letter_status[LETTER_E2_INDEX] = LETTER_UNCOLLECTED;

    enemies_count = MAX_TREATS_ONSCREEN;
    calculate_shuffle_array();

    hitbox.width = HASEE_WIDTH;
    hitbox.height = HASEE_HEIGHT;
    hitbox2.width = TREAT_WIDTH;
    hitbox2.height = TREAT_HEIGHT;

    seed_rng();
    srand(rand8());

    ppu_on_all();
    pal_bright(4);
}

void game_hasee_bounce(void) {
    HASEE_RESET_PLAYER_FLAGS_START_FRAME();

    pad1 = pad_poll(0); // read the first controller
    pad1_new = get_pad_new(0);
    pad2 = pad_poll(1); // Will only be used in 2 Player games
    pad2_new = get_pad_new(1); // Or maybe we just call a second function that lets the grounded hasee move only in 2p

    ppu_wait_nmi(); // wait till beginning of the frame

    clear_vram_buffer();

    // If not paused:
    hasee_singleplayer_movement();
    hasee_treat_movement();

    // Even if paused:
    // draw score and other things
    hasee_sprite_collisions();
    hasee_draw_sprites();
    handle_tile_clear_queue();

    if (HASEE_SCORE_CHANGED_THIS_FRAME) { hasee_update_score(); }
    
    // Decrement game timers (if not paused)
    --level_timer;
    if (level_timer == 0) {
        level_timer = LEVEL_FRAME_LENGTH;
        ++level;
    }

    --game_timer;
    if (game_timer == 0) {
        // Trigger stop to game next frame
    }

    --treat_timer;
    if (treat_timer == 0) {
        treat_timer = 32 + (rand8() & 0b11111);
        calculate_next_treat();
        pal_spr(hasee_palette_sp);
        // Also insert treat
        for (x = 0; x < MAX_TREATS_ONSCREEN; ++x) {
            if (!IS_ENEMY_ACTIVE(x)) {
                // insert here
                temp0 = rand8();
                enemies_type[x] = temp4;
                // Debug
                level_index = temp4;
                //
                enemies_flags[x] = temp0 & 0b1; // set initial direction based on RNG
                ACTIVATE_ENEMY(x);
                enemies_y[x] = divide_by_3(temp0) + 80;
                enemies_x[x] = ENEMY_DIRECTION(x) ? 0x00 : 0xf0;
                enemies_timer[x] = 0;
                break;
            }
        } // This is nice because it will just fail gracefully if the screen is full of treats...
    }

    if (player1_stun_timer) { --player1_stun_timer; }
    if (player2_stun_timer) { --player2_stun_timer; }

    // Temporary debug stuffs:
    if (pad1 & PAD_B) {
        pal_fade_to(4, 0);
        menu = MENU_HASEE_BOUNCE;
        switch_menu();
        pal_bright(4);
    }
    gray_line();
}

void hasee_draw_sprites(void) {
    set_prg_bank(HASEE_METASPRITE_BANK);
    oam_clear();

    // Draw hasees

    // Add animation switching block here for P1 hasee (see draw_player)
    temppointer = purple_hasee_idle_right;

    oam_meta_spr(high_byte(player1.x), high_byte(player1.y), temppointer);

    // Add animation switching block here for P2 hasee
    temppointer = orange_hasee_idle_left;

    oam_meta_spr(high_byte(player2.x), high_byte(player2.y), temppointer);

    // if (GAME_PAUSED) { oam_meta_spr(108, 116, hasee_paused_text); }

    // Draw goodies
    for (y = 0; y < shuffle_leg_size; ++y) {
        temp1 = y + shuffle_offset;
        AsmSet1ByteFromPtrAtIndexVar(x, shuffle_array, temp1);
        if (IS_ENEMY_ACTIVE(x)) {
            temp_x = enemies_x[x];
            temp_y = enemies_y[x];
            temp0 = enemies_type[x];
            if (temp0 > TREAT_ORANGE_E) {
                temppointer = purple_hasee_idle_left;
            } else {
                AsmSet2ByteFromPtrAtIndexVar(temppointer, hasee_treat_metasprite_lut, temp0);
            }
            oam_meta_spr(temp_x, temp_y, temppointer);
        }
    }

    // Debug HUD, drawn last because it's the least important.
    // if (ACTIVE_PLAYER_ON_BRANCH) {
    //     oam_spr(208, 50, 0x48, 1);
    // }
    // if (ACTIVE_PLAYER_JUMPING_OFF_BRANCH) {
    //     oam_spr(208, 60, 0x49, 2);
    // }
    // if (ACTIVE_PLAYER_JUMPING_TO_BRANCH) {
    //     oam_spr(208, 70, 0x58, 3);
    // }
    // set_prg_bank(1); HASEE_METASPRITE_BANK is bank 1 at the moment...
}

void hasee_update_score(void) {
    convert_to_decimal(score);
    // Reverse the score string (indices 0...4) while turning them into characters
    __asm__("lda %v+%b", score_string, 0);
    __asm__("ldy %v+%b", score_string, 4);
    __asm__("adc %b", '0');
    __asm__("sta %v+%b", score_string, 4);
    __asm__("tya");
    __asm__("adc %b", '0');
    __asm__("sta %v+%b", score_string, 0);

    __asm__("lda %v+%b", score_string, 1);
    __asm__("ldy %v+%b", score_string, 3);
    __asm__("adc %b", '0');
    __asm__("sta %v+%b", score_string, 3);
    __asm__("tya");
    __asm__("adc %b", '0');
    __asm__("sta %v+%b", score_string, 1);

    for (x = 0; x < 4; ++x) { // Only the first 4 digits; if the last digit is 0 then no point in blanking it
       // __asm__("ldy %v", x);
        //__asm__("lda %v,y", score_string);
        temp0 = score_string[x];
        if (temp0 != 0) { break; }
        score_string[x] = ' ';
    }

    vram_adr(NTADR_A(10, 2));
    for (x = 0; x < 5; ++x) {
        vram_put(score_string[x]);
    }
}

void hasee_singleplayer_movement(void) {
    temp0 = pad1; // Orange Hasee's Pad (Both the same in single-player)
    temp1 = pad1; // Purple Hasee's Pad
    if (ACTIVE_PLAYER) { // Orange Hasee/Player 2 active
        temp2 = pad1; // Temp2 = "Active player's pad"
        temp3 = pad1_new; // Temp3 = "Active player's pad_new"
        orange_hasee_lr_movement();
        old_x = player2.x;
        old_velocity_y = player2.velocity_y;
        old_y = player2.y;
    } else { // Purple Hasee/Player 1 active
        temp2 = pad1;
        temp3 = pad1_new;
        purple_hasee_lr_movement();
        old_x = player1.x;
        old_velocity_y = player1.velocity_y;
        old_y = player1.y;
    }
    temp4 = 0; // We will set this if the high_byte playerX.y should be set, and velocity should be zeroed out. 
    if (ACTIVE_PLAYER_ON_BRANCH) {
        if(temp2 & PAD_A) { // Possible that this is immediately triggered at the start of the game because it's seeing the 
            // Start jumping
            ACTIVE_PLAYER_MOVE_OFF_BRANCH();
            ACTIVE_PLAYER_SET_JUMPING_OFF_BRANCH();
            sfx_play(SFX_JUMP, 0);
            if (ACTIVE_PLAYER) {
                player2.velocity_y = -HASEE_MAX_SPEED;
                old_velocity_y = player2.velocity_y;
            } else {
                player1.velocity_y = -HASEE_MAX_SPEED;
                old_velocity_y = player1.velocity_y;
            }
        }
    } else if (ACTIVE_PLAYER_JUMPING_OFF_BRANCH) {
        if (high_byte(old_y) >= ON_GROUND_STARTING_Y_VALUE_HIGH_BYTE) { // Hit the ground
            FLIP_ACTIVE_PLAYER();
            ACTIVE_PLAYER_UNSET_JUMPING_OFF_BRANCH();
            ACTIVE_PLAYER_SET_JUMPING_TO_BRANCH();
            temp4 = ON_GROUND_STARTING_Y_VALUE_HIGH_BYTE;
            sfx_play(SFX_SMACK, 0);
        } else {
            old_velocity_y += HASEE_GRAVITY;
            if (old_velocity_y > HASEE_MAX_SPEED) {
                old_velocity_y = HASEE_MAX_SPEED;
            }
        }
    } else if (ACTIVE_PLAYER_JUMPING_TO_BRANCH) {
        if (
            high_byte(old_y) >= ON_BRANCH_STARTNIG_Y_VALUE_HIGH_BYTE &&
            ACTIVE_PLAYER_SHOULD_STOP_AT_BRANCH
        ) {
            ACTIVE_PLAYER_MOVE_ON_BRANCH();
            ACTIVE_PLAYER_UNSET_JUMPING_TO_BRANCH();
            ACTIVE_PLAYER_IGNORE_BRANCH();
            temp4 = ON_BRANCH_STARTNIG_Y_VALUE_HIGH_BYTE;
        } else if (high_byte(old_y) <= ON_BRANCH_STARTNIG_Y_VALUE_HIGH_BYTE) {
            old_velocity_y += 0x100;
            ACTIVE_PLAYER_STOP_AT_BRANCH();
        } else {
            old_velocity_y = -HASEE_MAX_SPEED;
        }
    }
    // else { //??? Not sure how this could happen
    //     oam_spr(100, 100, 0x94, 2);
    // }

    if (ACTIVE_PLAYER) {
        if (temp4) {
            high_byte(player2.y) = temp4;
            low_byte(player2.y) = 0;
            old_velocity_y = 0;
        } 
        player2.velocity_y = old_velocity_y;
        player2.y += old_velocity_y;
        old_y = player2.y;
    } else {
        if (temp4) {
            high_byte(player1.y) = temp4;
            low_byte(player1.y) = 0;
            old_velocity_y = 0;
        }
        player1.velocity_y = old_velocity_y;
        player1.y += old_velocity_y;
        old_y = player1.y;
    }

    // Process treat collisions here
    hitbox.x = high_byte(old_x);
    hitbox.y = high_byte(old_y);
}

void hasee_multiplayer_movement(void) {
    temp0 = pad2; // Let both players move left/right at all times when multiplayer
    temp1 = pad1;
    if (ACTIVE_PLAYER) {
        temp2 = temp0;
        temp3 = pad2_new;
    } else {
        temp2 = temp1;
        temp3 = pad1_new;
    }

    orange_hasee_lr_movement();
    purple_hasee_lr_movement();

    if (ACTIVE_PLAYER) {
        old_x = player2.x;
        old_velocity_y = player2.velocity_y;
    } else {
        old_x = player1.x;
        old_velocity_y = player1.velocity_y;
    }

    hitbox.x = high_byte(old_x);
    hitbox.y = high_byte(old_y);
}

void orange_hasee_lr_movement(void) {
    // Remember temp0 contains the status of the pad we want to read in this frame
    if (temp0 & PAD_LEFT) {
        ORANGE_SET_DIRECTION_LEFT();
        player2.velocity_x = -SPEED;
    } else if (temp0 & PAD_RIGHT) {
        ORANGE_SET_DIRECTION_RIGHT();
        player2.velocity_x = SPEED;
    } else {
        player2.velocity_x = 0;
    }
    player2.x += player2.velocity_x;

    if (high_byte(player2.x) < 0x18) { // tree trunk on left
        player2.x = 0x1800;
    } else if (high_byte(player2.x) > 0x48) { // tip of tree branch on left
        player2.x = 0x4800;
    }
}

void purple_hasee_lr_movement(void) {
    // Remember temp1 contains the status of the pad we want to read in this frame
    if (temp1 & PAD_LEFT) {
        PURPLE_SET_DIRECTION_LEFT();
        player1.velocity_x = -SPEED;
    } else if (temp1 & PAD_RIGHT) {
        PURPLE_SET_DIRECTION_RIGHT();
        player1.velocity_x = SPEED;
    } else {
        player1.velocity_x = 0;
    }
    player1.x += player1.velocity_x;

    if (high_byte(player1.x) < 0xA8) { // tip of branch on left
        player1.x = 0xA800;
    } else if (high_byte(player1.x) > 0xD8) { // tree trunk on right
        player1.x = 0xD800;
    }
}

#define PALETTE_3_RNG_THRESHOLD (unsigned int)(21 * MACCY_PROBABILITY + 52 * RAINBOW_PROBABILITY)
#define PALETTE_2_RNG_THRESHOLD (unsigned int)(PALETTE_3_RNG_THRESHOLD + 38 * RAINBOW_PROBABILITY + GREEN_PROBABILITY + BLUE_PROBABILITY)
// Calculate the next pickup item. It will be placed into temp4.
void calculate_next_treat(void) {
    temp4 = TREAT_YELLOW; // Default option

    temp5 = rand(); // 0...RAND_MAX (0x7FFF)
    if (temp5 < PALETTE_3_RNG_THRESHOLD && HASEE_IS_PALETTE_3_LOCKED) {
        return; // Only one palette 3 at a time...
    } else if (temp5 < PALETTE_2_RNG_THRESHOLD && HASEE_IS_PALETTE_2_LOCKED) {
        return; // ...and only one palette 2 at a time.
    }
    
    if (temp5 < MACCY_PROBABILITY) {
        // Originally a 1/10000ish chance but leaving this up to a constant makes for easy tweaks...
        temp4 = TREAT_MACCY;
        HASEE_LOCK_PALETTE_3();
        hasee_palette_sp[13] = hasee_subpal_maccy[1];
        hasee_palette_sp[14] = hasee_subpal_maccy[2];
        hasee_palette_sp[15] = hasee_subpal_maccy[3];
    } else if (temp5 < 5 * MACCY_PROBABILITY) {
        // Originally a ~1/2500 chance, just make it 4x as common as Maccy:
        temp4 = TREAT_FISH;
        HASEE_LOCK_PALETTE_3();
        hasee_palette_sp[13] = hasee_subpal_fish[1];
        hasee_palette_sp[14] = hasee_subpal_fish[2];
        hasee_palette_sp[15] = hasee_subpal_fish[3];  
    } else if (temp5 < 21 * MACCY_PROBABILITY) {
        // ~4 times as common as Fish
        temp4 = TREAT_RAINBOW;
        HASEE_LOCK_PALETTE_3();
        hasee_palette_sp[13] = hasee_subpal_rainbow[1];
        hasee_palette_sp[14] = hasee_subpal_rainbow[2];
        hasee_palette_sp[15] = hasee_subpal_rainbow[3];
    } else if (temp5 < 21 * MACCY_PROBABILITY + 4 * RAINBOW_PROBABILITY) {
        // ~4 times as common as Rainbow:
        temp4 = TREAT_ICY;
        HASEE_LOCK_PALETTE_3();
        hasee_palette_sp[13] = hasee_subpal_icy[1];
        hasee_palette_sp[14] = hasee_subpal_icy[2];
        hasee_palette_sp[15] = hasee_subpal_icy[3];
    } else if (temp5 < 21 * MACCY_PROBABILITY + 10 * RAINBOW_PROBABILITY) {
        // ~6 times as common as Rainbow:
        temp4 = TREAT_FIERY;
        HASEE_LOCK_PALETTE_3();
        hasee_palette_sp[13] = hasee_subpal_fiery[1];
        hasee_palette_sp[14] = hasee_subpal_fiery[2];
        hasee_palette_sp[15] = hasee_subpal_fiery[3];
    } else if (temp5 < 21 * MACCY_PROBABILITY + 18 * RAINBOW_PROBABILITY) {
        // ~8 times as common as Rainbow:
        temp4 = TREAT_SPONGE;
        HASEE_LOCK_PALETTE_3();
        hasee_palette_sp[13] = hasee_subpal_sponge[1];
        hasee_palette_sp[14] = hasee_subpal_sponge[2];
        hasee_palette_sp[15] = hasee_subpal_sponge[3];
    } else if (temp5 < 21 * MACCY_PROBABILITY + 27 * RAINBOW_PROBABILITY) {
        // ~9 times as common as Rainbow:
        temp4 = TREAT_CHECKERED;
        HASEE_LOCK_PALETTE_3();
        hasee_palette_sp[13] = hasee_subpal_checkered[1];
        hasee_palette_sp[14] = hasee_subpal_checkered[2];
        hasee_palette_sp[15] = hasee_subpal_checkered[3];
    } else if (temp5 < 21 * MACCY_PROBABILITY + 38 * RAINBOW_PROBABILITY) {
        // ~11 times as common as Rainbow:
        temp4 = TREAT_GOLDEN;
        HASEE_LOCK_PALETTE_3();
        hasee_palette_sp[13] = hasee_subpal_golden[1];
        hasee_palette_sp[14] = hasee_subpal_golden[2];
        hasee_palette_sp[15] = hasee_subpal_golden[3];
    } else if (temp5 < PALETTE_3_RNG_THRESHOLD) {
        // ~14 times as common as Rainbow:
        temp4 = TREAT_SILVER;
        HASEE_LOCK_PALETTE_3();
        hasee_palette_sp[13] = hasee_subpal_silver[1];
        hasee_palette_sp[14] = hasee_subpal_silver[2];
        hasee_palette_sp[15] = hasee_subpal_silver[3];
    } else if (temp5 < PALETTE_3_RNG_THRESHOLD + 38 * RAINBOW_PROBABILITY) {
        temp4 = TREAT_GRUNDOUGHNUTFRUIT;
        HASEE_LOCK_PALETTE_2();
        hasee_palette_sp[9] = hasee_subpal_green_grundo[1];
        hasee_palette_sp[10] = hasee_subpal_green_grundo[2];
        hasee_palette_sp[11] = hasee_subpal_green_grundo[3];
    } else if (temp5 < PALETTE_3_RNG_THRESHOLD + 38 * RAINBOW_PROBABILITY + GREEN_PROBABILITY) {
        // Originally a ~2% chance individually:
        temp4 = TREAT_GREEN;
        HASEE_LOCK_PALETTE_2();
        hasee_palette_sp[9] = hasee_subpal_green_grundo[1];
        hasee_palette_sp[10] = hasee_subpal_green_grundo[2];
        hasee_palette_sp[11] = hasee_subpal_green_grundo[3];
    } else if (temp5 < PALETTE_2_RNG_THRESHOLD) {
        // ~1.5x as common as green:
        temp4 = TREAT_BLUE;
        HASEE_LOCK_PALETTE_2();
        hasee_palette_sp[9] = hasee_subpal_blue[1];
        hasee_palette_sp[10] = hasee_subpal_blue[2];
        hasee_palette_sp[11] = hasee_subpal_blue[3];
    } else if (temp5 < PALETTE_2_RNG_THRESHOLD + 6553) { // ~10% of the time...
        // Gross item
        if (rand8() & 1) {
            temp4 = TREAT_GROSS_DUNG;
        } else {
            temp4 = TREAT_GROSS_SLIME;
        }
    } else if (temp5 < PALETTE_2_RNG_THRESHOLD + 6553 + 9830) { // ~15%ish of the time...
        // Letter!
        // Pick an uncollected letter
        temp0 = rand8();
        if (temp0 <= 51) { // Wanted to do this more intelligently but it seemed to not work :(
            temp1 = LETTER_H_INDEX;
        } else if (temp0 <= 102) {
            temp1 = LETTER_A_INDEX;
        } else if (temp0 <= 153) {
            temp1 = LETTER_S_INDEX;
        } else if (temp0 <= 204) {
            temp1 = LETTER_E_INDEX;
        } else {
            temp1 = LETTER_E2_INDEX;
        }
        while (letter_status[temp1] != LETTER_UNCOLLECTED) {
            ++temp1;
            if (temp1 > LETTER_E2_INDEX) { temp1 = LETTER_H_INDEX; }
        } // temp1 is now equal to the desired LETTER_WHATEVER_INDEX
        // Since LETTER_E and LETTER_E2 will use the same sprite:
        temp4 = TREAT_PURPLE_H + (MIN(temp1, LETTER_E_INDEX) | temp0 & 0b0100);
    }
    // Otherwise, yellow it stays.
}

void hasee_sprite_collisions(void) {
    // hitbox == the jumping player's hitbox. Should have been set by hasee_movement() somewhere.
    // hitbox2 == a treat's hitbox.

    // if (ACTIVE_PLAYER && player2_stun_timer > 0 || !ACTIVE_PLAYER && player1_stun_timer > 0) {
    //     return; // Do not process for a stunned player
    // }

    // To save on CPU time, we'll only check half of the collisions on each frame.
    // depending on the parity of get_frame_count(), we'll check only indexes of the same parity for a collision.
    //  x = get_frame_count() & 1;
    for (x = 0; x < ONSCREEN_TREATS_MAXIMUM; ++x) { // TODO: See if we can optimize this looping somehow
        if(IS_ENEMY_ACTIVE(x)) {
            hitbox2.x = enemies_x[x];
            hitbox2.y = enemies_y[x];
            check_collision(temp0, hitbox, hitbox2);
            if (temp0) {
                DEACTIVATE_ENEMY(x);
                if(previously_collected_treats_this_jump < 7) {
                    ++previously_collected_treats_this_jump;
                }
                temp0 = enemies_type[x];
                if (temp0 >= TREAT_PURPLE_H) {
                    // Letter
                    // Ensure it is the correct color or don't do anything
                    score += hasee_letter_points[previously_collected_treats_this_jump];
                    sfx_play(SFX_STAR_COLLECT, 0);
                } else if (temp0 <= TREAT_MACCY) {
                    // Unlock palette if applicable
                    if (temp0 >= TREAT_SILVER && temp0 != TREAT_GRUNDOUGHNUTFRUIT) {
                        HASEE_UNLOCK_PALETTE_3();
                    } else if (temp0 >= TREAT_BLUE) {
                        HASEE_UNLOCK_PALETTE_2();
                    }

                    // Regular Point Pickup
                    temp0 <<= 3;
                    temp0 |= previously_collected_treats_this_jump;
                    score += hasee_treat_points[temp0];
                    sfx_play(SFX_STAR_COLLECT, 0);
                } else {
                    // Gross
                    // set stunned timer
                    sfx_play(SFX_ENEMY_KILL, 0);
                    if (ACTIVE_PLAYER) {
                        player2_stun_timer = 240; // 4 second stun
                    } else {
                        player1_stun_timer = 240;
                    }
                }
            }
        }
    }
}

/**
 * temp0 = LETTER_[whatever]_INDEX value for the letter to be set
 * temp1 = color of letter collected (LETTER_COLLECTED_[whatever])
 */
void handle_letter_collection(void) {
    // Check if all letters collected
    // if (...) {}

    // Fill in single letter

    // Enqueue tile update
    AsmSet2ByteAtPtrWithOffset(tile_clear_queue, tile_clear_back, address);
    tile_clear_to_type_queue[tile_clear_back] = temp0;
}

void hasee_treat_movement(void) {
    for (x = 0; x < ONSCREEN_TREATS_MAXIMUM; ++x) {
        if (IS_ENEMY_ACTIVE(x)) {
            // temp3 = ENEMY_DIRECTION(x);
            // temp1 = hasee_leftright_movement_moving_lookup_table[temp3];
            // temp0 = enemies_x[x] + temp1;
            // enemies_x[x] = temp0;
            __asm__("ldy %v", x);
            __asm__("lda %v,y", enemies_flags);
            __asm__("and #%b", 0b00000001);
            __asm__("tay");
            __asm__("lda %v,y", hasee_leftright_movement_moving_lookup_table);
            __asm__("ldy %v", x);
            __asm__("clc");
            __asm__("adc %v,y", enemies_x);
            __asm__("sta %v,y", enemies_x);

            // Deactivate when going offscreen
            temp0 = enemies_timer[x] + 1;
            enemies_timer[x] = temp0;
            temp1 = enemies_x[x];
            if (temp0 > 20 && temp1 >= 0xF8) { // enemies_x will underflow eventually even when going left, so:
                DEACTIVATE_ENEMY(x);
                temp0 = enemies_type[x];
                if (temp0 <= TREAT_MACCY) {                
                    if (temp0 >= TREAT_SILVER && temp0 != TREAT_GRUNDOUGHNUTFRUIT) {
                        HASEE_UNLOCK_PALETTE_3();
                    } else if (temp0 >= TREAT_BLUE) {
                        HASEE_UNLOCK_PALETTE_2();
                    }
                }
            }
        }
    }
}

// hasee_clear_letters
// Technically there will only ever be 4 filled in at a time so we will not run afoul of this
// Need to be clever about only clearing the 4 that are filled in

#pragma rodata-name(pop)
