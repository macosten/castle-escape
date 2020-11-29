#include "dialogbox.h"

#include "lib/nesdoug.h"
#include "lib/neslib.h"

#include "constants.h"

unsigned char didDraw;
unsigned char lineIndex;
unsigned char charIndex;

// === Extern'd zero page symbols, defined in zeropage.h.
extern unsigned char temp0;
#pragma zpsym("temp0") // Remember this pragma.
// It tells the compiler to treat this as a zero page symbol instead of an absolute symbol

extern unsigned int scroll_y;
#pragma zpsym("scroll_y")

extern unsigned char nt;
#pragma zpsym("nt")

//Adding x-scrolling would probably involve making a pseudo_scroll_x for this sort of thing.

extern unsigned char coordinates;
#pragma zpsym("coordinates")

extern unsigned char game_mode;
#pragma zpsym("game_mode")

#define EMPTY 44
#define BOTTM 49

const unsigned char const dbox_tiles[] = {
	EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,
	EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,
	EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,
	BOTTM,BOTTM,BOTTM,BOTTM,BOTTM,BOTTM,BOTTM,BOTTM,BOTTM,BOTTM,BOTTM,BOTTM,BOTTM,BOTTM,BOTTM,BOTTM,
};

// Activate a dialog box, telling the game that one is rendering.
void trigger_dialog_box(DialogBoxData const * dboxdata) {
	active_dboxdata = dboxdata;
	game_mode = MODE_GAME_SHOWING_TEXT;
}

// The top 3 mt rows of the screen should become EMPTY_TILE, and the 4th should be DIALOG_BOX_BOTTOM.

void draw_box(void) {
	temp0 = high_byte(scroll_y);
	nt = (temp0 & 1) << 1; // 0 or 2 for vertical scrolling

	set_data_pointer(dbox_tiles);
	// ...

}

// Draw the dialog.
void draw_dialog_text(void) {

}
