#include "dialogbox.h"

#include "lib/nesdoug.h"
#include "lib/neslib.h"

#include "asm/macros.h"

#include "constants.h"

DialogBoxData active_dboxdata;

// Box control variables.

unsigned char dbox_status;
#define DBOX_STATUS_DRAWING_BOX 0
#define DBOX_STATUS_ERASING_BOX 1
#define DBOX_STATUS_DRAWING_TEXT 2
#define DBOX_STATUS_AWAITING_BUTTON 3
#define DBOX_STATUS_ERASING_TEXT 4

// Text control variables.
unsigned char dbox_x;
unsigned char dbox_y;
unsigned char dbox_string_index;
unsigned char dbox_char_index;
unsigned char dbox_erase_text_frame;
const char * dbox_current_string;

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

extern unsigned char game_mode;
#pragma zpsym("game_mode")

extern unsigned char debug_tile_x;
#pragma zpsym("debug_tile_x")

extern unsigned char debug_tile_y;
#pragma zpsym("debug_tile_y")

extern const unsigned char * const cmaps[];

// This reuses the draw_screen_sub code defined elsewhere (and used for drawing the screen).
extern void draw_screen_sub(void);
extern void set_prg_bank(unsigned char bank);

#define EMPTY 44
#define BOTTM 49

#define TEXT_START_X 2
#define TEXT_START_Y 3
#define TEXT_END_X 30

const unsigned char const dbox_tiles[] = {
	EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,
	EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,
	EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,
	BOTTM,BOTTM,BOTTM,BOTTM,BOTTM,BOTTM,BOTTM,BOTTM,BOTTM,BOTTM,BOTTM,BOTTM,BOTTM,BOTTM,BOTTM,BOTTM,
};

#define CURSOR_BASE_Y 16 * 3
const unsigned char const dbox_downward_cursor_y_offset_table[] = { 
	CURSOR_BASE_Y+0, CURSOR_BASE_Y+1, CURSOR_BASE_Y+2, CURSOR_BASE_Y+1, 
};

const char empty_string[] = "                            "; // 28 spaces

const void (* dbox_functions[])(void) = {
	dbox_draw_box,
	dbox_erase_box,
	dbox_draw_text,
	dbox_await_input,
	dbox_erase_text,
};

// Activate a dialog box, telling the game that one is rendering.
void trigger_dialog_box(DialogBoxData const * dboxdata) {
	active_dboxdata = *dboxdata; // Shallow copy. This means fewer dereferencings, which means smaller code.

	game_mode = MODE_GAME_SHOWING_TEXT;
	dbox_status = DBOX_STATUS_DRAWING_BOX;
	// Set control variables appropriately for the first step.
	dbox_y = 0;
	dbox_string_index = 0;
	dbox_char_index = 0;
	dbox_current_string = active_dboxdata.strings[0];
}

void dialog_box_handler(void) {
	// Call only one of the subfunctions per frame.
	AsmCallFunctionAtPtrOffsetByIndexVar(dbox_functions, dbox_status);
}

void dbox_draw_box(void) {
	set_data_pointer(dbox_tiles);

	add_scroll_y(pseudo_scroll_y, dbox_y, scroll_y);

	// Necessary:
	temp1 = high_byte(pseudo_scroll_y);
	// Yep: we're reusing this code to draw the box.
	draw_screen_sub();

	if (scroll_count == 0) {
		// Draw the next row.
		dbox_y += 0x20;
	}
	if (dbox_y > 0x20) { // ...but there are only two rows.
		dbox_status = DBOX_STATUS_DRAWING_TEXT;
		// Set the control variables appropriately for the next step.
		dbox_x = TEXT_START_X;
		dbox_y = TEXT_START_Y;
	}

}

