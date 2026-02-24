#define DECKSWABBER_CODE_BANK 1
#define DECKSWABBER_METASPRITE_BANK 1

#define DECKSWABBER_TILE_HEIGHT 8
#define DECKSWABBER_TILE_WIDTH 8

#define DECKSWABBER_LEVEL_PACK_SIZE 10

// Assumes an original-style 8 by 8 grid... will need to use something else if we want alternate grid sizes
#define DeckswabberGetTileIndex(destinationVar, x_coord, y_coord) { \
    __asm__("lda %v", y_coord); \
    __asm__("asl"); \
    __asm__("asl"); \
    __asm__("asl"); \
    __asm__("ora %v", x_coord); \
    __asm__("sta %v", destinationVar); \
};

#define DECKSWABBER_WATER_HOLE_ID (sizeof(deckswabber_metatiles)/5 - 2)
#define DECKSWABBER_EMPTY_HOLE_ID (sizeof(deckswabber_metatiles)/5 - 1)

// Flags that need to be cleared at the start of each frame...
#define DECKSWABBER_DID_SCORE_CHANGE_THIS_FRAME_MASK      0b00000001

#define DECKSWABBER_RESET_PLAYER_FLAGS_START_FRAME() (player_flags &= ~(DECKSWABBER_DID_SCORE_CHANGE_THIS_FRAME_MASK))

#define DECKSWABBER_SCORE_CHANGED_THIS_FRAME          (player_flags & DECKSWABBER_DID_SCORE_CHANGE_THIS_FRAME_MASK)
