#ifndef dialogbox_h
#define dialogbox_h

typedef struct dialogboxdata_t {
    // On a conceptual level, for each i, portraitSprites[i] and strings[i] are a pair.

    unsigned char ** portraitSprites; // One pointer to a metasprite for each dialog box. This will be shown alongside strings[i].
    unsigned char ** strings; // One string for each dialog box.
    unsigned char count; // The number of boxes full of dialog this data object holds.
    // 
} DialogBoxData;

DialogBoxData const * active_dboxdata;

// Activate a dialog box, telling the game that one is rendering.
void trigger_dialog_box(DialogBoxData const * dboxdata);

// Draw the dialog.
void draw_dialog_text(void);

#endif