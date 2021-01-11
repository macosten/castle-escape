// ====
// Castle Escape
//

#include "lib/neslib.h" // Shiru's library. Some functions were modified to reduce stack usage.
#include "lib/nesdoug.h" // dougeff's library. Some functions were modified to reduce stack usage.

#include "mmc1/bank_helpers.h" // MMC1 utility functions.

#include "structs.h" // Some general-purpose structs are defined here (these will probably be moved).
#include "metasprites.h" // Metasprite data is defined here.
#include "metatiles.h" // Metatile data is defined here.
#include "levels.h" // Level data is defined here (in a bunch of headers).
#include "enemies.h"
#include "zeropage.h" // Zeropage declarations are here. Probably better to just extern what you need than import this everywhere.
#include "constants.h" // Constant #defines are here.

#include "tilemaps/compressed_welcome_screen.h"

#include "asm/score.h" // A sort of stripped-down ultoa().
#include "asm/math.h" // A few supporting functions+declarations related to sine and cosine lookups.
#include "asm/macros.h" // Macros that are actually asm statements.
#include "asm/helper.h" // Misc functions I didn't want to import stdlib for.

#include "lib/lzgmini_6502.h" // My version of liblzg, interfaced to work with cc65.


#pragma bss-name(push, "BSS")


// Likewise for RODATA.
// Remember that RODATA is defined in the PRG (unswappable) segment.
#pragma rodata-name ("RODATA")

unsigned char palette_bg[]={
    0x0f, 0x00, 0x10, 0x30, // black, gray, lt gray, white
    0x0f, 0x17, 0x27, 0x38, // Animated. Probably yellow.
    0x0f, 0x06, 0x16, 0x26, // Reds, like the Red Doors
    0x0f, 0x17, 0x27, 0x38, // Yellow(ish)s, like the main level exit doors
};

const unsigned char const palette_sp[]={
    0x0f, 0x16, 0x27, 0x37, // Red, Yellow, Light Yellow
    0x0f, 0x01, 0x0f, 0x32, // valrigard's palette
    0x0f, 0x04, 0x14, 0x24, // Purples.
    0x0f, 0x30, 0x16, 0x00, // HUD(?) and Spikeball/Acid
};

// For shuffling MAX_ENEMIES enemies, plus HUD elements (score, energy).
// Todo: make this calculated at the start of a level.
#define SHUFFLE_ARRAY_LEG (MAX_ENEMIES)
#define SHUFFLE_ARRAY_SIZE (4 * SHUFFLE_ARRAY_LEG)

#if SHUFFLE_ARRAY_SIZE>256
    #warning "Careful: the shuffle array is larger than 256 bytes."
#endif

unsigned char shuffle_array[SHUFFLE_ARRAY_SIZE];
unsigned char shuffle_leg_size;

/*
The function describing the values might be something like:
    n = (MAX_ENEMIES + 3)
    0 <= x < n:  x
    n <= x < 2n: 2n - x
    2n<= x < 3n: (ascending: all even elements, then all odd elements)
    3n<= x < 4n: (descending: all odd elements, then all even elements) 
Example pattern:
     0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15, 
    15,14,13,12,11,10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0,
     0, 2, 4, 6, 8,10,12,14, 1, 3, 5, 7, 9,11,13,15,
    15,13,11, 9, 7, 5, 3, 1,14,12,10, 8, 6, 4, 2, 0,
I don't really know what the ideal pattern would be, though.
*/

// Enemy memory.
Enemies enemies;

// These macros here can probably be optimized a little more 

// Set high bit.
#define ACTIVATE_ENEMY(index) { \
    TEMP[0] = enemies.flags[index] | 0b10000000;\
    enemies.flags[index] = TEMP[0];\
}

// Unset high bit.
#define DEACTIVATE_ENEMY(index) { \
    TEMP[0] = enemies.flags[index] & 0b01111111;\
    enemies.flags[index] = TEMP[0];\
}

#define IS_ENEMY_ACTIVE(index) (enemies.flags[index] & 0b10000000) // Test if high bit is set.

#define GET_ENEMY_TYPE(index) (enemies.type[x])

#define ENEMY_SET_DIRECTION_LEFT(index) {\
    TEMP[0] = enemies.flags[index] & 0b11111110;\
    enemies.flags[index] = TEMP[0];\
}

#define ENEMY_SET_DIRECTION_RIGHT(index) {\
    TEMP[0] = enemies.flags[index] | 0b00000001;\
    enemies.flags[index] = TEMP[0];\
}

#define ENEMY_FLIP_DIRECTION(index) {\
    TEMP[0] = enemies.flags[index] ^ 0b00000001;\
    enemies.flags[index] = TEMP[0];\
}

#define ENEMY_DIRECTION(index)              (enemies.flags[index] &  0b00000001)


// For cannonballs and other 2-axis projectiles.
#define CANNONBALL_SET_NEG_X(index) {\
    TEMP[0] = enemies.flags[index] & 0b10111111;\
    enemies.flags[index] = TEMP[0];\
}

#define CANNONBALL_SET_POS_X(index) {\
    TEMP[0] = enemies.flags[index] | 0b01000000;\
    enemies.flags[index] = TEMP[0];\
}

#define CANNONBALL_SET_NEG_Y(index) {\
    TEMP[0] = enemies.flags[index] & 0b11011111;\
    enemies.flags[index] = TEMP[0];\
}

#define CANNONBALL_SET_POS_Y(index) {\
    TEMP[0] = enemies.flags[index] | 0b00100000;\
    enemies.flags[index] = TEMP[0];\
}

#define CANNONBALL_X_DIRECTION(index) (enemies.flags[index] & 0b01000000)
#define CANNONBALL_Y_DIRECTION(index) (enemies.flags[index] & 0b00100000)

// For Splykes.

#define SPLYKE_SET_STANDING_STILL(index) {\
    TEMP[0] = enemies.flags[index] & 0b11011111;\
    enemies.flags[index] = TEMP[0];\
}

#define SPLYKE_SET_MOVING_AROUND(index) {\
    TEMP[0] = enemies.flags[index] | 0b00100000;\
    enemies.flags[index] = TEMP[0];\
}

#define SPLYKE_IS_MOVING_AROUND(index)   (enemies.flags[index] &  0b00100000)

// For enemies that can be slashed for points, but don't die when slashed.

// Not that this makes sense, but...
#define SLASHABLE_UNKILLABLE_SET_UNSLASHED(index) {\
    TEMP[0] = enemies.flags[index] & 0b11011111;\
    enemies.flags[index] = TEMP[0];\
}

// #define SLASHABLE_UNKILLABLE_SET_SLASHED(index)     (enemies.flags[index] |= 0b00100000)
#define SLASHABLE_UNKILLABLE_SET_SLASHED(index) {\
    TEMP[0] = enemies.flags[index] | 0b00100000;\
    enemies.flags[index] = TEMP[0];\
}

#define SLASHABLE_UNKILLABLE_IS_SLASHED(index)      (enemies.flags[index] &  0b00100000)

#pragma bss-name(pop)

#pragma bss-name(push, "XRAM")

// Collision maps.
#define CMAP_COUNT 6

unsigned char cmap[240 * CMAP_COUNT];

// There's space for more, but I'm leaving it at this for now.

#pragma bss-name(pop)

// Maps the big cmap to nametable-sized chunks.
const unsigned char * const cmaps[] = {cmap, cmap + 240, cmap+240*2, cmap+240*3, cmap+240*4, cmap+240*5}; 

// MARK: Function Prototypes

#pragma code-name ("CODE")

// Drawing functions.
void draw_sprites(void);

void draw_player(void);

void draw_korbat(void);
void draw_grarrl(void);
void draw_spikeball(void);
void draw_cannon(void);
void draw_cannonball(void);
void draw_acid(void);
void draw_acid_drop(void);
void draw_splyke(void);
void draw_sun(void);
void draw_boss(void);
void draw_purple_death_effect(void);
void draw_splyke_death_effect(void);
void draw_magic_bolt(void);

void draw_score(void);
void draw_energy(void);

void draw_screen_sub(void);

void handle_tile_clear_queue(void);

void begin_level(void);
void load_level_new(void);

void calculate_shuffle_array(void);

void load_title_screen(void);
void load_game_over_screen(void);
void load_level_welcome_screen(void);

void clear_screen(void);
void put_str(unsigned int adr, const char * str);

// Physics/Logic functions.
void movement(void);

void swing_sword(void);

void bg_collision(void); // For the player
void bg_collision_sub(void);
void bg_collision_sub_collision_u(void);

void check_spr_objects(void); // For enemies
void sprite_collisions(void);

void collision_with_inert(void);
void collision_with_killable_slashable(void);
void collision_with_inert_slashable(void);
void collision_with_unkillable_unslashable(void);
void collision_with_splyke(void);
void collision_with_boss(void);


void enemy_movement(void);

void korbat_ai(void);
void spikeball_ai(void);
void cannon_ai(void);
void cannonball_ai(void);
void acid_ai(void);
void acid_drop_ai(void);
void splyke_ai(void);
void sun_ai(void);
void boss_ai(void);

void death_effect_timer_ai(void);

extern void dialog_box_handler(void);
extern void trigger_dialog_box(DialogBoxData const * dboxdata);

// MARK: Lookup Tables

// The lookup tables for draw_screen_sub().

const unsigned char const draw_screen_sub_lookup_addr_0[] = {0, 0x40, 0x80, 0xc0};
const unsigned char const draw_screen_sub_lookup_index_offset_0[] = {0, 4, 8, 12};
const unsigned char const draw_screen_sub_lookup_addr_1[] = {0x20, 0x60, 0xa0, 0xe0};
const unsigned char const draw_screen_sub_lookup_index_offset_1[] = {2, 6, 10, 14};

// The lookup tables for enemy_movement().
const unsigned char const leftright_movement_offset_lookup_table[] = {0xff, 15};
const unsigned char const leftright_movement_moving_lookup_table[] = {0xff, 1};
const unsigned char const updown_movement_offset_lookup_table[] = {0xff, 15};

// Offset lookup tables for determining a cannon's direction.
const unsigned char const cannon_ul_sprite_lookup_table[] = {6, 7, 0};
const unsigned char const cannon_ur_sprite_lookup_table[] = {2, 1, 0};
const unsigned char const cannon_dr_sprite_lookup_table[] = {2, 3, 4};
const unsigned char const cannon_dl_sprite_lookup_table[] = {6, 5, 4};
const unsigned char * const cannon_sprite_quadrant_lookup_table[] = {cannon_ul_sprite_lookup_table, cannon_ur_sprite_lookup_table, cannon_dl_sprite_lookup_table, cannon_dr_sprite_lookup_table};


// Lookup tables for valrigard's sprite.
#pragma rodata-name(push, "BANK5") // Metasprite Bank.

const unsigned char * const valrigard_idle_sprite_lookup_table[] = {
    valrigard_idle_left, valrigard_idle_right
};

const unsigned char * const valrigard_sword_swing_sprite_lookup_table[] = {
    // Animation for swinging (reversed): (Left, Right)
    valrigard_swing_left_followthrough, valrigard_swing_right_followthrough,              
    valrigard_swing_left_followthrough, valrigard_swing_right_followthrough,
    valrigard_swing_left_followthrough, valrigard_swing_right_followthrough,
    valrigard_idle_left,                valrigard_idle_right,               
    valrigard_swing_left_high,          valrigard_swing_right_high,
    valrigard_swing_left_mid,           valrigard_swing_right_mid,           
    valrigard_swing_left_low,           valrigard_swing_right_low,
    valrigard_swing_left_low,           valrigard_swing_right_low,
};

const unsigned char * const valrigard_dead_sprite_lookup_table[] = {
    valrigard_dead_left, valrigard_dead_right
};


