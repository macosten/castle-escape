#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

// Mark a variable referenced in a header file as being a zeropage symbol.
// Any time you set a variable as a ZEROPAGE_DEF, you will want to also update any header files referencing it
// with this function.
// (Usage; ZEROPAGE_EXTERN(type, variableName); eg ZEROPAGE_EXTERN(int, myInt))
#define ZEROPAGE_EXTERN(defa, defb) extern defa defb; _Pragma("zpsym (\"" STR(defb) "\")")
#define ZEROPAGE_ARRAY_EXTERN(defa, defb, defArr) extern defa defb[defArr]; _Pragma("zpsym (\"" STR(defb) "\")")

// Mark a variable referened in a header file as being SRAM. (SRAM isn't currently supported, but...)
// Technically this just creates a regular extern, and you could avoid using this symbol. It is used only for
// consistency with ZEROPAGE variables, to make the source easier to follow.
#define SRAM_EXTERN(defa, defb) extern defa defb;
#define SRAM_ARRAY_EXTERN(defa, defb, defArr) extern defa defb[defArr];

#define METATILE_IS_SOLID(mtid) (metatile_property_lookup_table[mtid] & METATILE_SOLID)
