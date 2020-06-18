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

#pragma bss-name(push, "ZEROPAGE")

//MARK: Zero Page Globals
unsigned char pad1;
unsigned char collision;
unsigned char collision_L;
unsigned char collision_R;
unsigned char collision_U;
unsigned char collision_D;
unsigned char coordinates;

// Best to assume that the values of these temps are NOT stored between function calls.
unsigned char temp1;
unsigned char temp2;
unsigned char temp3;
unsigned char temp4;

unsigned int temp5;
unsigned int temp6;

unsigned char eject_L; // from the left
unsigned char eject_R; // remember these from the collision sub routine
unsigned char eject_D; // from below
unsigned char eject_U; // from up
#define SPEED 0x180

unsigned char direction; //facing left or right? Todo - make this part of Player.
#define LEFT 0
#define RIGHT 1

int address;
const unsigned char * temppointer;

// load_room variables
unsigned char x;
unsigned char y;
unsigned char nt;
unsigned char index;
unsigned char map;

//unsigned int scroll_x;
unsigned int pseudo_scroll_y;
unsigned int scroll_y;
unsigned char scroll_count;
#define MAX_UP 0x4000
#define MIN_SCROLL 2


unsigned char L_R_switch;
unsigned int old_x;
unsigned int old_y;

unsigned char level_index;

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

#pragma bss-name(push, "BSS")

// Used for collisions.
unsigned char c_map[240];
unsigned char c_map2[240]; // not used in this example

const unsigned char palette_bg[]={
    0x0f, 0x00, 0x10, 0x30, // black, gray, lt gray, white
    0x0f, 0x01, 0x11, 0x31, // Blues
    0x0f, 0x06, 0x16, 0x26, // Reds, like the Red Doors
    0x0f, 0x17, 0x27, 0x38, // Yellow(ish)s, like the main level exit doors
};

const unsigned char palette_sp[]={
0x0f, 0x17, 0x27, 0x07,
0x0f, 0x01, 0x0f, 0x32, // valrigard's palette
0,0,0,0,
0,0,0,0
};

// X, Y, Width, Height
Player valrigard = {20, 40}; // A width of 12 makes Valrigard's hitbox a bit more forgiving. It also happens to match up with his nose.
Hitbox hitbox; // Functionally, a parameter for bg_collision (except using the C stack is not preferable to using a global in this use case)
Enemy star = {70, 40, 15, 15};

// MARK: Function Declarations
void draw_sprites(void);
void movement(void);
void load_level(void);
void load_room(void);
void bg_collision(void);
void bg_collision_sub(void);
void draw_screen_U(void);
void new_cmap(void);

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
    
    load_level();
    load_room();

    ppu_on_all(); // turn on screen
    
    level_index = 0;

    while (1){

        // infinite loop
        ppu_wait_nmi(); // wait till beginning of the frame
        // the sprites are pushed from a buffer to the OAM during nmi
        
        pad1 = pad_poll(0); // read the first controller
        // pad2 = pad_poll(1); // read the second controller
        
        clear_vram_buffer();
        
        movement();
        
        // set_scroll_x(scroll_x); Yeah... this game won't need to scroll in the X direction. I'll keep a more advanced 
        set_scroll_y(scroll_y);

        draw_screen_U();

        draw_sprites();

    }
    
}

void load_level(void) {
    nt_max = level_starting_nt[level_index+1];
    nt_current = valrigard_starting_nt[level_index];
    high_byte(scroll_y) = nt_current;
    low_byte(scroll_y) = 0;
    scroll_count = 0;
}

