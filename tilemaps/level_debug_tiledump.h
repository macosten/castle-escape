// Do not try to use pointers from another bank while that bank isn't 
// swapped in the correct place in RAM.
// Otherwise, you'll get garbage data.

#pragma rodata-name(push, "BANK0")

// bitpacked coords (0xXY), nt, direction (or other info) + type
// 0x10 + type for right-facing, just type for left-facing
// 0xff terminates

const unsigned char const level_debug_tiledump_enemy[]={ };

const unsigned char const level_debug_tiledump[] = {
	76,90,71,0,0,0,51,0,0,0,51,91,181,5,47,0,
	1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,
	17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,
	33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,
	49,50,51,
};

#pragma rodata-name(pop)