// Lookup tables for enemy sprites (not yet animated).
const unsigned char * const korbat_sprite_lookup_table[] = {
    korbat_left, korbat_right,
    korbat_flap1_left, korbat_flap1_right,
    korbat_flap2_left, korbat_flap2_right,
    korbat_flap3_left, korbat_flap3_right,
    korbat_flap4_left, korbat_flap4_right,
    korbat_flap5_left, korbat_flap5_right,
    korbat_flap6_left, korbat_flap6_right,
    korbat_flap6_left, korbat_flap6_right,
    korbat_flap5_left, korbat_flap5_right,
    korbat_flap4_left, korbat_flap4_right,
    korbat_flap3_left, korbat_flap3_right,
    korbat_flap2_left, korbat_flap2_right,
    korbat_flap1_left, korbat_flap1_right,
    korbat_left, korbat_right,
};


const unsigned char * const grarrl_sprite_lookup_table[] = {
    grarrl_left, grarrl_right,
    grarrl_backfoot_step0_left, grarrl_backfoot_step0_right,
    grarrl_backfoot_step1_left, grarrl_backfoot_step1_right,
    grarrl_left, grarrl_right,
    grarrl_frontfoot_step0_left, grarrl_frontfoot_step0_right,
    grarrl_frontfoot_step1_left, grarrl_frontfoot_step1_right,
};

// Cannon lookup tables.
const unsigned char * const cannon_sprite_lookup_table[] = {cannon_up, cannon_up_left, cannon_left, cannon_down_left, cannon_down, cannon_down_right, cannon_right, cannon_up_right};

// const unsigned char const enemy_contact_behavior_lookup_table[] = {0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1}

// Frames here are in reverse order so that they can be array-indexed and then have the index decremented.
const unsigned char * const acidblob_sprite_lookup_table[] = {acidblob0, acidblob3, acidblob0, acidblob1, acidblob2, acidblob1, acidblob0};

const unsigned char * const sun_sprite_lookup_table[] = {sun0, sun1};

// Indexed by a 4-bit number with bits arranged as follows:
// [Moving (idle/tornado)][Frame Number MSB][Frame Number LSB][Direction (left/right)]
const unsigned char * const splyke_sprite_lookup_table[] = {
    splyke_idle_left0,  splyke_idle_right0,     splyke_idle_left0,  splyke_idle_right0,
    splyke_idle_left1,  splyke_idle_right1,     splyke_idle_left1,  splyke_idle_right1,
    splyke_tornado0,    splyke_tornado0,        splyke_tornado1,    splyke_tornado1,
    splyke_tornado2,    splyke_tornado2,        splyke_tornado1,    splyke_tornado1
};

const unsigned char * const purple_death_effect_sprite_lookup_table[] = {
    purple_death_effect1, purple_death_effect1, purple_death_effect0
};

const unsigned char * const splyke_death_effect_sprite_lookup_table[] = {
    splyke_death_effect1, splyke_death_effect1, splyke_death_effect0
};

const unsigned char * const energy_bar_lookup_table[] = {
    energy_bar_0, energy_bar_1, energy_bar_2, energy_bar_3, energy_bar_4, energy_bar_5, energy_bar_6, energy_bar_7,
    energy_bar_8, energy_bar_9, energy_bar_a, energy_bar_b, energy_bar_c, energy_bar_d, energy_bar_e, energy_bar_f
};

const unsigned char const boss_magic_offset_table[] = { 0x80, 0x81, 0x82 };

#pragma rodata-name(pop)

void main (void) {

    ppu_off(); // screen off
    
    // load the palettes
    pal_bg(palette_bg);
    pal_spr(palette_sp);

    // use the second set of tiles for sprites
    // both bg and sprites are set to 0 by default
    // Set the swappable bank like this:
    set_prg_bank(0);
    set_chr_bank_0(0);
    //set_chr_bank_1(1);
    bank_spr(1);
    
    set_vram_buffer(); // do at least once, sets a pointer to a buffer
    clear_vram_buffer();
        
    // Set the level index to the first level.
    level_index = 0;

    load_title_screen();

    ppu_on_all(); // turn on screen

    //music_play(0);
    //set_music_speed(5);

    while (1){

        while (game_mode == MODE_TITLE) { 
            ppu_wait_nmi();
            // set_music_speed, etc
    
            

            // Just listen for desired inputs.
            pad1 = pad_poll(0); // read the first controller
            pad1_new = get_pad_new(0);
            
            // Clear the VRAM buffer if it gets used for anything...
            clear_vram_buffer();

            // I suppose if we ever want the game to start immediately,
            // We can just set this to if (1) for debugging.
            if (pad1_new & PAD_UP) {
                // level_index = 0;
                sfx_play(SFX_MENU_BEEP, 0);
                score = 0; // Reset the score.
                begin_level();
            } 

            if (pad1_new & PAD_LEFT) {
                // Decrement the level index, or purposefully underflow it.
                if (level_index != 0) {
                    --level_index;
                } else {
                    level_index = NUMBER_OF_LEVELS - 1;
                }

            } else if (pad1_new & PAD_RIGHT) {
                // Increment the level index, or purposefully overflow it.
                ++level_index;
                if (level_index == NUMBER_OF_LEVELS) {
                    level_index = 0;
                }

            }

            if (pad1_new & PAD_LEFT | pad1_new & PAD_RIGHT) {
                // Code shared between what we'd do with either button press.
                sfx_play(SFX_MENU_BEEP, 0);

                // Update the level name shown on the screen.
                // We must take into account that a remainder of the previous level name will be shown
                // if the length of the next one is less than the length of the current one.
                //memfill(&cmap, ' ', 28); // 28 accounts for the border of 2 tiles from each side
                for (temp0 = 0; temp0 < 28; ++temp0) { cmap[temp0] = ' '; }

                AsmSet2ByteFromPtrAtIndexVar(temppointer, level_names, level_index);
                temp0 = strlen(temppointer);
                for (temp1 = 0; temp1 < temp0; ++temp1) {
                    //temp2 = temppointer[temp1];
                    AsmSet1ByteFromZpPtrAtIndexVar(temp2, temppointer, temp1);
                    cmap[temp1] = temp2;
                }
                multi_vram_buffer_horz(cmap, 28, NTADR_A(3, 10));
            }



        }

        // The game is active and we're drawing frames.
        while (game_mode == MODE_GAME) {

            ppu_wait_nmi(); // wait till beginning of the frame
            // the sprites are pushed from a buffer to the OAM during nmi

            // set_music_speed(64);
            // For now, just set the same chr bank every frame
        
            set_chr_bank_0(0);
            pad1 = pad_poll(0); // read the first controller
            pad1_new = get_pad_new(0); 

            // pad1 contains buttons that are currently pressed.
            // pad1_new contains newly-pressed buttons - they won't be designated here in the next frame.
            // It's useful for times we don't want button holds to retrigger something (say, pausing and unpausing).
            
            clear_vram_buffer();

            // Reset anything that's supposed to be reset at the start of a frame.
            RESET_SCORE_CHANGED_THIS_FRAME();

            // Move the player.
            movement();
            
            // Check to see what's on-screen
            check_spr_objects();

            // Check the status of the sword swing.
            swing_sword();

            // Check sprite collisions
            sprite_collisions();
            
            // Move enemies.
            enemy_movement();
            
            set_scroll_y(scroll_y);
            
            if (SCORE_CHANGED_THIS_FRAME) { convert_to_decimal(score); }
            
            draw_sprites();
            
            // Draw tiles on the edge of the screen.
            if (valrigard.velocity_y >= 0) { // If this is true, draw down. Otherwise, draw up.
                //draw_screen_D();
                add_scroll_y(pseudo_scroll_y, 0x20, scroll_y);
                pseudo_scroll_y += 0xef; 
                // This 0xef (239, which is the height of the screen minus one) might possibly want to be either a 0xf0 (240) or a 
                // 0x100 (1 full nametable compensating for the fact that the last 16 values are masked off by add_scroll_y)
            }  else {
                //draw_screen_U();
                pseudo_scroll_y = sub_scroll_y(0x20, scroll_y);
            }

            temp1 = high_byte(pseudo_scroll_y);
            AsmSet2ByteFromPtrAtIndexVar(temppointer, cmaps, temp1);
            set_data_pointer(temppointer); // Should this value be clamped to the number of cmaps?

            draw_screen_sub();
            // Done drawing tiles on the edge of the screen.

            handle_tile_clear_queue();

            if (game_mode == MODE_GAME_OVER) {
                load_game_over_screen();
            }

            // Debug: clear death status.
            if (pad1 & PAD_DOWN) {
                SET_STATUS_ALIVE();
            }

            // debug:
            gray_line(); // The further down this renders, the fewer clock cycles were free this frame.


        }

        while (game_mode == MODE_GAME_SHOWING_TEXT) {
            // The mode the game is in while a dialog box should block all movement.
            ppu_wait_nmi(); 

            // Music...?

            set_chr_bank_0(0);
            pad1 = pad_poll(0); // read the first controller
            pad1_new = get_pad_new(0); 

            clear_vram_buffer();

            // No movement of any kind.

            draw_sprites();

            // And no screen-drawing (other than what the dialog box code does).

            dialog_box_handler();

        }

        // For now, "game over" is "you win"
        while (game_mode == MODE_GAME_OVER) {
            ppu_wait_nmi();

            pad1 = pad_poll(0);
            pad1_new = get_pad_new(0);

            if (pad1_new & PAD_DOWN) {
                load_title_screen();
            }

        }
        
        
    }
    
}

// In the literal sense, clear the screen. 
// You should call ppu_off() before calling this function.
void clear_screen(void) {
    oam_clear();
    clear_vram_buffer();
    set_scroll_y(0);

    // Clear VRAM.
    vram_adr(NAMETABLE_A);
    vram_fill(0,1024);
}

// Prints a string to the specified place. (Assumes the characters are correctly placed for ASCII.)
void put_str(unsigned int adr, const char *str) {
    vram_adr(adr);
    while (*str) {
        vram_put(*str);
        ++str;
    }
}

const char const title_string[] = "Castle Escape Alpha";
const char const author_string[] = "By macosten";
const char const instruction_string[] = "Press Up to start";

#define MENU_SELECTOR_SPRITE 0x10

void load_title_screen(void) {
    // Eventually we'll want to make a nicer title screen and vram_unrle it, but for now, a simple one:
    ppu_off();
    clear_screen();

    game_mode = MODE_TITLE;

    //multi_vram_buffer_horz(title_string, sizeof(title_string), NTADR_A(3, 2));
    put_str(NTADR_A(3, 2), title_string);
    put_str(NTADR_A(3, 4), author_string);
    put_str(NTADR_A(3, 6), __DATE__);
    put_str(NTADR_A(15, 6), __TIME__);
    put_str(NTADR_A(3, 8), instruction_string);

    AsmSet2ByteFromPtrAtIndexVar(temppointer, level_names, level_index);
    put_str(NTADR_A(3, 10), temppointer);

    ppu_on_all();
}

const char const level_complete_string[] = "Level complete!";
const char const down_to_restart_string[] = "Down to restart.";
void load_game_over_screen(void) {
    ppu_off();
    clear_screen();
    // Set the game mode properly.
    game_mode = MODE_GAME_OVER;

    // Write the message.
    multi_vram_buffer_horz(level_complete_string, sizeof(level_complete_string), NTADR_A(3, 5));
    multi_vram_buffer_horz(down_to_restart_string, sizeof(down_to_restart_string), NTADR_A(3, 7));

    // Turn the PPU back on.
    ppu_on_all();
}

