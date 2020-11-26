// Do not try to use pointers from another bank while that bank isn't 
// swapped in the correct place in RAM.
// Otherwise, you'll get garbage data.

#pragma rodata-name(push, "BANK0")

// bitpacked coords (0xXY), nt, direction (or other info) + type
// 0xff terminates

const unsigned char const level18_enemy[]={
	0x1B, 0, 4, // Cannon
	0x8D, 0, 3, // Splyke
	0x17, 0, 7, // Sun
	0x26, 0, 7,
	0xFF
};

const unsigned char const level18[] = {
	76,90,71,0,0,0,240,0,0,0,108,220,62,10,52,1,
	0,1,2,3,4,4,3,7,6,44,3,1,1,8,7,1,
	5,8,7,3,7,2,136,3,131,3,4,14,1,21,8,42,
	3,4,1,7,8,12,13,3,1,21,42,12,1,6,56,14,
	2,216,8,2,200,23,25,14,42,20,2,90,1,5,88,24,
	24,2,88,1,7,72,4,2,17,13,13,9,1,10,88,32,
	33,44,48,2,2,22,1,5,8,40,34,35,40,1,11,120,
	34,35,1,9,136,4,5,3,7,1,3,88,
};

#pragma rodata-name(pop)

