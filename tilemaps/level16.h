// Do not try to use pointers from another bank while that bank isn't 
// swapped in the correct place in RAM.
// Otherwise, you'll get garbage data.

#pragma rodata-name(push, "BANK0")

// bitpacked coords (0xXY), nt, direction (or other info) + type
// 0xff terminates

const unsigned char const level16_enemy[]={
	0x27, 0, 4, // Cannon
	0x97, 0, 4,
	0xFF
};

const unsigned char const level16[] = {
	76,90,71,0,0,0,240,0,0,0,110,228,84,8,178,1,
	1,2,3,10,4,7,10,7,6,44,10,1,10,131,10,4,
	14,2,5,8,28,29,2,10,8,38,36,10,1,39,3,2,
	41,3,136,4,5,13,10,1,9,23,25,44,12,3,179,4,
	6,22,3,99,24,24,44,8,2,5,8,10,162,11,13,21,
	2,11,8,10,2,2,10,8,40,10,33,2,23,24,32,33,
	40,3,91,2,7,8,34,35,2,12,40,3,8,3,7,2,
	7,8,0,10,5,2,5,8,4,5,10,6,3,136,
};

#pragma rodata-name(pop)

