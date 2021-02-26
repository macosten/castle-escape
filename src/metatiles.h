//
//  metatiles.h
//  Castle Escape
//
//  Created by Zaccari Silverman on 6/12/20.
//
#ifndef metatiles_h
#define metatiles_h
// Format: 4 tile indices (upper left, upper right, lower left, lower right), then the palette index.
const unsigned char const metatiles[];

#define QUAD_EDGE_STONE 15
#define QUESTION_BLOCK 22
#define BONKED_QUESTION_BLOCK 27
#define STAR_TILE 42
#define ENERGY_REFILL_TILE 43
#define EMPTY_TILE 44
#define DIALOG_BOX_BOTTOM 49


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
const unsigned char const metatile_property_lookup_table[];

#define METATILE_IS_SOLID(mtid) (metatile_property_lookup_table[mtid] & METATILE_SOLID)

#endif