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

typedef struct EnemySprite {
    unsigned char x;
    unsigned char y;
    unsigned char width;
    unsigned char height;
} Enemy;

// NESDoug named this something I forgot because it wasn't as descriptive (imo) as Hitbox
typedef struct Hitbox_t {
    unsigned char x;
    unsigned char y;
    unsigned char width;
    unsigned char height;
} Hitbox;
// Technically speaking, the same hitbox will be used to check the collisions of multiple things at once.

#endif
