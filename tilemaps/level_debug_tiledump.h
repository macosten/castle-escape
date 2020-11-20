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
	76,90,71,0,0,0,240,0,0,0,98,140,93,15,198,1,
	51,52,53,54,4,7,54,7,4,44,54,1,6,0,1,2,
	3,4,5,6,7,8,9,14,53,136,10,11,12,13,14,15,
	16,17,18,19,53,200,20,21,22,23,24,25,26,27,28,29,
	53,200,30,31,32,33,34,35,36,37,38,39,53,200,40,41,
	42,43,44,45,46,47,48,49,53,200,50,54,162,54,3,53,
	200,52,7,7,52,30,8,52,27,8,44,6,5,54,7,52,
	3,216,
};

#pragma rodata-name(pop)

