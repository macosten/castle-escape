//
//	macros.h
//	Castle Escape
//

// This file will contain C macros that are meant to be inline asm.
// Note that __asm__ is a cc65 extension to C.

// This ensures that a 1-byte copy from an array to a char variable takes only 3 instructions.
// This can avoid scenarios in which cc65 will try to put something into (ptr) unnecessarily.
#define AsmSet1ByteFromPtrAtIndexVar(variable, pointer, indexVariable) \
{ \
	__asm__("ldy %v", indexVariable); \
	__asm__("lda %v, %s", pointer, Y); \
	__asm__("sta %v", variable); \
}
// When used, something like
// temp0 = array[i];
// becomes:
// AsmSetCharFromPtrAtIndexVar(temp0, array, i);

// This ensures that a 2-byte copy from an array to a pointer or int takes only a few instructions.
// This can avoid scenarios in which cc65 will try to save the state of something irrelevant.
// Don't use this if indexVariable > 127.
// 1. asl the index variable to multiply it by 2.
// 2. Load the low byte into `variable`.
// 3. Load the high byte into `variable`.
#define AsmSet2ByteFromPtrAtIndexVar(variable, pointer, indexVariable) { \
	__asm__("lda %v", indexVariable); \
	__asm__("asl"); \
	__asm__("tay"); \
	__asm__("lda %v, %s", pointer, y); \
	__asm__("sta %v", variable); \
	__asm__("lda %v+1, %s", pointer, y); \
	__asm__("sta %v+1", variable); \
}
// Something like this that supports cases of indexVariable > 127 will be more complicated.