void load_room(void) {
    
    // Set initial nametable
    temp1 = nt_current;
    temppointer = level_nametables[temp1];
    
    // Set inital coordinates
    temp1 = valrigard_inital_coords[level_index];
    valrigard.x = ((temp1 >> 4) * 16) << 8;
    valrigard.y = ((temp1 & 0x0f) * 16) << 8;
    
    set_data_pointer(temppointer);
    set_mt_pointer(metatiles);
    
    temp2 = nt_current - nt_max;
    
    
    // ?? Is this line correct?
    temp1 = (((temp2 * 0x100) - 0x11) >> 8) + 1; // MAX_SCROLL in scroll_up.h, but different since we're keeping track of nt_max.
    
    temp1 = (temp1 & 1) << 1; // Not sure if the temp1 stuff here is necessary.
    
    for (y = 0; /*We'll break manually*/; y += 0x20) {
        for (x = 0; ; x += 0x20) {
            clear_vram_buffer(); // do this each frame, and before putting anything in the buffer.
            address = get_ppu_addr(temp1, x, y);
            index = (y & 0xf0) + (x >> 4);
            buffer_4_mt(address, index); // ppu_address, index to the data
            flush_vram_update_nmi();
            if (x == 0xe0) break;
        }
        if (y == 0xe0) break;
    }
    
    temp1 = temp1 ^ 2;
    
    //copy the room to the collision map
    //memcpy(c_map, temppointer, 240);
    new_cmap();
    
    if (nt_current == nt_max - 1){ // If we're at the top, there's not much of a reason to load what's above us.
        y = 0;
        temp2 = nt_current + 1;
    } else { // Usually we'll be going up, so we'll pre-load part of the nt above us.
        y = 0xe0;
        temp2 = nt_current - 1;
    }
    // temp2 will be the next nt to load.
    // temp3 is the starting y.
        
    /* If we want to load part of the next room (we probably don't... I didn't design the level maps that way.)
    for (x = 0; ; x += 0x20) {
        clear_vram_buffer(); // do this each frame, and before putting anything in the buffer.
        address = get_ppu_addr(temp1, x, y);
        index = (y & 0xf0) + (x >> 4);
        buffer_4_mt(address, index); // ppu_address, index to the data
        flush_vram_update_nmi();
        if (x == 0xe0) break;
    }*/
    
    // If it were any of the first 4 tiles, then we should die, because we're touching spikes -- still need to figure out how best to actually make this reliable...
    
}

void draw_sprites(void) {
    // clear all sprites from sprite buffer
    oam_clear();
    
    temp1 = valrigard.x >> 8;
    temp2 = valrigard.y >> 8;
    
    // draw valrigard
    if (direction == LEFT) {
        oam_meta_spr(temp1, temp2, valrigardIdleLeft);
    } else {
        oam_meta_spr(temp1, temp2, valrigardIdleRight);
    }
    
    oam_meta_spr(star.x, star.y, valrigardIdleLeftAlternate);
}

