// Do not try to use pointers from another bank while that bank isn't 
// swapped in the correct place in RAM.
// Otherwise, you'll get garbage data.

#pragma rodata-name(push, "BANK0")

// bitpacked coords (0xXY), nt, direction (or other info) + type
// 0xff terminates

const unsigned char const level14_enemy[]={
	0x7D, 1, 3, // Splyke
	0x28, 1, 3,
	0x74, 1, 3,
	0x54, 1, 3,
	0x20, 1, 6,
	0xFF
};

const unsigned char const level14[] = {
	76,90,71,0,0,1,224,0,0,0,164,111,153,16,116,1,
	1,2,3,15,4,7,15,7,6,44,15,1,15,131,15,4,
	14,2,31,8,2,19,8,23,25,44,47,2,10,8,24,24,
	3,193,2,5,8,4,13,21,3,1,12,13,5,9,2,8,
	56,48,44,14,42,8,6,2,10,72,3,8,10,2,6,40,
	15,1,5,6,42,3,5,2,8,24,8,2,13,8,11,10,
	42,11,2,2,79,2,21,152,12,3,58,15,1,2,39,56,
	14,42,42,8,4,2,39,72,20,13,10,42,42,11,10,2,
	21,200,2,3,120,15,2,2,7,88,40,15,1,2,10,88,
	40,32,33,40,44,20,9,42,2,2,139,3,136,34,35,3,
	24,14,2,14,8,0,0,3,136,4,2,2,130,15,1,15,
	225,2,3,99,
};

#pragma rodata-name(pop)

