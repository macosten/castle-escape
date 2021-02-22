#ifndef dialogbox_h
#define dialogbox_h

#include "structs.h"

// Activate a dialog box, telling the game that one is rendering.
void trigger_dialog_box(void);

// Called in the main game loop if a dialog box is active.
void dbox_handler(void);

// Actually draw the dialog box.
void dbox_draw_box(void);

// Draw the text.
void dbox_draw_text(void);

// Wait for the player to press the A button.
void dbox_await_input(void);

// "Erase" the dialog box (actually, just redraw the map in the area the dialog box used to be in).
void dbox_erase_box(void);

// Clear the text displayed in the box.
void dbox_erase_text(void);

int dbox_erase_text_addr_calc_sub(void);

#endif