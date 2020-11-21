// Do not try to use pointers from another bank while that bank isn't 
// swapped in the correct place in RAM.
// Otherwise, you'll get garbage data.

#pragma rodata-name(push, "BANK0")

// bitpacked coords (0xXY), nt, direction (or other info) + type
// 0xff terminates

const unsigned char const level_enemies_test_enemy[]={
	0x7B, 2, 5,
	0x8B, 2, 5,
	0x81, 2, 3,
	0x8E, 1, 3,
	0x4D, 1, 3,
	0x20, 2, 3,
	0x10, 1, 4,
	0x1E, 0, 4,
	0x50, 2, 7,
	0xFF
};

const unsigned char const level_enemies_test[] = {
	76,90,71,0,0,2,208,0,0,0,100,108,214,9,92,1,
	0,1,2,3,4,7,3,7,6,44,3,1,3,131,3,3,
	26,14,1,12,8,44,1,13,8,43,1,27,24,1,31,40,
	1,30,40,1,15,8,43,3,7,1,22,40,20,13,3,1,
	21,1,28,72,1,30,8,1,13,8,15,15,15,1,15,24,
	2,76,2,200,1,6,22,1,22,24,1,29,104,1,13,8,
	4,1,4,244,1,29,56,20,1,61,40,1,70,152,4,5,
	3,7,2,147,
};

#pragma rodata-name(pop)

