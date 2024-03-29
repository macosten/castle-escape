#ifndef zeropage_h
#define zeropage_h

#include "structs.h"

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
const unsigned char * temppointer1;
unsigned char * temp_mutablepointer;

unsigned char eject_L; // Used in the collision routine(s).
unsigned char eject_R;
unsigned char eject_D;
unsigned char eject_U; 


unsigned char player_frame_timer;
unsigned char player_sword_timer;
unsigned char player_death_timer;
unsigned char player_walking_timer;

unsigned char player_flags; // All of these flags should be such that the default value for this byte when starting a level is 0
// See macros.h for macros operating on this.
unsigned char player_flags2;

unsigned char game_mode; // A byte that generally tells the game how to behave depending on what's set.
unsigned char game_level_advance_behavior; // A byte that tells the game what to do when a level ends.

int address; // Used with get_ppu_addr and buffer_4_mt because they don't use pointer types for some reason.

unsigned char x; // Used as a loop index.
unsigned char y; // Used as a loop index.
unsigned char index; // Used as an index, for loops and otherwise.

unsigned char nt; // nametable index (though it's only used in 1 place, so...)

// unsigned int scroll_x;
unsigned int pseudo_scroll_y;
unsigned int scroll_y;
unsigned int min_scroll_y;
unsigned int max_scroll_y;
unsigned int initial_scroll;
unsigned char scroll_count;

unsigned char L_R_switch;
unsigned int old_x;
unsigned int old_y;
// For enemies:
unsigned char temp_x;
unsigned char temp_y;

unsigned char level_index;

unsigned char energy;

// Max score of 65535. That feels like it should be enough, right?
unsigned int score;
unsigned char enemy_score;

// 255 frames / 60 fps (NTSC) = 4.25 seconds
// Should we also take PAL machines into account and try to change frame counts in these cases?

unsigned char song;

// Level information.
unsigned char nt_max; // upper bound (not included) in the range of nametables we're allowed to scroll in right now. (The lower bound is always 0)
unsigned char nt_current; // The nametable Valrigard is currently in. This should help us determine what other nametable to load when scrolling...?

Player valrigard; // A width of 12 makes Valrigard's hitbox a bit more forgiving. It also happens to match up with his nose.
Hitbox hitbox; // Functionally, a parameter for bg_collision (except using the C stack is not preferable to using a global, generally speaking)
// I renamed nesdoug's "Generic" to "Hitbox" to remind me of what purpose it serves.

Hitbox hitbox2; // This hitbox is used for enemies.

unsigned char shuffle_offset;
unsigned char shuffle_maximum;

// Debug variables that can be rendered to the screen each frame as a janky printout.
// These will be removed in the future.
unsigned char debug_tile_x;
unsigned char debug_tile_y;

// Added to valrigard.x when standing on a conveyor belt.
signed char conveyor_delta;

unsigned char menu; // Used to keep track of which menu the game is displaying, or was previously displaying.
unsigned char menu_selection; // Used to keep track of what item is selected in a menu.

unsigned int tile_clear_queue[4]; // Each element is one result of get_ppu_addr
unsigned char tile_clear_to_type_queue[4]; // This is the tile ID to replace the cleared tile with.
// unsigned int tile_clear_attr_addr_queue[4]; // More complex math is required to adjust the attribute table mid-game.
unsigned char tile_clear_front;
unsigned char tile_clear_back;

// If nonzero, then valrigard should be jerked down (+Y) a bit this frame.
unsigned char did_headbonk; // possible that this should be a bit inside player_flags.

// If nonzero, then don't perform certain logic in the BG collision subroutines.
unsigned char enemy_is_using_bg_collision;

// Used to save intermediate values of advanced logic/conditionals.
unsigned char advanced_conditional;

// Keeps track of the highest and lowest index this frame with an active enemy.
unsigned char enemy_limit; // equal to the highest index plus one
unsigned char lowest_enemy_index;

unsigned char level_index_backup; // For when we need to remember the previous level due to being in a bonus level

// ~?? zp bytes left? (see ZP_LAST in labels.txt)

#pragma bss-name(pop)

#endif