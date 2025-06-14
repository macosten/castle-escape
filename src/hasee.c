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

// extern unsigned char eject_R;
// #pragma zpsym("eject_R")
// #define ...

extern unsigned char shuffle_leg_size;

// Importing only the enemies_whatever arrays we need to store Doughnutfruit data...
extern unsigned char enemies_x[MAX_ENEMIES]; // Object X coords
extern unsigned char enemies_y[MAX_ENEMIES]; // Object Y coords
extern unsigned char enemies_extra[MAX_ENEMIES]; // Doughnutfruit subpixel X
extern unsigned char enemies_type[MAX_ENEMIES];
extern unsigned char enemies_extra2[MAX_ENEMIES]; // doughnutfruit speed
extern unsigned char enemies_flags[MAX_ENEMIES]; // doughnutfruit speed
extern unsigned char enemies_count;

extern unsigned char cmap[];

Player *active_player_ptr;

// Since EfMC is in the non-swapping PRG segment, we can access all of its rodata
// Hasee Bounce ROdata bank will be Bank 1

#pragma rodata-name(push, "BANK0")

unsigned char const hasee_palette_sp[] = {
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

Player *player_lut[] = {
    &valrigard,
    &player2
};

extern void clear_screen(void);
extern void put_str_sub(void);
extern void set_prg_bank(unsigned char bank);
extern void calculate_shuffle_array(void);

// Make sure prg bank is 5 before calling:
extern void LZG_decode(const unsigned char *src, unsigned char *dest);

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

    player1.x = 8*8;
    player1.y = 6*8;
    player2.x = 24*8;
    player2.y = 26*8;

    active_player_ptr = &player1;

    player_flags = 0;
    player_flags2 = 0; 
    score = 0;

    set_prg_bank(5);
    temppointer = hasee_game_screen;
    LZG_decode(temppointer, cmap);
    vram_write(cmap, 32*32);

    enemies_count = MAX_TREATS_ONSCREEN;
    calculate_shuffle_array();

    ppu_on_all();
    pal_bright(4);
}

void game_hasee_bounce(void) {
    pad1 = pad_poll(0); // read the first controller
    pad1_new = get_pad_new(0);
    // pad2 = pad_poll(1); // Will only be used in 2 Player games
    // pad2_new = get_pad_new(1); // Or maybe we just call a second function that lets the grounded hasee move in 2p

    ppu_wait_nmi(); // wait till beginning of the frame

    clear_vram_buffer();

    hasee_singleplayer_movement();
    // hasee_check_spr_objects(); // ?

    // draw score

    hasee_draw_sprites();
    // hasee_treat_movement();
    // hasee_draw_sprites();

    if (SCORE_CHANGED_THIS_FRAME) { hasee_update_score(); }
    
    --game_timer;
    if (game_timer == 0) {
        // Trigger stop to game next frame
    }

    gray_line();
}

void hasee_draw_sprites(void) {
    set_prg_bank(HASEE_METASPRITE_BANK);
    oam_clear();

    // Draw hasees
    
    // Add animation switching block here for P1 hasee (see draw_player)
    temppointer = purple_hasee_idle_left;

    oam_meta_spr(high_byte(player1.x), high_byte(player1.y), temppointer);

    // Add animation switching block here for P2 hasee
    temppointer = orange_hasee_idle_right;

    oam_meta_spr(high_byte(player2.x), high_byte(player2.y), temppointer);

    // if (GAME_PAUSED) { oam_meta_spr(108, 116, hasee_paused_text); }

    // Draw goodies
    for (y = 0; y < shuffle_leg_size; ++y) {
        
    }

    set_prg_bank(1);
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
    if (ACTIVE_PLAYER) {
        if (high_byte(player2.x) < 0x20) { // tree trunk on left
            player1.x = 0x2000;
        } else if (high_byte(player2.x) > 0x50) { // tip of tree branch on left
            player1.x = 0x5000;
        }
        old_x = player2.x;
        old_y = player2.y;
    } else {
        if (high_byte(player1.x) < 0xb0) { // tip of branch on left
            player1.x = 0xb000;
        } else if (high_byte(player1.x) > 0xe0) { // tree trunk on right
            player1.x = 0xe000;
        }
        old_x = player1.x;
        old_y = player1.y;
    }

    hitbox.x = high_byte(old_x);
    hitbox.y = high_byte(old_y);
    hitbox.width = HASEE_WIDTH;
    hitbox.height = HASEE_HEIGHT;

    // Eventually we will likely want to emulate the seesaw being diagonal, but for now:
    if (high_byte(old_y) >= 0xd0) { // level of the top of the see-saw nearest the base
        FLIP_ACTIVE_PLAYER();
    }
}