void load_level_welcome_screen(void) {
    ppu_off();
    clear_screen();
    
    LZG_decode(welcome_screen, cmap);

    // Add the level name...
    // Try centering it:
    AsmSet2ByteFromPtrAtIndexVar(temppointer, level_names, level_index);
    temp0 = strlen(temppointer);
    temp1 = 16 - (temp0/2);

    temp_mutablepointer = (cmap + 256 + 64);
    temp_mutablepointer += temp1;

    for (temp2 = 0; temp2 < temp0; ++temp2) {
        //AsmSet1ByteFromZpPtrAtIndexVar(temp3, temppointer, temp2);
        temp_mutablepointer[temp2] = temppointer[temp2];
    }

    // Write what we've buffered to cmap into vram. (That WRAM sure is convenient...)
    vram_write(cmap, (32*30));

    ppu_on_all();

    // Draw a small Valrigard inside the box with the level text.
    set_prg_bank(METASPRITE_BANK);
    oam_meta_spr(120, 114, valrigard_idle_left);

    // Not going to set the game mode this time because we'll call ppu_wait_nmi (we'll wait a few frames) first.
    for (temp0 = 0; temp0 < 120; ++temp0) {
        ppu_wait_nmi();

        // Allow someone to escape this screen early by just pressing a button.
        pad1 = pad_poll(0);
        pad1_new = get_pad_new(0);
        if (pad1_new) { break; }
    }
    oam_clear();
}

void begin_level(void) {
    // Show the welcome screen - the screen that says the level's name (and that gives a bit of respite between levels).
    load_level_welcome_screen();

    ppu_off(); // We're going to make big changes to VRAM so we need to turn it off.
    
    // Set the game mode properly.
    game_mode = MODE_GAME;

    // We're alive now, so let's make sure we're marked as such.
    SET_STATUS_ALIVE();

    // Load the level information.
    load_level_new();

    // Let's seed the RNG...
    seed_rng();

    // ...and calculate this level's shuffle_array.
    calculate_shuffle_array();

    //set_scroll_x(1); // Yeah... this game won't need to scroll in the X direction.

    // Reset Valrigard's energy.
    energy = MAX_ENERGY;

    // Reset the tile clear queue.
    tile_clear_front = 0;
    tile_clear_back = 0;

    // Turn the PPU back on.
    ppu_on_all();
}

void load_level_new(void) {
    // Reset a few variables related to the player
    player_flags = 0; 
    scroll_count = 0; 

    nt_max = level_nt_length[level_index];
    nt_current = valrigard_starting_nt[level_index];
    high_byte(scroll_y) = nt_current; // The high byte of scroll_y is the nametable we're currently in (0-255).
    low_byte(scroll_y) = 2;
    
    high_byte(max_scroll_y) = nt_max - 1; // bottom of this level
    low_byte(max_scroll_y) = 2;
    
    high_byte(min_scroll_y) = 0; // Min Scroll
    low_byte(min_scroll_y) = 0x02;
    
    initial_scroll = ((nt_current * 0x100) - 0x11);
    
    // Set inital coordinates
    temp4 = valrigard_inital_coords[level_index];

    high_byte(valrigard.x) = (temp4 & 0xf0);
    high_byte(valrigard.y) = ((temp4 & 0x0f) * 16) + 2; 
    // +2 to eliminate a bug in which Valrigard could clip through the ceiling and wrap around to the bottom of the 
    // nametable when holding up at the start of a level (this would happen on the Star Test level).

    // Load the level into RAM.
    set_prg_bank(level_nametable_banks[level_index]);

    // New: decompress level data.
    AsmSet2ByteFromPtrAtIndexVar(temppointer, level_compressed_nametable_pointers, level_index);
    LZG_decode(temppointer, cmap);

    // Load inital room data into the PPU.
    AsmSet2ByteFromPtrAtIndexVar(temppointer, cmaps, nt_current);
    set_data_pointer(temppointer);
    set_mt_pointer(metatiles);
    temp1 = high_byte(initial_scroll) + 1;
    temp1 = (temp1 & 1) << 1;
    for(y=0; ;y+=0x20){
        for(x=0; ;x+=0x20){
            clear_vram_buffer(); // do each frame, and before putting anything in the buffer
            
            get_ppu_addr(address, temp1, x, y);
            index = (y & 0xf0) + (x >> 4);
            buffer_4_mt(address, index); // ppu_address, index to the data
            flush_vram_update_nmi();
            if (x == 0xe0) break;
        }
        if (y == 0xe0) break;
    }
    
    temp1 = temp1 ^ 2; // flip that 0000 0010 bit

    // Load a little bit of the next room.

    temp0 = nt_current == 0 ? nt_current + 1 : nt_current - 1;

    AsmSet2ByteFromPtrAtIndexVar(temppointer, cmaps, temp0);
    set_data_pointer(temppointer);
    for(x=0; ;x+=0x20){
        y = 0xe0;
        clear_vram_buffer(); // do each frame, and before putting anything in the buffer
        get_ppu_addr(address, temp1, x, y);
        index = (y & 0xf0) + (x >> 4);
        buffer_4_mt(address, index); // ppu_address, index to the data
        flush_vram_update_nmi();
        if (x == 0xe0) break;
    }
    clear_vram_buffer();


    // Load Enemies

    // Clear the enemy database.
    memfill(&enemies, 0, sizeof(enemies));

    // Load the enemy data for the current level.
    AsmSet2ByteFromPtrAtIndexVar(temppointer, level_enemy_data, level_index);

    for (x = 0, y = 0; x < MAX_ENEMIES; ++x){

        enemies.y[x] = 0;
        AsmSet1ByteFromZpPtrAtIndexVar(temp1, temppointer, y); // Get a byte of data - the bitpacked coords.

        if (temp1 == 0xff) break; // 0xff terminates the enemy data.

        temp2 = temp1 & 0xf0;
        enemies.x[x] = temp2;

        temp2 = (temp1 & 0x0f) << 4;
        enemies.actual_y[x] = temp2;

        ++y; // Next byte:

        AsmSet1ByteFromZpPtrAtIndexVar(temp1, temppointer, y); // the namtetable byte.
        enemies.nt[x] = temp1;

        ++y; // Next byte:

        AsmSet1ByteFromZpPtrAtIndexVar(temp1, temppointer, y); // the type byte.
        enemies.type[x] = temp1; 

        temp1 = GET_ENEMY_TYPE(x);
        if (temp1 == 4) { // ENEMY_CANNON
            // Load in the next enemy as a cannonball.
            ++x;
            enemies.type[x] = ENEMY_CANNONBALL;
        } else if (temp1 == 5) { // ENEMY_ACIDPOOL
            // Load in the next enemy as an acid drop.
            ++x;
            enemies.type[x] = ENEMY_ACIDDROP;
            // extra[x] for this enemy should be a number of frames to wait between acid drops.
            // This varied from drop to drop in the original so I'll make it somewhat random.
            temp2 = rand8() | 0b10000000;
            enemies.extra[x] = temp2;
        }

        ++y; // Next byte.
        
    }

    // Save the number of loaded enemies.
    enemies.count = x+1;
    
    // Set all the other enemies to be NONEs.
    for(++x; x < MAX_ENEMIES; ++x) {
        enemies.type[x] = ENEMY_NONE;
    }

    // This can probably be clumped with the above code, but for clarity,
    // we'll now initialize the extra[x] and extra2[x] values for enemies.

    for (x = 0; x < enemies.count; ++x) {
        switch (GET_ENEMY_TYPE(x)) {
            case 5: // ENEMY_ACIDPOOL
                // Add a random timer from 128 to 255 frames.
                enemies.timer[x] = 0b10000000;
                // fallthrough
            case 4: // ENEMY_CANNON
                // Add a random timer from 0 to 127 frames.
                temp0 = rand8() & 0b01111111;
                enemies.timer[x] = temp0;
                break;
            default:
                // Most enemies do not need special initialization here.
                break;
        }
    }

}

void calculate_shuffle_array(void) {
    temp0 = 0; // Index in the shuffle array
    shuffle_leg_size = enemies.count; // Shuffle array leg size. (1/4 the size of the shuffle array.)
    // First quarter: 0...n
    for (x = 0; x < shuffle_leg_size; ++x) {
        shuffle_array[temp0] = x;
        ++temp0;
    }
    // Second quarter: n...0
    for (x = shuffle_leg_size - 1; ; --x) {
        shuffle_array[temp0] = x;
        ++temp0;
        if (x == 0) { break; }
    }

    // Third quarter: ascending evens from 0 to n, then odds from 0 to n
    for (x = 0; x < shuffle_leg_size; x += 2){
        shuffle_array[temp0] = x;
        ++temp0;
    } 
    for (x = 1; x < shuffle_leg_size; x += 2){
        shuffle_array[temp0] = x;
        ++temp0;
    }

    // Fourth quarter: descending odds from n to 0, then evens from n to 0
    // ...or switch odds and evens here (it depends on the parity of enemies.count).
    // It shouldn't really matter either way.
    for (x = shuffle_leg_size - 1; ; x -= 2) {
        shuffle_array[temp0] = x;
        ++temp0;
        if (x < 2) { break; }
    }
    for (x = shuffle_leg_size - 2; ; x -= 2) {
        shuffle_array[temp0] = x;
        ++temp0;
        if (x < 2) { break; }
    }
    
    // Let's also deal with the other shuffling variables here.
    shuffle_offset = 0; 
    shuffle_maximum = 4 * shuffle_leg_size; // Set the size of the calculated portion of the shuffle array.

}

// A lookup table for enemy draw functions.
const void (* draw_func_pointers[])(void) = {
    draw_korbat,      // 0 - ENEMY_NONE;
    draw_korbat,      // 1 - ENEMY_KORBAT;
    draw_grarrl,      // 2 - ENEMY_GRARRL;
    draw_splyke,      // 3 - ENEMY_SPLYKE;
    draw_cannon,      // 4 - ENEMY_CANNON;
    draw_acid,        // 5 - ENEMY_ACIDPOOL;
    draw_spikeball,   // 6 - ENEMY_SPIKEBALL;
    draw_sun,         // 7 - ENEMY_SUN;
    draw_boss,        // 8 - ENEMY_BOSS;
    draw_cannonball,  // 9 - ENEMY_CANNONBALL;
    draw_acid_drop,   // 10 - ENEMY_ACIDDROP;
    draw_magic_bolt,  // 11 - ENEMY_BOSS_MAGIC_BOLT;
    draw_purple_death_effect, // 12 - ENEMY_PURPLE_DEATH_EFFECT;
    draw_splyke_death_effect, // 13 - ENEMY_SPLYKE_DEATH_EFFECT;
};

