// ====
// Castle Escape
//

#include "lib/neslib.h" // Shiru's library. Some functions were modified to reduce stack usage.
#include "lib/nesdoug.h" // dougeff's library. Some functions were modified to reduce stack usage.

#include "mmc1/bank_helpers.h" // MMC1 utility functions.

#include "structs.h" // Some general-purpose structs are defined here (these will probably be moved).
#include "metasprite_lookup_tables.h" // Metasprite data is defined here.
#include "metatiles.h" // Metatile data is defined here.
#include "levels.h" // Level data is defined here (in a bunch of headers).
#include "enemies.h"
#include "zeropage.h" // Zeropage declarations are here. Probably better to just extern what you need than import this everywhere.
#include "constants.h" // Constant #defines are here.

#include "player_macros.h"
#include "enemy_macros.h"

#include "tilemaps/compressed_welcome_screen.h"

#include "asm/score.h" // A sort of stripped-down ultoa().
#include "asm/math.h" // A few supporting functions+declarations related to sine and cosine lookups.
#include "asm/macros.h" // Macros that are actually asm statements.
#include "asm/helper.h" // Misc functions I didn't want to import stdlib for.

#include "lib/lzgmini_6502.h" // My version of liblzg, interfaced to work with cc65.

#include "titlescreen.h" // Title screen and associated data+functions
#include "menu_screens.h" // compressed menu data

#pragma bss-name(push, "BSS")


// Likewise for RODATA.
// Remember that RODATA is defined in the PRG (unswappable) segment.
#pragma rodata-name ("RODATA")

