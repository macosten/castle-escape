#ifndef hasee_constants_h
#define hasee_constants_h

#define HASEE_METASPRITE_BANK 1
#define HASEE_MOVEMENT_CODE_BANK 1
#define MAX_TREATS_ONSCREEN 12

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

#define ORANGE_FACING_RIGHT_MASK            0b00000001
#define PURPLE_FACING_RIGHT_MASK            0b00000010
#define ORANGE_HASEE_ACTIVE                 0b00000100 // True if it's Orange (Woogi)'s turn to jump off the branch
#define ACTIVE_PLAYER_BRANCH_STATUS         0b00001000 // True if pressing A should allow the active Hasee to jump down from the branch
#define ACTIVE_PLAYER_JUMPING_STATUS        0b00010000
#define ACTIVE_PLAYER_STOP_AT_BRANCH_MASK   0b00100000 // True if the active player should stop when they next touch the branch
#define ORANGE_MASK_HASEE_IS_WALKING        0b01000000
#define PURPLE_MASK_HASEE_IS_WALKING        0b10000000

// Flags that persist from frame to frame...
#define ACTIVE_PLAYER (player_flags & ORANGE_HASEE_ACTIVE)
#define ACTIVE_PLAYER_ON_BRANCH (player_flags & ACTIVE_PLAYER_BRANCH_STATUS)

#define FLIP_ACTIVE_PLAYER() player_flags ^= ORANGE_HASEE_ACTIVE

#define ORANGE_SET_DIRECTION_RIGHT() (player_flags &= ~ORANGE_FACING_RIGHT_MASK)
#define ORANGE_SET_DIRECTION_LEFT() (player_flags |= ORANGE_FACING_RIGHT_MASK)
#define PURPLE_SET_DIRECTION_RIGHT() (player_flags &= ~PURPLE_FACING_RIGHT_MASK)
#define PURPLE_SET_DIRECTION_LEFT() (player_flags |= PURPLE_FACING_RIGHT_MASK)
#define ORANGE_FACING_RIGHT (player_flags & ORANGE_FACING_RIGHT_MASK)
#define PURPLE_FACING_RIGHT (player_flags & PURPLE_FACING_RIGHT_MASK)

#define ACTIVE_PLAYER_MOVE_OFF_BRANCH() (player_flags &= ~ACTIVE_PLAYER_BRANCH_STATUS)
#define ACTIVE_PLAYER_MOVE_ON_BRANCH() (player_flags |= ACTIVE_PLAYER_BRANCH_STATUS)

#define ACTIVE_PLAYER_IGNORE_BRANCH() (player_flags &= ~ACTIVE_PLAYER_STOP_AT_BRANCH_MASK)
#define ACTIVE_PLAYER_STOP_AT_BRANCH() (player_flags |= ACTIVE_PLAYER_STOP_AT_BRANCH_MASK)
#define ACTIVE_PLAYER_SHOULD_STOP_AT_BRANCH (player_flags & ACTIVE_PLAYER_STOP_AT_BRANCH_MASK)

#define ORANGE_HASEE_IS_WALKING                 (player_flags & ORANGE_MASK_HASEE_IS_WALKING)
#define ORANGE_HASEE_SET_WALKING()              (player_flags |= ORANGE_MASK_HASEE_IS_WALKING)
#define ORANGE_HASEE_SET_STANDING()             (player_flags &= ~ORANGE_MASK_HASEE_IS_WALKING)
#define PURPLE_HASEE_IS_WALKING                 (player_flags & PURPLE_MASK_HASEE_IS_WALKING)
#define PURPLE_HASEE_SET_WALKING()              (player_flags |= PURPLE_MASK_HASEE_IS_WALKING)
#define PURPLE_HASEE_SET_STANDING()             (player_flags &= ~PURPLE_MASK_HASEE_IS_WALKING)

// Flags that need to be cleared at the start of each frame...
#define HASEE_DID_SCORE_CHANGE_THIS_FRAME_MASK  0b00000001
// Flags that don't
#define HASEE_JUMPING_OFF_BRANCH_MASK           0b00000010
#define HASEE_JUMPING_TO_BRANCH_MASK            0b00000100
#define HASEE_LOCK_ON_PALETTE_2_MASK            0b00001000
#define HASEE_LOCK_ON_PALETTE_3_MASK            0b00010000

