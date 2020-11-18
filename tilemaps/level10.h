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
	76,90,71,0,0,2,208,0,0,0,227,0,100,27,189,1,
	2,3,8,18,4,7,18,7,6,44,18,1,18,131,18,4,
	14,3,30,8,3,11,8,42,18,2,3,9,8,23,25,3,
	12,8,24,24,3,12,8,13,18,4,21,3,12,72,1,1,
	3,27,104,15,3,10,8,0,18,5,3,6,8,12,5,18,
	4,3,6,232,11,3,12,248,40,18,33,3,24,184,4,13,
	17,44,16,17,3,24,24,3,16,8,22,44,44,40,28,29,
	40,3,11,24,30,31,3,23,40,41,44,41,3,12,104,13,
	3,4,102,3,12,40,3,34,28,3,11,8,22,3,8,8,
	8,84,3,10,24,8,84,3,10,56,3,44,88,18,130,16,
	3,35,91,8,131,6,3,4,164,3,9,24,3,3,164,3,
	8,136,3,35,51,9,3,8,8,3,36,52,21,3,13,152,
	3,14,168,3,34,76,3,13,24,12,3,73,104,8,22,12,
	4,3,75,120,12,7,7,10,8,200,40,32,33,3,6,37,
	8,216,34,35,8,165,26,8,136,3,4,136,4,4,18,225,
	3,3,141,
};

/*
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

const unsigned char * const level10_ordered_nts[] = {
	level10_0, level10_1, level10_2,
};
*/

#pragma rodata-name(pop)

