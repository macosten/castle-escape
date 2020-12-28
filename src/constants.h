#ifndef constants_h
#define constants_h

// From chriscpp's nes-starter-kit -- not sure if I actually want to use these macros yet.
// NOTE: If you call this in a .c file and expose it, be sure to mark it with `ZEROPAGE_EXTERN` there.
#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

// Mark a variable referenced in a header file as being a zeropage symbol.
// Any time you set a variable as a ZEROPAGE_DEF, you will want to also update any header files referencing it
// with this function.
// (Usage; ZEROPAGE_EXTERN(type, variableName); eg ZEROPAGE_EXTERN(int, myInt))
#define ZEROPAGE_EXTERN(defa, defb) extern defa defb; _Pragma("zpsym (\"" STR(defb) "\")")
#define ZEROPAGE_ARRAY_EXTERN(defa, defb, defArr) extern defa defb[defArr]; _Pragma("zpsym (\"" STR(defb) "\")")

// Mark a variable referened in a header file as being SRAM.
// Technically this just creates a regular extern, and you could avoid using this symbol. It is used only for
// consistency with ZEROPAGE variables, to make the source easier to follow.
#define SRAM_EXTERN(defa, defb) extern defa defb;
#define SRAM_ARRAY_EXTERN(defa, defb, defArr) extern defa defb[defArr];


#define SPEED 0x150

#define ACCEL 0x20
#define GRAVITY 0x30
#define MAX_SPEED 0x150
#define MAX_FALL MAX_SPEED
#define FLY_VEL -0x600

// =====
// Constants + Macros for player_flags
#define DIRECTION (player_flags & 1) //facing left or right? (lsb of player_flags)
#define SET_DIRECTION_LEFT() (player_flags &= ~1) // Un-set the lsb
#define SET_DIRECTION_RIGHT() (player_flags |= 1) // Set the lsb
#define LEFT 0
#define RIGHT 1
#define UP 0
#define DOWN 1

#define STATUS_DEAD (player_flags & 2)
#define SET_STATUS_ALIVE() (player_flags &= ~2)
#define SET_STATUS_DEAD() (player_flags |= 2) // set bit 1
#define ALIVE 0
#define DEAD 2 // 0b10

#define IS_SWINGING_SWORD (player_flags & 4)
#define SET_STATUS_NOT_SWINGING_SWORD() (player_flags &= ~4)
#define SET_STATUS_SWINGING_SWORD() (player_flags |= 4)

#define SCORE_CHANGED_THIS_FRAME (player_flags & 8)
#define SET_SCORE_CHANGED_THIS_FRAME() (player_flags |= 8)
#define RESET_SCORE_CHANGED_THIS_FRAME() (player_flags &= ~8)

#define TOUCHING_YELLOW_DOOR (player_flags & 16)
#define SET_TOUCHING_YELLOW_DOOR() (player_flags |= 16)
#define RESET_TOUCHING_YELLOW_DOOR() (player_flags &= ~16)

// =====
// Constants + Macros for game_mode

// Generally preferring defines like this over enums as enums are ints
// under the hood, and ints are 2 bytes (which makes them slower). 
#define MODE_TITLE 0
#define MODE_GAME 1
#define MODE_PAUSE 2
#define MODE_LEVEL_WELCOME_SCREEN 3
#define MODE_GAME_OVER 4 
#define MODE_GAME_SHOWING_TEXT 5
// #define MODE_LEVEL_COMPLETE 6

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

#define METATILE_IS_SOLID(mtid) (metatile_property_lookup_table[mtid] & METATILE_SOLID)

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