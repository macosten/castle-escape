#define IGLOO_METASPRITE_BANK 1
#define IGLOO_CODE_BANK 1

// +1 to weight for each 8x8 sprite on-screen... 64 - 12 per Chia for now
#define MAX_JUNK_WEIGHT_ONSCREEN 40
#define ONSCREEN_JUNK_MAXIMUM 12

#define IGLOOT_BAG          0x00
#define IGLOOT_CHIAPOP      0x01
#define IGLOOT_UMBRELLA     0x02
#define IGLOOT_THINGY       0x03
#define IGLOOT_POTION       0x04
#define IGLOOT_COIN         0x05
#define IGLOOT_BOMB         0x06
#define IGLOOT_PIANO        0x07

#define IGLOOT_BOMB_EXPLOSION 0x08
#define IGLOOT_PIANO_EXPLOSION 0x09

#define IGLOOT_BASE_THRESHOLD 218

#define CHIA_STARTING_X (0x8000 - 0x0C00)
#define MIKA_STARTING_Y 0xB000
#define IGLOO_FLOOR_Y (MIKA_STARTING_Y + 0x1800 + 0x0800)
#define IGLOO_FLOOR_Y_CHAR (0xC8 + 0x08)

#define CARASSA_STARTING_Y 0x2000
#define IGLOO_BRIDGE_Y CARASSA_STARTING_Y + 0x1800
#define IGLOO_BRIDGE_Y_CHAR 0x38

#define IGLOO_GRAVITY 0x0060
#define IGLOO_MAX_SPEED 0x0400
#define IGLOO_ACCEL 0x00D0
#define IGLOO_FRICTION 0x0040

#define MIKA_JUMP_STRENGTH 0x0500
#define MIKA_WIDTH 3 * 8
#define MIKA_HEIGHT 4 * 8

#define IGLOOT_MIN_SPEED 0x0080

#define IGLOO_MIN_X 16
#define IGLOO_MAX_X (255 - IGLOO_MIN_X - MIKA_WIDTH)

// Flags that need to be cleared at the start of each frame...
#define IGLOO_DID_SCORE_CHANGE_THIS_FRAME_MASK      0b00000001
#define IGLOO_DID_BUFFER_MESSAGE_THIS_FRAME_MASK    0b00000010

#define IGLOO_RESET_PLAYER_FLAGS_START_FRAME() (player_flags &= ~(IGLOO_DID_SCORE_CHANGE_THIS_FRAME_MASK|IGLOO_DID_BUFFER_MESSAGE_THIS_FRAME_MASK))

// Flags that don't
#define IGLOO_JUMPING_MASK               0b00000100
#define IGLOO_CARASSA_WALKING_MASK       0b00001000

#define IGLOO_SCORE_CHANGED_THIS_FRAME          (player_flags & IGLOO_DID_SCORE_CHANGE_THIS_FRAME_MASK)
#define IGLOO_SET_SCORE_CHANGED_THIS_FRAME()    (player_flags |= IGLOO_DID_SCORE_CHANGE_THIS_FRAME_MASK)

#define IGLOO_DID_BUFFER_MESSAGE_THIS_FRAME     (player_flags & IGLOO_DID_BUFFER_MESSAGE_THIS_FRAME_MASK)
#define IGLOO_SET_BUFFERED_MESSAGE_THIS_FRAME() (player_flags |= IGLOO_DID_BUFFER_MESSAGE_THIS_FRAME_MASK)

#define IGLOO_IS_JUMPING                        (player_flags & IGLOO_JUMPING_MASK)
#define IGLOO_SET_JUMPING()                     (player_flags |= IGLOO_JUMPING_MASK)
#define IGLOO_SET_NOT_JUMPING()                 (player_flags &= ~IGLOO_JUMPING_MASK)
#define IGLOO_IS_CARASSA_WALKING                (player_flags & IGLOO_CARASSA_WALKING_MASK)
#define IGLOO_SET_CARASSA_WALKING()             (player_flags |= IGLOO_CARASSA_WALKING_MASK)
#define IGLOO_SET_CARASSA_NOT_WALKING()         (player_flags &= ~IGLOO_CARASSA_WALKING_MASK)

// Original macros defined in enemy_macros.h. Specifically chosen so zeroing out flags brings them to their first/default state

#define IGLOO_STOP_ITEM(index) CANNONBALL_SET_NEG_X(index)
#define IGLOO_START_MOVING_ITEM(index) CANNONBALL_SET_POS_X(index)
#define IGLOO_IS_ITEM_MOVING(index) CANNONBALL_X_DIRECTION(index)

#define IGLOO_SET_ITEM_UNBROKEN(index) ENEMY_SET_DIRECTION_LEFT(index)
#define IGLOO_SET_ITEM_BROKEN(index) ENEMY_SET_DIRECTION_RIGHT(index)
#define IGLOO_IS_ITEM_BROKEN(index) ENEMY_DIRECTION(index)