// Calculate the next pickup item. It will be placed into temp4.
void calculate_next_treat(void) {
    temp0 = rand8();
    temp4 = TREAT_YELLOW; // Default option
    if (temp0 > 230) { // ~10% (really a little less) of the time...
        if (rand8() > 252) { // Need to figure out if this is possible with PRNG
            temp4 = TREAT_MACCY; // Meant to be ~1/10000ish chance
        } else {
            // Gross item
            // Slime is the item ID directly after Dung, so:
            temp4 = TREAT_GROSS_DUNG + temp0 & 0b1;
        }
    } else if (temp0 > 192) { // ~15%ish of the time...
        // Letter!
        // We will need to figure out what letters have already been collected.
        // (Todo.)
    } else { // The rest (~75%) of the time...
        // Doughnutfruit! Let's see if we're lucky enough for a special one.
        temp1 = rand8();
        if (temp0 < 20) { // ~1/10th of the time 
            if (temp1 == 1) {
                // Meant to be around a ~1/2500 chance:
                temp4 = TREAT_FISH;
            } else if (temp1 < 5) {
                // ~4 times as common as Fish:
                temp4 = TREAT_RAINBOW;
            } else if (temp1 < 21) {
                // ~4 times as common as Rainbow:
                temp4 = TREAT_ICY;
            } else if (temp1 < 45) {
                // ~6 times as common as Rainbow:
                temp4 = TREAT_FIERY;
            } else if (temp1 < 77) {
                // ~8 times as common as Rainbow:
                temp4 = TREAT_SPONGE;
            } else if (temp1 < 113) {
                // ~9 times as common as Rainbow:
                temp4 = TREAT_CHECKERED;
            } else if (temp1 < 157) {
                // ~11 times as common as Rainbow:
                temp4 = TREAT_GOLDEN;
            } else if (temp1 < 213) {
                // ~14 times as common as Rainbow:
                temp4 = TREAT_SILVER;
            } 
            // Other rare ones could go here in this remaining space of 213-255
        } else {
            if (temp1 < 6) {
                // Meant to be a ~2% chance individually:
                temp4 = TREAT_GREEN;
            } else if (temp1 < 15) {
                // Meant to be a ~3% chance individually (~1.5x as common as green):
                temp4 = TREAT_BLUE;
            }
            // Otherwise, yellow it stays.
        }
    }
}

void hasee_sprite_collisions(void) {
    // hitbox == the jumping player's hitbox.
    hitbox.x = high_byte(player1.x);
    hitbox.y = high_byte(player1.y);

    // Sprite collisions should be more straightforward for this game than EfMC...
    hitbox.width = HASEE_WIDTH;
    hitbox.height = HASEE_HEIGHT;

    // hitbox2 == a treat's hitbox.

    // To save on CPU time, we'll only check half of the collisions on each frame.
    // depending on the parity of get_frame_count(), we'll check only indexes of the same parity for a collision.
    x = get_frame_count() & 1;
    for (x; x < ONSCREEN_TREATS_MAXIMUM; x += 2) { // TODO: See if we can optimize this looping somehow
        temp1 = enemies_flags[x];
        if(temp1 & TREAT_IS_ACTIVE) {
            hitbox2.width = TREAT_WIDTH;
            hitbox2.height = TREAT_HEIGHT;
            hitbox2.x = enemies_x[x];
            hitbox2.y = enemies_y[x];
            check_collision(temp0, hitbox, hitbox2);
            if (temp0) {
                if(previously_collected_treats_this_jump < 7) {
                    ++previously_collected_treats_this_jump;
                }
                temp0 = enemies_type[x];
                if (temp0 >= TREAT_PURPLE_H) {
                    // Letter
                    // Ensure it is the correct color or don't do anything
                    score += hasee_letter_points[previously_collected_treats_this_jump];
                } else if (temp0 <= TREAT_MACCY) {
                    // Regular Point Pickup
                    temp0 <<= 3;
                    temp0 |= previously_collected_treats_this_jump;
                    score += hasee_treat_points[temp0];
                } else {
                    // Gross
                    // set stunned timer
                }
            }
        }
        x += 2;
    }
}

#pragma rodata-name(pop)
