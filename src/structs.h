#ifndef structs_h
#define structs_h
// Struct definitions go here.

typedef struct PlayerSprite_t {
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

typedef struct DialogBoxData_t {
    // On a conceptual level, for each i, portraitSprites[i] and strings[i] are a pair.
    const unsigned char * const * portrait_sprites; // One pointer to a metasprite for each dialog box. This will be shown alongside strings[i].
    const unsigned char * const * strings; // One string for each dialog box.
    const unsigned char count; // The number of boxes full of dialog this data object holds.
    // 
} DialogBoxData;

#define MAX_ENEMIES 64

#endif
