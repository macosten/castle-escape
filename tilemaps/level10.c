//
//  levels.h
//  Castle Escape
//
//  Created by Zaccari Silverman on 6/17/20.
//

// Idea: what if we used the useless 4 bytes at the end of each row
// to encode a list of where this nametable's powerups/?blocks were?

// Do not try to use pointers from another bank while that bank isn't 
// swapped in the correct place in RAM.
// Otherwise, you'll get garbage data.

// #pragma rodata-name(push, "BANK0")

const unsigned char const level10_0[]={
4,7,7,7,7,7,7,7,7,7,7,6,44,44,44,44,
6,44,44,44,44,44,44,44,44,44,44,14,44,44,44,44,
6,44,44,44,44,44,44,44,44,44,44,14,44,44,44,44,
6,44,44,44,44,44,44,44,44,44,44,14,44,44,44,44,
6,44,44,44,44,44,44,44,44,44,44,14,44,44,44,44,
6,44,44,44,44,44,44,44,44,44,44,14,44,44,44,44,
6,44,44,44,44,44,44,44,44,44,44,14,44,44,44,44,
6,42,42,42,42,42,44,44,44,44,44,14,44,44,44,44,
6,23,25,42,42,42,44,44,44,44,44,14,44,44,44,44,
6,24,24,42,42,42,44,44,44,44,44,14,44,44,44,44,
6,13,13,13,13,13,13,13,21,44,44,14,44,44,44,44,
6,44,44,44,44,44,44,1,1,44,44,14,44,44,44,44,
6,44,44,44,44,44,44,44,44,44,44,14,44,44,44,44,
6,44,44,44,44,44,15,44,44,44,44,14,44,44,44,44,
6,44,44,0,0,0,0,0,0,0,0,14,44,44,44,44,
};

const unsigned char const level10_1[]={
6,44,44,12,5,5,5,5,5,5,5,6,44,44,44,44,
6,44,44,11,7,7,7,7,7,7,7,6,44,44,44,44,
6,44,40,44,40,44,44,44,44,44,44,14,44,44,44,44,
6,44,44,44,44,44,44,44,44,44,44,14,44,44,44,44,
4,13,17,44,16,17,44,44,44,44,44,14,44,44,44,44,
6,44,44,44,44,44,44,44,44,44,44,14,44,44,44,44,
6,44,44,44,44,44,44,44,44,44,44,14,44,44,44,44,
6,44,22,44,44,40,28,29,40,44,44,14,44,44,44,44,
6,44,44,44,44,44,30,31,44,44,44,14,44,44,44,44,
6,44,44,44,44,44,44,44,44,44,44,14,44,44,44,44,
6,41,44,41,44,44,44,44,44,44,44,14,44,44,44,44,
4,13,13,17,44,44,44,44,44,44,44,14,44,44,44,44,
6,44,44,44,44,44,44,42,42,42,44,14,44,44,44,44,
6,44,44,44,44,44,44,42,22,42,44,14,44,44,44,44,
6,44,44,42,42,42,44,42,42,42,44,14,44,44,44,44,
};

const unsigned char const level10_2[]={
6,44,44,42,22,42,44,44,44,44,44,14,44,44,44,44,
6,44,44,42,42,42,44,44,44,44,44,14,44,44,44,44,
6,44,44,44,44,16,13,13,13,13,13,6,44,44,44,44,
6,40,28,29,40,44,44,44,44,44,44,14,44,44,44,44,
6,44,30,31,44,44,44,44,44,44,44,14,44,44,44,44,
4,5,5,5,5,5,9,44,44,44,44,14,44,44,44,44,
4,7,7,7,7,7,7,21,44,44,44,14,44,44,44,44,
6,44,44,44,44,44,44,44,44,44,44,14,44,44,44,44,
6,44,44,44,44,44,44,44,40,44,44,14,44,44,44,44,
6,44,44,44,44,44,44,44,44,44,12,6,44,44,44,44,
6,44,44,44,44,44,40,44,44,12,4,6,44,44,44,44,
6,44,44,44,44,44,44,44,12,7,7,10,44,44,44,44,
6,44,40,32,33,44,44,12,6,44,44,44,44,44,44,44,
6,44,44,34,35,44,12,4,6,44,26,44,44,44,44,44,
4,5,5,5,5,5,4,4,4,5,5,9,44,44,44,44,
};

	// bitpacked coords (0xXY), nt, direction (or other info) + type
	// 0x10 + type for right-facing, just type for left-facing

const unsigned char const level10_enemy[]={
	0x49, 2, 4, // ENEMY_CANNON,
	0x59, 2, 10,  //debug
	0x69, 2, 10, //debug
	0x79, 2, 10,  //debug 
	0x24, 2, 6, // ENEMY_SPIKEBALL,
	0x71, 2, 6, // ENEMY_SPIKEBALL, 
	0x91, 2, 2, // ENEMY_GRARRL,
	0x2A, 1, 6, // ENEMY_SPIKEBALL,
	0x9C, 0, 1, // ENEMY_KORBAT,
	0xFF
};
