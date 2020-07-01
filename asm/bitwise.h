//
//  neszac.h
//  Castle Escape
//
//  Created by Zaccari Silverman on 6/30/20.
//

/// Object Flags -- for each bit in this block of memory, a 0 implies that the variable-state object should be present, and a 1 implies it was destroyed.
// char objectflags[32];



/// Clear the object bitfield for a new level.
void __fastcall__ clear_object_bitfield(void);

/// Set a bit, somewhere in the 32-byte object bitfield.
void __fastcall__ set_object_bit(unsigned char index); /*{
    objectflags[index >> 3] |= 1 << (index & 0x1f); // only the bottom 5 bits matter here
}*/

// void __fastcall__ clr_bit_array(char index);
/*{
    objectflags[index >> 5] &= ~(1 << (index & 0x1f));
}*/

