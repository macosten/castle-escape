#define IGLOO_METASPRITE_BANK 1
#define IGLOO_CODE_BANK 1

// +1 to weight for each 8x8 sprite on-screen... 64 - 12 per Chia for now
#define MAX_JUNK_WEIGHT_ONSCREEN 40

// Flags that need to be cleared at the start of each frame...
#define IGLOO_DID_SCORE_CHANGE_THIS_FRAME_MASK      0b00000001
#define IGLOO_DID_BUFFER_MESSAGE_THIS_FRAME_MASK    0b00000010
// Flags that don't
#define IGLOO_JUMPING_MASK               0b00000100

#define IGLOO_SCORE_CHANGED_THIS_FRAME          (player_flags & IGLOO_DID_SCORE_CHANGE_THIS_FRAME_MASK)
#define IGLOO_SET_SCORE_CHANGED_THIS_FRAME()    (player_flags |= IGLOO_DID_SCORE_CHANGE_THIS_FRAME_MASK)

#define IGLOO_DID_BUFFER_MESSAGE_THIS_FRAME     (player_flags & IGLOO_DID_BUFFER_MESSAGE_THIS_FRAME_MASK)
#define IGLOO_SET_BUFFERED_MESSAGE_THIS_FRAME() (player_flags |= IGLOO_DID_BUFFER_MESSAGE_THIS_FRAME_MASK)
