//
//	math.h
//	Castle Escape
//

unsigned char __fastcall__ atan2(unsigned char x1, unsigned char x2, unsigned char y1, unsigned char y2);

// Bear in mind that "theta" here is in brad (binary radians)
// 1 brad = (360/256) deg = ~1.4 deg
unsigned char __fastcall__ sin_lookup(unsigned char theta);
unsigned char __fastcall__ cos_lookup(unsigned char theta);

// Might be better to make this into a regular C array.
// For x delta values, cos_lookup(this). For y delta values, sin_lookup(this).
unsigned char __fastcall__ brads_lookup(unsigned char bitpacked_dydx);

unsigned char __fastcall__ abs_subtract(unsigned char a, unsigned char b);