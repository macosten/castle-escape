// Assorted functions, implemented in assembly, 
// that I don't really want to import the entire libraries for.


// Stuff like strlen doesn't really need to be fast as it won't be used in time-critical areas.
unsigned char __fastcall__ strlen(const char * str);