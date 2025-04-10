#include "lib/nesdoug.h"
#include "lib/neslib.h"

#include "asm/macros.h"
#include "enemy_macros.h"
#include "other_macros.h"
#include "structs.h"

#include "hasee_constants.h"
#include "hasee_data.h"

// === Extern'd zero page symbols, defined in zeropage.h.
ZEROPAGE_EXTERN(unsigned char, temp0);
ZEROPAGE_EXTERN(unsigned char, temp1);
ZEROPAGE_EXTERN(unsigned char, temp2);
ZEROPAGE_EXTERN(unsigned char, temp6);
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

// extern unsigned char eject_R;
// #pragma zpsym("eject_R")
// #define ...

// Importing only the enemies_whatever arrays we need to store Doughnutfruit data...
extern unsigned char enemies_x[MAX_ENEMIES]; // Object X coords
extern unsigned char enemies_y[MAX_ENEMIES]; // Object Y coords
extern unsigned char enemies_extra[MAX_ENEMIES]; // Doughnutfruit subpixel X
extern unsigned char enemies_type[MAX_ENEMIES];
extern unsigned char enemies_extra2[MAX_ENEMIES]; // doughnutfruit speed
extern unsigned char enemies_flags[MAX_ENEMIES]; // doughnutfruit speed


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

void game_hasee_bounce(void);
void calculate_next_treat(void);
// void spawn_next_treat(void);
void hasee_sprite_collisions(void);

void game_hasee_bounce(void) {
    return;
}

// Calculate the next pickup item. It will be placed into temp6.
void calculate_next_treat(void) {
    temp0 = rand8();
    temp6 = TREAT_YELLOW; // Default option
    if (temp0 > 230) { // ~10% (really a little less) of the time...
        if (rand8() > 252) { // Need to figure out if this is possible with PRNG
            temp6 = TREAT_MACCY; // Meant to be ~1/10000ish chance
        } else {
            // Gross item
            // Slime is the item ID directly after Dung, so:
            temp6 = TREAT_GROSS_DUNG + temp0 & 0b1;
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
                temp6 = TREAT_FISH;
            } else if (temp1 < 5) {
                // ~4 times as common as Fish:
                temp6 = TREAT_RAINBOW;
            } else if (temp1 < 21) {
                // ~4 times as common as Rainbow:
                temp6 = TREAT_ICY;
            } else if (temp1 < 45) {
                // ~6 times as common as Rainbow:
                temp6 = TREAT_FIERY;
            } else if (temp1 < 77) {
                // ~8 times as common as Rainbow:
                temp6 = TREAT_SPONGE;
            } else if (temp1 < 113) {
                // ~9 times as common as Rainbow:
                temp6 = TREAT_CHECKERED;
            } else if (temp1 < 157) {
                // ~11 times as common as Rainbow:
                temp6 = TREAT_GOLDEN;
            } else if (temp1 < 213) {
                // ~14 times as common as Rainbow:
                temp6 = TREAT_SILVER;
            } 
            // Other rare ones could go here in this remaining space of 213-255
        } else {
            if (temp1 < 6) {
                // Meant to be a ~2% chance individually:
                temp6 = TREAT_GREEN;
            } else if (temp1 < 15) {
                // Meant to be a ~3% chance individually (~1.5x as common as green):
                temp6 = TREAT_BLUE;
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

