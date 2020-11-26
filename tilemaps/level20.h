// Do not try to use pointers from another bank while that bank isn't 
// swapped in the correct place in RAM.
// Otherwise, you'll get garbage data.

#pragma rodata-name(push, "BANK0")

// bitpacked coords (0xXY), nt, direction (or other info) + type
// 0xff terminates

const unsigned char const level20_enemy[]={
	0xCA, 0, 8, // Boss (Wizard)
	0xFF
};

const unsigned char const level20[] = {
	76,90,71,0,0,0,240,0,0,0,69,230,17,6,177,1,
	0,1,2,3,44,44,3,27,3,1,12,9,3,139,44,8,
	7,13,13,13,7,3,131,6,2,72,14,1,8,35,2,131,
	1,27,8,3,131,28,29,3,69,1,5,8,30,31,3,69,
	1,28,56,1,27,8,2,131,8,5,3,7,1,3,152,8,
	4,3,7,2,8,
};

#pragma rodata-name(pop)

