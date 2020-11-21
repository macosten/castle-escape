// Do not try to use pointers from another bank while that bank isn't 
// swapped in the correct place in RAM.
// Otherwise, you'll get garbage data.

#pragma rodata-name(push, "BANK0")

// bitpacked coords (0xXY), nt, direction (or other info) + type
// 0xff terminates

const unsigned char const level07_enemy[]={
	0xAD, 2, 2, // Grarrl
	0x56, 2, 6, // Spikeball
	0x52, 2, 6,
	0x30, 2, 2,
	0x9D, 1, 6,
	0x2B, 1, 2,
	0xA6, 1, 5, // Acid pool
	0x86, 1, 1, // Korbat
	0x67, 1, 2,
	0x27, 1, 2,
	0x24, 1, 1,
	0x42, 1, 2,
	0x4C, 0, 5,
	0x5C, 0, 5,
	0x6C, 0, 5,
	0x6A, 0, 6,
	0x4A, 0, 6,
	0x69, 0, 1,
	0x18, 0, 4, // Cannon
	0xFF
};

const unsigned char const level07[] = {
	76,90,71,0,0,2,208,0,0,1,30,47,42,40,154,1,
	0,1,2,3,4,4,3,7,6,44,3,1,1,8,7,1,
	30,8,1,13,8,7,3,7,2,136,3,131,3,4,14,2,
	136,28,29,44,44,3,98,23,25,2,200,30,31,44,44,3,
	98,24,24,2,200,44,44,12,13,13,13,5,3,1,1,6,
	56,48,3,193,11,1,22,72,4,2,98,3,2,9,1,5,
	8,4,1,5,122,10,1,6,8,10,28,29,42,42,1,2,
	106,2,200,10,44,30,31,42,42,1,2,106,1,6,104,44,
	12,2,250,1,7,88,11,7,7,1,7,200,6,44,2,119,
	42,1,8,120,40,30,31,40,1,10,200,1,27,136,1,41,
	8,1,5,136,1,47,8,40,44,1,5,71,2,200,8,13,
	13,21,28,29,1,2,167,2,200,6,42,1,2,165,1,2,
	108,40,1,10,56,20,13,13,13,1,5,152,2,185,2,30,
	42,2,152,4,2,17,13,21,1,9,88,2,82,1,10,152,
	2,88,2,220,1,8,72,2,123,40,1,6,8,37,1,3,
	85,1,7,88,38,36,39,1,17,216,1,7,56,1,34,33,
	1,2,213,1,7,24,4,2,197,1,7,8,2,190,44,41,
	1,10,8,20,13,22,2,55,1,7,8,40,32,33,1,3,
	251,1,5,8,41,44,34,35,1,15,104,1,39,152,
};

#pragma rodata-name(pop)

