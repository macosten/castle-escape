#include "boss_ai.h"

#include "structs.h"
#include "player_macros.h"
#include "enemy_macros.h"

// Extra memory used by the boss.

extern void trigger_dialog_box(void);

extern unsigned char player_flags;
#pragma zpsym("player_flags")

extern const DialogBoxData const boss_dialog;
extern const DialogBoxData const defeat_dialog;

extern DialogBoxData active_dboxdata;

extern unsigned char boss_state;
#define BOSS_STATE_IDLE 1
#define BOSS_STATE_ASCENDING 2
#define BOSS_STATE_DESCENDING 3

extern unsigned char boss_memory[];

extern unsigned char get_frame_count(void);

void boss_ai_intro(void) {
    SET_DIRECTION_RIGHT();
    active_dboxdata = boss_dialog;
    trigger_dialog_box();
    boss_state = BOSS_STATE_IDLE;
}

void boss_ai_idle(void) {
    // Bug: triggering a second dialog box will have the first value of dbox_current_string set to 0x00/NULL (and thus it won't be correct).
    // Figure out why this is...
    //trigger_dialog_box();
}

void boss_ai_ascending(void) {

}

void boss_ai_descending(void) {

}