// using a buffer, we have more flexibility, since we can adjust the
// screen position of a PPU write, and can piggy back multiple data
// sets into 1 push, doing more than 1 update per frame, and
// the data sets can have zeroes, since they are not zero terminated

#include "lib/neslib.h"
#include "lib/nesdoug.h"

#include "mmc1/bank_helpers.h"

#include "structs.h"
#include "metasprites.h"
#include "metatiles.h"
#include "levels.h"
#include "enemies.h"

#include "asm/score.h"
#include "asm/math.h"
#include "asm/macros.h"
#include "asm/helper.h"

#include "lib/lzgmini_6502.h"

#define SPEED 0x150

#define ACCEL 0x20
#define GRAVITY 0x30
#define MAX_SPEED 0x150
#define MAX_FALL MAX_SPEED
#define FLY_VEL -0x600

// All the appropriate stuff below this (in this case, variables)
// should be placed into the named segment.
// This applies to all appropriate stuff between this pragma
// and the next bss-name(pop) pragma.
#pragma bss-name(push, "ZEROPAGE")

// If we wanted to put code or rodata into another bank, we'd do something like:
// #pragma rodata-name("BANK0")
// This will happen for level data and potentially other data we don't need often too.

// MARK: Zero Page Globals
unsigned char pad1; // Stores the state of the game controller.
unsigned char pad1_new; // Stores the state of the game controller.
unsigned char collision; // Used in collision routines.
unsigned char collision_L;
unsigned char collision_R;
unsigned char collision_U;
unsigned char collision_D;
unsigned char coordinates; // Used to index the collision maps.

// Temporary variables, used for a multitude of things.
// Since we don't have many registers in the actual CPU, these sort of act as the next best thing.
// Best to assume that the values of these temps are NOT stored between function calls.
// If the values of the temps *does* matter at the end, then the function name should end in "_sub".
unsigned char temp0;
unsigned char temp1;
unsigned char temp2;
unsigned char temp3;
unsigned char temp4;

unsigned int temp5;
unsigned int temp6;

// Pointers.
const unsigned char * temppointer;
unsigned char * temp_mutablepointer;

void (* temp_funcpointer)(void);

unsigned char eject_L; // Used in the collision routine(s).
unsigned char eject_R;
unsigned char eject_D;
unsigned char eject_U; 


unsigned char player_frame_timer;
unsigned char player_sword_timer;
//unsigned char player_timer;

unsigned char player_flags; // All of these flags should be such that the default value for this byte when starting a level is 0
#define DIRECTION (player_flags & 1) //facing left or right? (lsb of player_flags)
#define SET_DIRECTION_LEFT() (player_flags &= 0b11111110) // Un-set the lsb
#define SET_DIRECTION_RIGHT() (player_flags |= 1) // Set the lsb
#define LEFT 0
#define RIGHT 1
#define UP 0
#define DOWN 1

#define STATUS_DEAD (player_flags & 2)
#define SET_STATUS_ALIVE() (player_flags &= 0b11111101)
#define SET_STATUS_DEAD() (player_flags |= 2) // set bit 1
#define ALIVE 0
#define DEAD 2 // 0b10

#define IS_SWINGING_SWORD (player_flags & 4)
#define SET_STATUS_NOT_SWINGING_SWORD() (player_flags &= 0b11111011)
#define SET_STATUS_SWINGING_SWORD() (player_flags |= 4)

unsigned char game_mode;
// Generally preferring defines like this over enums as enums are ints
// under the hood, and ints are 2 bytes (which makes them slower). 
#define MODE_TITLE 0
#define MODE_GAME 1
#define MODE_PAUSE 2
#define MODE_LEVEL_WELCOME_SCREEN 3
#define MODE_GAME_OVER 4 
// (Uh, do we actually want game overs?) 
// Maybe we just nuke your score every N deaths...

int address; // Used with get_ppu_addr and buffer_4_mt.

unsigned char x; // Used as a loop index.
unsigned char y; // Used as a loop index.
unsigned char index; // Used as an index, for loops and otherwise.

unsigned char nt; // nametable index (though it's only used in 1 place, so...)

unsigned int scroll_x;
unsigned int pseudo_scroll_y;
unsigned int scroll_y;
unsigned int min_scroll_y;
unsigned int max_scroll_y;
unsigned int initial_scroll;
unsigned char scroll_count;
#define MAX_UP 0x4000 // The lowest Y value the player can have before the screen attempts to scroll up.
#define MIN_DOWN 0x8000 // The highest Y value the player can have before the screen attempts to scroll down.

unsigned char L_R_switch;
unsigned int old_x;
unsigned int old_y;
// For enemies:
unsigned char temp_x;
unsigned char temp_y;

unsigned char level_index;

unsigned char energy;
#define MAX_ENERGY 0x70 // 144: 9 (rough number of tiles of flight height with no tapping) * 16(height of [meta]tile in pixels)?
// Or should this be the number of frames which we should be able to fly for?

// Max score of 65535. That feels like it should be enough, right?
unsigned int score;
unsigned char enemy_score;

// At 100, you should get an extra life!
unsigned char stars;

// 255 frames / 60 fps (NTSC) = 4.25 seconds
// Should we also take PAL machines into account and try to change frame counts in these cases?

unsigned char timer;
#define TITLE_SCREEN_LENGTH 120 // ~2 seconds on NTSC machines.