void draw_sprites(void) {
    // Ensure the metasprite bank is banked.
    set_prg_bank(METASPRITE_BANK);

    // clear all sprites from sprite buffer
    oam_clear();

    // draw valrigard
    draw_player();
    
    // Drawing the HUD before the enemies.
    // Trying to shoehorn the HUD into the shuffle array proved too costly in terms of performance...
    
    if (game_mode != MODE_GAME_SHOWING_TEXT) {
        draw_score();
        draw_energy();
    }

    // draw enemies.
    for (y = 0; y < shuffle_leg_size; ++y) {
        // Unrolling this loop is a bit inconsistent from frame to frame, so I didn't.

        //x = shuffle_array[y + shuffle_offset];
        temp1 = y + shuffle_offset;
        AsmSet1ByteFromPtrAtIndexVar(x, shuffle_array, temp1);
        if (IS_ENEMY_ACTIVE(x)) {  
            temp_x = enemies.x[x];
            // Not that we should have enemies ever in these X values, 
            // but if we do someday, these lines will be here, waiting to be uncommented.
            //if (temp_x == 0) ++temp_x; // Basing this off NESDoug's report of problems with temp_x = 0.
            //if (temp_x > 0xf0) continue;
            temp_y = enemies.y[x];
            if (temp_y < 0xf0) {

                temp0 = GET_ENEMY_TYPE(x);
                // An assembly macro (defined in asm/macros.h) is used here to ensure that this is efficient.
                AsmCallFunctionAtPtrOffsetByIndexVar(draw_func_pointers, temp0);
            }
        }

    }

    // Handle the shuffle offset.
    shuffle_offset += shuffle_leg_size;
    if (shuffle_offset == shuffle_maximum) { shuffle_offset = 0; }
    
    // Debug HUD, drawn last because it's the least important.
    //oam_spr(232, 42, collision_D, 2);
    
    //oam_spr(200, 50, debug_tile_x >> 4, 1);
    //oam_spr(208, 50, debug_tile_x & 0x0f, 1);
    
    //oam_spr(224, 50, debug_tile_y >> 4, 1);
    //oam_spr(232, 50, debug_tile_y & 0x0f, 1);

    // Animate the animated palette.

    if (!(get_frame_count() & 7)) {
        temp0 = palette_bg[7];
        palette_bg[7] = palette_bg[6];
        palette_bg[6] = palette_bg[5];
        palette_bg[5] = temp0;
        // Almost a call to pal_bg, but only update part of the palette 
        // because only part of the palette is animating.
        // (This is kind of a dumb optimization...)
        __asm__("lda #<%v", palette_bg);
        __asm__("ldx #>%v", palette_bg);
        __asm__("sta %v", TEMP);
        __asm__("stx %v+1", TEMP);
        __asm__("ldx #$00"); // We'd make this #$10 if we wanted pal_spr.
        __asm__("lda #$08"); // Only update the first 8 bytes of the palette.
        __asm__("bne pal_copy");
        //pal_bg(palette_bg);
    }

}

void draw_player(void) {

    temp1 = high_byte(valrigard.x);
    temp2 = high_byte(valrigard.y);

    // I'm less worried about flattening (into one lookup table)/optimizing Valrigard's sprites
    // if only because this will only be called once per frame while the others may be called
    // more than once per frame.

    if (STATUS_DEAD) {
        temp0 = DIRECTION;
        AsmSet2ByteFromPtrAtIndexVar(temppointer, valrigard_dead_sprite_lookup_table, temp0);
    } else if (IS_SWINGING_SWORD) {
        temp0 = (player_frame_timer & 0b11111110) | DIRECTION;
        AsmSet2ByteFromPtrAtIndexVar(temppointer, valrigard_sword_swing_sprite_lookup_table, temp0);
    } else {
        temp0 = DIRECTION;
        AsmSet2ByteFromPtrAtIndexVar(temppointer, valrigard_idle_sprite_lookup_table, temp0);
    }

    oam_meta_spr(temp1, temp2, temppointer);

    // Decrement the player frame timer if it's nonzero.
    if (player_frame_timer) { 
        --player_frame_timer;
    }

}

void draw_score(void) {
    if (score == 0){
        oam_spr(232, 20, 0, 3);
        return;
    }

    y = 4;
    temp0 = 0;
    for (x = 200; x <= 232; x+=8) {
        if (temp0) {
            oam_spr(x, 20, score_string[y], 3);
        } else if (score_string[y]) {
            temp0 = 1;
            oam_spr(x, 20, score_string[y], 3);
        }
        --y;
    }
}

#define ENERGY_BAR_X 216
#define ENERGY_BAR_Y 28

void draw_energy(void) { 
    temp0 = energy >> 3;
    AsmSet2ByteFromPtrAtIndexVar(temppointer, energy_bar_lookup_table, temp0);
    oam_meta_spr(ENERGY_BAR_X, ENERGY_BAR_Y, temppointer);
}

// For all the draw_ functions, temp_x and temp_y are important

void draw_korbat(void) {
    temp3 = enemies.timer[x] & 0b11110; // Derive the frame number from the timer.
    if (temp3 >= (14 << 1)) { // Clamp the frame number to 14.
        temp3 = 0;
        enemies.timer[x] = 0;
    }

    temp3 = temp3 | ENEMY_DIRECTION(x);

    AsmSet2ByteFromPtrAtIndexVar(temppointer, korbat_sprite_lookup_table, temp3); 
    oam_meta_spr(temp_x, temp_y, temppointer);
}

void draw_grarrl(void) {
    temp3 = enemies.timer[x] & 0b111000; // Derive the frame number from the timer.
    if (temp3 >= (6 << 3)) { // Clamp the frame number to 6.
        temp3 = 0;
        enemies.timer[x] = 0;
    }

    temp3 = (temp3 >> 2) | ENEMY_DIRECTION(x); // Last 2 bits just make the animation slower.

    AsmSet2ByteFromPtrAtIndexVar(temppointer, grarrl_sprite_lookup_table, temp3); 
    oam_meta_spr(temp_x, temp_y, temppointer);
}

void draw_spikeball(void) {
    oam_meta_spr(temp_x, temp_y, spikeball);
}

void draw_cannon(void) {
    // Figure out direction of cannon
    temp3 = enemies.extra2[x];
    AsmSet2ByteFromPtrAtIndexVar(temppointer, cannon_sprite_lookup_table, temp3); 
    oam_meta_spr(temp_x, temp_y, temppointer);
}

void draw_cannonball(void) {
    oam_spr(temp_x, temp_y, CANNONBALL_SPRITE_OFFSET, 1);
}

void draw_acid(void) {
    // Tweak these numbers (and the number this is set to in acid_blob_ai) 
    // to adjust the animation speed.
    temp3 = enemies.extra2[x] >> 1;
    AsmSet2ByteFromPtrAtIndexVar(temppointer, acidblob_sprite_lookup_table, temp3);
    oam_meta_spr(temp_x, temp_y, temppointer);
}

void draw_acid_drop(void) {
    oam_spr(temp_x, temp_y, ACIDDROP_SPRITE_OFFSET, 3);
}

void draw_splyke(void) {
    // Assemble an index with which to find the correct sprite.
    // 16 possible values.
    // lsb is the direction.
    // msb is if this splyke is moving around or not (tornado if set or idle if not).
    // The middle two bits are the frame number in the respective animation.
    temp3 = enemies.extra2[x] & 0b110; // Mask the frame number.
    temp4 = ENEMY_DIRECTION(x) | temp3;
    temp4 = temp4 | SPLYKE_IS_MOVING_AROUND(x) >> 2;
    AsmSet2ByteFromPtrAtIndexVar(temppointer, splyke_sprite_lookup_table, temp4);
    oam_meta_spr(temp_x, temp_y, temppointer);
}

void draw_sun(void) {
    // Tweak these numbers to adjust the flashing speed
    temp3 = (enemies.actual_y[x] & 15) >> 3;
    AsmSet2ByteFromPtrAtIndexVar(temppointer, sun_sprite_lookup_table, temp3);
    oam_meta_spr(temp_x, temp_y, temppointer);
}

void draw_boss(void) {
    // Dummied out.
    draw_magic_bolt();
    //oam_spr(temp_x, temp_y, 0x10, 3);
}

void draw_purple_death_effect(void) {
    temp3 = enemies.timer[x] >> 2; // 12 frames -> 3 valid positions
    AsmSet2ByteFromPtrAtIndexVar(temppointer, purple_death_effect_sprite_lookup_table, temp3);
    oam_meta_spr(temp_x, temp_y, temppointer);
}

void draw_splyke_death_effect(void) {
    temp3 = enemies.timer[x] >> 2; // 12 frames -> 3 valid positions
    AsmSet2ByteFromPtrAtIndexVar(temppointer, splyke_death_effect_sprite_lookup_table, temp3);
    oam_meta_spr(temp_x, temp_y, temppointer);
}

void draw_magic_bolt(void) {
    temp3 = enemies.timer[x] >> 3;

    if (temp3 == 3) { 
        temp3 = 0;
        enemies.timer[x] = 0;
    }

    oam_spr(temp_x, temp_y, boss_magic_offset_table[temp3], 0);

    temp3 = enemies.timer[x]; 
    ++temp3;
    enemies.timer[x] = temp3;   
}

// MARK: -- Movement.

void movement(void) {
    
    // Reset the conveyor delta and headbonk status.
    conveyor_delta = 0;
    did_headbonk = 0;
    RESET_TOUCHING_YELLOW_DOOR();

    // Handle X.
    old_x = valrigard.x;
    
    // Left
    if (pad1 & PAD_LEFT) {
        // DIRECTION = LEFT;
        //player_flags &= 0b11111110;
        SET_DIRECTION_LEFT();
        
        if (valrigard.x <= 0x0200) { // Changed by 1 from nesdoug's example because Valrigard's hitbox is narrower by 1 pixel on both sides
            valrigard.velocity_x = 0;
            valrigard.x = 0x200;
        } else if (valrigard.x < 0x0600) { // Don't allow us to wrap to the other side
            valrigard.velocity_x = -0x100;
        } else {
            valrigard.velocity_x = -SPEED;
        }
    }
    // Right
    else if (pad1 & PAD_RIGHT){
        // DIRECTION = RIGHT;
        SET_DIRECTION_RIGHT();
        
        if (valrigard.x >= 0xf000) {  // Also changed by 1 from nesdoug's example because Valrigard's hitbox is narrower by 1 pixel on both sides
            // If VALRIGARD_WIDTH was 15, we'd change this >= into a >...
            valrigard.velocity_x = 0;
            valrigard.x = 0xf000; // ...and we'd add 0x100 to this constant here (and the 0x200 in the PAD_LEFT block would be a 0x100)
        } else if (valrigard.x > 0xec00) { // Don't allow us to wrap to the other side
            valrigard.velocity_x = 0x100;
        } else {
            valrigard.velocity_x = SPEED;
        }
    }
    // Neither Left nor Right
    else {
        valrigard.velocity_x = 0;
    }
    
    // Apply changes in x.
    valrigard.x += valrigard.velocity_x;
    // The collision routine also requires an 8-bit value anyway.

    if((high_byte(valrigard.x) < 0x01)||(high_byte(valrigard.x) > 0xf8)) { // make sure no wrap around to the other side
        valrigard.x = 0x100;
    }
    
    //L_R_switch = 1; // Shrinks the Y values in bg_coll. This makes head/foot collisions less problematic (examine this)
    
    // Copying these bytes like this is faster than passing a pointer to Valrigard.
    hitbox.x = high_byte(valrigard.x);
    hitbox.y = high_byte(valrigard.y);
    hitbox.width = VALRIGARD_WIDTH;
    hitbox.height = VALRIGARD_HEIGHT;
    
    bg_collision();
    if (collision_L && collision_R) { // Half-stuck in a wall, I'm guessing?
        valrigard.x = old_x;
    }
    else if (collision_L) {
        high_byte(valrigard.x) -= eject_L;
    }
    else if (collision_R) {
        high_byte(valrigard.x) -= eject_R;
    }
    
    // MARK: Handle Y.
    // We're probably going to eventually assign flying to A and sword-swinging to B, but... one thing at a time.
    old_y = valrigard.y;

    // MARK: - Gravity
    
    if (pad1 & PAD_UP && energy > 0) { // If we're holding up on the DPad...
        
        if (collision_D) { // If grounded...
            valrigard.velocity_y = -SPEED; // Top speed!
        } else {
            // Otherwise, movement is more nuanced.
            valrigard.velocity_y -= GRAVITY;
            if (valrigard.velocity_y < -SPEED) valrigard.velocity_y = -SPEED;
        }
        
        energy -= 1;
        
    } else {
        
        valrigard.velocity_y += GRAVITY;
        if (valrigard.velocity_y > MAX_FALL) valrigard.velocity_y = MAX_FALL;
        
    }
    
    valrigard.y += valrigard.velocity_y;
    
    // Make sure not to wrap around to the other side
    if (high_byte(valrigard.y) < 0x01) { valrigard.y = 0x100; }
    else if (high_byte(valrigard.y) > 0xf0) { valrigard.y = 0xf000; }
    
    // MARK: - Collision
    //L_R_switch = 0;
    
    hitbox.x = high_byte(valrigard.x);
    hitbox.y = high_byte(valrigard.y);
    // Shouldn't need to change the the height and width since those were already set
    bg_collision();

    // if (collision_U && collision_D) valrigard.y = old_y;
    
    if(collision_U) {
        high_byte(valrigard.y) -= eject_U;

        if (did_headbonk) { 
            high_byte(valrigard.y) += 4;
            valrigard.velocity_y = 0;
            // Play head bonked sound?
        }

    }
    else if (collision_D) {
        high_byte(valrigard.y) -= eject_D;
        // if ... (something was here, but I removed it)
        // Apply the conveyor delta.
        valrigard.x += conveyor_delta;

        energy += 4;
        if (energy > MAX_ENERGY) energy = MAX_ENERGY;

        if (TOUCHING_YELLOW_DOOR && (pad1 & PAD_UP)) {
            game_mode = MODE_GAME_OVER;
        }
    }
    
    // MARK: - Deal with scrolling
    
    temp5 = valrigard.y;
    if (valrigard.y < MAX_UP && scroll_y > min_scroll_y) {
        temp1 = (MAX_UP - valrigard.y + 0x80) >> 8; // "the numbers work better with +80 (like 0.5)". I'll take his word for it.
        scroll_y = sub_scroll_y(temp1, scroll_y);
        high_byte(valrigard.y) += temp1;
    }
    else if (valrigard.y > MIN_DOWN && scroll_y < max_scroll_y) {
        temp1 = (MIN_DOWN + valrigard.y + 0x80) >> 8;
        
        //scroll_y = add_scroll_y(temp1, scroll_y);
        add_scroll_y(scroll_y, temp1, scroll_y);

        high_byte(valrigard.y) -= temp1;
    }

}

