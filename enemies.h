#ifndef enemies_h
#define enemies_h

// Also the equivalent of TURN_OFF.
// An enemy will not be rendered in this case.
#define ENEMY_NONE 0;

#define ENEMY_KORBAT 1;
#define ENEMY_GRARRL 2;
#define ENEMY_SPLYKE 3;
#define ENEMY_CANNON 4;
#define ENEMY_ACIDPOOL 5;
#define ENEMY_SPIKEBALL 6;
#define ENEMY_SUN 7;
#define ENEMY_BOSS 8;
// These last two are going to be 8 by 8 
// (probably; I'm still not sure about the cannonball, since the original game would have an equivalent of a 16x16 ball)
#define ENEMY_CANNONBALL 9;
#define ENEMY_ACIDDROP 10;

// Do not to go over 16 types; 
// I do some bitpacking that requires that "type" fit in a nibble.

#define ENEMY_WIDTH 13;
#define ENEMY_HEIGHT 13;

#define PROJECTILE_WIDTH 6;
#define PROJECTILE_HEIGHT 6;

#endif