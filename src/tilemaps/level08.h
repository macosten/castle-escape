// Do not try to use pointers from another bank while that bank isn't 
// swapped in the correct place in RAM.
// Otherwise, you'll get garbage data.

#pragma rodata-name(push, "BANK0")

// bitpacked coords (0xXY), nt, direction (or other info) + type
// 0xff terminates

const unsigned char const level08_enemy[]={
	0x4A, 2, 4, // Cannon
	0x7A, 2, 4,
	0x45, 2, 4,
	0x75, 2, 4,
	0x34, 2, 2, // Grarrl
	0x94, 2, 2,
	0x3C, 1, 4,
	0x8C, 1, 4,
	0x66, 1, 4,
	0x19, 1, 1,
	0xA8, 1, 1,
	0x6E, 0, 4,
	0x5C, 0, 6,
	0x7C, 0, 6,
	0xFF
};

const unsigned char const level08[] = {
	76,90,71,0,0,2,208,0,0,0,191,192,91,17,110,1,
	2,3,8,9,4,7,9,7,6,44,9,1,9,131,9,4,
	14,3,30,8,3,27,8,23,25,3,12,8,24,24,3,11,
	8,4,13,13,21,3,28,72,3,10,8,20,13,9,1,8,
	45,3,6,8,1,1,1,9,97,3,26,56,42,42,42,3,
	12,8,22,3,13,8,3,13,24,3,13,56,20,5,13,5,
	3,9,104,44,1,48,44,48,3,27,104,3,28,8,3,7,
	8,0,0,15,9,226,15,0,0,3,37,8,3,37,7,20,
	13,3,28,56,8,200,47,0,0,47,3,10,8,11,13,13,
	10,3,29,136,3,8,8,20,3,3,121,9,161,3,6,24,
	3,34,20,9,130,3,5,8,8,135,8,73,3,8,8,22,
	22,3,9,8,3,14,40,3,15,72,3,13,104,40,32,33,
	40,3,11,8,34,35,3,9,8,4,5,9,7,8,179,
};

#pragma rodata-name(pop)

