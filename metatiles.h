//
//  metatiles.h
//  Castle Escape
//
//  Created by Zaccari Silverman on 6/12/20.
//

// Format: 4 tile indices (upper left, upper right, lower left, lower right), then the palette index.
const unsigned char const metatiles[]={
    // Solid Tiles
    160, 160, 176, 176,  0, // Upward Spikes
    161, 161, 177, 177,  0, // Downward Spikes
    162, 163, 162, 163,  0, // Rightward Spikes
    178, 179, 178, 179,  0, // Leftward Spikes

    230, 231, 246, 247,  0, // Regular Block No Edge
    130, 229, 246, 247,  0, // Regular Block U Edge
    230, 227, 246, 243,  0, // Regular Block R Edge
    230, 231, 146, 245,  0, // Regular Block D Edge
    226, 231, 242, 247,  0, // Regular Block L Edge
    130, 225, 246, 243,  0, // Reg. Block, U+R Edge
    230, 227, 146, 241,  0, // Reg. Block, D+R Edge
    226, 231, 240, 245,  0, // Reg. Block, D+L Edge
    224, 229, 242, 247,  0, // Reg. Block, U+L Edge
    130, 229, 146, 245,  0, // Reg. Block, U+D Edge
    226, 227, 242, 243,  0, // Reg. Block, L+R Edge
    224, 225, 240, 241,  0, // Reg, Block, All Edges

    // I reserve the right to make these into conveyor-related tiles.
    132, 133, 148, 149,  1, // Left Conveyor Left Edge
    136, 137, 152, 153,  1, // Left Conveyor Right Eedge
    134, 135, 150, 151,  1, // Right Conveyor Left Edge
    138, 139, 154, 155,  1, // Right Conveyor Right Edge

    // These will probably remain normal tiles, though.
    224, 129, 240, 145,  0, // Left Square Platform (U+L+D Edges)
    130, 225, 146, 241,  0, // Right Square Platform (U+R+D Edges)

    236, 237, 252, 253,  3, // ? Block
    // 22 (0x16)
    
    // Non-Solid Tiles, starting at 0x17
    164, 165, 180, 181,  3, // Door
    166, 165, 182, 181,  3,
    166, 167, 182, 183,  3,
    170, 171, 186, 187,  2, // Red Door
    
    168, 169, 184, 185,  0, // Prison Window
    0, 0, 0, 168,  0,
    0, 0, 169, 0,  0,
    0, 184, 0, 0,  0,
    185, 0, 0, 0,  0,
    
    172, 173, 188, 189,  0, // Prison Door (32)
    174, 175, 190, 191,  0, //
    188, 205, 188, 221,  0, //
    206, 191, 222, 191,  0, //
    
    196, 197, 212, 213,  0, // Bone Piles
    198, 199, 214, 215,  0,
    200, 201, 216, 217,  0,
    202, 203, 218, 219,  0,
    
    194, 195, 210, 211,  3, // Candle
    192, 193, 208, 209,  0, // Skeleton
    232, 233, 248, 249,  3, // Star
    234, 235, 250, 251,  1, // Energy Refill
    
    0,   0,   0,   0,    0, // Empty

    // Unfortunately I didn't have the foresight to make Empty the absolute last tile, so:

};

#define STAR_TILE 42
#define ENERGY_REFILL_TILE 43
#define EMPTY_TILE 44


#define METATILE_NO_EFFECT      0 // No collisions, or anything else
#define METATILE_SOLID          0b00000001
#define METATILE_SPIKES         0b00000010
#define METATILE_CONVEYOR_LEFT  0b00000100
#define METATILE_CONVEYOR_RIGHT 0b00001000
#define METATILE_YELLOW_DOOR    0b00010000
#define METATILE_RED_DOOR       0b00100000
#define METATILE_POWERUP        0b01000000
// One more possible optionset member


// Generally speaking, metatiles will only have one or two properties set to 1
// ...and if it's two, one of them is METATILE_SOLID.
const unsigned char const metatile_property_lookup_table[]={
    // Spikes
    METATILE_SPIKES,
    METATILE_SPIKES,
    METATILE_SPIKES,
    METATILE_SPIKES,

    // Solid castle blocks
    METATILE_SOLID,
    METATILE_SOLID,
    METATILE_SOLID,
    METATILE_SOLID,
    METATILE_SOLID,
    METATILE_SOLID,
    METATILE_SOLID,
    METATILE_SOLID,
    METATILE_SOLID,
    METATILE_SOLID,
    METATILE_SOLID,
    METATILE_SOLID,

    // These next 4 may or may not be made into conveyor blocks at some point
    METATILE_SOLID|METATILE_CONVEYOR_LEFT,
    METATILE_SOLID|METATILE_CONVEYOR_LEFT,
    METATILE_SOLID|METATILE_CONVEYOR_RIGHT,
    METATILE_SOLID|METATILE_CONVEYOR_RIGHT,


    METATILE_SOLID,
    METATILE_SOLID,

    // Question Block
    METATILE_SOLID,

    // Doors
    METATILE_YELLOW_DOOR,
    METATILE_YELLOW_DOOR,
    METATILE_YELLOW_DOOR,
    METATILE_RED_DOOR,

    // Prison Window
    METATILE_NO_EFFECT, 
    METATILE_NO_EFFECT,
    METATILE_NO_EFFECT,
    METATILE_NO_EFFECT,
    METATILE_NO_EFFECT,

    // Prison Door
    METATILE_NO_EFFECT, 
    METATILE_NO_EFFECT,
    METATILE_NO_EFFECT,
    METATILE_NO_EFFECT,

    // Bone piles
    METATILE_NO_EFFECT, 
    METATILE_NO_EFFECT,
    METATILE_NO_EFFECT,
    METATILE_NO_EFFECT,

    METATILE_NO_EFFECT, // Candle
    METATILE_NO_EFFECT, // Skeleton
    METATILE_POWERUP, // Star
    METATILE_POWERUP, // Energy Refill

    METATILE_NO_EFFECT, // Empty Tile    
};