void movement(void) {
    
    // Handle X.
    old_x = valrigard.x;
    
    // Left
    if (pad1 & PAD_LEFT) {
        direction = LEFT;
        if (valrigard.x <= 2) { // Changed by 1 from nesdoug's example because Valrigard's hitbox is narrower by 1 pixel on both sides
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
        direction = RIGHT;
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
    

    old_y = valrigard.y;
    // Handle Y. We're probably going to eventually assign flying to A and sword-swinging to B, but... one thing at a time.
    if (pad1 & PAD_UP) {
        if (valrigard.y < 0x0200) {
            valrigard.velocity_y = 0;
            valrigard.y = 0x100;
        }
        else if (valrigard.y < 0x0400) { // Stop sprite-wrapping
            valrigard.velocity_y = -0x100;
        }    
        else {
            valrigard.velocity_y = -SPEED;
        }
    }
    else if (pad1 & PAD_DOWN) {
        if (valrigard.y > 0xdf00) {
            valrigard.velocity_y = 0;
            valrigard.y = 0xdf00;
        }
        else if (valrigard.y > 0xdc00) { // If you could be half in the floor and half in the cieling, would you?
            valrigard.velocity_y = 0x100;
        } 
        else {
            valrigard.velocity_y = SPEED;
        }
    }
    else { // No direction.
        valrigard.velocity_y = 0;
    }

    valrigard.y += valrigard.velocity_y;
    
    L_R_switch = 0;
    
    hitbox.x = high_byte(valrigard.x);
    hitbox.y = high_byte(valrigard.y);

    //hitbox.width = VALRIGARD_WIDTH;
    //hitbox.height = VALRIGARD_HEIGHT;
    // Shouldn't need to change the the height and width since those were already set

    bg_collision();

    if (collision_U && collision_D) {
        valrigard.y = old_y;
    }
    else if(collision_U) {
        valrigard.y -= (eject_U << 8);
    }
    else if (collision_D) {
        valrigard.y -= (eject_D << 8);
        // if ... (something was here, but I removed it)
    }
    
    temp5 = valrigard.y;
    if (valrigard.y < MAX_UP) {
        temp1 = (MAX_UP - valrigard.y + 0x80) >> 8; // "the numbers work better with +80 (like 0.5)". I'll take his word for it.
        scroll_y = sub_scroll_y(temp1, scroll_y);
        valrigard.y += (temp1 << 8);
    }
    
    if((high_byte(scroll_y) >= 0x80) || (scroll_y <= MIN_SCROLL)) { // 0x80 = negative
        scroll_y = MIN_SCROLL;
        valrigard.y = temp5;
        if (high_byte(valrigard.y) < 2) valrigard.y = 0x0200;
        else if (high_byte(valrigard.y) > 0xf0) valrigard.y = 0x0200; // > 0xf0 wrapped to the bottom.
    }
    
    // Do we need a new collision map? (Did we scroll into a new room/nametable?)
    if ((scroll_y & 0xff) >= 0xec) {
        new_cmap();
    }
}


void bg_collision(void) {
    // Unlike the original code, !0 is not all we need for a collision
    // Luckily, I put all the solid metatiles at the beginning of the IDs and the non-solid ones at the end.
    // Eventually, I imagine that the powerup-collision code would go here too. Not sure.

    // This was borrowed from nesdoug, who borrowed it from "a multi-screen engine" -- the cycle of borrowing continues.

    collision_L = 0;
    collision_R = 0;
    collision_U = 0;
    collision_D = 0;
    
    if(hitbox.y >= 0xf0) return;
    
    temp6 = temp5 = hitbox.x /*+ scroll_x*/; // upper left (temp6 = save for reuse)
    temp1 = temp5 & 0xff; // low byte x
    temp2 = temp5 >> 8; // high byte x
    
    eject_L = temp1 | 0xf0;
    
    temp3 = hitbox.y; // y top
    
    eject_U = temp3 | 0xf0;
    
    if(L_R_switch) temp3 += 2; // fix bug, walking through walls
    
    // temp2's value matters in bg_collision_sub -- be aware of that.
    bg_collision_sub();
    
    if(collision){ // find a corner in the collision map
        ++collision_L;
        ++collision_U;
    }
    
    // upper right
    temp5 += hitbox.width;
    temp1 = temp5 & 0xff; // low byte x
    temp2 = temp5 >> 8; // high byte x
    
    eject_R = (temp1 + 1) & 0x0f;
    
    // temp3 is unchanged
    bg_collision_sub();
    
    if(collision){ // find a corner in the collision map
        ++collision_R;
        ++collision_U;
    }
    
    // again, lower
    
    // bottom right, x hasn't changed

    temp3 = hitbox.y + hitbox.height; //y bottom
    if(L_R_switch) temp3 -= 2; // fix bug, walking through walls
    eject_D = (temp3 + 1) & 0x0f;
    if(temp3 >= 0xf0) return;
    
    bg_collision_sub();
    
    if(collision){ // find a corner in the collision map
        ++collision_R;
        ++collision_D;
    }
    
    // bottom left
    temp1 = temp6 & 0xff; // low byte x
    temp2 = temp6 >> 8; // high byte x
    
    //temp3, y is unchanged

    bg_collision_sub();
    
    if(collision){ // find a corner in the collision map
        ++collision_L;
        ++collision_D;
    }
}


void bg_collision_sub(void) {
    // Also borrowed from nesdoug -- I'm guessing this is a subroutine to save ROM space.

    coordinates = (temp1 >> 4) + (temp3 & 0xf0); // upper left
    
    map = temp2&1; // high byte
    if (!map) {
        collision = (c_map[coordinates] < 0x17 && c_map[coordinates] > 0x03); // 0x17 is the first non-solid tile, so if the tile is less than that, it's a collision
        //spikeDeath = (c_map[coordinates] < 0x04);
    }
    else {
        collision = (c_map2[coordinates] < 0x17 && c_map[coordinates] > 0x03);
        //spikeDeath = (c_map2[coordinates] < 0x04);
    }

}

/*
 NESDoug function that I'm betting I can make use of by modifying it
 void break_wall(void){
     temp1 = BoxGuy1.x + 0x16;
     temp2 = BoxGuy1.y + 5;
     coordinates = (temp1>>4) + (temp2 & 0xf0);
     if(c_map[coordinates] == 1){ // if brick
        c_map[coordinates] = 0; // can walk through it
        address = get_ppu_addr(0, temp1, temp2);
        buffer_1_mt(address, 0); // put metatile #0 here = blank grass
 
        address = get_at_addr(0, (temp1>>4), (temp2 & 0xf0)); // Not sure if the stuff I did to this line is correct
        one_vram_buffer(0x00, address); // Set to the 0th palette
     }
 }
 */

void draw_screen_U(void){
    pseudo_scroll_y = sub_scroll_y(0x20,scroll_y);
    
    temp1 = pseudo_scroll_y >> 8;
    
    set_data_pointer(level_nametables[temp1]);
    nt = (temp1 & 1) << 1; // 0 or 2
    y = pseudo_scroll_y & 0xff;
    
    // important that the main loop clears the vram_buffer
    
    switch(scroll_count){
        case 0:
            address = get_ppu_addr(nt, 0, y);
            index = (y & 0xf0) + 0;
            buffer_4_mt(address, index); // ppu_address, index to the data
            
            address = get_ppu_addr(nt, 0x20, y);
            index = (y & 0xf0) + 2;
            buffer_4_mt(address, index); // ppu_address, index to the data
            break;
            
        case 1:
            address = get_ppu_addr(nt, 0x40, y);
            index = (y & 0xf0) + 4;
            buffer_4_mt(address, index); // ppu_address, index to the data
            
            address = get_ppu_addr(nt, 0x60, y);
            index = (y & 0xf0) + 6;
            buffer_4_mt(address, index); // ppu_address, index to the data
            break;
            
        case 2:
            address = get_ppu_addr(nt, 0x80, y);
            index = (y & 0xf0) + 8;
            buffer_4_mt(address, index); // ppu_address, index to the data
            
            address = get_ppu_addr(nt, 0xa0, y);
            index = (y & 0xf0) + 10;
            buffer_4_mt(address, index); // ppu_address, index to the data
            break;
            
        default:
            address = get_ppu_addr(nt, 0xc0, y);
            index = (y & 0xf0) + 12;
            buffer_4_mt(address, index); // ppu_address, index to the data
            
            address = get_ppu_addr(nt, 0xe0, y);
            index = (y & 0xf0) + 14;
            buffer_4_mt(address, index); // ppu_address, index to the data
    }

    
    
    ++scroll_count;
    scroll_count &= 3; //mask off top bits, keep it 0-3
}

// (mostly nesdoug): copy a new collision map to one of the 2 c_map arrays
void new_cmap(void) {
    // copy a new collision map to one of the 2 c_map arrays
    nt_current = high_byte(scroll_y); //high byte = the index of the nametable we're in?, one to the right
    
    map = nt_current & 1; //even or odd?
    if (!map) {
        memcpy (c_map, level_nametables[nt_current], 240);
    }
    else {
        memcpy (c_map2, level_nametables[nt_current], 240);
    }
}
