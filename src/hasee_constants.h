#ifndef hasee_constants_h
#define hasee_constants_h

#define HASEE_METASPRITE_BANK 1
#define MAX_TREATS_ONSCREEN 13

#define TREAT_YELLOW            0x00
#define TREAT_BLUE              0x01
#define TREAT_GREEN             0x02
#define TREAT_SILVER            0x03
#define TREAT_GOLDEN            0x04
#define TREAT_CHECKERED         0x05
#define TREAT_SPONGE            0x06
#define TREAT_FIERY             0x07
#define TREAT_ICY               0x08
#define TREAT_RAINBOW           0x09
#define TREAT_FISH              0x0A
#define TREAT_GRUNDOUGHNUTFRUIT 0x0B

#define TREAT_MACCY             0x0C
#define TREAT_GROSS_DUNG        0x0D
#define TREAT_GROSS_SLIME       0x0E

#define TREAT_PURPLE_H          0x0F
#define TREAT_PURPLE_A          0x10
#define TREAT_PURPLE_S          0x11
#define TREAT_PURPLE_E          0x12
#define TREAT_ORANGE_H          0x13
#define TREAT_ORANGE_A          0x14
#define TREAT_ORANGE_S          0x15
#define TREAT_ORANGE_E          0x16

#define HASEE_WIDTH     11
#define HASEE_HEIGHT    13
#define TREAT_WIDTH     16
#define TREAT_HEIGHT    16

#define TREAT_STUNS     0b10000000
#define TREAT_FILLS_H   0b01000000
#define TREAT_FILLS_A   0b00100000
#define TREAT_FILLS_S   0b00010000
#define TREAT_FILLS_E   0b00001000
#define TREAT_ADDS_TIME 0b00000100
#define TREAT_IS_ACTIVE 0b00000010

#define ORANGE_HASEE_ACTIVE         0b00000001 // True if it's Orange (Woogi)'s turn to jump off the branch
#define ACTIVE_PLAYER_BRANCH_STATUS 0b00000010

#define ACTIVE_PLAYER (player_flags & ORANGE_HASEE_ACTIVE)
#define ACTIVE_PLAYER_ON_BRANCH (player_flags & ACTIVE_PLAYER_BRANCH_STATUS)

#define FLIP_ACTIVE_PLAYER() player_flags ^= ORANGE_HASEE_ACTIVE


#define ONSCREEN_TREATS_MAXIMUM 13 // 16 2x2 metasprites, minus the 2 Hasees, minus 2 2x1 dizzy indicators

#endif