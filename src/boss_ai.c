#include "boss_ai.h"

#include "constants.h"
#include "structs.h"
#include "enemies.h"
#include "player_macros.h"
#include "enemy_macros.h"
#include "lib/neslib.h"
#include "lib/nesdoug.h"
#include "asm/macros.h"

#include "player_macros.h"
#include "other_macros.h"
#include "metatiles.h"

extern unsigned char TEMP[];
#pragma zpsym("TEMP");

// Required externs.
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
ZEROPAGE_EXTERN(unsigned char, game_mode);
ZEROPAGE_EXTERN(unsigned char, game_level_advance_behavior);

ZEROPAGE_EXTERN(unsigned int, temp5);
ZEROPAGE_EXTERN(unsigned int, temp6);

ZEROPAGE_EXTERN(const unsigned char *, temppointer);
ZEROPAGE_EXTERN(unsigned char *, temp_mutablepointer);

ZEROPAGE_EXTERN(Hitbox, hitbox);
ZEROPAGE_EXTERN(Player, valrigard);

ZEROPAGE_EXTERN(unsigned int, score);
ZEROPAGE_EXTERN(unsigned char, debug_tile_x);
ZEROPAGE_EXTERN(unsigned char, debug_tile_y);

extern const unsigned char * const cmaps[];

extern const unsigned char * const boss_body_sprite_idle_lookup_table[];
extern const unsigned char * const boss_head_sprite_lookup_table[];
extern const unsigned char * const boss_body_sprite_flying_lookup_table[];
extern const unsigned char * const boss_dying_sprite_lookup_table[];

extern const DialogBoxData const boss_dialog;
// extern const DialogBoxData const defeat_dialog;

extern DialogBoxData active_dboxdata;

extern Enemies enemies;

extern unsigned char boss_state;
#define BOSS_STATE_INTRO 0
#define BOSS_STATE_IDLE 1
#define BOSS_STATE_ASCENDING 2
#define BOSS_STATE_DESCENDING 3
#define BOSS_STATE_DAMAGED 4
#define BOSS_STATE_DYING 5

extern unsigned char boss_memory[];

#define BOSS_BRADS_TARGET boss_memory[0]
#define BOSS_FIREBALL_COOLDOWN boss_memory[1]
#define BOSS_HP boss_memory[2]
#define BOSS_IFRAMES 

extern void trigger_dialog_box(void);
extern void fire_at_target(void);
extern void cannonball_ai_sub(void);
extern void bg_collision(void);

// extra[x] will be a logic timer target. 
// extra2[x] will be an animation timer.
// timer[x] will be a logic/frame timer.
// extra memory is available for this enemy at boss_memory.

// Trigger the dialog box if it hasn't been triggered yet.

// The Kau Wizard should have multiple (potential) states:
// BOSS_STATE_DESCENDING -- Flying to the ground (probably after shooting fireballs). this == 1.
// BOSS_STATE_IDLE -- Idle and vulnerable -- only vulnerable in this state. this == 2.
// BOSS_STATE_ASCENDING -- Flying upwards. Shoots fireballs in this mode. this == 3.


void boss_shoot_fireball(void) {
    // Find a free space for a fireball...

    temp1 = x + 4;
    for (temp_x = x+1; temp_x < temp1; ++temp_x) {
        
        if (IS_ENEMY_ACTIVE(temp_x)) { continue; } // ENEMY_NONE

        //temp1 = GET_ENEMY_TYPE(temp_x);

        enemies.type[temp_x] = ENEMY_BOSS_FIREBALL;

        // Move the fireball into place.
        temp0 = enemies.x[x] + 6;
        temp1 = enemies.actual_y[x] + 6;
        temp2 = enemies.nt[x];

        enemies.x[temp_x] = temp0;
        enemies.actual_y[temp_x] = temp1;
        enemies.nt[temp_x] = temp2;


        // target center x and y
        temp0 = high_byte(valrigard.x) + (VALRIGARD_WIDTH/2);
        temp1 = high_byte(valrigard.y) + 4;
            

        // source center x and y
        temp2 = enemies.x[x] + 6; // ENEMY_WIDTH/2
        enemies.x[temp_x] = temp2;

        temp3 = enemies.y[x] + 6; // ENEMY_HEIGHT/2
        enemies.y[temp_x] = temp3;


        // values of temp0 through temp3 are pseudo-parameters for this.
        fire_at_target();
        // values of temp0 and temp4 are pseudo-returns for this.
        enemies.timer[temp_x] = temp0; // the brads value for this fireball.

        BOSS_FIREBALL_COOLDOWN = 32;

        sfx_play(SFX_CANNON_FIRE, 0);

        return;

    }

    
}

