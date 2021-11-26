#include "checksum.h"

#include "lib/nesdoug.h"
#include "lib/neslib.h"
#include "asm/macros.h"

#include "structs.h"
#include "other_macros.h"

ZEROPAGE_EXTERN(unsigned char, temp0);
ZEROPAGE_EXTERN(unsigned int, temp5);

extern unsigned int checksum;
extern unsigned int level_high_scores[]; // [256] ints ong
extern unsigned int gauntlet_high_score;

extern unsigned char settings_memory[];

// Our checksum will basically just be the sum of every saved value in memory, disregarding overflow.

// Optimizing these for size over speed since this won't be happening most frames.
void clear_saved_data(void) {
    checksum = 0;
    gauntlet_high_score = 0;
    for (temp0 = 0; ; ++temp0) {
        // Zero out level_high_scores quicker by treating it like 2 char[256] arrays
        ((unsigned char *)level_high_scores)[temp0] = 0;
        ((unsigned char *)level_high_scores + 256)[temp0] = 0;
        if (temp0 == 255) { break; }
    }
    settings_memory[0] = 0;
}

void calculate_checksum(void) {
    temp5 = gauntlet_high_score;
    for (temp0 = 0; ; ++temp0) {
        temp5 += level_high_scores[temp0];
        if (temp0 == 255) { break; }
    }
    temp5 += settings_memory[0];
}

void update_checksum(void) {
    calculate_checksum();
    checksum = temp5;
}

void check_checksum(void) {
    calculate_checksum();
    if (checksum != temp5) {
        // Incorrect checksum, clear the saved data
        clear_saved_data();
    }
}