#define HASEE_SCORE_CHANGED_THIS_FRAME          (player_flags2 & HASEE_DID_SCORE_CHANGE_THIS_FRAME_MASK)
#define HASEE_SET_SCORE_CHANGED_THIS_FRAME()    (player_flags2 |= HASEE_DID_SCORE_CHANGE_THIS_FRAME_MASK)

#define ACTIVE_PLAYER_JUMPING_TO_BRANCH           (player_flags2 & HASEE_JUMPING_TO_BRANCH_MASK)
#define ACTIVE_PLAYER_JUMPING_OFF_BRANCH          (player_flags2 & HASEE_JUMPING_OFF_BRANCH_MASK)
#define ACTIVE_PLAYER_SET_JUMPING_TO_BRANCH()     (player_flags2 |= HASEE_JUMPING_TO_BRANCH_MASK)
#define ACTIVE_PLAYER_SET_JUMPING_OFF_BRANCH()    (player_flags2 |= HASEE_JUMPING_OFF_BRANCH_MASK)
#define ACTIVE_PLAYER_UNSET_JUMPING_TO_BRANCH()   (player_flags2 &= ~HASEE_JUMPING_TO_BRANCH_MASK)
#define ACTIVE_PLAYER_UNSET_JUMPING_OFF_BRANCH()  (player_flags2 &= ~HASEE_JUMPING_OFF_BRANCH_MASK)

#define HASEE_IS_PALETTE_2_LOCKED   (player_flags2 & HASEE_LOCK_ON_PALETTE_2_MASK)
#define HASEE_IS_PALETTE_3_LOCKED   (player_flags2 & HASEE_LOCK_ON_PALETTE_3_MASK)
#define HASEE_LOCK_PALETTE_2()      (player_flags2 |= HASEE_LOCK_ON_PALETTE_2_MASK)
#define HASEE_LOCK_PALETTE_3()      (player_flags2 |= HASEE_LOCK_ON_PALETTE_3_MASK)
#define HASEE_UNLOCK_PALETTE_2()    (player_flags2 &= ~HASEE_LOCK_ON_PALETTE_2_MASK)
#define HASEE_UNLOCK_PALETTE_3()    (player_flags2 &= ~HASEE_LOCK_ON_PALETTE_3_MASK)

#define HASEE_RESET_PLAYER_FLAGS_START_FRAME() (player_flags2 &= ~(HASEE_DID_SCORE_CHANGE_THIS_FRAME_MASK))

#define ONSCREEN_TREATS_MAXIMUM 13 // 16 2x2 metasprites, minus the 2 Hasees, minus 2 2x1 dizzy indicators

#define ON_BRANCH_STARTING_Y_VALUE 0x3300
#define ON_BRANCH_STARTNIG_Y_VALUE_HIGH_BYTE 0x33

#define ON_GROUND_STARTING_Y_VALUE 0xD000
#define ON_GROUND_STARTING_Y_VALUE_HIGH_BYTE 0xD0

#define HASEE_GRAVITY 0x0100
#define HASEE_MAX_SPEED 0x500

#define LETTER_UNCOLLECTED      0
#define LETTER_COLLECTED_PURPLE 1
#define LETTER_COLLECTED_ORANGE 2

#define LETTER_H_INDEX 0
#define LETTER_A_INDEX 1
#define LETTER_S_INDEX 2
#define LETTER_E_INDEX 3
#define LETTER_E2_INDEX 4

#define LETTER_ORANGENESS_MASK 0b0100
#define LETTER_TYPE_MASK 0b0011
#define LETTER_IS_ORANGE(type) (type & LETTER_ORANGENESS_MASK)

#define LEVEL_FRAME_LENGTH 1800

#define MACCY_PROBABILITY 6
#define RAINBOW_PROBABILITY 16 * MACCY_PROBABILITY
#define GREEN_PROBABILITY (unsigned int)(12 * RAINBOW_PROBABILITY)
#define BLUE_PROBABILITY (unsigned int)(18 * RAINBOW_PROBABILITY)

#define HASEE_TIME_BONUS 20
#define HASEE_SUPER_TIME_BONUS 40

#endif