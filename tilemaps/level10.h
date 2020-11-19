// Do not try to use pointers from another bank while that bank isn't 
// swapped in the correct place in RAM.
// Otherwise, you'll get garbage data.

#pragma rodata-name(push, "BANK0")

// bitpacked coords (0xXY), nt, direction (or other info) + type
// 0x10 + type for right-facing, just type for left-facing
// 0xff terminates

const unsigned char const level10_enemy[]={
	0x49, 2, 4, // ENEMY_CANNON,
	0x57, 2, 5,  //debug
	0x69, 2, 7, //debug
	//0x79, 2, 7,  //debug 
	//0x24, 2, 6, // ENEMY_SPIKEBALL,
	0x24, 2, 3, //debug
	0x71, 2, 6, // ENEMY_SPIKEBALL, 
	0x91, 2, 2, // ENEMY_GRARRL,
	0x2A, 1, 6, // ENEMY_SPIKEBALL,
	0x9C, 0, 1, // ENEMY_KORBAT,
	0xFF
};

const unsigned char const level10_compressed[] = {
	76,90,71,0,0,2,208,0,0,0,225,25,166,28,106,1,
	2,3,8,16,4,7,16,7,6,44,16,1,16,131,16,4,
	14,3,30,8,3,11,8,42,16,2,3,9,8,23,25,3,
	12,8,24,24,3,11,8,4,13,16,4,21,3,12,72,1,
	1,3,27,104,15,3,10,8,0,16,5,3,6,8,12,5,
	16,4,3,6,232,11,3,12,248,40,16,33,3,24,184,4,
	13,21,44,20,21,3,24,24,3,16,8,22,44,44,40,28,
	29,40,3,11,24,30,31,3,23,40,41,44,41,3,12,104,
	3,2,243,3,15,88,3,34,28,3,11,8,22,3,8,8,
	8,84,3,10,24,8,84,3,10,56,3,44,104,16,130,20,
	3,35,91,8,131,6,3,4,164,3,9,24,3,3,164,3,
	8,136,3,35,51,9,3,8,8,3,36,52,3,2,156,3,
	59,8,3,12,248,16,2,12,3,73,104,8,22,12,4,3,
	75,120,12,7,7,10,8,200,40,32,33,3,6,37,8,216,
	34,35,8,165,26,8,136,3,4,136,4,4,16,225,3,3,
	141,
};

#pragma rodata-name(pop)

