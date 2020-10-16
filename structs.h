#ifndef structs_h
#define structs_h
// Struct definitions go here.

typedef struct PlayerSprite {
    unsigned int x; // Low byte == subpixel, high byte == pixel
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

// The Struct of Arrays pattern plays much nicer with cc65 than an array of structs would.
// In other words, this design decision is another concession to the compiler.
typedef struct EnemyDatabase {
    // These might want to be unsigned ints like with PlayerSprite for subpixel accuracy...
    unsigned char x[MAX_ENEMIES]; // The x value of this enemy.
    unsigned char y[MAX_ENEMIES]; // The y value this will be rendered at (after taking scrolling, etc into account)
    unsigned char actual_y[MAX_ENEMIES]; // The "actual" y value of this enemy, in absolute terms.
    unsigned char nt[MAX_ENEMIES]; // The nametable this lives in. Sort of like a high byte of actual_y.
    // Todo: Now that we have lots of extra RAM, should flags_type[] be separated into flags[] and type[]?
    unsigned char flags_type[MAX_ENEMIES]; // high nibble is flags (including direction and active/inactive), low nibble is type.
    unsigned char extra[MAX_ENEMIES]; // The use of this depends on the enemy.
    unsigned char extra2[MAX_ENEMIES]; // The use of this depends on the enemy.
    // Examples of what the extra bytes will contain: timers, animation frame numbers, cached return values...
    unsigned char count; // How many enemies are actually loaded into RAM.
} Enemies;

// I think "Hitbox" is a better name for this than "Generic" because it feels more descriptive
// as to what the actual role of this struct will actually end up being
typedef struct Hitbox_t {
    unsigned char x;
    unsigned char y;
    unsigned char width;
    unsigned char height;
} Hitbox;
// Technically speaking, the same hitbox will be used to check the collisions of multiple things in one frame.

#endif
