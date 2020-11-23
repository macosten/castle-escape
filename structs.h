#ifndef structs_h
#define structs_h
// Struct definitions go here.

typedef struct PlayerSprite {
    unsigned int x; // Low byte == subpixel, high byte == pixel
    unsigned int y;
    signed int velocity_x;
    signed int velocity_y;
} Player;

// I think "Hitbox" is a better name for this than "Generic" because it feels more descriptive
// as to what the actual role of this struct will actually end up being
typedef struct Hitbox_t {
    unsigned char x;
    unsigned char y;
    unsigned char width;
    unsigned char height;
} Hitbox;
// Technically speaking, the same hitbox will be used to check the collisions of multiple things in one frame.


#define MAX_ENEMIES 40
// The Struct of Arrays pattern plays much nicer with cc65 than an array of structs would.
// In other words, this design decision is another concession to the compiler.
typedef struct EnemyDatabase {
    // These might want to be unsigned ints like with PlayerSprite for subpixel accuracy...
    unsigned char x[MAX_ENEMIES]; // The x value of this enemy.
    unsigned char y[MAX_ENEMIES]; // The y value this will be rendered at (after taking scrolling, etc into account)
    unsigned char actual_y[MAX_ENEMIES]; // The "actual" y value of this enemy, in absolute terms.
    unsigned char nt[MAX_ENEMIES]; // The nametable this lives in. Sort of like a high byte of actual_y.
    // Todo: Now that we have lots of extra RAM, should flags_type[] be separated into flags[] and type[]?
    unsigned char flags[MAX_ENEMIES]; // high nibble is flags (including direction and active/inactive), low nibble is type.
    unsigned char type[MAX_ENEMIES];
    unsigned char extra[MAX_ENEMIES]; // The use of this depends on the enemy.
    unsigned char extra2[MAX_ENEMIES]; // The use of this depends on the enemy.
    // Examples of what the extra bytes will contain: animation frame numbers, cached return values, subpixel values...
    unsigned char timer[MAX_ENEMIES]; // A timer value - most likely an animation timer. Probably gets decremented once per frame.
    unsigned char count; // How many enemies are actually loaded into RAM.
} Enemies;

#endif