void boss_start_flying(void) {
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

void boss_ai_intro(void) {
    SET_DIRECTION_RIGHT();
    active_dboxdata = boss_dialog;

    BOSS_HP = 3;

    trigger_dialog_box();
    music_play(BOSS_SONG);
    boss_state = BOSS_STATE_DESCENDING;
}

void boss_ai_idle(void) {
    // Bug: triggering a second dialog box will have the first value of dbox_current_string set to 0x00/NULL (and thus it won't be correct).
    // Figure out why this is...
    // trigger_dialog_box();

    // Wait some number of frames
    if (temp0 == enemies.extra[x]) {
        boss_start_flying(); // Start flying
    }

}

void boss_ai_ascending(void) {

    temp1 = BOSS_BRADS_TARGET;
    // Move diagonally.

    // Using this subroutine *does* mean the path will be a little strange because we're overloading the functionality of 
    // extra[x] and extra2[x], but I like the effect this has, so I'll keep it this way.
    cannonball_ai_sub();
    // Every other frame, move again.
    if (temp0 & 1) { cannonball_ai_sub(); }
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

    // Consider shooting a fireball at Valrigard.

    if (BOSS_FIREBALL_COOLDOWN) { --BOSS_FIREBALL_COOLDOWN; }
    else {
        if (rand8() > 250) { 
            boss_shoot_fireball(); ;
        }
    }
    

    temp1 = enemies.extra[x]; // This gets modified by cannonball_ai_sub.
    temp0 = enemies.timer[x]; // (Already should be set to this)
    if (temp0 == temp1) { 
        boss_state = BOSS_STATE_DESCENDING;
        enemies.timer[x] = 0;
    }

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
        temp0 = rand8();
        enemies.extra[x] = temp0;
        boss_state = BOSS_STATE_IDLE;
    }

    // Update the actual enemy model itself.
    enemies.nt[x] = high_byte(temp5);
    enemies.actual_y[x] = low_byte(temp5);
}

void boss_ai_damaged(void) {
    // I got slashed and am currently in my invincibility frames.
    if (temp0 == 0) {
        boss_start_flying();
    } else if ((temp0 & 0b11) == 0b11) {
        // Rise up in the air slowly.
        high_byte(temp5) = enemies.nt[x];
        low_byte(temp5) = enemies.actual_y[x];

        temp1 = enemies.x[x] + 8; 
        
        // UP (adding to y)
        temp6 = sub_scroll_y(1, temp5);

        temp2 = low_byte(temp6); // Y of tile of interest
        temp4 = high_byte(temp6); // NT of tile of interest
        
        coordinates = (temp1 >> 4) + (temp2 & 0xf0);

        // Which cmap should I look at?
        AsmSet2ByteFromPtrAtIndexVar(temppointer, cmaps, temp4);
        AsmSet1ByteFromZpPtrAtIndexVar(collision, temppointer, coordinates);

        if (!METATILE_IS_SOLID(collision)) {
            // Update the actual enemy model itself only if we're not colliding.
            enemies.nt[x] = high_byte(temp6);
            enemies.actual_y[x] = low_byte(temp6);
        }
        
    } 

}

void boss_ai_dying(void) {
    // Just do the death animation, and end the game after 2 seconds.
    if (enemies.timer[x] == 0) {
        game_level_advance_behavior = LEVEL_UP_BEHAVIOR_EXIT;
        game_mode = MODE_LEVEL_COMPLETE;
    }
}

const unsigned char const boss_state_deadliness[] = {
    0,
    0,
    1,
    1,
    0,
    0
};

void collision_with_boss(void) {
    if (IS_SWINGING_SWORD && boss_state == BOSS_STATE_IDLE) {
        --BOSS_HP;
        if (BOSS_HP == 0) {
            boss_state = BOSS_STATE_DYING; // Begin to end the level.
            sfx_play(SFX_ENEMY_KILL, 0);
        } else {
            boss_state = BOSS_STATE_DAMAGED;
        }
        enemies.timer[x] = 127; // ~2 seconds of iframes (these will be incremented until it overflows)
    } else if (boss_state_deadliness[boss_state]) {
        SET_STATUS_DEAD();
    }
}

void draw_boss_flying(void) {
    // Flying
    temp3 >>= 2;
    temp3 &= 0b110; // Mask the frame number.
    temp4 |= temp3;// | ENEMY_DIRECTION(x);
    AsmSet2ByteFromPtrAtIndexVar(temppointer, boss_body_sprite_flying_lookup_table, temp4);
}

void draw_boss_idle(void) {
    AsmSet2ByteFromPtrAtIndexVar(temppointer, boss_body_sprite_idle_lookup_table, temp4);
}

void draw_boss_dying(void) {
    temp3 >>= 2;
    temp3 &= 0b11;
    AsmSet2ByteFromPtrAtIndexVar(temppointer, boss_dying_sprite_lookup_table, temp3);
    music_stop();
}

