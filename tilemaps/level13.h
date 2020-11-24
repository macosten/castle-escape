// Do not try to use pointers from another bank while that bank isn't 
// swapped in the correct place in RAM.
// Otherwise, you'll get garbage data.

#pragma rodata-name(push, "BANK0")

// bitpacked coords (0xXY), nt, direction (or other info) + type
// 0xff terminates

const unsigned char const level13_enemy[]={
	0x49, 2, 4, // Cannon
	0x75, 2, 4,
	0x44, 2, 6, // Spikeball
	0x40, 2, 1, // Korbat
	0x3E, 1, 1,
	0x2D, 1, 1,
	0x2A, 1, 6,
	0x65, 1, 4,
	// The original game has two acid drops here:
	0x55, 1, 10,
	0x45, 1, 10,
	// These drops don't actually move (I actually don't even know if they damage Valrigard)
	// but I'll put them here for now.
	0x19, 0, 6,
	0x39, 0, 6,
	0x79, 0, 6,
	0xFF
};

const unsigned char const level13[] = {
	76,90,71,0,0,2,208,0,0,0,252,71,20,31,171,1,
	1,2,3,8,4,7,8,7,6,44,8,1,8,131,8,4,
	14,2,29,8,2,20,8,20,13,8,3,9,2,13,24,14,
	2,6,8,40,44,15,40,2,10,8,44,32,33,2,9,24,
	4,3,246,21,2,9,24,2,13,56,2,29,8,44,12,5,
	8,2,6,2,7,8,11,7,4,7,7,7,10,2,8,136,
	40,3,130,2,9,56,2,9,8,4,13,21,44,20,7,13,
	9,44,20,13,2,42,40,3,84,2,10,104,11,2,8,232,
	22,44,44,40,28,29,40,2,11,136,30,31,2,23,152,41,
	44,41,2,10,8,4,2,2,244,44,44,42,42,2,12,40,
	42,22,2,28,232,2,10,8,12,2,7,216,8,4,48,2,
	11,168,12,2,2,92,3,200,2,2,164,3,131,0,3,200,
	2,2,164,3,136,20,2,6,232,12,2,34,53,2,7,40,
	44,20,2,66,52,13,9,28,29,2,11,88,14,30,31,2,
	12,8,2,25,24,15,2,11,24,2,37,247,2,7,24,34,
	35,8,193,12,6,23,25,2,9,8,12,4,6,24,24,3,
	136,4,2,35,197,4,4,8,225,2,3,83,
};

#pragma rodata-name(pop)

