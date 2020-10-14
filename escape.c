// using a buffer, we have more flexibility, since we can adjust the
// screen position of a PPU write, and can piggy back multiple data
// sets into 1 push, doing more than 1 update per frame, and
// the data sets can have zeroes, since they are not zero terminated

#include "lib/neslib.h"
#include "lib/nesdoug.h"
#include "structs.h"
#include "metasprites.h"
#include "metatiles.h"
#include "levels.h"
#include "enemies.h"
// #include "asm/bitwise.h"
#include "asm/score.h"
#include "asm/math.h"

#define SPEED 0x150

#define ACCEL 0x20
#define GRAVITY 0x30
#define MAX_SPEED 0x150
#define MAX_FALL MAX_SPEED
#define FLY_VEL -0x600

#pragma bss-name(push, "ZEROPAGE")

//MARK: Zero Page Globals
unsigned char pad1;
unsigned char pad1_new;
unsigned char collision;
unsigned char collision_L;
unsigned char collision_R;
unsigned char collision_U;
unsigned char collision_D;
unsigned char coordinates;

// Best to assume that the values of these temps are NOT stored between function calls.
unsigned char temp0;
unsigned char temp1;
unsigned char temp2;
unsigned char temp3;
unsigned char temp4;

unsigned int temp5;
unsigned int temp6;

const unsigned char * temppointer;
unsigned char * temp_mutablepointer;

unsigned char eject_L; // from the left
unsigned char eject_R; // remember these from the collision sub routine
unsigned char eject_D; // from below
unsigned char eject_U; // from up


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
#define DEAD 2

#define IS_SWINGING_SWORD (player_flags & 4)
#define SET_STATUS_NOT_SWINGING_SWORD() (player_flags &= 0b11111011)
#define SET_STATUS_SWINGING_SWORD() (player_flags |= 4)

unsigned char game_mode;
#define MODE_TITLE 0
#define MODE_GAME 1
#define MODE_PAUSE 2
#define MODE_LEVEL_WELCOME_SCREEN 3
#define MODE_GAME_OVER 4 
// (Uh, do we actually want game overs?) 
// Maybe we just nuke your score every N deaths...


int address;

unsigned char x;
unsigned char y;
unsigned char index;

unsigned char nt;
unsigned char map;

unsigned int scroll_x;
unsigned int pseudo_scroll_y;
unsigned int scroll_y;
unsigned int min_scroll_y;
unsigned int max_scroll_y;
unsigned int initial_scroll;
unsigned char scroll_count;
#define MAX_UP 0x4000
#define MIN_DOWN 0x8000
#define MIN_SCROLL 2

unsigned char L_R_switch;
unsigned int old_x;
unsigned int old_y;
// For enemies:
unsigned char temp_x;
unsigned char temp_y;

unsigned char level_index;

unsigned char energy;
#define MAX_ENERGY 0x70 // 144: 9 (number of tiles of flight height with no tapping) * 16(height of [meta]tile in pixels)?
// Or should this be the number of frames which we should be able to fly for?

// At 100, you should get an extra life!
unsigned char stars;

// 255 frames / 60 fps (NTSC) = 4.25 seconds

unsigned char timer;
#define TITLE_SCREEN_LENGTH 120

#define SONGS 0
unsigned char song;
// enum {SONG_NAME1, SONG_NAME2};
// enum {SFX_FLAP, ...};

// Level information.
unsigned char nt_min; // lower bound (included) in the range of nametables we're allowed to scroll in right now.
unsigned char nt_max; // upper bound (not included) in the range of nametables we're allowed to scroll in right now.
unsigned char nt_current; // The nametable Valrigard is currently in. This should help us determine what other nametable to load when scrolling...?

#define VALRIGARD_WIDTH 11
#define VALRIGARD_HEIGHT 13

#define METATILE_IS_SOLID(mtid) (metatile_property_lookup_table[mtid] & METATILE_SOLID)

unsigned int score; // Is the score important enough to place in the zp?

Player valrigard; // A width of 12 makes Valrigard's hitbox a bit more forgiving. It also happens to match up with his nose.
Hitbox hitbox; // Functionally, a parameter for bg_collision (except using the C stack is not preferable to using a global, generally speaking)
// I renamed nesdoug's "Generic" to "Hitbox" to remind me of what purpose it serves.

Hitbox hitbox2; // This hitbox is used for enemies.

// Debug variables - these will be removed in the future.
unsigned char debug_tile_x;
unsigned char debug_tile_y;

// ~101 zp bytes left?

#pragma bss-name(push, "BSS")

