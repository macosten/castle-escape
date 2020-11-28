//
//	macros.h
//	Castle Escape
//

// This file will contain C macros that are meant to be inline asm.
// Note that __asm__ is a cc65 extension to C.

// This ensures that a 1-byte copy from an array to a char variable takes only 3 instructions.
// This can avoid scenarios in which cc65 will try to put something into (ptr) unnecessarily.
#define AsmSet1ByteFromPtrAtIndexVar(variable, pointer, indexVariable) { \
	__asm__("ldy %v", indexVariable); \
	__asm__("lda %v, %s", pointer, Y); \
	__asm__("sta %v", variable); \
}
// When used, something like
// temp0 = array[i];
// becomes:
// AsmSet1ByteFromPtrAtIndexVar(temp0, array, i);

#define AsmSet1ByteFromZpPtrAtIndexVar(variable, zppointer, indexVariable) { \
	__asm__("ldy %v", indexVariable); \
	__asm__("lda (%v), %s", zppointer, Y); \
	__asm__("sta %v", variable); \
}

// When used, something like
// array[i] = temp0;
// becomes:
// AsmSet1ByteAtPtrWithOffset(array, i, temp0);

#define AsmSet1ByteAtPtrWithOffset(pointer, offset, variable) {\
	__asm__("ldy %v", offset);\
	__asm__("lda %v", variable);\
	__asm__("sta %v,%s", pointer, Y);\
}

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

#define AsmSet2ByteAtPtrWithOffset(pointer, offset, variable) {\
	__asm__("lda %v", offset);\
	__asm__("asl");\
	__asm__("tay");\
	__asm__("lda %v", variable);\
	__asm__("sta %v, %s", pointer, y);\
	__asm__("lda %v+1", variable);\
	__asm__("sta %v+1,%s", pointer, y);\
}

// Something like this that supports cases of indexVariable > 127 will be more complicated.

// Since I'm using a lot of lookup tables of function pointers: 
// This is a macro that loads a function pointer into ax and then calls it.
// (callax is provided by the cc65 runtime)
#define AsmCallFunctionAtPtrOffsetByIndexVar(pointer, indexVariable) { \
	__asm__("lda %v", indexVariable); \
	__asm__("asl"); \
	__asm__("tay"); \
	__asm__("lda %v, %s", pointer, y); \
	__asm__("ldx %v+1, %s", pointer, y); \
	__asm__("jsr callax"); \
}