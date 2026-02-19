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
