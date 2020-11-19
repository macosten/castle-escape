// Do not try to use pointers from another bank while that bank isn't 
// swapped in the correct place in RAM.
// Otherwise, you'll get garbage data.

#pragma rodata-name(push, "BANK0")

// bitpacked coords (0xXY), nt, direction (or other info) + type
// 0x10 + type for right-facing, just type for left-facing
// 0xff terminates

const unsigned char const level_debug_tiledump_enemy[]={
	0xFF
};

const unsigned char const level_debug_tiledump[] = {
	76,90,71,0,0,0,240,0,0,0,99,162,254,16,53,1,
	52,53,54,55,4,7,55,7,4,44,55,1,6,0,1,2,
	3,4,5,6,7,8,9,14,54,136,10,11,12,13,14,15,
	16,17,18,19,54,200,20,21,22,23,24,25,26,27,28,29,
	54,200,30,31,32,33,34,35,36,37,38,39,54,200,40,41,
	42,43,44,45,46,47,48,49,54,200,50,51,55,194,55,2,
	54,200,53,6,6,53,30,8,53,27,8,54,8,5,55,7,
	53,3,216,
};

#pragma rodata-name(pop)