void swing_sword(void) {
    if ((pad1 & PAD_A) && !player_sword_timer && !(pad1 & PAD_UP)) {
        player_sword_timer = 37;
        player_frame_timer = 14;
        SET_STATUS_SWINGING_SWORD();
        sfx_play(SFX_SWORD_SLASH,0);
    }

    if (player_sword_timer) { 
        --player_sword_timer;
    } else {
        SET_STATUS_NOT_SWINGING_SWORD();
    }

}

void bg_collision(void){

    // note, !0 = collision
    // sprite collision with backgrounds
    // load the object's x,y,width,height to hitbox, then call this
    
    collision_L = 0;
    collision_R = 0;
    collision_U = 0;
    collision_D = 0;
    
    temp3 = hitbox.y;

    // if(L_R_switch) temp3 += 2; // fix bug, walking through walls
    // Removed this line and I'm not having any issues walking through walls.
    // On the contrary, star collection is now more consistent - before, it would be a little messed up 
    // the top of hitbox was on a different nametable than the bottom.
    // Keeping it here just in case...

    // if(temp3 >= 0xf0) return; // This line will probably only really be relevant if there's no floor/cieling.
    // There shouldn't really be no floor in this game, but maybe I'll want to reuse this code in
    // another game, so I'll leave this line in...

    // For star pickup: recalculate nt_current.
    add_scroll_y(temp6, temp3, scroll_y);
    nt_current = high_byte(temp6);
    // This value of nt_current is correct for the top of the player.

    // Upper left... 

    add_scroll_y(temp5, temp3, scroll_y); // upper left
    temp3 = low_byte(temp5); // low byte y

    temp1 = hitbox.x; // x left
    
    eject_L = temp1 | 0xf0;
    eject_U = temp3 | 0xf0;
    
    bg_collision_sub();
    
    if(collision){ // find a corner in the collision map
        ++collision_L;
        ++collision_U;
        bg_collision_sub_collision_u();
    }

    // Upper right...
    
    temp1 += hitbox.width; // x right
    
    eject_R = (temp1 + 1) & 0x0f;
    
    // temp2,temp3 (high and low byte of 7) are unchanged
    bg_collision_sub();
    
    if(collision){ // find a corner in the collision map
        ++collision_R;
        ++collision_U;
        bg_collision_sub_collision_u();
    }
    
    // Now for the bottom.
    add_scroll_y(temp6, VALRIGARD_HEIGHT, temp6);
    nt_current = high_byte(temp6);
    // This value of nt_current is correct for the bottom of the player.
    // Notice that it *could* be different.
    
    // bottom right, x hasn't changed

    temp3 = hitbox.y + hitbox.height; // y bottom
    // if(L_R_switch) temp3 -= 2; // fix bug, walking through walls -- commented out for now (because *it* was causing bugs).
    
    add_scroll_y(temp5, temp3, scroll_y); // upper left
    temp3 = low_byte(temp5); // low byte y
    
    eject_D = (temp3 + 1) & 0x0f;
    
    if(temp3 >= 0xf0) return;
    
    bg_collision_sub();
    
    if(collision){ // find a corner in the collision map
        ++collision_R;
        ++collision_D;
    }
    
    // bottom left
    temp1 = hitbox.x; // x left
    
    //temp2,temp3 is unchanged

    bg_collision_sub();
    
    if(collision){ // find a corner in the collision map
        ++collision_L;
        ++collision_D;
    }

}

void bg_collision_sub(void) {
    coordinates = (temp1 >> 4) + (temp3 & 0xf0); 
    
    // Instead of selecting from one of two c_maps, select from one of the many cmaps.
    // The index is stored in the high byte of temp5 right now.
    // temp_mutablepointer = (unsigned char *)cmaps[high_byte(temp5)];
    temp0 = high_byte(temp5);
    AsmSet2ByteFromPtrAtIndexVar(temp_mutablepointer, cmaps, temp0);

    //temp4 = temp_mutablepointer[coordinates];
    AsmSet1ByteFromZpPtrAtIndexVar(temp4, temp_mutablepointer, coordinates);

    // Fetch all the properties about this tile.
    temp0 = metatile_property_lookup_table[temp4];

    collision = METATILE_IS_SOLID(temp4);

    // If the collision is with a tile that is solid but has no other properties, return.
    if (temp0 == METATILE_NO_EFFECT || temp0 == METATILE_SOLID) { return; }

    // No? We must have touched a special tile, then.
    if (temp0 & METATILE_YELLOW_DOOR) {
        SET_TOUCHING_YELLOW_DOOR();
    } else if (temp0 & METATILE_POWERUP) {
        // Collect the powerup.
        temp_mutablepointer[coordinates] = EMPTY_TILE;

        // But what powerup was it?
        if (temp4 == STAR_TILE) { 
            ++score;
            sfx_play(SFX_STAR_COLLECT,1); // Star collection
            SET_SCORE_CHANGED_THIS_FRAME();
        }
        else if (temp4 == ENERGY_REFILL_TILE) { energy = MAX_ENERGY; }
    
        // Enqueue a tile to the tile clear queue.
        nt = (nt_current & 1) << 1;
        get_ppu_addr(address, nt, temp1, temp3 & 0xf0);

        // Enqueue.
        AsmSet2ByteAtPtrWithOffset(tile_clear_queue, tile_clear_back, address);
        tile_clear_to_type_queue[tile_clear_back] = EMPTY_TILE;

        ++tile_clear_back;
        tile_clear_back &= 0b11; // Mask to <4

    } else if (temp0 & METATILE_CONVEYOR_LEFT) {
        // this could behave a little strangely if Valrigard specifically walks into a
        // conveyor block from the side. Let's deal with that later, I guess.
        conveyor_delta = LEFT_CONVEYOR_DELTA;
    } else if (temp0 & METATILE_CONVEYOR_RIGHT) {
        conveyor_delta = RIGHT_CONVEYOR_DELTA;
    } else if (temp0 & METATILE_SPIKES) {
        SET_STATUS_DEAD();
    } else if (temp0 & METATILE_RED_DOOR) {
        game_mode = MODE_GAME_OVER;
    }

}

void bg_collision_sub_collision_u(void) {
    // If we just collided with a question block
    // *and* if the bottom of our Y value is 15
    // (otherwise we may have struck it from the side)...
    // It's still *technically* possible to strike it from the side, 
    // though I guess we'd have to check temp1 against something to try preventing it.
    
    // Pull RNG!
    temp0 = rand8();

    if (temp4 == QUESTION_BLOCK && (temp3 & 0x0f) == 0x0f 
        && (temp1 & 0xf0) == ( (high_byte(valrigard.x) + VALRIGARD_HEIGHT/2 ) & 0xf0)) {

        temp_mutablepointer[coordinates] = BONKED_QUESTION_BLOCK;

        // Figure out the correct bonus amount.
        if (temp0 > 128) { ++score; }
        else if (temp0 > 86) { score += 2; }
        else if (temp0 > 43) { score += 3; }
        else if (temp0 > 2) { score += 4; }
        else { score += 100; } /*if (temp0 < 3)*/ 
        SET_SCORE_CHANGED_THIS_FRAME();

        did_headbonk = 1;

        sfx_play(SFX_BUMP, 0);

        // Calculate the correct address to update.
        nt = (nt_current & 1) << 1;
        get_ppu_addr(address, nt, temp1, temp3 & 0xf0);

        // Enqueue a tile update.
        AsmSet2ByteAtPtrWithOffset(tile_clear_queue, tile_clear_back, address);
        tile_clear_to_type_queue[tile_clear_back] = BONKED_QUESTION_BLOCK;

        // Turns out it's actually more complicated to update a single tile's attribute mid-game;
        // as-is this recolors a 2 by 2 mt area (not ideal).
        //address = get_at_addr(nt, temp1, temp3 & 0xf0);
        //AsmSet2ByteAtPtrWithOffset(tile_clear_attr_addr_queue, tile_clear_back, address);
        
        ++tile_clear_back;
        tile_clear_back &= 0b11;

    }
}

// MARK: -- Screen Buffering Functions

// At some point, I should probably inline these functions (manually).

void handle_tile_clear_queue(void) {
    // Since the game sometimes goes a little crazy when you try to collect
    // more than about one star in a frame or so, this queue will prevent any
    // odd screen glitching or stars not going away (ideally).

    // If the queue is empty, return.
    if (tile_clear_front == tile_clear_back) { return; }

    // Otherwise, dequeue and clear the tile.
    
    // Set the color palette (attribute in the attribute table) -- except this does a 2x2 mt area.
    // Not sure if it's worth trying to "solve", but we'll see.
    // AsmSet2ByteFromPtrAtIndexVar(address, tile_clear_attr_addr_queue, tile_clear_front);
    // one_vram_buffer(0x00, address); // Set to the 0th palette

    AsmSet2ByteFromPtrAtIndexVar(address, tile_clear_queue, tile_clear_front);
    AsmSet1ByteFromPtrAtIndexVar(temp0, tile_clear_to_type_queue, tile_clear_front);

    // Buffer 1 mt.
    buffer_1_mt(address, temp0);

    ++tile_clear_front;
    tile_clear_front &= 0b11; // Mask to <4

}

void draw_screen_sub(void) {    
    nt = (temp1 & 1) << 1; // 0 or 2 for vertical scrolling
    y = low_byte(pseudo_scroll_y);
    
    // Important that the main loop clears the vram_buffer.
    
    temp2 = draw_screen_sub_lookup_index_offset_0[scroll_count];
    temp4 = draw_screen_sub_lookup_index_offset_1[scroll_count];

    get_ppu_addr(address, nt, draw_screen_sub_lookup_addr_0[scroll_count], y);
    index = (y & 0xf0) + temp2;
    buffer_4_mt(address, index); // ppu_address, index to the data
            
    get_ppu_addr(address, nt, draw_screen_sub_lookup_addr_1[scroll_count], y);
    index = (y & 0xf0) + temp4;
    buffer_4_mt(address, index); // ppu_address, index to the data
    
    ++scroll_count;
    scroll_count &= 3; //mask off top bits, keep it 0-3
}

