//
//  structs.h
//  Castle Escape
//
//  Created by Zaccari Silverman on 6/11/20.
//

#ifndef structs_h
#define structs_h


#endif /* structs_h */

// Struct definitions go here.

typedef struct PlayerSprite {
    unsigned char x;
    unsigned char y;
} Player;

typedef struct EnemySprite {
    unsigned char x;
    unsigned char y;
    unsigned char width;
    unsigned char height;
} Enemy;

typedef struct Hitbox_t {
    unsigned char x;
    unsigned char y;
    unsigned char width;
    unsigned char height;
} Hitbox;