#define SONGS 0 // No songs yet. I might look into FamiStudio...
unsigned char song;
// enum {SONG_NAME1, SONG_NAME2};
// enum {SFX_FLAP, ...};

// Level information.
unsigned char nt_min; // lower bound (included) in the range of nametables we're allowed to scroll in right now.
unsigned char nt_max; // upper bound (not included) in the range of nametables we're allowed to scroll in right now.
unsigned char nt_current; // The nametable Valrigard is currently in. This should help us determine what other nametable to load when scrolling...?

#define VALRIGARD_WIDTH 11
#define VALRIGARD_HEIGHT 13

// The swinging hitbox should be expanded slightly.
#define VALRIGARD_SWINGING_WIDTH 15
#define VALRIGARD_SWINGING_HEIGHT 15

#define METATILE_IS_SOLID(mtid) (metatile_property_lookup_table[mtid] & METATILE_SOLID)


Player valrigard; // A width of 12 makes Valrigard's hitbox a bit more forgiving. It also happens to match up with his nose.
Hitbox hitbox; // Functionally, a parameter for bg_collision (except using the C stack is not preferable to using a global, generally speaking)
// I renamed nesdoug's "Generic" to "Hitbox" to remind me of what purpose it serves.

Hitbox hitbox2; // This hitbox is used for enemies.

unsigned char shuffle_offset;
unsigned char shuffle_maximum;

// Debug variables that get rendered to the screen each frame.
// These will be removed in the future.
unsigned char debug_tile_x;
unsigned char debug_tile_y;

// Added to valrigard.x when standing on a conveyor belt.
#define LEFT_CONVEYOR_DELTA -127
#define RIGHT_CONVEYOR_DELTA 127
signed char conveyor_delta;

unsigned char menu_index;

unsigned int tile_clear_queue[4]; // Each element is one result of get_ppu_addr
unsigned char tile_clear_front;
unsigned char tile_clear_back;

// ~93 zp bytes left?

#pragma bss-name(pop)

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

#define ACTIVATE_ENEMY(index) (enemies.flags[index] |= 0b10000000) // Set high bit.
#define DEACTIVATE_ENEMY(index) (enemies.flags[index] &= 0b01111111) // Unset high bit.
#define IS_ENEMY_ACTIVE(index) (enemies.flags[index] & 0b10000000) // Test if high bit is set.
#define GET_ENEMY_TYPE(index) (enemies.type[x])

#define ENEMY_SET_DIRECTION_LEFT(index)     (enemies.flags[index] &= 0b11111110)
#define ENEMY_SET_DIRECTION_RIGHT(index)    (enemies.flags[index] |= 0b00000001)
#define ENEMY_FLIP_DIRECTION(index)         (enemies.flags[index] ^= 0b00000001)
#define ENEMY_DIRECTION(index)              (enemies.flags[index] &  0b00000001)


// For cannonballs and other 2-axis projectiles.
#define CANNONBALL_SET_NEG_X(index) (enemies.flags[index] &= 0b10111111)
#define CANNONBALL_SET_POS_X(index) (enemies.flags[index] |= 0b01000000)

#define CANNONBALL_SET_NEG_Y(index) (enemies.flags[index] &= 0b11011111)
#define CANNONBALL_SET_POS_Y(index) (enemies.flags[index] |= 0b00100000)

#define CANNONBALL_X_DIRECTION(index) (enemies.flags[index] & 0b01000000)
#define CANNONBALL_Y_DIRECTION(index) (enemies.flags[index] & 0b00100000)

// For Splykes.

#define SPLYKE_SET_STANDING_STILL(index) (enemies.flags[index] &= 0b11011111)
#define SPLYKE_SET_MOVING_AROUND(index)  (enemies.flags[index] |= 0b00100000) 
#define SPLYKE_IS_MOVING_AROUND(index)   (enemies.flags[index] &  0b00100000)

// For enemies that can be slashed for points, but don't die when slashed.

#define SLASHABLE_UNKILLABLE_SET_UNSLASHED(index)   (enemies.flags[index] &= 0b11011111) // Not that this makes sense, but...
#define SLASHABLE_UNKILLABLE_SET_SLASHED(index)     (enemies.flags[index] |= 0b00100000)
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

void draw_score(void);
void draw_energy(void);

void draw_screen_U(void);
void draw_screen_D(void);
void draw_screen_sub(void);

void handle_tile_clear_queue(void);

void begin_level(void);
void load_level_new(void);

void calculate_shuffle_array(void);

void load_title_screen(void);
void load_game_over_screen(void);

void clear_screen(void);
void put_str(unsigned int adr, const char * str);

// Physics/Logic functions.
void movement(void);

void swing_sword(void);

void bg_collision(void); // For the player
void bg_collision_sub(void);


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

// Lookup tables for valrigard's sprite.

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
const unsigned char * const korbat_sprite_lookup_table[] = {korbat_left, korbat_right};
const unsigned char * const grarrl_sprite_lookup_table[] = {grarrl_left, grarrl_right};

// Cannon lookup tables.
const unsigned char * const cannon_sprite_lookup_table[] = {cannon_up, cannon_up_left, cannon_left, cannon_down_left, cannon_down, cannon_down_right, cannon_right, cannon_up_right};

