// These macros here can probably be optimized a little more 

// Set high bit.
#define ACTIVATE_ENEMY(index) { \
    TEMP[0] = enemies.flags[index] | 0b10000000;\
    enemies.flags[index] = TEMP[0];\
}

// Unset high bit.
#define DEACTIVATE_ENEMY(index) { \
    TEMP[0] = enemies.flags[index] & 0b01111111;\
    enemies.flags[index] = TEMP[0];\
}

#define IS_ENEMY_ACTIVE(index) (enemies.flags[index] & 0b10000000) // Test if high bit is set.

#define GET_ENEMY_TYPE(index) (enemies.type[x])

#define ENEMY_SET_DIRECTION_LEFT(index) {\
    TEMP[0] = enemies.flags[index] & 0b11111110;\
    enemies.flags[index] = TEMP[0];\
}

#define ENEMY_SET_DIRECTION_RIGHT(index) {\
    TEMP[0] = enemies.flags[index] | 0b00000001;\
    enemies.flags[index] = TEMP[0];\
}

#define ENEMY_FLIP_DIRECTION(index) {\
    TEMP[0] = enemies.flags[index] ^ 0b00000001;\
    enemies.flags[index] = TEMP[0];\
}

#define ENEMY_DIRECTION(index)              (enemies.flags[index] &  0b00000001)


// For cannonballs and other 2-axis projectiles.
#define CANNONBALL_SET_NEG_X(index) {\
    TEMP[0] = enemies.flags[index] & 0b10111111;\
    enemies.flags[index] = TEMP[0];\
}

#define CANNONBALL_SET_POS_X(index) {\
    TEMP[0] = enemies.flags[index] | 0b01000000;\
    enemies.flags[index] = TEMP[0];\
}

#define CANNONBALL_SET_NEG_Y(index) {\
    TEMP[0] = enemies.flags[index] & 0b11011111;\
    enemies.flags[index] = TEMP[0];\
}

#define CANNONBALL_SET_POS_Y(index) {\
    TEMP[0] = enemies.flags[index] | 0b00100000;\
    enemies.flags[index] = TEMP[0];\
}

#define CANNONBALL_X_DIRECTION(index) (enemies.flags[index] & 0b01000000)
#define CANNONBALL_Y_DIRECTION(index) (enemies.flags[index] & 0b00100000)

// For Splykes.

#define SPLYKE_SET_STANDING_STILL(index) {\
    TEMP[0] = enemies.flags[index] & 0b11011111;\
    enemies.flags[index] = TEMP[0];\
}

#define SPLYKE_SET_MOVING_AROUND(index) {\
    TEMP[0] = enemies.flags[index] | 0b00100000;\
    enemies.flags[index] = TEMP[0];\
}

#define SPLYKE_IS_MOVING_AROUND(index)   (enemies.flags[index] &  0b00100000)

// For enemies that can be slashed for points, but don't die when slashed.

// Not that this makes sense, but...
#define SLASHABLE_UNKILLABLE_SET_UNSLASHED(index) {\
    TEMP[0] = enemies.flags[index] & 0b11011111;\
    enemies.flags[index] = TEMP[0];\
}

// #define SLASHABLE_UNKILLABLE_SET_SLASHED(index)     (enemies.flags[index] |= 0b00100000)
#define SLASHABLE_UNKILLABLE_SET_SLASHED(index) {\
    TEMP[0] = enemies.flags[index] | 0b00100000;\
    enemies.flags[index] = TEMP[0];\
}

#define SLASHABLE_UNKILLABLE_IS_SLASHED(index)      (enemies.flags[index] &  0b00100000)