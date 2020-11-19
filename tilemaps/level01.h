// Do not try to use pointers from another bank while that bank isn't 
// swapped in the correct place in RAM.
// Otherwise, you'll get garbage data.

#pragma rodata-name(push, "BANK0")

// bitpacked coords (0xXY), nt, direction (or other info) + type
// 0xff terminates

const unsigned char const level01_enemy[]={
	0x61, 2, 2, // ENEMY_GRARRL
	0x75, 1, 2, // ENEMY_GRARRL
	0x60, 1, 2, // ENEMY_GRARRL
	0xFF
};

const unsigned char const level01[] = {
	76,90,71,0,0,2,208,0,0,0,255,190,149,34,164,1,
	0,2,11,15,4,7,15,7,6,44,15,1,15,131,15,4,
	14,2,30,8,2,11,8,42,15,2,2,9,8,23,25,2,
	12,8,24,24,2,11,8,4,13,15,1,21,2,9,24,1,
	15,2,44,44,38,36,39,2,10,88,3,20,13,13,11,131,
	2,3,88,15,225,11,29,11,216,42,28,29,15,65,2,9,
	120,30,31,15,65,2,12,136,44,11,54,2,6,56,11,36,
	11,231,2,5,24,11,36,11,231,2,6,136,21,44,20,2,
	12,136,44,15,65,2,8,8,11,131,11,155,2,7,8,38,
	39,2,12,24,2,11,56,11,232,2,23,8,4,2,6,246,
	2,61,104,2,11,8,2,3,205,2,9,8,2,3,205,2,
	5,8,40,22,40,2,3,248,5,2,37,248,37,11,246,44,
	8,11,200,38,36,2,4,182,2,5,8,4,5,15,2,9,
	2,8,8,15,4,21,2,12,8,10,2,12,24,11,7,40,
	2,9,8,11,86,2,7,24,7,7,11,150,44,12,4,2,
	75,120,12,4,2,6,8,40,32,33,40,11,71,2,5,8,
	38,39,34,35,11,135,11,200,2,4,136,11,71,11,200,
};

#pragma rodata-name(pop)