const unsigned char const cannon_ul_sprite_lookup_table[] = {6, 7, 0};
const unsigned char const cannon_ur_sprite_lookup_table[] = {2, 1, 0};
const unsigned char const cannon_dr_sprite_lookup_table[] = {2, 3, 4};
const unsigned char const cannon_dl_sprite_lookup_table[] = {6, 5, 4};
const unsigned char * const cannon_sprite_quadrant_lookup_table[] = {cannon_ul_sprite_lookup_table, cannon_ur_sprite_lookup_table, cannon_dl_sprite_lookup_table, cannon_dr_sprite_lookup_table};
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
                score = 0; // Reset the score.
                begin_level();
            }

            if (pad1_new & PAD_LEFT && level_index != 0) {
                --level_index;
                
                temp0 = strlen(level_names[level_index]);
                multi_vram_buffer_horz(level_names[level_index], temp0, NTADR_A(3, 8));

            }

            if (pad1_new & PAD_RIGHT && level_index < NUMBER_OF_LEVELS-1) {
                ++level_index;

                // Update the level name shown on the screen.
                temp0 = strlen(level_names[level_index]);
                multi_vram_buffer_horz(level_names[level_index], temp0, NTADR_A(3, 8));

            }



        }

        // The game is active and we're drawing frames.
        while (game_mode == MODE_GAME) {

            ppu_wait_nmi(); // wait till beginning of the frame
            // the sprites are pushed from a buffer to the OAM during nmi

            // set_music_speed(???);
            // For now, just set the same chr bank every frame
        
            set_chr_bank_0(0);
            pad1 = pad_poll(0); // read the first controller
            pad1_new = get_pad_new(0); 

            // pad1 contains buttons that are currently pressed.
            // pad1_new contains newly-pressed buttons - they won't be designated here in the next frame.
            // It's useful for times we don't want button holds to retrigger something (say, pausing and unpausing).
            
            clear_vram_buffer();

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
            
            //set_scroll_x(0); // Yeah... this game won't need to scroll in the X direction.
            set_scroll_y(scroll_y);

            convert_to_decimal(score);
            
            draw_sprites();

            if (valrigard.velocity_y >= 0) { // If this is true, draw down. Otherwise, draw up.
                draw_screen_D();
            }  else {
                draw_screen_U();
            } 

            handle_tile_clear_queue();

            if (game_mode == MODE_GAME_OVER) {
                load_game_over_screen();
            }

            // Debug: clear death status.
            if (pad1 & PAD_DOWN) {
                SET_STATUS_ALIVE();
            }

            // debug:
            //gray_line(); // The further down this renders, the fewer clock cycles were free this frame.


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
    put_str(NTADR_A(3, 6), instruction_string);

    put_str(NTADR_A(3, 8), level_names[level_index]);

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

void begin_level(void) {
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
    high_byte(valrigard.y) = ((temp4 & 0x0f) * 16);

    // Load the level into RAM.
    set_prg_bank(level_nametable_banks[level_index]);

    // New: decompress level data.
    LZG_decode(level_compressed_nametable_pointers[level_index], cmap);

    // Load inital room data into the PPU.
    set_data_pointer(cmaps[nt_current]);
    set_mt_pointer(metatiles);
    temp1 = high_byte(initial_scroll) + 1;
    temp1 = (temp1 & 1) << 1;
    for(y=0; ;y+=0x20){
        for(x=0; ;x+=0x20){
            clear_vram_buffer(); // do each frame, and before putting anything in the buffer
            
            address = get_ppu_addr(temp1, x, y);
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

    set_data_pointer(cmaps[temp0]);
    for(x=0; ;x+=0x20){
        y = 0xe0;
        clear_vram_buffer(); // do each frame, and before putting anything in the buffer
        address = get_ppu_addr(temp1, x, y);
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
    temppointer = level_enemy_data[level_index];

    for (x = 0, y = 0; x < MAX_ENEMIES; ++x){

        enemies.y[x] = 0;
        temp1 = temppointer[y]; // Get a byte of data - the bitpacked coords.

        if (temp1 == 0xff) break; // 0xff terminates the enemy data.

        enemies.x[x] = temp1 & 0xf0;
        enemies.actual_y[x] = (temp1 & 0x0f) << 4;

        ++y; // Next byte:

        temp1 = temppointer[y]; // the namtetable byte.
        enemies.nt[x] = temp1;

        ++y; // Next byte:

        temp1 = temppointer[y]; // the type byte.
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
            enemies.extra[x] = rand8() & 0b01111111;
            enemies.extra[x] += 128;
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
                enemies.timer[x] += rand8() & 0b01111111;
                break;
            default:
                // Most enemies do not need special initialization here.
                break;
        }
    }

}

void calculate_shuffle_array(void) {
    temp0 = 0; // Index in the shuffle array
    // First quarter: 0...n
    for (x = 0; x < enemies.count; ++x) {
        shuffle_array[temp0] = x;
        ++temp0;
    }
    // Second quarter: n...0
    for (x = enemies.count - 1; ; --x) {
        shuffle_array[temp0] = x;
        ++temp0;
        if (x == 0) { break; }
    }

    // Third quarter: ascending evens from 0 to n, then odds from 0 to n
    for (x = 0; x < enemies.count; x += 2){
        shuffle_array[temp0] = x;
        ++temp0;
    } 
    for (x = 1; x < enemies.count; x += 2){
        shuffle_array[temp0] = x;
        ++temp0;
    }

    // Fourth quarter: descending odds from n to 0, then evens from n to 0
    // ...or switch odds and evens here (it depends on the parity of enemies.count).
    // It shouldn't really matter either way.
    for (x = enemies.count - 1; ; x -= 2) {
        shuffle_array[temp0] = x;
        ++temp0;
        if (x < 2) { break; }
    }
    for (x = enemies.count - 2; ; x -= 2) {
        shuffle_array[temp0] = x;
        ++temp0;
        if (x < 2) { break; }
    }
    
    // Let's also deal with the other shuffling variables here.
    shuffle_offset = 0; 
    shuffle_maximum = 4 * enemies.count; // Set the size of the calculated portion of the shuffle array.

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
    draw_purple_death_effect, // 11 - ENEMY_PURPLE_DEATH_EFFECT;
    draw_splyke_death_effect, // 12 - ENEMY_SPLYKE_DEATH_EFFECT;
};

void draw_sprites(void) {
    // clear all sprites from sprite buffer
    oam_clear();

    // draw valrigard
    draw_player();
    
    // draw enemies.
    for (y = 0; y < enemies.count; ++y) {
        x = shuffle_array[y + shuffle_offset];

        if (!IS_ENEMY_ACTIVE(x)) { continue; }

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

    // Handle the shuffle offset.
    shuffle_offset += enemies.count;
    if (shuffle_offset >= shuffle_maximum) { shuffle_offset = 0; }

    // Draw the energy level as sprites.
    draw_energy();
    
    // Draw the score.
    // This doesn't really interact with the metasprite system yet, so
    // this can result in a sprite getting hidden in the score somewhere.
    // We could address this by making the score take up a "slot" in the shuffle array.
    // Maybe by doing something like "if x > enemies.count, then draw the score" in the loop above?
    // Same could go for the energy.
    draw_score();

    // Debug HUD, drawn last because it's the least important.
    //oam_spr(232, 42, collision_D, 2);
    
    oam_spr(200, 50, debug_tile_x >> 4, 1);
    oam_spr(208, 50, debug_tile_x & 0x0f, 1);
    
    oam_spr(224, 50, debug_tile_y >> 4, 1);
    oam_spr(232, 50, debug_tile_y & 0x0f, 1);

    // Animate the animated palette.

    if (!(get_frame_count() & 7)) {
        temp0 = palette_bg[7];
        palette_bg[7] = palette_bg[6];
        palette_bg[6] = palette_bg[5];
        palette_bg[5] = temp0;
        pal_bg(palette_bg);
    }

}

void draw_player(void) {

    temp1 = high_byte(valrigard.x);
    temp2 = high_byte(valrigard.y);

    // I'm less worried about flattening (into one lookup table)/optimizing Valrigard's sprites
    // if only because this will only be called once per frame while the others may be called
    // more than once per frame.

    if (STATUS_DEAD) {
        oam_meta_spr(temp1, temp2, valrigard_dead_sprite_lookup_table[DIRECTION]);
        return;
    }

    if (IS_SWINGING_SWORD) {
        temp0 = (player_frame_timer & 0b11111110) | DIRECTION;
        oam_meta_spr(temp1, temp2, valrigard_sword_swing_sprite_lookup_table[temp0]);
    } else {
        oam_meta_spr(temp1, temp2, valrigard_idle_sprite_lookup_table[DIRECTION]);
    }

    // Decrement the player frame timer if it's nonzero.
    if (player_frame_timer) { 
        --player_frame_timer;
    }

}

void draw_score(void) {
    y = 4;
    for (x = 200; x <= 232; x+=8) {
        if (score_string[y] != 0 || y == 0){
            oam_spr(x, 20, score_string[y], 3);
        }
        --y;
        // if (x == 232) break; //Is this more efficient, or is it more efficient to have the condition in the loop's head?
    }
}

void draw_energy(void) {
    temp1 = energy >> 4; 
    oam_spr(200, 28, temp1, 1);
    temp1 = energy & 0x0f;
    oam_spr(208, 28, temp1, 1);
}

// For all the draw_ functions, temp_x and temp_y are important

void draw_korbat(void) {
    temp3 = ENEMY_DIRECTION(x);
    //temppointer = ...;
    oam_meta_spr(temp_x, temp_y, korbat_sprite_lookup_table[temp3]);
}

void draw_grarrl(void) {
    temp3 = ENEMY_DIRECTION(x);
    oam_meta_spr(temp_x, temp_y, grarrl_sprite_lookup_table[temp3]);
}

void draw_spikeball(void) {
    oam_meta_spr(temp_x, temp_y, spikeball);
}

void draw_cannon(void) {
    // Figure out direction of cannon
    temp3 = enemies.extra2[x];
    oam_meta_spr(temp_x, temp_y, cannon_sprite_lookup_table[temp3]);
}

void draw_cannonball(void) {
    oam_spr(temp_x, temp_y, CANNONBALL_SPRITE_OFFSET, 1);
}

void draw_acid(void) {
    // Tweak these numbers (and the number this is set to in acid_blob_ai) 
    // to adjust the animation speed.
    temp3 = enemies.extra2[x];
    temp3 = temp3 >> 1; 
    oam_meta_spr(temp_x, temp_y, acidblob_sprite_lookup_table[temp3]);
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

    oam_meta_spr(temp_x, temp_y, splyke_sprite_lookup_table[temp4]);
}

void draw_sun(void) {
    // Tweak these numbers to adjust the flashing speed
    temp3 = enemies.actual_y[x] & 15;
    temp3 = temp3 >> 3;
    oam_meta_spr(temp_x, temp_y, sun_sprite_lookup_table[temp3]);
}

void draw_boss(void) {
    // Dummied out.
    oam_spr(temp_x, temp_y, 0x10, 3);
}

void draw_purple_death_effect(void) {
    temp3 = enemies.timer[x] >> 2; // 12 frames -> 3 valid positions
    oam_meta_spr(temp_x, temp_y, purple_death_effect_sprite_lookup_table[temp3]);
}

void draw_splyke_death_effect(void) {
    temp3 = enemies.timer[x] >> 2; // 12 frames -> 3 valid positions
    oam_meta_spr(temp_x, temp_y, splyke_death_effect_sprite_lookup_table[temp3]);
}


// MARK: -- Movement.

void movement(void) {
    
    // Reset the conveyor delta.
    conveyor_delta = 0;

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

    if((valrigard.x < 0x100)||(valrigard.x > 0xf800)) { // make sure no wrap around to the other side
        valrigard.x = 0x100;
    }
    
    L_R_switch = 1; // Shrinks the Y values in bg_coll. This makes head/foot collisions less problematic (examine this)
    
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
        valrigard.velocity_y -= GRAVITY;
        if (valrigard.velocity_y < -SPEED) valrigard.velocity_y = -SPEED;
    
        energy -= 1;
        
    } else {
        
        valrigard.velocity_y += GRAVITY;
        if (valrigard.velocity_y > MAX_FALL) valrigard.velocity_y = MAX_FALL;
        
    }
    
    valrigard.y += valrigard.velocity_y;
    
    // Make sure not to wrap around to the other side
    if (valrigard.y < 0x100)  valrigard.y = 0x100;
    if (valrigard.y > 0xf000) valrigard.y = 0xf000;
    
    // MARK: - Collision
    L_R_switch = 0;
    
    hitbox.x = high_byte(valrigard.x);
    hitbox.y = high_byte(valrigard.y);
    // Shouldn't need to change the the height and width since those were already set
    bg_collision();

    // if (collision_U && collision_D) valrigard.y = old_y;
    
    if(collision_U) {
        high_byte(valrigard.y) -= eject_U;
        // Play head_hit sound
    }
    else if (collision_D) {
        high_byte(valrigard.y) -= eject_D;
        // if ... (something was here, but I removed it)
        // Apply the conveyor delta.
        valrigard.x += conveyor_delta;

        energy += 4;
        if (energy > MAX_ENERGY) energy = MAX_ENERGY;
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
        scroll_y = add_scroll_y(temp1, scroll_y);
        high_byte(valrigard.y) -= temp1;
    }

}

void swing_sword(void) {
    if ((pad1 & PAD_A) && !player_sword_timer && !(pad1 & PAD_UP)) {
        player_sword_timer = 37;
        player_frame_timer = 14;
        SET_STATUS_SWINGING_SWORD();
    }

    if (player_sword_timer) { 
        --player_sword_timer;
    } else {
        SET_STATUS_NOT_SWINGING_SWORD();
    }

}

void bg_collision(void){

    temp5 = add_scroll_y(high_byte(valrigard.y), scroll_y);
    nt_current = high_byte(temp5);

    // note, !0 = collision
    // sprite collision with backgrounds
    // load the object's x,y,width,height to hitbox, then call this
    
    collision_L = 0;
    collision_R = 0;
    collision_U = 0;
    collision_D = 0;
    
    temp3 = hitbox.y;
    if(L_R_switch) temp3 += 2; // fix bug, walking through walls
    
    if(temp3 >= 0xf0) return;
    
    // Upper left... 

    temp5 = add_scroll_y(temp3, scroll_y); // upper left
    temp3 = temp5 & 0xff; // low byte y

    temp1 = hitbox.x; // x left
    
    eject_L = temp1 | 0xf0;
    eject_U = temp3 | 0xf0;
    
    bg_collision_sub();
    
    if(collision){ // find a corner in the collision map
        ++collision_L;
        ++collision_U;
    }

    // Upper right...
    
    temp1 += hitbox.width; // x right
    
    eject_R = (temp1 + 1) & 0x0f;
    
    // temp2,temp3 (high and low byte of 7) are unchanged
    bg_collision_sub();
    
    if(collision){ // find a corner in the collision map
        ++collision_R;
        ++collision_U;
    }
    
    
    // again, lower
    
    // bottom right, x hasn't changed

    temp3 = hitbox.y + hitbox.height; // y bottom
    if(L_R_switch) temp3 -= 2; // fix bug, walking through walls
    temp5 = add_scroll_y(temp3, scroll_y); // upper left
    temp3 = temp5 & 0xff; // low byte y
    
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
    temp_mutablepointer = (unsigned char *)cmaps[high_byte(temp5)];
    temp4 = temp_mutablepointer[coordinates];
    
    // Fetch all the properties about this tile.
    temp0 = metatile_property_lookup_table[temp4];

    collision = METATILE_IS_SOLID(temp4);

    // If the collision is with a tile that is solid but has no other properties, return.
    if (temp0 == METATILE_NO_EFFECT || temp0 == METATILE_SOLID) { return; }

    // No? We must have touched a special tile, then.
    if (temp0 & METATILE_SPIKES) {
        SET_STATUS_DEAD();
    } else if (temp0 & METATILE_POWERUP) {
        // Collect the powerup.
        temp_mutablepointer[coordinates] = EMPTY_TILE;

        // But what powerup was it?
        if (temp4 == STAR_TILE) { score += 1; }
        else if (temp4 == ENERGY_REFILL_TILE) { energy = MAX_ENERGY; }
    
        // Enqueue a tile to the tile clear queue.
        nt = (nt_current & 1) << 1;
        address = get_ppu_addr(nt, temp1, temp3 & 0xf0);

        // Enqueue.
        tile_clear_queue[tile_clear_back] = address;
        ++tile_clear_back;
        tile_clear_back &= 0b11; // Clamp to <4 

    } else if (temp0 & METATILE_CONVEYOR_LEFT) {
        // this could behave a little strangely if Valrigard specifically walks into a
        // conveyor block from the side. Let's deal with that later, I guess.
        conveyor_delta = LEFT_CONVEYOR_DELTA;
    } else if (temp0 & METATILE_CONVEYOR_RIGHT) {
        conveyor_delta = RIGHT_CONVEYOR_DELTA;
    } else if (temp0 & METATILE_YELLOW_DOOR) {
        // For now, end the game.
        if (pad1 & PAD_UP) {
            game_mode = MODE_GAME_OVER;
        }
    } else if (temp0 & METATILE_RED_DOOR) {
        game_mode = MODE_GAME_OVER;
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
    address = tile_clear_queue[tile_clear_front]; 

    ++tile_clear_front;
    tile_clear_front &= 0b11; // Clamp to <4

    // Buffer 1 empty mt.
    buffer_1_mt(address, EMPTY_TILE);

    // Shouldn't really need to change the palette, but:
    // one_vram_buffer(0, address); Set to the 0th palette

}

void draw_screen_U(void) {
    pseudo_scroll_y = sub_scroll_y(0x20, scroll_y);
    
    draw_screen_sub();
}

void draw_screen_D(void) {
    pseudo_scroll_y = add_scroll_y(0x20, scroll_y) + 0xef; 
    // This 0xef (239, which is the height of the screen minus one) might possibly want to be either a 0xf0 (240) or a 
    // 0x100 (1 full nametable compensating for the fact that the last 16 values are masked off by add_scroll_y)
    
    draw_screen_sub();
}

void draw_screen_sub(void) {
    temp1 = high_byte(pseudo_scroll_y);
    
    //set_data_pointer(level_nametables[temp1]);
    set_data_pointer(cmaps[temp1]); // TODO: clamp this value to 6.
    nt = (temp1 & 1) << 1; // 0 or 2
    y = pseudo_scroll_y & 0xff;
    
    // Important that the main loop clears the vram_buffer.
    
    temp2 = draw_screen_sub_lookup_index_offset_0[scroll_count];
    temp4 = draw_screen_sub_lookup_index_offset_1[scroll_count];

    address = get_ppu_addr(nt, draw_screen_sub_lookup_addr_0[scroll_count], y);
    index = (y & 0xf0) + temp2;
    buffer_4_mt(address, index); // ppu_address, index to the data
            
    address = get_ppu_addr(nt, draw_screen_sub_lookup_addr_1[scroll_count], y);
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
        if (GET_ENEMY_TYPE(x) == 0 /*ENEMY_NONE*/) continue; 
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
        enemies.y[x] = temp5 & 0xff;

        // If the topmost nametable currently on-screen (nt_current) is
        // not the enemy's native nametable, it'll be shifted down (positive y) by 16.

        // Let's counteract that...
        if (nt_current != enemies.nt[x]) { enemies.y[x] -= 16; }

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
    0,  // ...Particle effect of some sort? 
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
    0,  // ...Particle effect of some sort? 
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

        if(IS_ENEMY_ACTIVE(x)) {
            temp1 = GET_ENEMY_TYPE(x);

            // Determine the enemy hitbox size.
            hitbox2.width = enemy_hitbox_width_lookup_table[temp1];
            if (!hitbox2.width) { continue; } // Continue if width of the hitbox is 0.

            hitbox2.height = enemy_hitbox_height_lookup_table[temp1];
            
            hitbox2.x = enemies.x[x];
            hitbox2.y = enemies.y[x];

            if (check_collision(&hitbox, &hitbox2)) {
                AsmCallFunctionAtPtrOffsetByIndexVar(collision_functions, temp1);
            }
        }
        // Todo: Loop Unrolling?
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
        // (todo: make a counter of total active enemies to mimic scoring from original)
    }
}

void collision_with_inert_slashable(void) {
    if (IS_SWINGING_SWORD && !SLASHABLE_UNKILLABLE_IS_SLASHED(x)) { 
        // If swinging and unslashed:
        SLASHABLE_UNKILLABLE_SET_SLASHED(x);
        score += 1;
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
    }
    // Yes tornado, Yes swinging: do nothing.
}

void collision_with_boss(void) {

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
    death_effect_timer_ai, // 11 - ENEMY_PURPLE_DEATH_EFFECT;
    death_effect_timer_ai, // 12 - ENEMY_SPLYKE_DEATH_EFFECT;
};

// Enemy AI.
void enemy_movement(void) {
    // This one's a bit of an uncharted realm. 
    // I'm thinking we'll want to optimize this one somehow...

    for (x = 0; x < enemies.count; ++x) {
        if (IS_ENEMY_ACTIVE(x)) {
            temp1 = GET_ENEMY_TYPE(x);
            // An assembly macro (defined in asm/macros.h) is used here to ensure that this is efficient.
            // Do we want to delete (set type to ENEMY_NONE) any projectiles (CANNONBALL/ACIDDROP) that go offscreen?
            AsmCallFunctionAtPtrOffsetByIndexVar(ai_pointers, temp1);
        }
    }

}

// I reordered these to be listed in the order in which they were implemented.

void korbat_ai(void) {
    // Look to see if the metatile ahead of me is solid. If it is, turn around.
    // Then, move forward.

    // Convert the coords of this enemy to metatile coords.

    temp3 = ENEMY_DIRECTION(x);

    temp1 = enemies.x[x];

    // If we're going right, we actually want to look to the right of us.
    temp1 += leftright_movement_offset_lookup_table[temp3];

    temp2 = enemies.actual_y[x] + 6; // center y
    coordinates = (temp1 >> 4) + (temp2 & 0xf0); 

    // Which cmap should I look at?
    temp0 = enemies.nt[x];
    temppointer = cmaps[temp0];
    collision = temppointer[coordinates];
    
    if (METATILE_IS_SOLID(collision)) {
        ENEMY_FLIP_DIRECTION(x);
        temp3 ^= 1;
    }

    temp1 = leftright_movement_moving_lookup_table[temp3];
    enemies.x[x] += temp1;

}

void spikeball_ai(void) {
    // Look to see if the metatile beneath me is solid. If it isn't, turn around.
    // Look to see if the metatile in front of me is solid. If it is, turn around.
    // Then, move forward.

    // Should also be used for Grarrls.

    // TODO: Consider the case of us being on the exact edge of a nametable.
    // (i.e we're in nt 1, y tile = 0xD; below us should be nt 2, y tile = 0x0)
    
    temp3 = ENEMY_DIRECTION(x);

    temp1 = enemies.x[x];

    // If we're going right, we actually want to look to the right of us.
    temp1 += leftright_movement_offset_lookup_table[temp3];

    // First, check beneath us.

    temp2 = enemies.actual_y[x] + 18; // Y beneath us
    coordinates = (temp1 >> 4) + (temp2 & 0xf0); 

    // Account for being on the edge of a nametable...
    temp4 = temp2 >> 4;

    // ...by checking. 
    // If temp4 == 0xf, then we were on the bottom of a nametable and should look at the other one.
    if (temp4 == 0xf) {
        temp4 = enemies.nt[x] + 1;
    } else {
        temp4 = enemies.nt[x];
    }

    // Which cmap should I look at?
    temppointer = cmaps[temp4];
    collision = temppointer[coordinates];
    
    if (!METATILE_IS_SOLID(collision)) {
        ENEMY_FLIP_DIRECTION(x);
        temp3 ^= 1;
    }

    // Now, check ahead of us.

    temp2 = enemies.actual_y[x] + 6; // center y
    coordinates = (temp1 >> 4) + (temp2 & 0xf0); 

    // Which cmap should I look at?
    temp4 = enemies.nt[x];
    temppointer = cmaps[temp4];
    collision = temppointer[coordinates];
    
    if (METATILE_IS_SOLID(collision)) {
        ENEMY_FLIP_DIRECTION(x);
        temp3 ^= 1;
    }

    temp1 = leftright_movement_moving_lookup_table[temp3];
    enemies.x[x] += temp1;

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
        temp5 = add_scroll_y(1, temp5);
        temp6 = add_scroll_y(15, temp5); // 15 being the cosmetic size of an enemy

        temp2 = low_byte(temp6); // Y of tile of interest
        temp4 = high_byte(temp6); // NT of tile of interest
    }

    //temp2 += updown_movement_offset_lookup_table[temp3];

    coordinates = (temp1 >> 4) + (temp2 & 0xf0);

    // Which cmap should I look at?
    temppointer = cmaps[temp4];
    collision = temppointer[coordinates];

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

    temp1 = enemies.x[x];

    temp5 = add_scroll_y(1, temp5);
    temp6 = add_scroll_y(6, temp5); // 8 being the cosmetic projectile height

    coordinates = (temp1 >> 4) + (low_byte(temp6) & 0xf0);

    // Which cmap should I look at?
    temppointer = cmaps[high_byte(temp6)];
    collision = temppointer[coordinates];

    if (METATILE_IS_SOLID(collision)) {
        // Clear the type and flags, then return.
        enemies.type[x] = ENEMY_NONE;
        enemies.flags[x] = 0;
        return;
    }

    enemies.nt[x] = high_byte(temp5);
    enemies.actual_y[x] = low_byte(temp5);

}

void cannon_ai(void) {
    // Wait a while. Turn towards Valrigard. Fire a cannonball in his direction.

    // extra[x] should be the brads_lookup result for the bitpacked dydx
    // extra2[x] should be the specific sprite we should be showing.

    // The next enemy should be a cannonball. If it's active, don't do anything.
    if (IS_ENEMY_ACTIVE(x+1)) { return; }

    // Decrement the timer this frame.
    // That'll be *slightly* longer than 1 second on NTSC systems, and
    // it'll be more like 1.28 seconds on PAL systems.
    if (--enemies.timer[x] == 20) { 

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
                CANNONBALL_SET_NEG_X(x+1);
            } else {
                // ...otherwise, it'll go in the positive x direction.
                CANNONBALL_SET_POS_X(x+1);
                ++temp4;
            }

            // ...and now we'll figure out if Valrigard is above or below us.

            if (temp1 < temp3) {
                // Above - go in -Y
                CANNONBALL_SET_NEG_Y(x+1);
            } else {
                // Below - go in +Y
                CANNONBALL_SET_POS_Y(x+1);
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
            temppointer = cannon_sprite_quadrant_lookup_table[temp4];

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

        // Move the cannonball into place.
        temp0 = enemies.x[x] + 3;
        temp1 = enemies.actual_y[x] + 3;
        temp2 = enemies.nt[x];

        enemies.x[x+1] = temp0;
        enemies.actual_y[x+1] = temp1;
        enemies.nt[x+1] = temp2;

        enemies.type[x+1] = ENEMY_CANNONBALL;

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
    temp1 = enemies.extra[x - 1];
    
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
        temp6 = add_scroll_y(0x00, temp6);

        // Save the corrected nt.
        enemies.nt[x] = high_byte(temp6);

        // Save the actual_y value.
        enemies.actual_y[x] = low_byte(temp6);

        // Figure out where the collision should be detected.
        temp5 = add_scroll_y(7, temp6);

    } else {
        // Negative Y - subtract.
        temp6 -= temp3;

        // Return the low byte to the appropriate place.
        enemies.extra2[x] = low_byte(temp6);

        // Move the actual_y value into the low byte
        low_byte(temp6) = high_byte(temp6);
        // and make nt the new high byte.
        high_byte(temp6) = enemies.nt[x];

        // Clamp the value. 
        temp6 = sub_scroll_y(0x00, temp6);

        // Save the corrected nt.
        enemies.nt[x] = high_byte(temp6);

        // Save the actual_y value.
        enemies.actual_y[x] = low_byte(temp6);
        
        // Figure out where the collision should be detected.
        temp5 = add_scroll_y(1, temp6);
    }

    // Now that we know where we are, it's time to check to see if this cannonball
    // is colliding with any tiles - if it is, it should disappear.
    
    // The collision nt is in high_byte(temp5).
    // The y value we're looking at is in low_byte(temp5).
    // The x value we're looking at is in temp1.

    coordinates = (temp1 >> 4) + (low_byte(temp5) & 0xf0);

    // Check for a collision in the center.
    temppointer = cmaps[high_byte(temp5)];
    collision = temppointer[coordinates];

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
    if (enemies.extra2[x] > 0) {
        --enemies.extra2[x];
    }

    // The next enemy should be an acid drop. If it's active, don't do anything.
    if (IS_ENEMY_ACTIVE(x+1)) { return; }

    if (--enemies.timer[x] == 0) {
        // Reset our timer.
        temp0 = enemies.extra[x];
        enemies.timer[x] = temp0;

        // Move the acid drop into place.
        temp0 = rand8() & 0b111; // Bottom 3 bits, 0 - 7
        temp1 = enemies.x[x] + temp0; 
        temp2 = enemies.actual_y[x];
        temp3 = enemies.nt[x];

        enemies.x[x+1] = temp1;
        enemies.actual_y[x+1] = temp2;
        enemies.nt[x+1] = temp3;

        // Set the enemy in motion.
        enemies.type[x+1] = ENEMY_ACIDDROP;

        enemies.extra2[x] = 12; // ACIDBLOB_ANIMATION_FRAME_COUNT

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
    ++enemies.extra2[x];
    enemies.extra2[x] &= 7; // Crop the animation counter to 7. (this will get >> 1'd in draw_sprites)

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
        coordinates = (temp1 >> 4) + (temp2 & 0xf0); 

        // Account for being on the edge of a nametable...
        temp4 = temp2 >> 4;

        // ...by checking. 
        // If temp4 == 0xf, then we were on the bottom of a nametable and should look at the other one.
        if (temp4 == 0xf) {
            temp4 = enemies.nt[x] + 1;
        } else {
            temp4 = enemies.nt[x];
        }

        // Which cmap should I look at?
        temppointer = cmaps[temp4];
        collision = temppointer[coordinates];
        
        if (!METATILE_IS_SOLID(collision)) {
            ENEMY_FLIP_DIRECTION(x);
            temp3 ^= 1;
        }

        // Now, check ahead of us.

        temp2 = enemies.actual_y[x] + 6; // center y
        coordinates = (temp1 >> 4) + (temp2 & 0xf0); 

        // Which cmap should I look at?
        temp4 = enemies.nt[x];
        temppointer = cmaps[temp4];
        collision = temppointer[coordinates];
        
        if (METATILE_IS_SOLID(collision)) {
            ENEMY_FLIP_DIRECTION(x);
            temp3 ^= 1;
        }

        // Move 2 pixels instead of 1.
        temp1 = leftright_movement_moving_lookup_table[temp3];
        temp1 += temp1;

        enemies.x[x] += temp1;

    }

}

void boss_ai(void) {

}

void death_effect_timer_ai(void) {
    // Decrement this thing's timer, then check if it's zero.
    if (--enemies.timer[x] == 0){
        // If it's zero, turn this into a None and clear its flags.
        enemies.type[x] = ENEMY_NONE;
        enemies.flags[x] = 0;
    }
}