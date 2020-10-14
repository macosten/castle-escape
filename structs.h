//
//  boxes.h
//  Castle Escape
//
//  Created by Zaccari Silverman on 6/11/20.
//

#ifndef structs_h
#define structs_h
// Struct definitions go here.

typedef struct PlayerSprite {
    unsigned int x; // Low byte == subpixel
    unsigned int y;
    signed int velocity_x;
    signed int velocity_y;
} Player;


// Counting possible cannonballs and acid droplets as individual enemies separate from the entities that spawn them,
// If I counted correctly, there should be no more than 32 enemies on any of the original levels.
// I'll set that as the hard cap for now...
#define MAX_ENEMIES 32

// Idea: projectile shooters (Cannons/Acid Blobs) can always have their projectiles located exactly 1 position after them
// in the enemy database. (This is viable because there should only be a maximum of 1 projectile per shooter in the game)

typedef struct EnemyDatabase {
    // These might want to be unsigned ints like with PlayerSprite for subpixel accuracy...
    unsigned char x[MAX_ENEMIES];
    unsigned char y[MAX_ENEMIES];
    unsigned char actual_y[MAX_ENEMIES];
    unsigned char nt[MAX_ENEMIES];
    unsigned char flags_type[MAX_ENEMIES]; 
    // high nibble is flags (including direction and active/inactive), low nibble is type
    unsigned char extra[MAX_ENEMIES];
    unsigned char extra2[MAX_ENEMIES];
    // Can probably be an animation frame counter for most enemies, but for some it'll contain other data.
    unsigned char count; // How many enemies are actually loaded into RAM.
} Enemies;

#define MAX_OBJECTS 64
typedef struct ObjectDatabase {
    unsigned char coordinates[MAX_OBJECTS];
    unsigned char nt[MAX_OBJECTS];
};

// NESDoug named this something I forgot because it wasn't as descriptive (imo) as Hitbox
typedef struct Hitbox_t {
    unsigned char x;
    unsigned char y;
    unsigned char width;
    unsigned char height;
} Hitbox;
// Technically speaking, the same hitbox will be used to check the collisions of multiple things at once.

// That being said, enemies/entities other than the cannonballs only move in one axis (X or Y) at a time;
// Can this fact be used to simplify code in the future?

#endif