unsigned char palette_bg[]={
    0x0f, 0x00, 0x10, 0x30, // black, gray, lt gray, white
    0x0f, 0x17, 0x27, 0x38, // Animated -- for the conveyor belts.
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

// Extra memory used for bosses or the like.
unsigned char boss_state;
unsigned char boss_memory[8];

#define BOSS_DIALOG_ALREADY_GIVEN 128

// Score as it was when the level started. If you die, your score will be reset to this.
unsigned int previous_score;

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
void draw_boss_fireball(void);

void draw_score(void);
void draw_energy(void);

void draw_screen_sub(void);

void handle_tile_clear_queue(void);

void begin_level(void);
void load_level_new(void);

void calculate_shuffle_array(void);
void load_level_welcome_screen(void);

void clear_screen(void);
void put_str_sub(void);

// Physics/Logic functions.
void movement(void);

void swing_sword(void);

void bg_collision(void); // For the player
void bg_collision_sub(void);
void bg_collision_sub_collision_u(void);

void check_spr_objects(void); // For enemies
void sprite_collisions(void);

void empty_function(void);

void collision_with_killable_slashable(void);
void collision_with_inert_slashable(void);
void collision_with_unkillable_unslashable(void);
void collision_with_splyke(void);
extern void collision_with_boss(void);

void enemy_movement(void);

void korbat_ai(void);
void spikeball_ai(void);
void cannon_ai(void);
void cannonball_ai(void);
void cannonball_ai_sub(void);
void acid_ai(void);
void acid_drop_ai(void);
void splyke_ai(void);
void sun_ai(void);
void boss_ai(void);
void boss_fireball_ai(void);

void death_effect_timer_ai(void);

void fire_at_target(void);

// Menu functions.
void switch_menu();

void menu_level_select(void);
void menu_game_type_select(void);
void menu_about_screen(void);
void menu_game_complete_screen(void);

void load_level_selector(void);
void load_about_screen(void);
void load_game_complete_screen(void);

// Functions in other files.
extern void dialog_box_handler(void);
extern void trigger_dialog_box(DialogBoxData const * dboxdata);

extern void boss_ai_intro(void);
extern void boss_ai_idle(void);
extern void boss_ai_ascending(void);
extern void boss_ai_descending(void);
extern void boss_ai_damaged(void);
extern void boss_ai_dying(void);

extern void draw_boss_flying(void);
extern void draw_boss_idle(void);
extern void draw_boss_dying(void);

extern unsigned char const * titlescreen;

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

// Lookup table so the game knows what to do at a given menu.
const void (* const menu_logic_functions[])(void) = {
    menu_game_type_select,
    menu_level_select,
    menu_about_screen,
    menu_game_complete_screen,
};

// If a menu needs something extra/special to be done before showing it, it'll do so in one of these functions.
const void (* const menu_load_functions[])(void) = {
    empty_function, // If no special action needs to be taken
    load_level_selector,
    load_about_screen,
    load_game_complete_screen,
};

const unsigned char * const menu_compressed_data[] = {
    game_select_screen,
    level_select_screen,
    about_screen,
    game_complete_screen,
};

void main (void) {

    ppu_off(); // screen off
    
    // use the second set of tiles for sprites
    // both bg and sprites are set to 0 by default
    // Set the swappable bank like this:
    //set_prg_bank(0);
    set_chr_bank_0(0);
    //set_chr_bank_1(1);
    bank_spr(1);
    
    set_vram_buffer(); // do at least once, sets a pointer to a buffer
    clear_vram_buffer();

    set_prg_bank(4);
    show_title_screen();
        
    // Set the level index to the first level.
    level_index = 0;

    //menu = MENU_GAME_SELECT; // = 0
    switch_menu();

    // pal_bright set to zero by show_title_screen(). Set it back to 4.
    pal_bright(4);

    // (re)load the palettes
    pal_bg(palette_bg);
    pal_spr(palette_sp);

    // ppu_on_all(); // turn on screen

    //music_play(0);
    //set_music_speed(5);

    while (1){

        while (game_mode == MODE_MENU) { 
            ppu_wait_nmi();
            // set_music_speed, etc

            // Clear the VRAM buffer if it gets used for anything...
            clear_vram_buffer();
            oam_clear(); // ...and the OAM, in case we use a sprite.

            // Call the correct menu function.
            AsmCallFunctionAtPtrOffsetByIndexVar(menu_logic_functions, menu);
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
            conveyor_delta = 0;
            did_headbonk = 0;
            RESET_TOUCHING_YELLOW_DOOR();
            RESET_TOUCHING_SPIKES();
            RESET_SCORE_CHANGED_THIS_FRAME();
            RESET_IS_WALKING();

            // Move the player.
            movement();
            
            // Check to see what's on-screen
            check_spr_objects();

            // Check the status of the sword swing.
            if (!STATUS_DEAD) { swing_sword(); }

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

            // TODO: Make this a flag instead of a separate game mode.
            if (game_mode == MODE_LEVEL_COMPLETE) {
                // Figure out what to do based on the selected game mode.
                if (game_level_advance_behavior == LEVEL_UP_BEHAVIOR_EXIT || level_index == NUMBER_OF_LEVELS - 1) {
                    menu = MENU_COMPLETE_SCREEN;
                    switch_menu();
                } else { // == LEVEL_UP_BEHAVIOR_CONTINUE and there are levels left
                    // Next level.
                    level_index += 1;
                    previous_score = score; // Bank the score
                    begin_level();
                }

            } 
            else if (STATUS_DEAD && high_byte(valrigard.y) == high_byte(old_y)) {
                // Whenever we stop moving down (this should still work if we drop off the bottom of the screen):
                // Frame 1 of this, play whatever death music we end up getting.
                ++player_death_timer;
                // After that music ends after however many frames:
                if (player_death_timer == 120) {
                    score = previous_score; // Revert score to pre-death value
                    begin_level(); // Restart this level.    
                }
            }

            // Debug: clear death status.
            if (pad1 & PAD_DOWN) {
                SET_STATUS_ALIVE();
                player_death_timer = 0;
            }

            // debug:
            // gray_line(); // The further down this renders, the fewer clock cycles were free this frame.

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
#define put_str(adr, str) { \
    address = adr;\
    temppointer = str;\
    put_str_sub();\
}
// Subroutine for put_str. This sort of "macro that calls a function" 
// lets us pretend to use function parameters without actually using the (slow) stack.
void put_str_sub(void) {
    vram_adr(address);
    temp0 = *temppointer;
    while (temp0) {
        vram_put(temp0);
        ++temppointer;
        temp0 = *temppointer;
    }
}

void switch_menu(void) {
    // We always want to do these things before changing a menu:
    ppu_off();
    clear_screen();

    // Devensive programming: clear these buffers.
    clear_vram_buffer();
    oam_clear();

    game_mode = MODE_MENU; // Ensure the correct game mode is active.
    menu_selection = 0; // Just to make sure we don't accidentally point to an invalid menu item somehow.

    // Decode the menu visuals from libLZG'd data in the Menu Data Bank to WRAM.

    set_prg_bank(MENU_DATA_BANK);

    AsmSet2ByteFromPtrAtIndexVar(temppointer, menu_compressed_data, menu);
    LZG_decode(temppointer, cmap);

    // Write the buffer to VRAM.
    vram_write(cmap, (32*30));

    // Do a menu-specific thing.
    AsmCallFunctionAtPtrOffsetByIndexVar(menu_load_functions, menu);

    // Turn the PPU back on.
    ppu_on_all();
}

#define MENU_SELECTOR_SPRITE 0x10

// Menu -- Level Select.

void load_level_selector(void) {
    // Print the currently-selected level's name.
    AsmSet2ByteFromPtrAtIndexVar(temppointer, level_names, level_index);
    put_str(NTADR_A(3, 12), temppointer);
}

void menu_level_select(void) {
    // Listen for desired inputs.
    pad1 = pad_poll(0); // read the first controller
    pad1_new = get_pad_new(0);
            
    // I suppose if we ever want the game to start immediately,
    // We can just set this to if (1) for debugging.
    if (pad1_new & PAD_UP || pad1_new & PAD_A) {
        sfx_play(SFX_MENU_BEEP, 0);
        score = 0; // Reset the score...
        previous_score = 0; // and the previous score.
        begin_level();
        return; // Prevent the execution of any of the following code.
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
        multi_vram_buffer_horz(cmap, 28, NTADR_A(3, 12));
    }
}

// Menu -- Game Type Select.

const unsigned char const game_type_select_menu_links[] = { 
    MENU_GAME_SELECT,
    MENU_LEVEL_SELECT,
    MENU_ABOUT_SCREEN,
};

const unsigned char const game_type_select_menu_selector_x[] = {
    10 * 8 + 4,
    8 * 8 + 4,
    10 * 8 + 4,
};

const unsigned char const game_type_select_menu_selector_y[] = {
    13 * 8 - 1,
    15 * 8 - 1,
    17 * 8 - 1,
};

#define GAME_TYPE_MENU_OPTIONS 3
void menu_game_type_select(void) {
    // Listen for desired inputs.
    pad1 = pad_poll(0); // read the first controller
    pad1_new = get_pad_new(0);

    if (pad1_new & PAD_DOWN) {
        ++menu_selection;
        if (menu_selection == GAME_TYPE_MENU_OPTIONS) {  // Wrap around
            menu_selection = 0;
        }
    }

    if (pad1_new & PAD_UP) {
        if (menu_selection != 0) {
            --menu_selection;
        } else {
            menu_selection = GAME_TYPE_MENU_OPTIONS - 1;
        }
    }

    if (pad1_new & PAD_A | pad1_new & PAD_RIGHT) {
        switch (menu_selection) {
            case 0: // Gauntlet
                // new Gauntlet game at level 1.
                level_index = 0;
                score = 0;
                game_level_advance_behavior = LEVEL_UP_BEHAVIOR_CONTINUE;
                begin_level();
                return;
            default:
                menu = game_type_select_menu_links[menu_selection];
                game_level_advance_behavior = LEVEL_UP_BEHAVIOR_EXIT;
                switch_menu();
                return;
        }
    }

    if (pad1_new & PAD_UP | pad1_new & PAD_DOWN | pad1_new & PAD_A) {
        // Code shared between what we'd do with either button press.
        sfx_play(SFX_MENU_BEEP, 0);
    }

    // Show the cursor.
    oam_spr(game_type_select_menu_selector_x[menu_selection], game_type_select_menu_selector_y[menu_selection], 0x10, 0);

}

// Menu -- About.

void load_about_screen(void) {
    put_str(NTADR_A(6, 6), __DATE__);
    put_str(NTADR_A(18, 6), __TIME__);
}

void menu_about_screen(void) {
    // Listen for desired inputs.
    pad1 = pad_poll(0); // read the first controller
    pad1_new = get_pad_new(0);

    if (pad1_new) {
        // Go back.
        sfx_play(SFX_MENU_BEEP, 0);
        menu = MENU_GAME_SELECT;
        switch_menu();
    }
}

// Menu -- Game Complete.
void load_game_complete_screen(void) {
    // Set the game mode properly.
    game_mode = MODE_MENU;
}

void menu_game_complete_screen(void) {
    pad1 = pad_poll(0);
    pad1_new = get_pad_new(0);

    if (pad1_new) { // TODO: Make this only activate on a B button press?
        menu = 0;
        switch_menu();
        return;
    }

    oam_meta_spr(123, 146, valrigard_idle_right);
}

void load_level_welcome_screen(void) {
    ppu_off();
    clear_screen();

    // Decode the blank welcome screen. Buffer it into the cmap RAM.
    LZG_decode(welcome_screen, cmap);

    // Add the level name...
    // Try centering it: calculate the text's offset from the left side of the screen.
    AsmSet2ByteFromPtrAtIndexVar(temppointer, level_names, level_index);
    temp0 = strlen(temppointer);
    temp1 = 16;
    temp1 -= temp0 >> 1;
    // Figure out where the text should be. Note that each row is 32 tiles, so:
    temp_mutablepointer = (cmap + 256 + 64); // This sets the correct row
    temp_mutablepointer += temp1; // and then this sets the correct distance from the left of the screen

    // Actually copy the text.
    for (temp2 = 0; temp2 < temp0; ++temp2) {
        AsmSet1ByteFromZpPtrAtIndexVar(temp3, temppointer, temp2);
        AsmSet1ByteAtZpPtrWithOffset(temp_mutablepointer, temp2, temp3);
    }

    // Write what we've buffered to cmap into vram. (That WRAM sure is convenient...)
    vram_write(cmap, (32*30));

    ppu_on_all();

    // Draw a small Valrigard inside the box with the level text.
    set_prg_bank(METASPRITE_BANK);
    oam_meta_spr(120, 114, valrigard_idle_left);

    // Not going to set the game mode this time because we'll call ppu_wait_nmi here directly.
    for (temp0 = 0; temp0 < 120; ++temp0) {
        ppu_wait_nmi();

        // Allow someone to escape this screen early by just pressing a button.
        pad1 = pad_poll(0);
        pad1_new = get_pad_new(0);
        if (pad1_new) { break; }
    }
    oam_clear(); // Clear the Valrigard sprite.
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

    // In case we died after the first level of a gauntlet and needed to reset the score, re-decimalize it:
    convert_to_decimal(score);

    // Reset Valrigard's energy.
    energy = MAX_ENERGY;

    // Reset the tile clear queue.
    tile_clear_front = 0;
    tile_clear_back = 0;

    // Reset a few variables related to the player.
    player_flags = 0; 
    scroll_count = 0; 
    player_death_timer = 0;

    // Reset boss flags.
    boss_state = 0;

    // Turn the PPU back on.
    ppu_on_all();
}

void load_level_new(void) {
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
            // extra[x] for this enemy should be a number of frames to wait between acid drops.
            // This varied from drop to drop in the original so I'll make it somewhat random.
            temp2 = rand8() | 0b10000000;
            enemies.extra[x] = temp2;
            // Make room for the next enemy to be an acid drop.
            ++x;
        } else if (temp1 == 8) { // ENEMY_BOSS
            ++x; // += 3; // Make room for magic bolts.
            enemies.type[x] = ENEMY_NONE;
            ++x;
            enemies.type[x] = ENEMY_NONE;
            ++x;
            enemies.type[x] = ENEMY_NONE;
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
const void (* const draw_func_pointers[])(void) = {
    empty_function,   // 0 - ENEMY_NONE;
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
    draw_boss_fireball,  // 11 - ENEMY_BOSS_FIREBALL;
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

    if (STATUS_DEAD) { // Dead
        temp0 = DIRECTION;
        AsmSet2ByteFromPtrAtIndexVar(temppointer, valrigard_dead_sprite_lookup_table, temp0);
    } else if (IS_SWINGING_SWORD) { // Swinging sword
        temp0 = (player_frame_timer & 0b11111110) | DIRECTION;
        AsmSet2ByteFromPtrAtIndexVar(temppointer, valrigard_sword_swing_sprite_lookup_table, temp0);
    } else if (pad1 & PAD_UP && energy > 0) { // Flying
        temp0 = (player_frame_timer & 0b11111110) | DIRECTION;
        AsmSet2ByteFromPtrAtIndexVar(temppointer, valrigard_flying_sprite_lookup_table, temp0);
    } else if (IS_WALKING) { // Walking
        temp0 = ((player_walking_timer & 0b11111100) | (DIRECTION << 1)) >> 1;
        AsmSet2ByteFromPtrAtIndexVar(temppointer, valrigard_walking_sprite_lookup_table, temp0);
    } else { // Idle
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
    if (score == 0){ // Special case. (Haven't thought about optimizing this one away, perhaps I could.)
        oam_spr(232, 20, 0, 3);
        return;
    }

    y = 4; // 5 digits (indices 0...4)
    temp0 = 0; // Found a nonzero digit? We want to skip leading zeroes.
    for (x = 200; x <= 232; x+=8) {
        if (temp0) { // Already found a nonzero digit.
            oam_spr(x, 20, score_string[y], 3);
        } else if (score_string[y]) { // This is the first nonzero digit.
            oam_spr(x, 20, score_string[y], 3);
            temp0 = 1; // Mark as having found a nonzero digit.
        }
        --y; // Next index.
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

const void (* const draw_boss_functions[])(void) = { // defined+implemented in boss_ai.h+.c
    draw_boss_flying,
    draw_boss_idle,
    draw_boss_flying,
    draw_boss_flying,
    draw_boss_flying,
    draw_boss_dying
};

void draw_boss(void) {
    // extra2 is the animation timer for this enemy as timer[] is used for timing and other logic.

    temp3 = enemies.extra2[x]; // Copy that timer here.

   
    if (boss_state == 4 && temp3 & 0b10) { // BOSS_STATE_DAMAGED
        // Blink if damaged.
        return; // without drawing anything.
    }


    temp4 = ENEMY_DIRECTION(x);

    // Draw the head...
    if (boss_state != 5) { // not BOSS_STATE_DYING:
        AsmSet2ByteFromPtrAtIndexVar(temppointer, boss_head_sprite_lookup_table, temp4);
        oam_meta_spr(temp_x, temp_y, temppointer);
    }

    // And then the body.
    temp0 = boss_state;
    AsmCallFunctionAtPtrOffsetByIndexVar(draw_boss_functions, temp0);

    oam_meta_spr(temp_x, temp_y, temppointer);
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

void draw_boss_fireball(void) {
    // This enemy's timer[x] is used to save its angle, so we'll need to figure out another way to animate/flicker this
    temp3 = enemies.timer[x]; 
    temp3 += get_frame_count();
    temp3 &= 1;
    temp3 += BOSS_MAGIC_SPRITE_OFFSET;

    oam_spr(temp_x, temp_y, temp3, 0);
}

// MARK: -- Movement.

void movement(void) {

    // Handle X.
    old_x = valrigard.x;

    if (STATUS_DEAD) {
        // Don't respond to this input if we're dead.
        valrigard.velocity_x = 0;
    }
    // Left
    else if (pad1 & PAD_LEFT) {
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
    if (STATUS_DEAD) { // Fall at maximum speed if we're dead.
        valrigard.velocity_y = MAX_FALL;
    }
    else if (pad1 & PAD_UP && energy > 0) { // If we're holding up on the DPad...
        
        if (collision_D) { // If grounded...
            valrigard.velocity_y = -SPEED; // Top speed!
        } else {
            // Otherwise, movement is more nuanced.
            valrigard.velocity_y -= GRAVITY;
            if (valrigard.velocity_y < -SPEED) { valrigard.velocity_y = -SPEED; }
        }
        
        energy -= 1;

        // Handle flying animation timer stuff
        if (!player_frame_timer && !IS_SWINGING_SWORD) {
            player_frame_timer = 14;
        }

    } else {
        
        valrigard.velocity_y += GRAVITY;
        if (valrigard.velocity_y > MAX_FALL) { valrigard.velocity_y = MAX_FALL; }
        
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
        // Only do this if we're alive:
        if (!STATUS_DEAD) { 
            // Apply the conveyor delta.
            valrigard.x += conveyor_delta;
            // Recharge energy.
            energy += 4;
            if (energy > MAX_ENERGY) { energy = MAX_ENERGY; }
            // Recognize entering a yellow door.
            if (TOUCHING_YELLOW_DOOR && (pad1 & PAD_UP)) {
                game_mode = MODE_LEVEL_COMPLETE;
            }
            // Detect: are we walking?
            if (pad1 & (PAD_LEFT | PAD_RIGHT)) {
                SET_IS_WALKING();
                ++player_walking_timer;
                if (player_walking_timer > 23) {
                    player_walking_timer = 0;
                }
            }
        }
    }

    // Nothing below this matters if we're already dead. Skip it if we are.
    if (STATUS_DEAD) { return; }

    // Check if we touched spikes. 
    // The same apparent hitbox for spikes should be more forgiving than that of the solid blocks so this should do that.
    if (TOUCHING_SPIKES) {
        hitbox.x = high_byte(valrigard.x) + VALRIGARD_SPIKE_HITBOX_WIDTH_OFFSET; 
        hitbox.y = high_byte(valrigard.y) + VALRIGARD_SPIKE_HITBOX_HEIGHT_OFFSET; 
        hitbox.width = VALRIGARD_SPIKE_HITBOX_WIDTH;
        hitbox.height = VALRIGARD_SPIKE_HITBOX_HEIGHT;

        RESET_TOUCHING_SPIKES(); // Clear the spike flag, then try to collide with the background again.
        bg_collision();

        if (TOUCHING_SPIKES) { // If we're still touching spikes even with a smaller hitbox, we die.
            SET_STATUS_DEAD();
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
    // New: temp0 is a parameter -- if temp0 == 0, then an enemy/NPC is using this routine, 
    // so not all logic should be executed.

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
        if (!enemy_is_using_bg_collision) { bg_collision_sub_collision_u(); }
    }

    // Upper right...
    
    temp1 += hitbox.width; // x right
    
    eject_R = (temp1 + 1) & 0x0f;
    
    // temp2,temp3 (high and low byte of 7) are unchanged
    bg_collision_sub();
    
    if(collision){ // find a corner in the collision map
        ++collision_R;
        ++collision_U;
        if (!enemy_is_using_bg_collision) { bg_collision_sub_collision_u(); }
    }
    
    // Now for the bottom.
    add_scroll_y(temp6, hitbox.height, temp6);
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
    if (temp0 == METATILE_NO_EFFECT || temp0 == METATILE_SOLID || enemy_is_using_bg_collision) { return; }

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
        //SET_STATUS_DEAD();
        SET_TOUCHING_SPIKES();
    } else if (temp0 & METATILE_RED_DOOR) {
        game_mode = MODE_LEVEL_COMPLETE;
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

    // (temp1 & 0xf0) == ( (high_byte(valrigard.x) + VALRIGARD_HEIGHT/2 ) & 0xf0))
    advanced_conditional = (high_byte(valrigard.x) + VALRIGARD_HEIGHT/2 ) & 0xf0;

    // Should be fine because whenever we poll the pads we immediately check the values after.
    // This will have to become a hard rule rather than just a personal idiosyncrasy... 

    if (temp4 == QUESTION_BLOCK && (temp3 & 0x0f) == 0x0f 
        && (temp1 & 0xf0) == advanced_conditional ){

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
        // Formerly a (partially) unrolled loop.
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

const void (* const collision_functions[])(void) = {
    empty_function,                   // None
    collision_with_killable_slashable,      // Korbat
    collision_with_killable_slashable,      // Grarrl
    collision_with_splyke,                  // Splyke
    collision_with_inert_slashable,    // Cannon
    empty_function,                   // Acidpool
    collision_with_unkillable_unslashable,  // Spikeball
    collision_with_unkillable_unslashable,  // Sun
    collision_with_boss,                    // Boss
    collision_with_unkillable_unslashable,  // Cannonball
    collision_with_unkillable_unslashable,  // Aciddrop
    collision_with_unkillable_unslashable,  // Magic Bolt
    empty_function,                   // Purple Death Effect
    empty_function,                   // Splyke Death Effect
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
        // Formerly a (Partially) unrolled loop.
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

// collision_with_inert:
void empty_function(void) { }

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

// void collision_with_boss(void) -- moved to boss_ai.c

// A lookup table for enemy AI functions.
const void (* const ai_pointers[])(void) = {
    empty_function,     // 0 - ENEMY_NONE;
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
    boss_fireball_ai,      // 11 - ENEMY_BOSS_FIREBALL;
    death_effect_timer_ai, // 12 - ENEMY_PURPLE_DEATH_EFFECT;
    death_effect_timer_ai, // 13 - ENEMY_SPLYKE_DEATH_EFFECT;
};

// Enemy AI.
void enemy_movement(void) {
    // This one's a bit of an uncharted realm. 
    // I'm thinking we'll want to optimize this one somehow...
    for (x = 0; x < enemies.count; ++x) {
        // Formerly a (Partially) unrolled loop.
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
            
            // target center x and y
            temp0 = high_byte(valrigard.x) + (VALRIGARD_WIDTH/2);
            temp1 = high_byte(valrigard.y) + 4; // Tweaked for maximum accuracy - may need to be tweaked more.

            // source center x and y
            temp2 = enemies.x[x] + 6; // ENEMY_WIDTH/2
            temp3 = enemies.y[x] + 6; // ENEMY_HEIGHT/2

            // values of temp0 through temp3 are pseudo-parameters for this.
            fire_at_target();
            // values of temp0 and temp4 are pseudo-returns for this.
            enemies.extra[x] = temp0;

            // temppointer will be a pointer to the correct sprite lookup table.
            AsmSet2ByteFromPtrAtIndexVar(temppointer, cannon_sprite_quadrant_lookup_table, temp4);

            // Within the sprite lookup table, figure out the sprite.
            if (temp0 > 0x30) { // over 0x30 brads
                temp3 = temppointer[2]; // y-axis aligned
            } else if (temp0 > 0x10) { // over 0x10 brads
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

void fire_at_target(void) {
    // Aim a cannonball or other projectile at Valrigard.

    // Note that his function does have parameters and a return value, but to avoid using the stack, 
    // they're passed in via the temp variables:
    //
    // temp_x: the index of the projectile/thing we're moving in the enemy database.
    // temp0, temp1: the target's center x and y values, respectively (i.e the exact pixel we want to aim enemy #temp_x at)
    // temp2, temp3: the source's center x and y values, respectively

    // This function *does* actually have return values, but to avoid using the stack,
    // they're passed out via the temp variables:
    //
    // temp0: primary return -- the brads_lookup(coordinates) results.
    // temp4: used as an index for cannon_sprite_quadrant_lookup_table 
    // (any other future uses of it will) also make use of this in a similar way)

    temp4 = 0;

    // Let's figure out if our target is to our left or right.
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

    temp0 = brads_lookup(coordinates);
}

void cannonball_ai(void) {
    // Continue moving forward in the direction fired. If I hit a solid metatile, die.

    // There is a second type of cannonball in the original game that is beholden to gravity but also faster.
    // I'm not going to prioritize it...

    // extra[x] should be the sub_x.
    // extra2[x] should be the sub_y.

    // To start, we need to find the correct brads (binary radians) value for this cannonball.
    // brads
    temp_x = x-1;
    temp1 = enemies.extra[temp_x];
    
    // The middle of this routine is shared between multiple enemies that move in diagonal lines.
    cannonball_ai_sub();

    // The end of this routine will determine the behavior of this projectile when it collides with a solid tile.
    // In this case, we just want it to disappear.
    if (METATILE_IS_SOLID(collision)) {
        // Clear the enemy type and flags. (Both must be cleared.)
        enemies.type[x] = ENEMY_NONE;
        enemies.flags[x] = 0;

        // Note: These will turn into Spikeballs (with id 0, because that's the AI+Sprite combo I chose for id 0 at the moment)
        // on impact if the flags byte is not set to zero.
        // This could be used to make a new type of cannon/ball if we're really looking to expand what we're doing.

        // It's possible we want to play a sound effect or animation here.
    }
}

void boss_fireball_ai(void) {
    // Continue moving forward in the direction fired. If I hit a solid metatile, die.

    // extra[x] should be sub_x (as cannonball_ai_sub will expect this)
    // extra2[x] should be sub_y (for the same reasons)

    // timer[x] will store the brads for this projectile.

    // Get the brads
    temp1 = enemies.timer[x];
    // call cannonball_ai_sub as the main logic will be identical
    cannonball_ai_sub();

    // Disappear on contact with a solid tile, or if the boss is dead/dying.
    if (METATILE_IS_SOLID(collision) || boss_state == 5 /*BOSS_STATE_DYING*/) {
        // Clear the enemy type and flags. (Both must be cleared.)
        enemies.type[x] = ENEMY_NONE;
        enemies.flags[x] = 0;
    }
}

void cannonball_ai_sub(void) {
    // Any projectile-like object will have its AI here.

    // temp1 should be the correct brads value.

    // add/sub this to x
    temp2 = cos_lookup(temp1);

    // add/sub this to y
    temp3 = sin_lookup(temp1);

    // Among enemies, only cannonballs will really have sub_x and sub_y.

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

    // (don't change temp1 and temp5 after this in this subroutine, these might be useful assumptions later.)
    coordinates = (temp1 >> 4) + (low_byte(temp5) & 0xf0);

    // Check for a collision in the center.
    
    temp4 = high_byte(temp5);
    //temppointer = cmaps[temp4];
    AsmSet2ByteFromPtrAtIndexVar(temppointer, cmaps, temp4);

    //collision = temppointer[coordinates];
    AsmSet1ByteFromZpPtrAtIndexVar(collision, temppointer, coordinates);

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

const void (* const boss_ai_functions[])(void) = { // defined+implemented in boss_ai.h+.c
    boss_ai_intro,
    boss_ai_idle,
    boss_ai_ascending,
    boss_ai_descending,
    boss_ai_damaged,
    boss_ai_dying
};

void boss_ai(void) {

    // extra[x] will be a logic timer target. 
    // extra2[x] will be an animation timer.
    // timer[x] will be a logic/frame timer.
    // extra memory is available for this enemy at boss_memory.

    // Trigger the dialog box if it hasn't been triggered yet.

    // The Kau Wizard should have multiple (potential) states:
    // BOSS_STATE_DESCENDING -- Flying to the ground (probably after shooting fireballs).
    // BOSS_STATE_IDLE -- Idle and vulnerable -- only vulnerable in this state.
    // BOSS_STATE_ASCENDING -- Flying upwards. Shoots fireballs in this mode.

    // Always do the following:
    if (enemies.x[x] < high_byte(valrigard.x) ) {
        ENEMY_SET_DIRECTION_RIGHT(x);
    } else {
        ENEMY_SET_DIRECTION_LEFT(x);
    }

    // Timers will need to exist for both invincibility frames and animation frames.

    temp0 = enemies.extra2[x];
    ++temp0;
    enemies.extra2[x] = temp0;

    temp0 = enemies.timer[x];
    ++temp0;
    enemies.timer[x] = temp0;

    // Now figure out the correct dispatch:
    AsmCallFunctionAtPtrOffsetByIndexVar(boss_ai_functions, boss_state);
    // temp0 will be pre-initialized with the new value of enemies.timer[x] so let's use that.
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