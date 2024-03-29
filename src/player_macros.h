// =====
// Constants + Macros for player_flags
#define DIRECTION (player_flags & 1) //facing left or right? (lsb of player_flags)
#define SET_DIRECTION_LEFT() (player_flags &= ~1) // Un-set the lsb
#define SET_DIRECTION_RIGHT() (player_flags |= 1) // Set the lsb
#define LEFT 0
#define RIGHT 1
#define UP 0
#define DOWN 1

#define STATUS_DEAD (player_flags & 2)
#define SET_STATUS_ALIVE() (player_flags &= ~2)
#define SET_STATUS_DEAD() (player_flags |= 2) // set bit 1
#define ALIVE 0
#define DEAD 2 // 0b10
// Timed; isn't reset at the start of each frame
#define IS_SWINGING_SWORD (player_flags & 4)
#define SET_STATUS_NOT_SWINGING_SWORD() (player_flags &= ~4)
#define SET_STATUS_SWINGING_SWORD() (player_flags |= 4)
// Should be checked every frame:
#define SCORE_CHANGED_THIS_FRAME (player_flags & 8)
#define SET_SCORE_CHANGED_THIS_FRAME() (player_flags |= 8)
#define RESET_SCORE_CHANGED_THIS_FRAME() (player_flags &= ~8)
// Should be checked every frame:
#define TOUCHING_YELLOW_DOOR (player_flags & 16)
#define SET_TOUCHING_YELLOW_DOOR() (player_flags |= 16)
#define RESET_TOUCHING_YELLOW_DOOR() (player_flags &= ~16)
// Should be checked every frame:
#define TOUCHING_SPIKES (player_flags & 32)
#define SET_TOUCHING_SPIKES() (player_flags |= 32)
#define RESET_TOUCHING_SPIKES() (player_flags &= ~32)
// Should be checked every frame:
#define IS_WALKING (player_flags & 64)
#define SET_IS_WALKING() (player_flags |= 64)
#define RESET_IS_WALKING() (player_flags &= ~64)
// Should be checked every frame:
#define SHOULD_GO_TO_BONUS_LEVEL (player_flags & 128)
#define SET_SHOULD_GO_TO_BONUS_LEVEL() (player_flags |= 128)
#define RESET_SHOULD_GO_TO_BONUS_LEVEL() (player_flags &= ~128)
// Reset all the flags that should be reset each frame for the first flag byte.
#define RESET_PLAYER_FLAGS_1_START_FRAME() (player_flags &= ~(128 + 64 + 32 + 16 + 8))

#define GAME_PAUSED (player_flags2 & 1)
#define SET_GAME_PAUSED() (player_flags2 |= 1)
#define RESET_GAME_PAUSED() (player_flags2 &= ~1)
#define TOGGLE_GAME_PAUSED() (player_flags2 ^= 1)

#define DID_HEADBONK (player_flags2 & 2)
#define SET_DID_HEADBONK() (player_flags2 |= 2)

#define RESET_PLAYER_FLAGS_2_START_FRAME() (player_flags2 &= ~(2))
