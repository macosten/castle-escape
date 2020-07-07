//
//  metatiles.h
//  Castle Escape
//
//  Created by Zaccari Silverman on 6/12/20.
//

// Format: 4 tile indices (upper left, upper right, lower left, lower right), then the palette index.
const unsigned char metatiles[]={
    // Solid Tiles
    160, 160, 176, 176,  0, // Upward Spikes
    161, 161, 177, 177,  0, // Downward Spikes
    162, 163, 162, 163,  0, // Rightward Spikes
    178, 179, 178, 179,  0, // Leftward Spikes
    230, 231, 246, 247,  0, // Regular Block No Edge
    132, 229, 246, 247,  0, // Regular Block U Edge
    230, 227, 246, 243,  0, // Regular Block R Edge
    230, 231, 148, 245,  0, // Regular Block D Edge
    226, 231, 242, 247,  0, // Regular Block L Edge
    132, 141, 246, 243,  0, // Reg. Block, U+R Edge
    230, 227, 148, 157,  0, // Reg. Block, D+R Edge
    226, 231, 240, 245,  0, // Reg. Block, D+L Edge
    224, 229, 242, 247,  0, // Reg. Block, U+L Edge
    132, 229, 148, 245,  0, // Reg. Block, U+D Edge
    226, 227, 242, 243,  0, // Reg. Block, L+R Edge
    224, 141, 240, 157,  0, // Reg, Block, All Edges
    128, 129, 144, 145,  0, // Left Ledge
    132, 133, 148, 149,  0, // Right Ledge
    134, 129, 150, 145,  0, // Left Circular Platform
    132, 137, 148, 153,  0, // Right Circular Platform
    138, 129, 154, 145,  0, // Left Square Platform
    132, 141, 148, 157,  0, // Right Square Platform
    236, 237, 252, 253,  3, // ? Block
    // 22 (0x16)
    
    // Non-Solid Tiles, starting at 0x17
    164, 165, 180, 181,  3, // Door
    166, 165, 182, 181,  3,
    166, 169, 182, 185,  3,
    170, 171, 186, 187,  2, // Red Door
    
    142, 143, 158, 159,  0, // Prison Window
    0, 0, 0, 142,  0,
    0, 0, 143, 0,  0,
    0, 158, 0, 0,  0,
    159, 0, 0, 0,  0,
    
    172, 173, 188, 189,  0, // Prison Door
    174, 175, 190, 191,  0,
    188, 205, 188, 221,  0,
    206, 191, 222, 191,  0,
    
    196, 197, 212, 213,  0, // Bone Piles
    198, 199, 214, 215,  0,
    200, 201, 216, 217,  0,
    202, 203, 218, 219,  0,
    
    194, 195, 210, 211,  3, // Candle
    192, 193, 208, 209,  0, // Skeleton
    232, 233, 248, 249,  3, // Star
    234, 235, 250, 251,  1, // Energy Refill
    
    0,   0,   0,   0,    0, // Empty
};

#define EMPTY_TILE 44