// Draw the dialog.
void dbox_draw_text(void) {
	// Ensure the correct bank is selected.
	// In the future, dboxdata should hold what bank it's in (?).
	set_prg_bank(0);

	// Strings no more than 255 characters long.
	temp0 = dbox_current_string[dbox_char_index];

	// Increment the character index (since we'll want to do that almost no matter what).
	++dbox_char_index;	

	// Now: what did we actually read?
	if (temp0 == '\0') {
		// A null terminator? Start awaiting button input.
		dbox_status = DBOX_STATUS_AWAITING_BUTTON;
		dbox_y = 0;
	} else if (temp0 == '\n') {
		// A newline? Act like it.
		dbox_x = TEXT_START_X;
		++dbox_y;
	} else {
		// Write temp0.
		// Figure out the correct nt addr.
		if (nt == 0) {
			address = NTADR_A(dbox_x, dbox_y);
		} else {
			address = NTADR_C(dbox_x, dbox_y);
		}
		// Write the character.
		one_vram_buffer(temp0, address);

		// Advance to the next char.	
		++dbox_x;
		if (dbox_x >= TEXT_END_X) {
			dbox_x = TEXT_START_X;
			++dbox_y;
		}
	}
	
}

void dbox_await_input(void) {
	// Ensure the correct bank is selected.
	// In the future, dboxdata should hold what bank it's in (?).
	set_prg_bank(0);

	if (pad1_new) {
		// Advance the status of the dialog box.
		// If there's still text to be drawn, clear it and then draw the next text.
		
		++dbox_string_index;
		if (dbox_string_index == active_dboxdata.count) { 
			dbox_status = DBOX_STATUS_ERASING_BOX;
			// Set control variables.
			dbox_y = 0;
		} else {
			dbox_current_string = active_dboxdata.strings[dbox_string_index];

			dbox_status = DBOX_STATUS_ERASING_TEXT;
			// Set control variables.
			dbox_char_index = 0;
			dbox_x = TEXT_START_X;
			dbox_y = TEXT_START_Y;
			dbox_erase_text_frame = 0;
		}
	

	} else {
		// Show the downward-facing cursor.
		temp1 = dbox_y >> 3;
		temp0 = dbox_downward_cursor_y_offset_table[temp1];
		oam_spr(232, temp0, 0x11, 0);
		++dbox_y;
		dbox_y &= 0b11111;
	}
}

const unsigned char const dbox_erase_text_x_values[] = {
	TEXT_START_X, 
	TEXT_START_X, 
	TEXT_START_X, // 3rd row only gets half-drawn (cleared).
	TEXT_START_X,
};

const unsigned char const dbox_erase_text_y_values[] = {
	TEXT_START_Y,
	TEXT_START_Y + 1,
	TEXT_START_Y + 2,
	TEXT_START_Y + 3,
};

const unsigned char const dbox_erase_text_lengths[] = { 28, 28 };

int dbox_erase_text_addr_calc_sub(void) {
	if (nt == 0) {
		return NTADR_A(dbox_x, dbox_y);
	} else {
		return NTADR_C(dbox_x, dbox_y);
	}
}

void dbox_erase_text(void) {

	for (temp0 = 0; temp0 < 2; ++temp0) {
		dbox_x = dbox_erase_text_x_values[dbox_erase_text_frame];
		dbox_y = dbox_erase_text_y_values[dbox_erase_text_frame];
		address = dbox_erase_text_addr_calc_sub();
		multi_vram_buffer_horz(empty_string, dbox_erase_text_lengths[temp0], address);
		++dbox_erase_text_frame;
	}

	if (dbox_erase_text_frame == 4) {
		dbox_status = DBOX_STATUS_DRAWING_TEXT;
		dbox_x = TEXT_START_X;
		dbox_y = TEXT_START_Y;
	}
	
}

void dbox_erase_box(void) {
	//pseudo_scroll_y = sub_scroll_y(dbox_y, scroll_y);
	add_scroll_y(pseudo_scroll_y, dbox_y, scroll_y);

	temp1 = high_byte(pseudo_scroll_y);
	AsmSet2ByteFromPtrAtIndexVar(temppointer, cmaps, temp1);
	set_data_pointer(temppointer);

	draw_screen_sub();

	if (scroll_count == 0) {
		dbox_y += 0x20;
	}

	if (dbox_y > 0x20) {
		game_mode = MODE_GAME;
	}

}
