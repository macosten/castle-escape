#ifndef constants_h
#define constants_h

#define SPEED 0x150

#define ACCEL 0x20
#define GRAVITY 0x30
#define MAX_SPEED 0x150
#define MAX_FALL MAX_SPEED
#define FLY_VEL -0x600

// =====
// Constants + Macros for game_mode

// Generally preferring defines like this over enums as enums are ints
// under the hood, and ints are 2 bytes (which makes them slower). 
#define MODE_MENU 0
#define MODE_GAME 1
#define MODE_PAUSE 2
#define MODE_LEVEL_WELCOME_SCREEN 3
#define MODE_LEVEL_COMPLETE 4 
#define MODE_GAME_SHOWING_TEXT 5

// Should the game continue to the next level, or exit the game when we finish this?
#define LEVEL_UP_BEHAVIOR_EXIT 0
#define LEVEL_UP_BEHAVIOR_CONTINUE 1

#define MAX_UP 0x4000 // The lowest Y value the player can have before the screen attempts to scroll up.
#define MIN_DOWN 0x8000 // The highest Y value the player can have before the screen attempts to scroll down.

#define MAX_ENERGY 0x70 // 144: 9 (rough number of tiles of flight height with no tapping) * 16(height of [meta]tile in pixels)?
// Or should this be the number of frames which we should be able to fly for?

#define TITLE_SCREEN_LENGTH 120 // ~2 seconds on NTSC machines.

// ===
// Constants for songs or music.
#define SONGS 0 // No songs yet. I might look into FamiStudio...


#define VALRIGARD_WIDTH 11
#define VALRIGARD_HEIGHT 13

// The swinging hitbox should be expanded slightly.
#define VALRIGARD_SWINGING_WIDTH 15
#define VALRIGARD_SWINGING_HEIGHT 15

// ...but on the other hand, the dimensions for touching spikes should be more forgiving.
#define VALRIGARD_SPIKE_HITBOX_WIDTH 9
#define VALRIGARD_SPIKE_HITBOX_WIDTH_OFFSET ((VALRIGARD_WIDTH - VALRIGARD_SPIKE_HITBOX_WIDTH)/2) // Should just be 1...
#define VALRIGARD_SPIKE_HITBOX_HEIGHT 11
#define VALRIGARD_SPIKE_HITBOX_HEIGHT_OFFSET ((VALRIGARD_HEIGHT - VALRIGARD_SPIKE_HITBOX_HEIGHT)/2)

// ===
// Constants for conveyor_delta. Used when standing on a conveyor belt tile.
#define LEFT_CONVEYOR_DELTA -127
#define RIGHT_CONVEYOR_DELTA 127

#define SFX_SWORD_SLASH 0
#define SFX_STAR_COLLECT 1
#define SFX_ENEMY_KILL 2
#define SFX_CANNON_FIRE 3
#define SFX_MENU_BEEP 4
#define SFX_BUMP 5
#define SFX_ACID_DROP 6


#endif