// Check to see what's on-screen.
void check_spr_objects(void) {
    
    // This will be used later. Let's only make it update once.
    // Todo: make it only update once a frame? Save a few clock cycles that way?
    nt_current = high_byte(scroll_y);

    // Check enemies...
    for (x = 0; x < enemies.count; ++x) {
        // (Partially) unrolled loop.
        if (GET_ENEMY_TYPE(x)) {
            // Check to see where this enemy is supposed to be.

            //temp5 = (enemies.nt[x] << 8) + enemies.actual_y[x];
            high_byte(temp5) = enemies.nt[x];
            low_byte(temp5) = enemies.actual_y[x];
            
            temp5 -= scroll_y;
            if (high_byte(temp5)) {
                // This enemy isn't on-screen, deactivate it...
                DEACTIVATE_ENEMY(x);
                continue;
            }
            
            ACTIVATE_ENEMY(x); // This enemy is active if it's on-screen.
            enemies.y[x] = low_byte(temp5);

            // If the topmost nametable currently on-screen (nt_current) is
            // not the enemy's native nametable, it'll be shifted down (positive y) by 16.

            // Let's counteract that...
            if (nt_current != enemies.nt[x]) { 
                temp0 = enemies.y[x] - 16;
                enemies.y[x] = temp0;
            }
        }
        ++x;

        if (GET_ENEMY_TYPE(x)) {
            high_byte(temp5) = enemies.nt[x];
            low_byte(temp5) = enemies.actual_y[x];
            temp5 -= scroll_y;
            if (high_byte(temp5)) {
                DEACTIVATE_ENEMY(x);
                continue;
            }
            ACTIVATE_ENEMY(x);
            enemies.y[x] = low_byte(temp5);
            if (nt_current != enemies.nt[x]) { 
                temp0 = enemies.y[x] - 16;
                enemies.y[x] = temp0;
            }
        }
        ++x;

        if (GET_ENEMY_TYPE(x)) {
            high_byte(temp5) = enemies.nt[x];
            low_byte(temp5) = enemies.actual_y[x];
            temp5 -= scroll_y;
            if (high_byte(temp5)) {
                DEACTIVATE_ENEMY(x);
                continue;
            }
            ACTIVATE_ENEMY(x);
            enemies.y[x] = low_byte(temp5);
            if (nt_current != enemies.nt[x]) { 
                temp0 = enemies.y[x] - 16;
                enemies.y[x] = temp0;
            }
        }
        ++x;

        if (GET_ENEMY_TYPE(x)) {
            high_byte(temp5) = enemies.nt[x];
            low_byte(temp5) = enemies.actual_y[x];
            temp5 -= scroll_y;
            if (high_byte(temp5)) {
                DEACTIVATE_ENEMY(x);
                continue;
            }
            ACTIVATE_ENEMY(x);
            enemies.y[x] = low_byte(temp5);
            if (nt_current != enemies.nt[x]) { 
                temp0 = enemies.y[x] - 16;
                enemies.y[x] = temp0;
            }
        }

    }
}

// Widths and heights for enemies.
const unsigned char const enemy_hitbox_width_lookup_table[] = {
    0,  // None
    13, // Korbat 
    13, // Grarrl
    13, // Splyke
    13, // Cannon
    0,  // Acidpool
    13, // Spikeball
    13, // Sun
    13, // Boss
    6,  // Cannonball
    6,  // Aciddrop
    6,  // Magic Bolt
    0,  // Purple Death Effect
    0,  // Splyke Death Effect
};

const unsigned char const enemy_hitbox_height_lookup_table[] = {
    0,  // None
    13, // Korbat 
    13, // Grarrl
    13, // Splyke
    13, // Cannon
    0,  // Acidpool
    13, // Spikeball
    13, // Sun
    13, // Boss
    6,  // Cannonball
    6,  // Aciddrop
    6,  // Magic Bolt
    0,  // ...Particle effect of some sort? 
};

// In case the hitbox isn't quite centered in the sprite:
const unsigned char const enemy_hitbox_x_offset_lookup_table[] = {
    0,
    0,
    0,
    1, // Cannon
    0,
    0,
    0,
    1, // Sun
    0,
    1, // Cannonball
    1, // Aciddrop
    1, // Magic Bolt
    0,
    0,
};

const void (* collision_functions[])(void) = {
    collision_with_inert,                   // None
    collision_with_killable_slashable,      // Korbat
    collision_with_killable_slashable,      // Grarrl
    collision_with_splyke,                  // Splyke
    collision_with_inert_slashable,    // Cannon
    collision_with_inert,                   // Acidpool
    collision_with_unkillable_unslashable,  // Spikeball
    collision_with_unkillable_unslashable,  // Sun
    collision_with_boss,                    // Boss
    collision_with_unkillable_unslashable,  // Cannonball
    collision_with_unkillable_unslashable,  // Aciddrop
    collision_with_unkillable_unslashable,  // Magic Bolt
    collision_with_inert,                   // Purple Death Effect
    collision_with_inert,                   // Splyke Death Effect
};

// Check for sprite collisions with the player.
void sprite_collisions(void) {
    // hitbox == the player's hitbox.
    hitbox.x = high_byte(valrigard.x);
    hitbox.y = high_byte(valrigard.y);

    if (IS_SWINGING_SWORD) {
        // The hitbox should be a little bigger if the sword is swinging.
        hitbox.width = VALRIGARD_SWINGING_WIDTH;
        hitbox.height = VALRIGARD_SWINGING_HEIGHT;
    } else {
        hitbox.width = VALRIGARD_WIDTH;
        hitbox.height = VALRIGARD_HEIGHT;    
    }
    

    // hitbox2 == an enemy's hitbox.
    // The width and height of this will actually be different depending on the enemy's type.

    for (x = 0; x < enemies.count; ++x) {
        // (Partially) unrolled loop.
        if(IS_ENEMY_ACTIVE(x)) {
            temp1 = GET_ENEMY_TYPE(x);

            // Determine the enemy hitbox size.
            hitbox2.width = enemy_hitbox_width_lookup_table[temp1];
            if (!hitbox2.width) { continue; } // Continue if width of the hitbox is 0.

            hitbox2.height = enemy_hitbox_height_lookup_table[temp1];
            
            hitbox2.x = enemies.x[x];
            hitbox2.x += enemy_hitbox_x_offset_lookup_table[temp1];

            hitbox2.y = enemies.y[x];

            check_collision(temp0, hitbox, hitbox2);
            if (temp0) {
                AsmCallFunctionAtPtrOffsetByIndexVar(collision_functions, temp1);
            }
        }
        ++x;
        if(IS_ENEMY_ACTIVE(x)) {
            temp1 = GET_ENEMY_TYPE(x);

            // Determine the enemy hitbox size.
            hitbox2.width = enemy_hitbox_width_lookup_table[temp1];
            if (!hitbox2.width) { continue; } // Continue if width of the hitbox is 0.

            hitbox2.height = enemy_hitbox_height_lookup_table[temp1];
            
            hitbox2.x = enemies.x[x];
            hitbox2.x += enemy_hitbox_x_offset_lookup_table[temp1];

            hitbox2.y = enemies.y[x];

            check_collision(temp0, hitbox, hitbox2);
            if (temp0) {
                AsmCallFunctionAtPtrOffsetByIndexVar(collision_functions, temp1);
            }
        }
        ++x;
        if(IS_ENEMY_ACTIVE(x)) {
            temp1 = GET_ENEMY_TYPE(x);

            // Determine the enemy hitbox size.
            hitbox2.width = enemy_hitbox_width_lookup_table[temp1];
            if (!hitbox2.width) { continue; } // Continue if width of the hitbox is 0.

            hitbox2.height = enemy_hitbox_height_lookup_table[temp1];
            
            hitbox2.x = enemies.x[x];
            hitbox2.x += enemy_hitbox_x_offset_lookup_table[temp1];

            hitbox2.y = enemies.y[x];

            check_collision(temp0, hitbox, hitbox2);
            if (temp0) {
                AsmCallFunctionAtPtrOffsetByIndexVar(collision_functions, temp1);
            }
        }
        ++x;
        if(IS_ENEMY_ACTIVE(x)) {
            temp1 = GET_ENEMY_TYPE(x);

            // Determine the enemy hitbox size.
            hitbox2.width = enemy_hitbox_width_lookup_table[temp1];
            if (!hitbox2.width) { continue; } // Continue if width of the hitbox is 0.

            hitbox2.height = enemy_hitbox_height_lookup_table[temp1];
            
            hitbox2.x = enemies.x[x];
            hitbox2.x += enemy_hitbox_x_offset_lookup_table[temp1];

            hitbox2.y = enemies.y[x];

            check_collision(temp0, hitbox, hitbox2);
            if (temp0) {
                AsmCallFunctionAtPtrOffsetByIndexVar(collision_functions, temp1);
            }
        }
    }

}

// A collision with something that doesn't inherently react.
// It's possible that calls to this can be optimized away.
// (i.e skip if a certain condition?)
void collision_with_inert(void) { }

void collision_with_killable_slashable(void) {
    if (!IS_SWINGING_SWORD) { 
        SET_STATUS_DEAD();
    }
    else {
        // Turn this into a particle effect.
        enemies.type[x] = ENEMY_PURPLE_DEATH_EFFECT;
        enemies.timer[x] = 12;
        score += 1; // Add to the score 
        SET_SCORE_CHANGED_THIS_FRAME();

        sfx_play(SFX_ENEMY_KILL, 0);
        // (todo: make a counter of total active enemies to mimic scoring from original)
    }
}

void collision_with_inert_slashable(void) {
    if (IS_SWINGING_SWORD && !SLASHABLE_UNKILLABLE_IS_SLASHED(x)) { 
        // If swinging and unslashed:
        SLASHABLE_UNKILLABLE_SET_SLASHED(x);
        score += 1;
        SET_SCORE_CHANGED_THIS_FRAME();

        sfx_play(SFX_ENEMY_KILL, 0);
    }
}

void collision_with_unkillable_unslashable(void) {
    // Just die.
    SET_STATUS_DEAD();
}

void collision_with_splyke(void) {
    if (!IS_SWINGING_SWORD) { // Not swinging the sword?
        // Die.
        SET_STATUS_DEAD();
    } else if (!SPLYKE_IS_MOVING_AROUND(x)){ // Not tornado:
        // Kill this.
        enemies.type[x] = ENEMY_SPLYKE_DEATH_EFFECT;
        enemies.timer[x] = 12;
        score += 1;
        SET_SCORE_CHANGED_THIS_FRAME();

        sfx_play(SFX_ENEMY_KILL, 0);
    }
    // Yes tornado, Yes swinging: do nothing.
}

void collision_with_boss(void) {
    game_mode = MODE_GAME_OVER; // Just end the level for now.
}

// A lookup table for enemy AI functions.
const void (* ai_pointers[])(void) = {
    korbat_ai,          // 0 - ENEMY_NONE;
    korbat_ai,          // 1 - ENEMY_KORBAT;
    spikeball_ai,       // 2 - ENEMY_GRARRL;
    splyke_ai,          // 3 - ENEMY_SPLYKE;
    cannon_ai,          // 4 - ENEMY_CANNON;
    acid_ai,            // 5 - ENEMY_ACIDPOOL;
    spikeball_ai,       // 6 - ENEMY_SPIKEBALL;
    sun_ai,             // 7 - ENEMY_SUN;
    boss_ai,            // 8 - ENEMY_BOSS;
    cannonball_ai,      // 9 - ENEMY_CANNONBALL;
    acid_drop_ai,       // 10 - ENEMY_ACIDDROP;
    cannonball_ai,      // 11 - ENEMY_BOSS_MAGIC_BOLT;
    death_effect_timer_ai, // 12 - ENEMY_PURPLE_DEATH_EFFECT;
    death_effect_timer_ai, // 13 - ENEMY_SPLYKE_DEATH_EFFECT;
};

