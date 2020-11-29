// Do not try to use pointers from another bank while that bank isn't 
// swapped in the correct place in RAM.
// Otherwise, you'll get garbage data.

#pragma rodata-name(push, "BANK0")

// bitpacked coords (0xXY), nt, direction (or other info) + type
// 0xff terminates

const unsigned char const level_question_block_test[] = {
	76,90,71,0,0,0,240,0,0,0,74,20,95,8,38,1,
	0,1,2,3,4,7,3,11,4,6,40,3,11,8,6,22,
	3,3,1,11,8,3,225,22,3,33,2,200,1,14,40,3,
	195,1,6,23,1,22,24,2,153,1,2,80,1,16,24,2,
	78,2,248,1,17,24,26,1,5,24,1,9,56,1,7,147,
	1,18,104,24,8,4,5,3,11,4,
};

#pragma rodata-name(pop)

