#include "boss_ai.h"

#include "structs.h"
#include "player_macros.h"
#include "enemy_macros.h"
#include "lib/neslib.h"
#include "lib/nesdoug.h"
#include "asm/macros.h"

#include "other_macros.h"
#include "metatiles.h"

extern unsigned char TEMP[];
#pragma zpsym("TEMP");

ZEROPAGE_EXTERN(unsigned char, temp0);
ZEROPAGE_EXTERN(unsigned char, temp1);
ZEROPAGE_EXTERN(unsigned char, temp2);
ZEROPAGE_EXTERN(unsigned char, temp3);
ZEROPAGE_EXTERN(unsigned char, temp4);
ZEROPAGE_EXTERN(unsigned char, x);
ZEROPAGE_EXTERN(unsigned char, temp_x);
ZEROPAGE_EXTERN(unsigned char, coordinates);
ZEROPAGE_EXTERN(unsigned char, collision);
ZEROPAGE_EXTERN(unsigned char, collision_U);
ZEROPAGE_EXTERN(unsigned char, collision_D);
ZEROPAGE_EXTERN(unsigned char, collision_L);
ZEROPAGE_EXTERN(unsigned char, collision_R);
ZEROPAGE_EXTERN(unsigned char, player_flags);
ZEROPAGE_EXTERN(unsigned char, enemy_is_using_bg_collision);

ZEROPAGE_EXTERN(unsigned int, temp5);
ZEROPAGE_EXTERN(unsigned int, temp6);

ZEROPAGE_EXTERN(const unsigned char *, temppointer);
ZEROPAGE_EXTERN(unsigned char *, temp_mutablepointer);

ZEROPAGE_EXTERN(Hitbox, hitbox);

ZEROPAGE_EXTERN(unsigned int, score);

extern const unsigned char * const cmaps[];

extern const DialogBoxData const boss_dialog;
// extern const DialogBoxData const defeat_dialog;

extern DialogBoxData active_dboxdata;

extern Enemies enemies;

extern unsigned char boss_state;
#define BOSS_STATE_IDLE 1
#define BOSS_STATE_ASCENDING 2
#define BOSS_STATE_DESCENDING 3

extern unsigned char boss_memory[];

#define BOSS_BRADS_TARGET boss_memory[0]
#define BOSS_TARGET_X boss_memory[1]
#define BOSS_TARGET_Y boss_memory[2]

extern void trigger_dialog_box(void);
extern void fire_at_target(void);
extern void cannonball_ai_sub(void);
extern void bg_collision(void);

void boss_ai_intro(void) {
    SET_DIRECTION_RIGHT();
    active_dboxdata = boss_dialog;
    trigger_dialog_box();
    boss_state = BOSS_STATE_DESCENDING;
}

void boss_ai_idle(void) {
    // Bug: triggering a second dialog box will have the first value of dbox_current_string set to 0x00/NULL (and thus it won't be correct).
    // Figure out why this is...
    // trigger_dialog_box();

    // Wait some number of frames
    if (temp0 == enemies.extra[x]) {
        // Change state.
        boss_state = BOSS_STATE_ASCENDING;

        // Setup for fire_at_target:

        // Pick some amount of distance to fly.
        temp0 = enemies.x[x];
        temp0 += rand8() & 0b00111111; // x -- MSB is sign
        //temp0 += 1;

        temp1 = enemies.y[x];
        temp1 += rand8() & 0b00111111; // y -- MSB is sign
        //temp1 += 1;


        temp2 = enemies.x[x];
        temp3 = enemies.y[x];

        // We're "firing" ourselves, so:
        temp_x = x;

        fire_at_target();

        BOSS_BRADS_TARGET = temp0;
        // Consider: if this is below a certain value/in a certain range, 
        // modify it somehow to make it impossible for the boss to move only in X

    }

}

void boss_ai_ascending(void) {

    temp1 = BOSS_BRADS_TARGET;
    // Move diagonally.
    cannonball_ai_sub();
    // temppointer is set to the correct cmap for us to check collisions against.

    // Bounce off the wall, if necessary.
    // Do some sort of bg_collision().
    enemy_is_using_bg_collision = 1;

    hitbox.x = enemies.x[x];
    hitbox.y = enemies.y[x];
    hitbox.width = 16;
    hitbox.height = 16;

    bg_collision();

    if (collision_U >= 2 && !CANNONBALL_Y_DIRECTION(x)) { CANNONBALL_SET_POS_Y(x); } 
    else if (collision_D >= 2 && CANNONBALL_Y_DIRECTION(x)) { CANNONBALL_SET_NEG_Y(x); }

    if (collision_L >= 2 && !CANNONBALL_X_DIRECTION(x)) { CANNONBALL_SET_POS_X(x); }
    else if (collision_R >= 2 && CANNONBALL_X_DIRECTION(x)) { CANNONBALL_SET_NEG_X(x); }

    enemy_is_using_bg_collision = 0;

    // TODO -- figure out when to stop or re-aim. Add fireball shooting.

}

void boss_collide_sub(void) {
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
}

void boss_ai_descending(void) {
    // Look for a solid block underneath me.
    // Adapted from sun_ai.
    high_byte(temp5) = enemies.nt[x];
    low_byte(temp5) = enemies.actual_y[x];

    temp1 = enemies.x[x] + 8; 
    
    // DOWN (adding to y)
    add_scroll_y(temp5, 1, temp5);
    add_scroll_y(temp6, 16, temp5); 

    temp2 = low_byte(temp6); // Y of tile of interest
    temp4 = high_byte(temp6); // NT of tile of interest
    
    coordinates = (temp1 >> 4) + (temp2 & 0xf0);

    // Which cmap should I look at?
    AsmSet2ByteFromPtrAtIndexVar(temppointer, cmaps, temp4);
    AsmSet1ByteFromZpPtrAtIndexVar(collision, temppointer, coordinates);

    if (METATILE_IS_SOLID(collision)) {
        // Set initial state for BOSS_STATE_IDLE logic.
        enemies.extra[x] = rand8();
        boss_state = BOSS_STATE_IDLE;
    }

    // Update the actual enemy model itself.
    enemies.nt[x] = high_byte(temp5);
    enemies.actual_y[x] = low_byte(temp5);
}