// Enemy AI.
void enemy_movement(void) {
    // This one's a bit of an uncharted realm. 
    // I'm thinking we'll want to optimize this one somehow...
    for (x = 0; x < enemies.count; ++x) {
        // (Partially) unrolled loop.
        if (IS_ENEMY_ACTIVE(x)) {
            temp1 = GET_ENEMY_TYPE(x);
            // An assembly macro (defined in asm/macros.h) is used here to ensure that this is efficient.
            // Do we want to delete (set type to ENEMY_NONE) any projectiles (CANNONBALL/ACIDDROP) that go offscreen?
            AsmCallFunctionAtPtrOffsetByIndexVar(ai_pointers, temp1);
        }
        ++x;
        if (IS_ENEMY_ACTIVE(x)) {
            temp1 = GET_ENEMY_TYPE(x);
            AsmCallFunctionAtPtrOffsetByIndexVar(ai_pointers, temp1);
        }
        ++x;
        if (IS_ENEMY_ACTIVE(x)) {
            temp1 = GET_ENEMY_TYPE(x);
            AsmCallFunctionAtPtrOffsetByIndexVar(ai_pointers, temp1);
        }
        ++x;
        if (IS_ENEMY_ACTIVE(x)) {
            temp1 = GET_ENEMY_TYPE(x);
            AsmCallFunctionAtPtrOffsetByIndexVar(ai_pointers, temp1);
        }
    }

}

// I reordered these to be listed in the order in which they were implemented.

void korbat_ai(void) {
    // Look to see if the metatile ahead of me is solid. If it is, turn around.
    // Then, move forward.

    // Increment the timer. The meaning of the timer will depend on the enemy.
    // Intermediate variable used here because the compiled code is better this way.
    temp0 = enemies.timer[x];
    ++temp0;
    enemies.timer[x] = temp0;

    temp3 = ENEMY_DIRECTION(x);

    temp1 = enemies.x[x];

    // If we're going right, we actually want to look to the right of us.
    temp1 += leftright_movement_offset_lookup_table[temp3];

    temp2 = enemies.actual_y[x] + 6; // center y
    coordinates = (temp1 >> 4) + (temp2 & 0xf0); 

    // Which cmap should I look at?
    temp0 = enemies.nt[x];

    //temppointer = cmaps[temp0];
    AsmSet2ByteFromPtrAtIndexVar(temppointer, cmaps, temp0);
    
    //collision = temppointer[coordinates];
    AsmSet1ByteFromZpPtrAtIndexVar(collision, temppointer, coordinates);
    
    if (METATILE_IS_SOLID(collision)) {
        ENEMY_FLIP_DIRECTION(x);
        temp3 ^= 1;
    }

    temp1 = leftright_movement_moving_lookup_table[temp3];
    
    temp0 = enemies.x[x] + temp1;

    enemies.x[x] = temp0;

}

void spikeball_ai(void) {
    // Look to see if the metatile beneath me is solid. If it isn't, turn around.
    // Look to see if the metatile in front of me is solid. If it is, turn around.
    // Then, move forward.

    // Should also be used for Grarrls.
    
    // Increment the timer. The meaning of the timer will depend on the enemy.
    // Intermediate variable used here because the compiled code is better this way.
    temp0 = enemies.timer[x];
    ++temp0;
    enemies.timer[x] = temp0;

    temp3 = ENEMY_DIRECTION(x);

    temp1 = enemies.x[x];

    // If we're going right, we actually want to look to the right of us.
    temp1 += leftright_movement_offset_lookup_table[temp3];

    // First, check beneath us.

    temp2 = enemies.actual_y[x] + 18; // Y beneath us

    // Account for being on the edge of a nametable by checking. 
    // If temp2 >= 0xf0, then we were on the bottom of a nametable and should look at the other one.
    if (temp2 >= 0xf0) {
        temp4 = enemies.nt[x] + 1;
        temp2 = 0;
    } else {
        temp4 = enemies.nt[x];
    }

    coordinates = (temp1 >> 4) + (temp2 & 0xf0); 

    // Which cmap should I look at?
    //temppointer = cmaps[temp4];
    AsmSet2ByteFromPtrAtIndexVar(temppointer, cmaps, temp4);

    //collision = temppointer[coordinates];
    AsmSet1ByteFromZpPtrAtIndexVar(collision, temppointer, coordinates);
    
    if (!METATILE_IS_SOLID(collision)) {
        ENEMY_FLIP_DIRECTION(x);
        temp3 ^= 1;
    } else { // Don't flip twice -- this causes a bug if we're on an edge and there's a block ahead of us, but not one block down+ahead of us.
       // Now, check ahead of us.

        temp2 = enemies.actual_y[x] + 6; // center y
        coordinates = (temp1 >> 4) + (temp2 & 0xf0); 

        // Which cmap should I look at?
        temp4 = enemies.nt[x];

        //temppointer = cmaps[temp4];
        AsmSet2ByteFromPtrAtIndexVar(temppointer, cmaps, temp4);

        //collision = temppointer[coordinates];
        AsmSet1ByteFromZpPtrAtIndexVar(collision, temppointer, coordinates);
    
        if (METATILE_IS_SOLID(collision)) {
            ENEMY_FLIP_DIRECTION(x);
            temp3 ^= 1;
        } 
    }

    temp1 = leftright_movement_moving_lookup_table[temp3];

    temp0 = enemies.x[x] + temp1;

    enemies.x[x] = temp0;

}

void sun_ai(void) {
    // Look to see if the metatile in front of me is solid. If so, turn around.
    // Then, move vertically (depending on direction)

    temp3 = ENEMY_DIRECTION(x);

    // We'll make use of the add_scroll_y and sub_scroll_y functions for this...

    high_byte(temp5) = enemies.nt[x];
    low_byte(temp5) = enemies.actual_y[x];

    temp1 = enemies.x[x] + 6; 
    
    if (temp3 == UP) { // subtracting from y
        temp5 = sub_scroll_y(1, temp5);

        temp2 = low_byte(temp5); // Y of tile of interest
        temp4 = high_byte(temp5); // NT of tile of interest
    } else { // DOWN (adding to y)
        add_scroll_y(temp5, 1, temp5);
        add_scroll_y(temp6, 15, temp5); // 15 being the cosmetic size of an enemy

        temp2 = low_byte(temp6); // Y of tile of interest
        temp4 = high_byte(temp6); // NT of tile of interest
    }

    //temp2 += updown_movement_offset_lookup_table[temp3];

    coordinates = (temp1 >> 4) + (temp2 & 0xf0);

    // Which cmap should I look at?
    // temppointer = cmaps[temp4];
    AsmSet2ByteFromPtrAtIndexVar(temppointer, cmaps, temp4);

    // collision = temppointer[coordinates];
    AsmSet1ByteFromZpPtrAtIndexVar(collision, temppointer, coordinates);

    if (METATILE_IS_SOLID(collision)) {
        ENEMY_FLIP_DIRECTION(x);
        temp3 ^= 1;
    }

    // Update the actual enemy model itself.
    enemies.nt[x] = high_byte(temp5);
    enemies.actual_y[x] = low_byte(temp5);

}

void acid_drop_ai(void) {
    // If I'm touching a solid metatile, die. Otherwise, go down.

    high_byte(temp5) = enemies.nt[x];
    low_byte(temp5) = enemies.actual_y[x];

    add_scroll_y(temp5, 1, temp5);
    add_scroll_y(temp6, 6, temp5); // 8 being the cosmetic projectile height

    coordinates = (enemies.x[x] >> 4) + (low_byte(temp6) & 0xf0);

    // Which cmap should I look at?
    //temppointer = cmaps[high_byte(temp6)];
    temp0 = high_byte(temp6);
    AsmSet2ByteFromPtrAtIndexVar(temppointer, cmaps, temp0);
    
    //collision = temppointer[coordinates];
    AsmSet1ByteFromZpPtrAtIndexVar(collision, temppointer, coordinates);

    if (METATILE_IS_SOLID(collision)) {
        // Clear the type and flags, then return.
        enemies.type[x] = ENEMY_NONE;
        enemies.flags[x] = 0;
    } else {
        enemies.nt[x] = high_byte(temp5);
        enemies.actual_y[x] = low_byte(temp5);
    }

}

void cannon_ai(void) {
    // Wait a while. Turn towards Valrigard. Fire a cannonball in his direction.

    // extra[x] should be the brads_lookup result for the bitpacked dydx
    // extra2[x] should be the specific sprite we should be showing.

    // The next enemy should be a cannonball. If it's active, don't do anything.
    temp_x = x + 1;
    if (IS_ENEMY_ACTIVE(temp_x)) { return; }

    // Decrement the timer this frame.
    // That'll be *slightly* longer than 1 second on NTSC systems, and
    // it'll be more like 1.28 seconds on PAL systems.
    temp0 = enemies.timer[x];
    --temp0;
    enemies.timer[x] = temp0;

    if (enemies.timer[x] == 20) { 

        // If the high nibble is 0...
        if (!(enemies.extra2[x] & 0xf0)) {
            // Calculate the dydx between my center and Valrigard's center.
            temp0 = high_byte(valrigard.x) + (VALRIGARD_WIDTH/2);
            temp1 = high_byte(valrigard.y) + 4; // Tweaked for maximum accuracy - may need to be tweaked more.

            temp2 = enemies.x[x] + 6; // ENEMY_WIDTH/2
            temp3 = enemies.y[x] + 6; // ENEMY_HEIGHT/2

            // Let's figure out if Valrigard to our left or right.
            // temp4 will be used as an index for cannon_sprite_quadrant_lookup_table.

            temp4 = 0;

            if (temp0 < temp2) {
                // if Valrigard's center X is less, then the cannonball will go in the negative X direction.
                CANNONBALL_SET_NEG_X(temp_x);
            } else {
                // ...otherwise, it'll go in the positive x direction.
                CANNONBALL_SET_POS_X(temp_x);
                ++temp4;
            }

            // ...and now we'll figure out if Valrigard is above or below us.

            if (temp1 < temp3) {
                // Above - go in -Y
                CANNONBALL_SET_NEG_Y(temp_x);
            } else {
                // Below - go in +Y
                CANNONBALL_SET_POS_Y(temp_x);
                temp4 += 0b10;
            }

            // If this is too much work to do in one frame, we *could* use
            // enemies.timer[x] to ensure the rest of this happens next frame.

            // dx
            temp0 = abs_subtract(temp0, temp2);
            if ((temp0 & 0x0f) > 8) { temp0 += 0x10; } // Round metatile x up.
            // This increases the accuracy somewhat.

            // dy
            temp1 = abs_subtract(temp1, temp3);

            // bitpack the dydx (thus converting it into metatile coords)
            // Then look it up in brads_lookup.
            coordinates = (temp1 & 0xf0) + (temp0 >> 4);

            temp2 = brads_lookup(coordinates);

            enemies.extra[x] = temp2;

            // temppointer will be a pointer to the correct sprite lookup table.
            AsmSet2ByteFromPtrAtIndexVar(temppointer, cannon_sprite_quadrant_lookup_table, temp4);

            // Within the sprite lookup table, figure out the sprite.
            if (temp2 > 0x30) { // over 0x30 brads
                temp3 = temppointer[2]; // y-axis aligned
            } else if (temp2 > 0x10) { // over 0x10 brads
                temp3 = temppointer[1]; // diagonal
            } else { // 0x10 or fewer brads
                temp3 = temppointer[0]; // x-axis aligned
            }
            enemies.extra2[x] = temp3;

        }
    } 

    if (enemies.timer[x] == 0) {
        // Fire the cannonball.
        enemies.timer[x] = 120;

        sfx_play(SFX_CANNON_FIRE, 0);

        // Move the cannonball into place.
        temp0 = enemies.x[x] + 3;
        temp1 = enemies.actual_y[x] + 3;
        temp2 = enemies.nt[x];

        enemies.x[temp_x] = temp0;
        enemies.actual_y[temp_x] = temp1;
        enemies.nt[temp_x] = temp2;

        enemies.type[temp_x] = ENEMY_CANNONBALL;

    }
    // Subtract from a (randomly-seeded?) timer of some sort.
    // If that timer == 0, turn to valrigard, then "fire a new cannonball" 
    // (i.e set the cannonball's x to my x, y to my y, etc).
    // Don't forget to tell it what direction it should be going in as well.

}

