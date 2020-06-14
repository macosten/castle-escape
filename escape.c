// using a buffer, we have more flexibility, since we can adjust the
// screen position of a PPU write, and can piggy back multiple data
// sets into 1 push, doing more than 1 update per frame, and
// the data sets can have zeroes, since they are not zero terminated

#include "lib/neslib.h"
#include "lib/nesdoug.h"
#include "colors.h"
#include "structs.h"
#include "metasprites.h"
#include "metatiles.h"
#include "tilemaps/testmap.h"

#pragma bss-name(push, "ZEROPAGE")

//MARK: Zero Page Globals
unsigned char pad1;
unsigned char collision;
unsigned char collision_L;
unsigned char collision_R;
unsigned char collision_U;
unsigned char collision_D;
unsigned char coordinates;
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
unsigned char direction; //facing left or right?
#define LEFT 0
#define RIGHT 1

int address;
unsigned char x; // room loader code
unsigned char y;
unsigned char index;
unsigned char map;
unsigned int scroll_x;
unsigned int scroll_y;
signed int hero_velocity_x; // signed, low byte is sub-pixel
signed int hero_velocity_y;
unsigned int hero_x;
unsigned int hero_y;
unsigned char L_R_switch;
unsigned char old_x;
unsigned char old_y;

#define VALRIGARD_WIDTH 11
#define VALRIGARD_HEIGHT 13

#define SPEED 0x180

#pragma bss-name(push, "BSS")

const unsigned char text[]="E is for Emrakul";

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
0x0f, VALRIGARD_BODY, BLACK, VALRIGARD_WING, // valrigard's palette
0,0,0,0,
0,0,0,0
};

// X, Y, Width, Height
Player valrigard = {20, 40, 15, 15}; // A width of 12 simulates the original game's quirky, slightly-shifted-forward hitbox
Enemy star = {70, 40, 15, 15};
    
// MARK: Function Declarations
void draw_sprites(void);
void movement(void);
void test_collision(void);
void load_room(void);
void bg_collision(void);
void bg_collision_sub(void);

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
    
    // load the text
    // vram_adr(NTADR_A(x,y));
    // vram_adr(NTADR_A(7,14)); // set a start position for the text
    
    // vram_write draws the array to the screen
    // vram_write(text,sizeof(text));

    load_room();
    
    set_scroll_y(0xff); // Shift the background down by 1 pixel to offset the inherent shift in sprites on the NES. (Probably not necessary but we'll see)
    
    ppu_on_all(); // turn on screen
    
    

    while (1){
        // infinite loop
        ppu_wait_nmi(); // wait till beginning of the frame
        // the sprites are pushed from a buffer to the OAM during nmi
        
        pad1 = pad_poll(0); // read the first controller
        // pad2 = pad_poll(1); // read the second controller
        
        movement();
        
        test_collision();
        
        draw_sprites();
    }
    
}

void load_room(void) {
    set_data_pointer(testmap);
    set_mt_pointer(metatiles);
    
    for (y = 0; /*We'll break manually*/; y += 0x20) {
        for (x = 0; ; x += 0x20) {
            clear_vram_buffer(); // do this each frame, and before putting anything in the buffer.
            address = get_ppu_addr(0, x, y);
            index = (y & 0xf0) + (x >> 4);
            buffer_4_mt(address, index); // ppu_address, index to the data
            flush_vram_update_nmi();
            if (x == 0xe0) break;
        }
        if (y == 0xe0) break;
    }
    
    set_vram_update(NULL); // just turn ppu updates OFF for this example
    
    //copy the room to the collision map
    memcpy (c_map, testmap, 240);
    

    hero_y = valrigard.y << 8;
    hero_x = valrigard.x << 8;
    
}

void draw_sprites(void) {
    // clear all sprites from sprite buffer
    oam_clear();
    
    // draw valrigard
    if (direction == LEFT) {
        oam_meta_spr(valrigard.x, valrigard.y, valrigardIdleLeft);
    } else {
        oam_meta_spr(valrigard.x, valrigard.y, valrigardIdleRight);
    }
    
    oam_meta_spr(star.x, star.y, valrigardIdleLeftAlternate);
}
    

void movement(void) {
    
    old_x = valrigard.x;
    
    if(pad1 & PAD_LEFT){
        direction = LEFT;
        if (valrigard.x < 2) {
            hero_velocity_x = 0;
            hero_x = 0x100;
        } else if (valrigard.x < 4) {
            hero_velocity_x = -0x100;
        } else {
            hero_velocity_x = -SPEED;
        }
        
    }
    else if (pad1 & PAD_RIGHT){
        valrigard.x += 1;
    }
    if(pad1 & PAD_UP){
        valrigard.y -= 1;
    }
    else if (pad1 & PAD_DOWN){
        valrigard.y += 1;
    }
    
}



void test_collision(void) {
    collision = check_collision(&valrigard, &star);
        
    // change the BG color, if sprites are touching
    if (collision){
        pal_col(0,0x29);
    }
    else{
        pal_col(0,0x0f);
    }
}
