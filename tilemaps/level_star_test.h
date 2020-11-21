// Do not try to use pointers from another bank while that bank isn't 
// swapped in the correct place in RAM.
// Otherwise, you'll get garbage data.

#pragma rodata-name(push, "BANK0")

// bitpacked coords (0xXY), nt, direction (or other info) + type
// 0xff terminates

const unsigned char const level_star_test_enemy[]={
	0xFF
};

const unsigned char const level_star_test[] = {
	76,90,71,0,0,2,208,0,0,0,78,0,86,9,21,1,
	0,1,2,3,4,7,3,7,6,44,3,1,6,42,23,25,
	42,3,4,14,2,200,24,24,1,12,8,20,21,1,12,8,
	3,7,1,28,8,1,8,8,2,62,1,29,56,1,8,8,
	1,4,123,1,29,56,1,31,184,1,31,184,1,31,184,1,
	30,184,3,5,26,2,136,4,5,3,7,1,67,184,
};

#pragma rodata-name(pop)