void cannonball_ai(void) {
    // Continue moving forward in the direction fired. If I hit a solid metatile, die.

    // There is a second type of cannonball in the original game that is beholden to gravity but also faster.
    // I'm not going to prioritize it...

    // extra[x] should be the sub_x.
    // extra2[x] should be the sub_y.

    // brads
    temp_x = x-1;
    temp1 = enemies.extra[temp_x];
    
    // add/sub this to x
    temp2 = cos_lookup(temp1);

    // add/sub this to y
    temp3 = sin_lookup(temp1);

    // Among enemies, only cannonballs will really have sub_x and sub_y.

    // (It's possible I'm prematurely optimizing for RAM space. Not sure.)
    // (If there's enough space, perhaps a sub_x and a sub_y array are appropriate
    // as additional properties of "enemies" - but at the same time, I've only got
    // 2k of RAM in total...)

    // Since there will only ever be a maximum of MAX_ENEMIES/2 cannonballs onscreen
    // at once (as for each cannonball there must also be a cannon), 
    // as long as MAX_ENEMIES <= 60, this solution should be fine:

    // Deal with the x-direction first.

    high_byte(temp5) = enemies.x[x];
    low_byte(temp5) = enemies.extra[x];

    if (CANNONBALL_X_DIRECTION(x)) {
        // Positive X - add temp2
        temp5 += temp2;
        // temp1 will be the x coordinate at which we check for a collision.
        temp1 = high_byte(temp5) + 7;
    } else {
        // Negative X - subtract temp2
        temp5 -= temp2;
        // temp1 will be the x coordinate at which we check for a collision.
        temp1 = high_byte(temp5) + 0xff;
    }
    // Save the high and low bytes.
    enemies.x[x] = high_byte(temp5);
    enemies.extra[x] = low_byte(temp5);

    // It's a bit strange because we have a sub_y and a super_y (nt) in this case, 
    // but we'll take care of the sub_y first...
    high_byte(temp6) = enemies.actual_y[x];
    low_byte(temp6) = enemies.extra2[x];

    
    if (CANNONBALL_Y_DIRECTION(x)) {
        // Positive Y - add.
        temp6 += temp3;

        // Save the low byte to the appropriate place.
        enemies.extra2[x] = low_byte(temp6);
        
        // Move the actual_y value into the low byte
        low_byte(temp6) = high_byte(temp6);
        // and make nt the new high byte.
        high_byte(temp6) = enemies.nt[x];

        // Using add_scroll_y(0, ___) will 
        // correct the value if the low byte is > 0xef.
        // Clamp the value. 
        add_scroll_y(temp6, 0x00, temp6);

        // Save the corrected nt.
        enemies.nt[x] = high_byte(temp6);

        // Save the actual_y value.
        enemies.actual_y[x] = low_byte(temp6);

        // Figure out where the collision should be detected.
        add_scroll_y(temp5, 7, temp6);

    } else {
        // Negative Y - subtract.
        temp6 -= temp3;

        // Return the low byte to the appropriate place.
        enemies.extra2[x] = low_byte(temp6);

        // Move the actual_y value into the low byte
        low_byte(temp6) = high_byte(temp6);
        // and make nt the new high byte.
        high_byte(temp6) = enemies.nt[x];

        // Bugfix: prevent the cannonball from getting stuck at the edge of a nametable.

        // Clamp the value.
        if (!low_byte(temp6)) {
            temp6 = sub_scroll_y(0x01, temp6);
        } else {
            temp6 = sub_scroll_y(0x00, temp6);
        }

        // Save the corrected nt.
        enemies.nt[x] = high_byte(temp6);

        // Save the actual_y value.
        enemies.actual_y[x] = low_byte(temp6);
        
        // Figure out where the collision should be detected.
        add_scroll_y(temp5, 1, temp6);
    }

    // Now that we know where we are, it's time to check to see if this cannonball
    // is colliding with any tiles - if it is, it should disappear.
    
    // The collision nt is in high_byte(temp5).
    // The y value we're looking at is in low_byte(temp5).
    // The x value we're looking at is in temp1.

    coordinates = (temp1 >> 4) + (low_byte(temp5) & 0xf0);

    // Check for a collision in the center.
    
    temp4 = high_byte(temp5);
    //temppointer = cmaps[temp4];
    AsmSet2ByteFromPtrAtIndexVar(temppointer, cmaps, temp4);

    //collision = temppointer[coordinates];
    AsmSet1ByteFromZpPtrAtIndexVar(collision, temppointer, coordinates);

    if (METATILE_IS_SOLID(collision)) {
        // Clear the lower nibble, then return.

        // Clear the enemy type and flags. (Both are necessary.)
        enemies.type[x] = ENEMY_NONE;
        enemies.flags[x] = 0; 

        // Note: These will turn into Spikeballs (with id 0, because that's the AI+Sprite combo I chose for id 0 at the moment)
        // on impact if the flags byte is not set to zero.
        // This could be used to make a new type of cannon if we're really looking to expand what we're doing.

        // It's possible we want to play a sound effect or animation here.
    }

}

void acid_ai(void) {
    // Wait a while, then drop an acid drop.

    // extra[x] should be number of frames this acid blob will wait after its previous drop went away before making another. (Between 128 and 255)
    // extra2[x] should be the specific sprite we should be showing.

    // Animations for this won't yet be implemented, but for now:
    // Animate this enemy.
    if (enemies.extra2[x]) {
        temp0 = enemies.extra2[x];
        --temp0;
        enemies.extra2[x] = temp0;
    }

    // The next enemy should be an acid drop. If it's active, don't do anything.
    temp_x = x + 1;
    if (IS_ENEMY_ACTIVE(temp_x)) { return; }

    temp0 = enemies.timer[x];
    --temp0;
    enemies.timer[x] = temp0;
    if (temp0 == 0) {
        // Reset our timer.
        temp0 = enemies.extra[x];
        enemies.timer[x] = temp0;

        // Move the acid drop into place.
        temp0 = rand8() & 0b111; // Bottom 3 bits, 0 - 7
        temp1 = enemies.x[x] + temp0; 
        temp2 = enemies.actual_y[x];
        temp3 = enemies.nt[x];

        enemies.x[temp_x] = temp1;
        enemies.actual_y[temp_x] = temp2;
        enemies.nt[temp_x] = temp3;

        // Set the enemy in motion.
        enemies.type[temp_x] = ENEMY_ACIDDROP;

        enemies.extra2[x] = 12; // ACIDBLOB_ANIMATION_FRAME_COUNT

        sfx_play(SFX_ACID_DROP, 0);

    }

}


void splyke_ai(void) {
    // Two things happen somewhat randomly when it comes to this enemy:
    // 1: Switch between tornado (unkillable) and regular forms.
    // 2: Move forward (Splyke doesn't move forward every frame).

    // When colliding with a Splyke, the player only dies if they're not swinging their sword.
    // When the Splyke collides with a player, the Splyke only dies if it's not in its Tornado form.

    // timer[x] can be...
    // extra[x] can be...
    // extra2[x] can be a frame counter of some description. (Splykes were not very animated in the original game.)

    temp0 = rand8() & 0x0f; // random from 0 to 15
    temp1 = SPLYKE_IS_MOVING_AROUND(x);

    // Increment the animation counter...
    temp2 = (enemies.extra2[x] + 1) & 7; // Bitmask the animation counter to 7. (this will get >> 1'd in draw_sprites)
    enemies.extra2[x] = temp2;

    if (temp0 == 0 && temp1 == 0) {
        // If we're standing still and we roll a 0...
        SPLYKE_SET_MOVING_AROUND(x); // Start moving.
    } else if (temp1){
        // Should we stop moving?
        if (temp0 == 0) {
            SPLYKE_SET_STANDING_STILL(x);
        }

        // Move. This is identical to a spikeball_ai that 2 x-units instead of one.

        temp3 = ENEMY_DIRECTION(x);

        temp1 = enemies.x[x];

        // If we're going right, we actually want to look to the right of us.
        temp1 += leftright_movement_offset_lookup_table[temp3];
        // Look another pixel in the appropriate direction since we'll be moving two pixels.
        temp1 += leftright_movement_moving_lookup_table[temp3];

        // First, check beneath us.

        temp2 = enemies.actual_y[x] + 18; // Y beneath us
        
        // Account for being on the edge of a nametable by checking. 
        // If temp2 >= 0xf0, then we were on the bottom of a nametable and should look at the other one.
        if (temp2 >= 0xf0) {
            temp4 = enemies.nt[x] + 1;
            temp2 = 0;
        } else {
            temp4 = enemies.nt[x];
        }

        coordinates = (temp1 >> 4) + (temp2 & 0xf0); 


        // Which cmap should I look at?
        //temppointer = cmaps[temp4];
        AsmSet2ByteFromPtrAtIndexVar(temppointer, cmaps, temp4);

        //collision = temppointer[coordinates];
        AsmSet1ByteFromZpPtrAtIndexVar(collision, temppointer, coordinates);
        
        if (!METATILE_IS_SOLID(collision)) {
            ENEMY_FLIP_DIRECTION(x);
            temp3 ^= 1;
        } else { // Don't flip twice.
            // Now, check ahead of us.

            temp2 = enemies.actual_y[x] + 6; // center y
            coordinates = (temp1 >> 4) + (temp2 & 0xf0); 

            // Which cmap should I look at?
            temp4 = enemies.nt[x];

            //temppointer = cmaps[temp4];
            AsmSet2ByteFromPtrAtIndexVar(temppointer, cmaps, temp4);

            //collision = temppointer[coordinates];
            AsmSet1ByteFromZpPtrAtIndexVar(collision, temppointer, coordinates);
            
            if (METATILE_IS_SOLID(collision)) {
                ENEMY_FLIP_DIRECTION(x);
                temp3 ^= 1;
            }
        }

        // Move 2 pixels instead of 1.
        temp1 = leftright_movement_moving_lookup_table[temp3];
        temp1 += temp1;

        temp0 = enemies.x[x] + temp1;

        enemies.x[x] = temp0;

    }

}

void boss_ai(void) {

    // extra[x] will be a bunch of flags. 
    // extra2[x] can be a target of some sort.
    if (enemies.extra[x] == 0) {
        SET_DIRECTION_RIGHT();
        enemies.extra[x] = 128;
        trigger_dialog_box(&boss_dialog);
    } else {

    }

}

void death_effect_timer_ai(void) {
    // Decrement this thing's timer, then check if it's zero.
    temp0 = enemies.timer[x];
    --temp0;
    enemies.timer[x] = temp0;

    if (temp0 == 0){
        // If it's zero, turn this into a None and clear its flags.
        enemies.type[x] = ENEMY_NONE;
        enemies.flags[x] = 0;
    }
}