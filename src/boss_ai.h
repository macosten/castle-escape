#ifndef bossai_h
#define bossai_h

// Boss AI subfunctions are here.

void boss_ai_idle(void);

void boss_ai_ascending(void);

void boss_ai_descending(void);

void boss_ai_damaged(void);

void collision_with_boss(void);

#define BOSS_STATE_INTRO 0
#define BOSS_STATE_IDLE 1
#define BOSS_STATE_ASCENDING 2
#define BOSS_STATE_DESCENDING 3
#define BOSS_STATE_DAMAGED 4

#endif