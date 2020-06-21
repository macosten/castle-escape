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

unsigned int scroll_x;
unsigned int pseudo_scroll_y;
unsigned int scroll_y;
unsigned int min_scroll_y;
unsigned int max_scroll_y;
unsigned int initial_scroll;
unsigned char scroll_count;
unsigned char scroll_count_2;
#define MAX_UP 0x4000
#define MIN_DOWN 0x8000
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

// MARK: Function Prototypes
void draw_sprites(void);
void movement(void);
void load_level(void);
void load_room(void);
void bg_collision(void);
void bg_collision_sub(void);
void draw_screen_U(void);
void draw_screen_D(void);

void draw_screen_edges(void);

void new_cmap(void);
void new_cmap_D(void);

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
        
        set_scroll_x(scroll_x); // Yeah... this game won't need to scroll in the X direction. I'll keep a more advanced
        set_scroll_y(scroll_y);

        if (valrigard.velocity_y < 0) {
            draw_screen_U();
        } else {
            draw_screen_D();
        }
        
        draw_sprites();

    }
    
}

void load_level(void) {
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
    valrigard.x = ((temp4 >> 4) * 16) << 8;
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
    set_data_pointer(level_nametables[nt_current-1]);
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
    memcpy(c_map, level_nametables[nt_current], 240);
    
    max_scroll_y = scroll_y;
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
}

void movement(void) {
    
    // Handle X.
    old_x = valrigard.x;
    
    // Left
    if (pad1 & PAD_LEFT) {
        direction = LEFT;
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
    

    old_y = valrigard.y;
    // Handle Y. We're probably going to eventually assign flying to A and sword-swinging to B, but... one thing at a time.
    if (pad1 & PAD_UP) {
        valrigard.velocity_y = -SPEED;
    }
    else if (pad1 & PAD_DOWN) {
        valrigard.velocity_y = SPEED;
    }
    else { // No direction.
        valrigard.velocity_y = 0;
    }

    valrigard.y += valrigard.velocity_y;
    
    if((valrigard.y < 0x100)||(valrigard.y > 0xf000)) { // make sure not to wrap around to the other side
        valrigard.y = 0x100;
    }
    
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
    
    /*
    if((high_byte(scroll_y) >= 0x80) || (scroll_y <= MIN_SCROLL)) { // 0x80 = negative
        scroll_y = MIN_SCROLL;
        valrigard.y = temp5;
        if (high_byte(valrigard.y) < 2) valrigard.y = 0x0200;
        else if (high_byte(valrigard.y) > 0xf0) valrigard.y = 0x0200; // > 0xf0 wrapped to the bottom.
    }*/
    
    // Do we need a new collision map? (Did we scroll into a new room/nametable?)
    
    // This portion of the code can definitely be optimized into just one function, but I'm tired, so for now:
    if (valrigard.velocity_y <= 0) {
        if ((scroll_y & 0xff) >= 0xec) {
            new_cmap();
        }
    } else {
        if ((scroll_y & 0xff) <= 0x02) {
            new_cmap_D();
        }
    }
    
    /*
    if ((scroll_y & 0xff) >= 0xec) {
        new_cmap();
    } else {
        //pal_col(0, 0x0f);
    }*/
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
    // Also borrowed from nesdoug -- I'm guessing this is a subroutine to save ROM space.

    coordinates = (temp1 >> 4) + (temp3 & 0xf0); // upper left
    
    map = temp2&1;
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
    pseudo_scroll_y = sub_scroll_y(0x20, scroll_y);
    
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

void draw_screen_D(void) {
    pseudo_scroll_y = add_scroll_y(0x20, scroll_y) + 0xef; 
    // This 0xef (239, which is the height of the screen minus one) might possibly want to be either a 0xf0 (240) or a 
    // 0x100 (1 full nametable compensating for the fact that the last 16 values are masked off by add_scroll_y)
    
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
    nt_current = (scroll_y >> 8); //high byte = the index of the nametable we're in?, one to the right
    
    map = nt_current & 1; //even or odd?
    if (!map) {
        memcpy(c_map, level_nametables[nt_current], 240);
    }
    else {
        memcpy(c_map2, level_nametables[nt_current], 240);
    }
    
    // pal_col(0, nt_current);
}

// If we're scrolling down and want to load a nametable that was below us,
// we'll call this modified cmap function instead...
void new_cmap_D(void) {
    // copy a new collision map to one of the 2 c_map arrays
    nt_current = (scroll_y >> 8) + 1; //high byte = the index of the nametable we're in?, one to the right
    
    // pal_col(0, nt_current);
    
    map = nt_current & 1; //even or odd?
    if (!map) {
        memcpy(c_map, level_nametables[nt_current], 240);
    }
    else {
        memcpy(c_map2, level_nametables[nt_current], 240);
    }
}