// Used for collisions.
// We could save some RAM by coming up with a system
// to convert from the current bitpacked coords to
// one that functions with rows of 12 instead of 16...
// But we don't currently have enough space in RAM to store 
// 6 c_maps of size 180 anyway, so there's not much point.
unsigned char c_map[240];
unsigned char c_map2[240];

// ~347 bytes of BSS ram left?


const unsigned char const palette_bg[]={
    0x0f, 0x00, 0x10, 0x30, // black, gray, lt gray, white
    0x0f, 0x01, 0x11, 0x31, // Blues
    0x0f, 0x06, 0x16, 0x26, // Reds, like the Red Doors
    0x0f, 0x17, 0x27, 0x38, // Yellow(ish)s, like the main level exit doors
};

const unsigned char const palette_sp[]={
0x0f, 0x16, 0x27, 0x37, // Red, Yellow, Light Yellow
0x0f, 0x01, 0x0f, 0x32, // valrigard's palette
0x0f, 0x04, 0x14, 0x24, // Purples.
0x0f, 0x30, 0x16, 0x00, // HUD(?) and Spikeball/Acid
};

// For shuffling 32 enemies...
const unsigned char const shuffle_array[]={
 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15, 
16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,
31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,
15,14,13,12,11,10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0,

 0, 2, 4, 6, 8,10,12,14,16,18,20,22,24,26,28,30,
 1, 3, 5, 7, 9,11,13,15,17,19,21,23,25,27,29,31,
31,29,27,25,23,21,19,17,15,13,11, 9, 7, 5, 3, 1,
30,28,26,24,22,20,18,16,14,12,10, 8, 6, 4, 2, 0,
};

// Enemy memory.
Enemies enemies;

#define ACTIVATE_ENEMY(index) (enemies.flags_type[index] |= 0b10000000) // Set high bit.
#define DEACTIVATE_ENEMY(index) (enemies.flags_type[index] &= 0b01111111) // Unset high bit.
#define IS_ENEMY_ACTIVE(index) (enemies.flags_type[index] & 0b10000000) // Test if high bit is set.
#define GET_ENEMY_TYPE(index) (enemies.flags_type[x] & 0x0f) // Bottom nibble == type

#define ENEMY_SET_DIRECTION_LEFT(index) (enemies.flags_type[index] &= 0b10111111)
#define ENEMY_SET_DIRECTION_RIGHT(index) (enemies.flags_type[index] |= 0b01000000)
#define ENEMY_FLIP_DIRECTION(index) (enemies.flags_type[index] ^= 0b01000000)
#define ENEMY_DIRECTION(index) (enemies.flags_type[index] & 0b01000000)


// For cannonballs and other 2-axis projectiles.
#define CANNONBALL_SET_NEG_X(index) (enemies.flags_type[index] &= 0b10111111)
#define CANNONBALL_SET_POS_X(index) (enemies.flags_type[index] |= 0b01000000)

#define CANNONBALL_SET_NEG_Y(index) (enemies.flags_type[index] &= 0b11011111)
#define CANNONBALL_SET_POS_Y(index) (enemies.flags_type[index] |= 0b00100000)

#define CANNONBALL_X_DIRECTION(index) (enemies.flags_type[index] & 0b01000000)
#define CANNONBALL_Y_DIRECTION(index) (enemies.flags_type[index] & 0b00100000)

// MARK: Function Prototypes
//void set_sprite_zero(void);

void draw_sprites(void);
void movement(void);

void begin_level(void);
void load_level(void);
void load_room(void);

void bg_collision(void); // For the player
void bg_collision_sub(void);
void draw_screen_U(void);
void draw_screen_D(void);
void draw_screen_sub(void);
void new_cmap(void);

void check_spr_objects(void);
void sprite_collisions(void);
void enemy_movement(void);
void enemy_movement_sub(void);

void korbat_ai(void);
void spikeball_ai(void);
void cannon_ai(void);
void cannonball_ai(void);
void acid_ai(void);
void acid_drop_ai(void);
void splyke_ai(void);
void sun_ai(void);

void load_title_screen(void);
void load_game_over_screen(void);

void clear_screen(void);

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

// Lookup tables for enemy sprites (not yet animated).
const unsigned char * const korbat_sprite_lookup_table[] = {korbat_left, korbat_right};
const unsigned char * const grarrl_sprite_lookup_table[] = {grarrl_left, grarrl_right};

// const unsigned char const enemy_contact_behavior_lookup_table[] = {0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1}

// Possibly a silly way to reuse the lost 4 * 15 (or 2 * 35) bytes of RAM in each c_map:
// List offsets from the beginning of the c_maps at (16 * n + 12)
// Then, (c_map + extra_cmap_data_offsets[n]) can be used as 2 bytes of extra RAM.
const unsigned char const extra_cmap_data_offsets[]={ 
    12, 14, 28, 30, 44, 46, 60, 62, 
    76, 78, 92, 94,108,110,124,126,
   140,142,156,158,172,174,188,190,
   204,206,220,222,236,238
};

