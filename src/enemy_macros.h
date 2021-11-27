// These macros here can probably be optimized a little more 

// Set high bit.
#define ACTIVATE_ENEMY(index) { \
    //enemies_flags[index] |= 0b10000000;\
    __asm__("ldy %v", index);\
    __asm__("lda %v,y", enemies_flags);\
    __asm__("ora #$80");\
    __asm__("sta %v,y", enemies_flags);\
}

// Unset high bit.
#define DEACTIVATE_ENEMY(index) { \
    //enemies_flags &= 0b01111111;\
    __asm__("ldy %v", index); \
    __asm__("lda %v,y", enemies_flags); \
    __asm__("and #%b", 0b01111111); \
    __asm__("sta %v,y", enemies_flags); \
}

#define IS_ENEMY_ACTIVE(index) (enemies_flags[index] & 0b10000000) // Test if high bit is set.

#define GET_ENEMY_TYPE(index) (enemies_type[x])

#define ENEMY_SET_DIRECTION_LEFT(index) {\
   // enemies_flags[index] &= 0b11111110;\
    __asm__("ldy %v", index);\
    __asm__("lda %v,y", enemies_flags);\
    __asm__("and #$FE");\
    __asm__("sta %v,y", enemies_flags);\
}

#define ENEMY_SET_DIRECTION_RIGHT(index) {\
    // enemies_flags[index] |= 0b00000001;\
    __asm__("ldy %v", index);\
    __asm__("lda %v,y", enemies_flags);\
    __asm__("ora #$01");\
    __asm__("sta %v,y", enemies_flags);\
}

#define ENEMY_FLIP_DIRECTION(index) {\
    // enemies_flags[index] ^= 0b00000001;\
    __asm__("ldy %v", index);\
    __asm__("lda %v,y", enemies_flags);\
    __asm__("eor #$01");\
    __asm__("sta %v,y", enemies_flags);\
}

#define ENEMY_DIRECTION(index) (enemies_flags[index] &  0b00000001)


// For cannonballs and other 2-axis projectiles.
#define CANNONBALL_SET_NEG_X(index) {\
    //enemies_flags[index] &= 0b10111111;\
    __asm__("ldy %v", index);\
    __asm__("lda %v,y", enemies_flags);\
    __asm__("and #$BF");\
    __asm__("sta %v,y", enemies_flags);\
}

#define CANNONBALL_SET_POS_X(index) {\
    //enemies_flags[index] |= 0b01000000;\
    __asm__("ldy %v", index);\
    __asm__("lda %v,y", enemies_flags);\
    __asm__("ora #$40");\
    __asm__("sta %v,y", enemies_flags);\
}

#define CANNONBALL_SET_NEG_Y(index) {\
    // enemies_flags[index] &= 0b11011111;\
    __asm__("ldy %v", index);\
    __asm__("lda %v,y", enemies_flags);\
    __asm__("and #$DF");\
    __asm__("sta %v,y", enemies_flags);\
}

#define CANNONBALL_SET_POS_Y(index) {\
    // enemies_flags[index] |= 0b00100000;\
    __asm__("ldy %v", index);\
    __asm__("lda %v,y", enemies_flags);\
    __asm__("ora #$20");\
    __asm__("sta %v,y", enemies_flags);\
}

#define CANNONBALL_X_DIRECTION(index) (enemies_flags[index] & 0b01000000)
#define CANNONBALL_Y_DIRECTION(index) (enemies_flags[index] & 0b00100000)

// For Splykes.

#define SPLYKE_SET_STANDING_STILL(index) {\
    // enemies_flags[index] &= 0b11011111;\
    __asm__("ldy %v", index);\
    __asm__("lda %v,y", enemies_flags);\
    __asm__("and #$DF");\
    __asm__("sta %v,y", enemies_flags);\
}

#define SPLYKE_SET_MOVING_AROUND(index) {\
    // enemies_flags[index] |= 0b00100000;\
    __asm__("ldy %v", index);\
    __asm__("lda %v,y", enemies_flags);\
    __asm__("ora #$20");\
    __asm__("sta %v,y", enemies_flags);\
}

#define SPLYKE_IS_MOVING_AROUND(index)   (enemies_flags[index] &  0b00100000)

// For enemies that can be slashed for points, but don't die when slashed.

// Not that this makes sense, but...
#define SLASHABLE_UNKILLABLE_SET_UNSLASHED(index) {\
    // enemies_flags[index] &= 0b11011111;\
    __asm__("ldy %v", index);\
    __asm__("lda %v,y", enemies_flags);\
    __asm__("and #$DF");\
    __asm__("sta %v,y", enemies_flags);\
}

#define SLASHABLE_UNKILLABLE_SET_SLASHED(index) {\
    // enemies_flags[index] |= 0b00100000;\
    __asm__("ldy %v", index);\
    __asm__("lda %v,y", enemies_flags);\
    __asm__("ora #$20");\
    __asm__("sta %v,y", enemies_flags);\
}

#define SLASHABLE_UNKILLABLE_IS_SLASHED(index)      (enemies_flags[index] &  0b00100000)