#ifndef bossai_h
#define bossai_h

// Boss AI subfunctions are here.

void boss_ai_idle(void);
void boss_ai_ascending(void);
void boss_ai_descending(void);
void boss_ai_damaged(void);
void boss_ai_dying(void);

void collision_with_boss(void);

void draw_boss_flying(void);
void draw_boss_idle(void);
void draw_boss_dying(void);

#endif