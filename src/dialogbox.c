#include "dialogbox.h"

#include "lib/nesdoug.h"
#include "lib/neslib.h"

#include "asm/macros.h"

#include "constants.h"

DialogBoxData const * active_dboxdata;

// Box control variables.
unsigned char dbox_bg_part; // 0 if none have been drawn, 1 if the first 4 calls to buffer_4_mt were made, etc.

unsigned char dbox_status;
#define DBOX_STATUS_DRAWING_BOX 0
#define DBOX_STATUS_ERASING_BOX 1
#define DBOX_STATUS_DRAWING_TEXT 2
#define DBOX_STATUS_AWAITING_BUTTON 3

// Text control variables.
unsigned char dbox_x;
unsigned char dbox_y;

// === Extern'd zero page symbols, defined in zeropage.h.
extern unsigned char temp0;
#pragma zpsym("temp0") // Remember this pragma.
// It tells the compiler to treat this as a zero page symbol instead of an absolute symbol

extern unsigned char temp1;
#pragma zpsym("temp1")

extern unsigned char pad1;
#pragma zpsym("pad1")

extern unsigned char pad1_new;
#pragma zpsym("pad1_new")

extern int address;
#pragma zpsym("address")

extern unsigned char index;
#pragma zpsym("index")

extern const unsigned char * temppointer;
#pragma zpsym("temppointer")

extern const unsigned char * temppointer1;
#pragma zpsym("temppointer1")

extern unsigned char y;
#pragma zpsym("y")

extern unsigned int scroll_y;
#pragma zpsym("scroll_y")

extern unsigned int pseudo_scroll_y;
#pragma zpsym("pseudo_scroll_y")

extern unsigned char nt;
#pragma zpsym("nt")

extern unsigned char scroll_count;
#pragma zpsym("scroll_count")

//Adding x-scrolling would probably involve making a pseudo_scroll_x for this sort of thing.

extern unsigned char coordinates;
#pragma zpsym("coordinates")

extern unsigned char game_mode;
#pragma zpsym("game_mode")


extern unsigned char debug_tile_x;
#pragma zpsym("debug_tile_x")

extern unsigned char debug_tile_y;
#pragma zpsym("debug_tile_y")

extern const unsigned char * const cmaps[];

extern void draw_screen_sub(void);

#define EMPTY 44
#define BOTTM 49

const unsigned char const dbox_tiles[] = {
	EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,
	EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,
	EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,
	BOTTM,BOTTM,BOTTM,BOTTM,BOTTM,BOTTM,BOTTM,BOTTM,BOTTM,BOTTM,BOTTM,BOTTM,BOTTM,BOTTM,BOTTM,BOTTM,
};

// The top 3 mt rows of the screen should become EMPTY_TILE, and the 4th should be DIALOG_BOX_BOTTOM.

const unsigned char const dbox_x_offset_lookup0[] = {0, 0x20, 0x40, 0x60};
const unsigned char const dbox_x_offset_lookup1[] = {0x80, 0xA0, 0xC0, 0xE0};

// Line number N has its y value offset by dbox_y_offset_lookup[N].
// N = dbox_bg_part, probably.
const unsigned char const dbox_y_offset_lookup[] = { 0x00, 0x00, 0x20, 0x20 };

const unsigned char const dbox_index_offset_lookup0[] = {0, 2, 4, 6};
const unsigned char const dbox_index_offset_lookup1[] = {8, 10, 12, 14};

const void (* dbox_functions[])(void) = {
	dbox_draw_box,
	dbox_erase_box,
	dbox_draw_text,
	dbox_await_input,
};

// Activate a dialog box, telling the game that one is rendering.
void trigger_dialog_box(DialogBoxData const * dboxdata) {
	active_dboxdata = dboxdata;
	game_mode = MODE_GAME_SHOWING_TEXT;
	dbox_status = DBOX_STATUS_DRAWING_BOX;
	dbox_x = 0;
	dbox_y = 0;
}

void dialog_box_handler(void) {
	// Call only one of the subfunctions per frame.
	AsmCallFunctionAtPtrOffsetByIndexVar(dbox_functions, dbox_status);
}

void dbox_draw_box(void) {
	set_data_pointer(dbox_tiles);

	add_scroll_y(pseudo_scroll_y, dbox_y, scroll_y);


	draw_screen_sub();

	if (scroll_count == 0) {
		dbox_y += 0x20;
	}
	if (dbox_y > 0x20) {
		dbox_status = DBOX_STATUS_DRAWING_TEXT;
		dbox_y = 0;
	}

}

// Draw the dialog.
void dbox_draw_text(void) {
	// ...
	dbox_status = DBOX_STATUS_AWAITING_BUTTON;
}

void dbox_await_input(void) {
	if (pad1 & PAD_DOWN) {
		// Advance the status of the dialog box.
		// If there's still text to be drawn, clear it and then draw the next text.
		dbox_status = DBOX_STATUS_ERASING_BOX;
	}
}

void dbox_erase_box(void) {
	//pseudo_scroll_y = sub_scroll_y(dbox_y, scroll_y);
	add_scroll_y(pseudo_scroll_y, dbox_y, scroll_y);


	temp0 = high_byte(pseudo_scroll_y);
	AsmSet2ByteFromPtrAtIndexVar(temppointer, cmaps, temp0);
	//temppointer = cmaps[temp0];
	set_data_pointer(temppointer);

	draw_screen_sub();

	if (scroll_count == 0) {
		dbox_y += 0x20;
	}

	if (dbox_y > 0x20) {
		game_mode = MODE_GAME;
	}

}