void main (void) {
        
    ppu_off(); // screen off
    
    // load the palettes
    pal_bg(palette_bg);
    pal_spr(palette_sp);

    // use the second set of tiles for sprites
    // both bg and sprites are set to 0 by default
    bank_spr(1);

    set_vram_buffer(); // do at least once, sets a pointer to a buffer
    clear_vram_buffer();
        
    //Debug: set the first half of the bitfield to FF.
    //for (temp1 = 0; temp1 < 128; ++temp1) { set_object_bit(temp1); }
        
    // Set the level index to the first level.
    level_index = 0;

    load_title_screen();

    temp1 = brads_lookup(0x36);//atan2(0, 120, 0, 120);
    temp2 = sin_lookup(temp1);
    temp3 = cos_lookup(temp1);
    temp4 = abs_subtract(0x20, 0x20);

    ppu_on_all(); // turn on screen

    while (1){

        while (game_mode == MODE_TITLE) {
            ppu_wait_nmi();
            // set_music_speed, etc

            // Just listen for desired inputs.
            pad1 = pad_poll(0); // read the first controller
            pad1_new = get_pad_new(0);
            
            // Clear the VRAM buffer if it gets used for anything...
            // clear_vram_buffer();

            // I suppose if we ever want the game to start immediately,
            // We can just set this to if (1) for debugging.
            if (pad1_new & PAD_UP) {
                level_index = 0;
                begin_level();
            }

        }

        // The game is active and we're drawing frames.
        while (game_mode == MODE_GAME) {
            ppu_wait_nmi(); // wait till beginning of the frame
            // the sprites are pushed from a buffer to the OAM during nmi

            // set_music_speed(???);

            pad1 = pad_poll(0); // read the first controller
            pad1_new = get_pad_new(0);
            
            clear_vram_buffer();
            
            // Move the player.
            movement();

            // Check to see what's on-screen
            check_spr_objects();

            // Check sprite collisions
            sprite_collisions();

            // Move enemies (todo)
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

            // debug:
            gray_line();
            if (pad1 & PAD_DOWN) {
                SET_STATUS_ALIVE();
            }

            if (game_mode == MODE_GAME_OVER) {
                load_game_over_screen();
            }

            // Debug
            debug_tile_x = get_frame_count();

        }

        // For now, "game over" is "you win"
        while (game_mode == MODE_GAME_OVER) {
            ppu_wait_nmi();

            pad1 = pad_poll(0);
            pad1_new = get_pad_new(0);

            if (pad1_new & PAD_DOWN) {
                level_index = 0;
                begin_level();
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

const char title_string[] = "Castle Escape Alpha";
const char author_string[] = "By macosten";
const char instruction_string[] = "Press Up to start";

void load_title_screen(void) {
    // Eventually we'll want to make a nicer title screen and vram_unrle it, but for now, a simple one:
    
    vram_adr(NTADR_A(3, 2));

    x = 0;
    while(title_string[x]) {
        vram_put(title_string[x]);
        ++x;
    }

    vram_adr(NTADR_A(3, 4));

    x = 0;
    while(author_string[x]) {
        vram_put(author_string[x]);
        ++x;
    }

    vram_adr(NTADR_A(3, 6));

    x = 0;
    while(instruction_string[x]) {
        vram_put(instruction_string[x]);
        ++x;
    }

}

const char game_over_string[] = "Demo Over! Down to restart.";
void load_game_over_screen(void) {
    ppu_off();
    clear_screen();
    // Set the game mode properly.
    game_mode = MODE_GAME_OVER;

    // Write the message.
    vram_adr(NTADR_A(3, 6));
    x = 0;
    while (game_over_string[x]) {
        vram_put(game_over_string[x]);
        ++x;
    }

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
    load_level();
    load_room();

    // Reset Valrigard's energy.
    energy = MAX_ENERGY;

    // Turn the PPU back on.
    ppu_on_all();
}

// The following two functions... may possibly get stuck together. We'll see.
void load_level(void) {
    player_flags = 0; // Clear the player flags
    
    nt_max = level_starting_nt[level_index+1];
    nt_current = valrigard_starting_nt[level_index];
    high_byte(scroll_y) = nt_current; // The high byte of scroll_y is the nametable we're currently in (0-255).
    low_byte(scroll_y) = 2;
    
    high_byte(max_scroll_y) = nt_max - 1; // bottom of this level
    low_byte(max_scroll_y) = 0xef;
    
    // nt_min = level_starting_nt[level_index]
    high_byte(min_scroll_y) = level_starting_nt[level_index]; // Min Scroll
    low_byte(min_scroll_y) = 0x02;
    
    scroll_count = 0;
    
    initial_scroll = ((nt_current * 0x100) - 0x11);
    
    // Set inital coordinates
    temp4 = valrigard_inital_coords[level_index];

    // This combination of (uncommented) lines results in the smallest code... bytewise.
    // Still not sure what cc65 makes the fastest.
    //valrigard.x = ((temp4 >> 4) * 16) << 8;
    //valrigard.y = (temp4 & 0x0f) << 12;
    valrigard.x = (temp4 & 0xf0) << 8;
    valrigard.y = ((temp4 & 0x0f) * 16) << 8;

}

void load_room(void) {
    set_data_pointer(level_nametables[nt_current]);
    set_mt_pointer(metatiles);
    temp1 = (initial_scroll >> 8) + 1;
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
    // a little bit in the other room
    set_data_pointer(level_nametables[(char)nt_current-1]); // NOTE: Don't call if nt_current = 0, or who really knows what will happen
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
    
    //copy the room to the collision map
    new_cmap();

    max_scroll_y = scroll_y;

    // Load the enemy data.
    // ... todo.

    // sprite_obj_init / Load Enemies

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

        temp1 = temppointer[y]; // the direction+type byte.
        enemies.flags_type[x] = temp1; 
        // The high nibble will is direction; the low one will be type.

        temp1 = GET_ENEMY_TYPE(x);
        if (temp1 == 4) { // ENEMY_CANNON
            // Load in the next enemy as a cannonball.
            ++x;
            enemies.flags_type[x] = ENEMY_CANNONBALL;
        } else if (temp1 == 5) { // ENEMY_ACIDPOOL
            // Load in the next enemy as an acid drop.
            ++x;
            enemies.flags_type[x] = ENEMY_ACIDDROP;
        }

        ++y; // Next byte.
        
    }
    
    // Set all the other enemies to be NONEs.
    for(++x; x < MAX_ENEMIES; ++x) {
        enemies.flags_type[x] = ENEMY_NONE;
    }
    
}

void draw_sprites(void) {
    // clear all sprites from sprite buffer
    oam_clear();
    //set_sprite_zero(); // Ensure sprite 0 exists

    temp1 = valrigard.x >> 8;
    temp2 = valrigard.y >> 8;
    
    // draw valrigard
    if (DIRECTION == LEFT) {
        oam_meta_spr(temp1, temp2, valrigardIdleLeft);
    } else {
        oam_meta_spr(temp1, temp2, valrigardIdleRight);
    }
    
    // draw enemies.
    temp1 = get_frame_count() & 3;
    temp1 = temp1 << 8; // * 32, since that's the size of our shuffle array.
    for (x = 0; x < MAX_ENEMIES; ++x) {
        y = shuffle_array[temp1];
        ++temp1;
        
        temp2 = enemies.flags_type[y];
        // 0 == ENEMY_NONE (cc65 complained when I used ENEMY_NONE... I wish I knew why...)
        if (temp2 == 0) continue;
        
        if (!IS_ENEMY_ACTIVE(y)) continue;

        temp_x = enemies.x[y];
        
        // Not that we should have enemies ever in these X values, but...
        // (We may be able to optimize this away)
        if (temp_x == 0) ++temp_x; // Basing this off NESDoug's report of problems with temp_x = 0.
        if (temp_x > 0xf0) continue;

        temp_y = enemies.y[y];
        if (temp_y < 0xf0) {
            // These should be changed to part of an enemy_anim array at some point.
            
            // This bit of code will need to be refactored when animations arrive...
            switch (GET_ENEMY_TYPE(y)) {
                case 1: // Korbat
                    temp3 = ENEMY_DIRECTION(y) >> 6;
                    temppointer = korbat_sprite_lookup_table[temp3];
                    oam_meta_spr(temp_x, temp_y, temppointer);
                    break;
                case 2: // Grarrl
                    temp3 = ENEMY_DIRECTION(y) >> 6;
                    temppointer = grarrl_sprite_lookup_table[temp3];
                    oam_meta_spr(temp_x, temp_y, temppointer);
                    break;
                case 4: // Cannon
                    // Figure out direction of cannon - todo
                    oam_meta_spr(temp_x, temp_y, cannon_down_right);
                    break;
                case 6: // Spikeball
                    oam_meta_spr(temp_x, temp_y, spikeball);
                    break;
                case 7: // Sun
                    oam_meta_spr(temp_x, temp_y, sun0);
                    break;
                case 9: // Cannonball
                    oam_spr(temp_x, temp_y, CANNONBALL_SPRITE_OFFSET, 1);
                    break;
                case 10: // Acid Drop
                    oam_spr(temp_x, temp_y, ACIDDROP_SPRITE_OFFSET, 3);
                    break;
                default:
                    oam_meta_spr(temp_x, temp_y, spikeball);
                    break;
            }
        }

    }

    // Draw the energy level as sprites.
    
    temp1 = energy >> 4; 
    oam_spr(200, 28, temp1, 1);
    temp1 = energy & 0x0f;
    oam_spr(208, 28, temp1, 1);
    
    // Draw the score.
    y = 4;
    for (x = 200; x <= 232; x+=8) {
        oam_spr(x, 20, score_string[y], 3);
        --y;
        // if (x == 232) break; //Is this more efficient, or is it more efficient to have the condition in the loop's head?
    }

    // Debug HUD, drawn last because it's the least important.
    oam_spr(232, 42, STATUS_DEAD, 2);
    
    oam_spr(200, 50, debug_tile_x >> 4, 1);
    oam_spr(208, 50, debug_tile_x & 0x0f, 1);
    
    oam_spr(224, 50, debug_tile_y >> 4, 1);
    oam_spr(232, 50, debug_tile_y & 0x0f, 1);

}

// MARK: -- Movement.

void movement(void) {
    
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
        valrigard.x -= (eject_L << 8);
    }
    else if (collision_R) {
        valrigard.x -= (eject_R << 8);
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
        //valrigard.y -= (eject_U << 8);
        high_byte(valrigard.y) -= eject_U;
        // Play head_hit sound
    }
    else if (collision_D) {
        //valrigard.y -= (eject_D << 8);
        high_byte(valrigard.y) -= eject_D;
        // if ... (something was here, but I removed it)
        
        energy += 4;
        if (energy > MAX_ENERGY) energy = MAX_ENERGY;
    }
    
    // MARK: - Deal with scrolling
    
    temp5 = valrigard.y;
    if (valrigard.y < MAX_UP && scroll_y > min_scroll_y) {
        temp1 = (MAX_UP - valrigard.y + 0x80) >> 8; // "the numbers work better with +80 (like 0.5)". I'll take his word for it.
        scroll_y = sub_scroll_y(temp1, scroll_y);
        valrigard.y += (temp1 << 8);
    }
    
    if (valrigard.y > MIN_DOWN && scroll_y < max_scroll_y) {
        temp1 = (MIN_DOWN + valrigard.y + 0x80) >> 8;
        // temp1 = (MIN_DOWN + valrigard.y + 0x80) >> 8;
        scroll_y = add_scroll_y(temp1, scroll_y);
        valrigard.y -= (temp1 << 8);
    }
    
    // Do we need a new collision map? (Did we scroll into a new room/nametable?)
    
    // This portion of the code can definitely be optimized into just one function, but I'm tired, so for now:
    if (valrigard.velocity_y <= 0) {
        if ((scroll_y & 0xff) >= 0xec) {
            nt_current = (scroll_y >> 8);
            new_cmap();
        }
    } else {
        if ((scroll_y & 0xff) <= 0x02) {
            nt_current = (scroll_y >> 8) + 1;
            new_cmap();
        }
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
    if(L_R_switch) temp3 += 2; // fix bug, walking through walls
    
    if(temp3 >= 0xf0) return;
    
    // Upper left... 

    temp5 = add_scroll_y(temp3, scroll_y); // upper left
    temp2 = temp5 >> 8; // high byte y
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
    
    // temp2,temp3 is unchanged
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
    temp2 = temp5 >> 8; // high byte y
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
    
    map = temp2&1;
    
    // Select the correct collision map based on the high byte of y.
    if (!map) {
        temp4 = c_map[coordinates];
        temp_mutablepointer = &(c_map[coordinates]); 
        // temp5 is now a pointer to the tile in memory that we're checking.
    }
    else {
        temp4 = c_map2[coordinates];
        temp_mutablepointer = &(c_map2[coordinates]);
    }
    
    // Fetch all the properties about this tile.
    temp0 = metatile_property_lookup_table[temp4];

    collision = METATILE_IS_SOLID(temp4); // 0x17 is the first non-solid tile, so if the tile is less than that, it's a collision
    // At some point we should figure out what else should be calculated here
    
    // If the collision is with a tile that is solid but has no other properties, return.
    if (temp0 == METATILE_NO_EFFECT || temp0 == METATILE_SOLID) { return; }
    // if (collision <= METATILE_SOLID)

    // No? We must have touched a special tile, then.
    if (temp0 & METATILE_SPIKES) {
        SET_STATUS_DEAD();
    } else if (temp0 & METATILE_POWERUP) {
        // Collect the powerup.
        *temp_mutablepointer = EMPTY_TILE;

        // But what powerup was it?
        if (temp4 == STAR_TILE) { score += 1; }
        else if (temp4 == ENERGY_REFILL_TILE) { energy = MAX_ENERGY; }

        // Shouldn't really need to change the palette, but:
        // address = get_at_addr(nt, (temp1>>4), (temp2 & 0xf0));
        // one_vram_buffer(0, address); Set to the 0th palette
            
        // Bug: stars don't consistently get cleared.
        // They are collected in memory (and thus only collided with once),
        // but the metatiles are not always actually set to EMPTY_TILE.
        address = get_ppu_addr(nt, temp1, temp3 & 0xf0);
        buffer_1_mt(address, EMPTY_TILE);

    } else if (temp0 & METATILE_CONVEYOR_LEFT) {
        // Can confirm that this works, though I need to add
        // checks to ensure the movement only occurs when Valrigard
        // is standing (and not when he's just touching or flying into these tiles).
        valrigard.x -= 0x0080;
    } else if (temp0 & METATILE_CONVEYOR_RIGHT) {
        valrigard.x += 0x0080;
    } else if (temp0 & METATILE_YELLOW_DOOR) {
        // For now, end the game.
        if (pad1 & PAD_UP) {
            game_mode = MODE_GAME_OVER;
        }
    } else if (temp0 & METATILE_RED_DOOR) {
        game_mode = MODE_GAME_OVER;
    }

    // debug_tile_x = temp1 >> 4;
    // debug_tile_y = temp3 >> 4;
}

// MARK: -- Screen Buffering Functions

// At some point, I should probably inline these functions (manually).

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
    temp1 = pseudo_scroll_y >> 8;
    
    set_data_pointer(level_nametables[temp1]);
    nt = (temp1 & 1) << 1; // 0 or 2
    y = pseudo_scroll_y & 0xff;
    
    // Important that the main loop clears the vram_buffer.
    
    //temp1 = draw_screen_sub_lookup_addr_0[scroll_count];
    temp2 = draw_screen_sub_lookup_index_offset_0[scroll_count];
    //temp3 = draw_screen_sub_lookup_addr_1[scroll_count];
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

// (adapted from nesdoug): copy a new collision map to one of the 2 c_map arrays.
// In this version, nt_current should be set before this function is called.
void new_cmap(void) {
    // copy a new collision map to one of the 2 c_map arrays
    map = nt_current & 1; //even or odd?
    if (!map) {
        memcpy(c_map, level_nametables[nt_current], 240);
    }
    else {
        memcpy(c_map2, level_nametables[nt_current], 240);
    }
}

// Check to see what's on-screen.
void check_spr_objects(void) {
    
    // This will be used later. Let's only make it update once.
    // Todo: make it only update once a frame? Save a few clock cycles that way?
    nt_current = high_byte(scroll_y);

    // Check enemies...
    for (x = 0; x < MAX_ENEMIES; ++x) {
        if (GET_ENEMY_TYPE(x) == 0 /*ENEMY_NONE*/) continue; 
        // Check to see where this enemy is supposed to be.
        temp5 = (enemies.nt[x] << 8) + enemies.actual_y[x];

        // temp1 = get_position(); Just going to inline this for now.
        // If we end up having to make stars/energy powerups into sprites,
        // then we can revisit this...

        temp5 -= scroll_y;
        if (high_byte(temp5)) {
            // This enemy isn't on-screen, deactivate it...
            DEACTIVATE_ENEMY(x);
            continue;
        }
        // temp_y = 10;
        ACTIVATE_ENEMY(x); // This enemy is active if it's on-screen.
        enemies.y[x] = temp5 & 0xff;

        // If the topmost nametable currently on-screen (nt_current) is
        // not the enemy's native nametable, it'll be shifted down (positive y) by 16.

        // Let's counteract that...
        if (nt_current != enemies.nt[x]) enemies.y[x] -= 16;

    }
}

// Check for sprite collisions with the player.
void sprite_collisions(void) {
    // hitbox == the player's hitbox.
    hitbox.x = high_byte(valrigard.x);
    hitbox.y = high_byte(valrigard.y);
    hitbox.width = VALRIGARD_WIDTH;
    hitbox.height = VALRIGARD_HEIGHT;

    // hitbox2 == an enemy's hitbox.
    // The width and height of this will actually be different depending on the enemy's type.

    for (x = 0; x < MAX_ENEMIES; ++x) {

        if(IS_ENEMY_ACTIVE(x)) {
            temp1 = GET_ENEMY_TYPE(x);

            // Determine the enemy hitbox size.
            if (temp1 <= 8 /*ENEMY_BOSS*/) { // It's an enemy
                hitbox2.width = ENEMY_WIDTH;
                hitbox2.height = ENEMY_HEIGHT;
            } else { // It's a projectile
                hitbox2.width = PROJECTILE_WIDTH;
                hitbox2.height = PROJECTILE_HEIGHT;
            }

            hitbox2.x = enemies.x[x];
            hitbox2.y = enemies.y[x];

            if (check_collision(&hitbox, &hitbox2)) {
                // Did collide with enemy. For now, just set our status to dead...
                SET_STATUS_DEAD();
            }

        }
    }

}

// Enemy AI.
void enemy_movement(void) {
    // This one's a bit of an uncharted realm. 
    // I'm thinking we'll want to optimize this one somehow...

    temp4 = 0;

    for (x = 0; x < MAX_ENEMIES; ++x) {
        if (IS_ENEMY_ACTIVE(x)) {
            temp1 = GET_ENEMY_TYPE(x);
            
            // Method 1: Big switch block
            switch (temp1) {
                case 1: // ENEMY_KORBAT
                    korbat_ai();
                    break;
                case 2: // ENEMY_GRARRL
                case 6: // ENEMY_SPIKEBALL
                    spikeball_ai();
                    break;
                case 4: // ENEMY_CANNON;
                    cannon_ai();
                    break;
                case 5: // ENEMY_ACIDPOOL;
                    acid_ai();
                    break;
                case 7: // ENEMY_SUN
                    // Technically these guys are called "Flamers"
                    // but I always called them Suns growing up, so they're Suns
                    sun_ai();
                    break;
                // case 8: ENEMY_BOSS
                case 9: // ENEMY_CANNONBALL
                    cannonball_ai();
                    break;
                case 10: // ENEMY_ACIDDROP
                    acid_drop_ai();
                    break;
                default: // Unimplemented
                    break;
            }
            // enemy_movement_sub();
        }
    }

}

void enemy_movement_sub(void) {
    // gray_line() implies that using this as a subroutine 
    // is at least a couple of scanlines slower than the switch statement
    // currently implemented, so I'm not convinced this is the way to go...
    // but I'm still somehow not completely sure.
    /*
    // Is a jump table like this faster?
    static const void * const enemy_ai_jump_table[] = {
        &&no_ai,
        &&korbat_ai,
        &&spikeball_ai,
        &&splyke_ai,
        &&cannon_ai,
        &&acid_ai,
        &&spikeball_ai,
        &&sun_ai,
        &&boss_ai,
        &&cannonball_ai,
        &&acid_drop_ai
    };

    //continue;
    // Method 2: Jump table
    // (enemy_movement_lookup[temp1])();
    goto *enemy_ai_jump_table[temp1];
    // Bug: CC65 breaks when I try using this method.
    // It just doesn't seem to like it... or I'm doing something wrong.
    
    
    no_ai:
        return;
    korbat_ai:
        korbat_ai();
        return;
    spikeball_ai:
        spikeball_ai();
        return;
    splyke_ai:
        splyke_ai();
        return;
    cannon_ai:
        cannon_ai();
        return;
    acid_ai:
        acid_ai();
        return;
    sun_ai:
        sun_ai();
        return;
    boss_ai:
        return;
    cannonball_ai:
        cannonball_ai();
        return;
    acid_drop_ai:
        acid_drop_ai();
        return;
    */
}

// I reordered these to be listed in the order in which they were implemented.

void korbat_ai(void) {
    // Look to see if the metatile ahead of me is solid. If it is, turn around.
    // Then, move forward.

    // Convert the coords of this enemy to metatile coords.

    temp3 = ENEMY_DIRECTION(x) >> 6;

    temp1 = enemies.x[x];

    // If we're going right, we actually want to look to the right of us.
    temp1 += leftright_movement_offset_lookup_table[temp3];

    temp2 = enemies.actual_y[x] + 6; // center y
    coordinates = (temp1 >> 4) + (temp2 & 0xf0); 

    // Which cmap should I look at?
    if (enemies.nt[x] & 1) { // Even or odd?
        collision = c_map2[coordinates];
    } else {
        collision = c_map[coordinates];
    }
    
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
    
    temp3 = ENEMY_DIRECTION(x) >> 6;

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
    if (temp4 & 1) { // Even or odd?
        collision = c_map2[coordinates];
    } else {
        collision = c_map[coordinates];
    }
    
    if (!METATILE_IS_SOLID(collision)) {
        ENEMY_FLIP_DIRECTION(x);
        temp3 ^= 1;
    }

    // Now, check ahead of us.

    temp2 = enemies.actual_y[x] + 6; // center y
    coordinates = (temp1 >> 4) + (temp2 & 0xf0); 

    // Which cmap should I look at?
    if (enemies.nt[x] & 1) { // Even or odd?
        collision = c_map2[coordinates];
    } else {
        collision = c_map[coordinates];
    }
    
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

    temp3 = ENEMY_DIRECTION(x) >> 6;

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
    if (temp4 & 1) { // Even or odd?
        collision = c_map2[coordinates];
    } else {
        collision = c_map[coordinates];
    }

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

    if (high_byte(temp6) & 1) {
        collision = c_map2[coordinates];        
    } else {
        collision = c_map[coordinates];
    }

    if (METATILE_IS_SOLID(collision)) {
        // Clear the lower nibble, then return.
        enemies.flags_type[x] = ENEMY_NONE;
        return;
    }

    enemies.nt[x] = high_byte(temp5);
    enemies.actual_y[x] = low_byte(temp5);

}

void cannon_ai(void) {
    // Wait a while. Turn towards Valrigard. Fire a cannonball in his direction.
    // Todo.

    // High byte of extra should be 

    // The next enemy should be a cannonball. If it's active, don't do anything.
    if (!IS_ENEMY_ACTIVE(x+1)) {
        // Subtract from a (randomly-seeded?) timer of some sort.
        // If that timer == 0, turn to valrigard, then "fire a new cannonball" 
        // (i.e set the cannonball's x to my x, y to my y, etc).
        // Don't forget to tell it what direction it should be going in as well.

    }

}

void cannonball_ai(void) {
    // Continue moving forward in the direction fired. If I hit a solid metatile, die.

    // There is a second type of cannonball in the original game that is beholden to gravity but also faster.
    // I'm not going to prioritize it...

    // Todo.

    // brads
    temp1 = brads_lookup(enemies.extra[x]);
    
    // add/sub this to x
    temp2 = cos_lookup(temp1);

    // add/sub this to y
    temp3 = sin_lookup(temp1);

    // Among enemies, only cannonballs will really have sub_x and sub_y.
    // they'll be held in the spare areas in c_map and/or c_map2...

    // (It's possible I'm prematurely optimizing for RAM space. Not sure.)
    // (If there's enough space, perhaps a sub_x and a sub_y array are appropriate
    // as additional properties of "enemies" - but at the same time, I've only got
    // 2k of RAM in total...)

    // Since there will only ever be a maximum of MAX_ENEMIES/2 cannonballs onscreen
    // at once (as for each cannonball there must also be a cannon), 
    // as long as MAX_ENEMIES <= 60, this solution should be fine:

    temp4 = x >> 1;

    // Deal with the x-direction first.

    high_byte(temp5) = enemies.x[x];
    low_byte(temp5) = c_map2[temp4];

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
    c_map2[temp4] = low_byte(temp5);

    // It's a bit strange because we have a sub_y and a super_y (nt) in this case, 
    // but we'll take care of the sub_y first...
    high_byte(temp6) = enemies.actual_y[x];
    low_byte(temp6) = c_map2[temp4 + 1];

    
    if (CANNONBALL_Y_DIRECTION(x)) {
        // Positive Y - add.
        temp6 += temp3;

        // Return the low byte to the appropriate place in c_map2.
        c_map2[temp4 + 1] = low_byte(temp6);
        
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

        // Return the low byte to the appropriate place in c_map2.
        c_map2[temp4 + 1] = low_byte(temp6);

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
        temp5 = sub_scroll_y(1, temp6);
    }

    // Now that we know where we are, it's time to check to see if this cannonball
    // is colliding with any tiles - if it is, it should disappear.
    
    // The collision nt is in high_byte(temp5).
    // The y value we're looking at is in low_byte(temp5).
    // The x value we're looking at is in temp1.

    coordinates = (temp1 >> 4) + (low_byte(temp5) & 0xf0);

    // Check for a collision in the center.
    if (high_byte(temp5) & 1) {
        collision = c_map2[coordinates];        
    } else {
        collision = c_map[coordinates];
    }

    if (METATILE_IS_SOLID(collision)) {
        // Clear the lower nibble, then return.
        enemies.flags_type[x] = ENEMY_NONE;
        // It's possible we might want to play a sound effect or animation here.
    }

    // Yeah, my ordering here was a little different
    // but this enemy has by far the most complicated AI so far...
    // so I'm going to cut myself a little bit of slack.

}

void acid_ai(void) {
    // Wait a while, then drop an acid drop.
    // Todo.
    // We will probably need to use "x" in this function.
}


void splyke_ai(void) {

}
