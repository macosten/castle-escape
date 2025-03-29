#include "lib/nesdoug.h"
#include "lib/neslib.h"

#include "asm/macros.h"

#include "hasee_constants.h"
#include "hasee_data.h"

// === Extern'd zero page symbols, defined in zeropage.h.
extern unsigned char temp0;
#pragma zpsym("temp0")

extern unsigned char temp1;
#pragma zpsym("temp1")

extern unsigned char temp2;
#pragma zpsym("temp2")

extern unsigned char temp6;
#pragma zpsym("temp6")

extern unsigned char pad1;
#pragma zpsym("pad1")

extern unsigned char pad1_new;
#pragma zpsym("pad1_new")

extern int address;
#pragma zpsym("address")

extern unsigned char index;
#pragma zpsym("index")

extern const unsigned char * temppointer;
#pragma zpsym("temppointer")

// Since EfMC is in the non-swapping PRG segment, we can access all of its rodata
// Hasee Bounce ROdata bank will be Bank 1

#pragma rodata-name(push, "BANK0")


unsigned char const hasee_palette_sp[] = {
    0x21, 0x0f, 0x5a, 0x29, // sky, black, yellow, green - regular doughnutfruit, stars
    0x21, 0x0f, 0x23, 0x26, // sky, black, purple, orange - Hasees and letters
    0x21, 0x0f, 0x00, 0x00, // Reserved for common special doughnutfruit (blue or green)
    0x21, 0x0f, 0x00, 0x00, // Reserved for rare special doughnutfruit (others)
};

unsigned char const hasee_palette_bg[] = {
    0x21, 0x07, 0x17, 0x27, // Wood + Trees
    0x21, 0x07, 0x19, 0x29, // Grass + Bushes
    0x21, 0x0f, 0x23, 0x26, // sky, black, purple, orange - Hasees and letters,
    0x21, 0x38, 0x00, 0x30, // Misc - Yellow (Sun) + Black + White (text)
};

void calculate_next_treat(void);
// void spawn_next_treat(void);

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