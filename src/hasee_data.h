// Do not try to use pointers from another bank while that bank isn't 
// swapped in the correct place in RAM.
// Otherwise, you'll get garbage data.

#pragma rodata-name(push, "BANK1")

const char * const hasee_yay_phrases[] = {
    " Yay!!!!! ",
    "Yippee!!!!",
    "OMG PLS!!!",
};

#define HASEE_YAY_PHRASE_LENGTH 10

const char * const hasee_treat_names[] = {
    "Yellow",
    "Blue",
    "Green",
    "Silver",
    "Golden",
    "Checkered", // Longest
    "Sponge",
    "Fiery",
    "Icy",
    "Rainbow",
    "Fish",
    "Grun",
};

const char * const hasee_treat_postfix = "Doughnutfruit!";
#define HASEE_TREAT_NAME_POSTFIX_LEN (15-1)
#define HASEE_LONGEST_COMPLETE_PHRASE_LEN 24

// Remember all of these should have spaces (i.e print out the \0) after them except "Grun"
const unsigned char const hasee_treat_name_lens[] = {
    8-1,
    6-1,
    7-1,
    8-1,
    8-1,
    11-1,
    8-1,
    7-1,
    5-1,
    9-1,
    6-1,
    5-1,
};

#define HASEE_LONGEST_TREAT_NAME_LEN (11-1)

const char * doughnutfruit_string = "Doughnutfruit!";

const char * const maccy_confusion_quote = "Huh?!?! What's that?!?!";
const char * const bleh_gross_quote = "Bleh!! Gross!!! Yuck!!!";

const char * const hasee_letter_bonus_quote = "    +2 Seconds!!!      ";
const char * const hasee_omg_time_bonus_yay_quote = "Hasee OMG!";
const char * const hasee_time_bonus_quote = "Hasee +20 Second Bonus!";
const char * const hasee_super_time_bonus_quote = "Super +40 Second Bonus!"; // Length of this = Longest Phrase Length
const char * const hasee_time_up_quote = "Time's up!";

const char * const hasee_sad_ending_quote = "Still feeling hungry...";
const char * const hasee_happy_ending_quote = "The Hasees are happy!!!";
const char * const hasee_quit_instructions = "B: Back To Menu";

#define HASEE_QUIT_INSTRUCTIONS_LEN 16

// Specific palettes for special Doughnutfruits
const unsigned char const hasee_subpal_blue[] = { 0x21, 0x0f, 0x0c, 0x19 };
const unsigned char const hasee_subpal_green_grundo[] = { 0x21, 0x0f, 0x29, 0x19 };
const unsigned char const hasee_subpal_silver[] = { 0x21, 0x0f, 0x10, 0x00 };
const unsigned char const hasee_subpal_golden[] = { 0x21, 0x37, 0x27, 0x17 };
const unsigned char const hasee_subpal_checkered[] = { 0x21, 0x0f, 0x30, 0x1f };
const unsigned char const hasee_subpal_sponge[] = { 0x21, 0x27, 0x38, 0x37 };
const unsigned char const hasee_subpal_fiery[] = { 0x21, 0x06, 0x16, 0x27 };
const unsigned char const hasee_subpal_icy[] = { 0x21, 0x0C, 0x3c, 0x2c };
const unsigned char const hasee_subpal_rainbow[] = { 0x21, 0x01, 0x38, 0x06 };
const unsigned char const hasee_subpal_fish[] = { 0x21, 0x0f, 0x2c, 0x3c };
const unsigned char const hasee_subpal_maccy[] = { 0x21, 0x0f, 0x27, 0x30 };
// const unsigned char const hasee_subpal_gross[] = { 0x21, 0x0f, 0x17, 0x29 };

// Metasprites.
const unsigned char const purple_hasee_idle_left[] = {
      0xfe, 0xfd,   0x08,   1,
      6,    0xfd,   0x09,   1,
      0xfe,    5,   0x18,   1,
      6,       5,   0x19,   1,
    128
};

const unsigned char const purple_hasee_idle_right[] = {
         6,  0xfd,  0x08,   1|OAM_FLIP_H,
      0xfe,  0xfd,  0x09,   1|OAM_FLIP_H,
         6,     5,  0x18,   1|OAM_FLIP_H,
      0xfe,     5,  0x19,   1|OAM_FLIP_H,
    128
};

const unsigned char const orange_hasee_idle_left[] = {
      0xfe, 0xfd,   0x28,   1,
      6,    0xfd,   0x29,   1,
      0xfe,    5,   0x38,   1,
      6,       5,   0x39,   1,
    128
};

const unsigned char const orange_hasee_idle_right[] = {
         6,  0xfd,  0x28,   1|OAM_FLIP_H,
      0xfe,  0xfd,  0x29,   1|OAM_FLIP_H,
         6,     5,  0x38,   1|OAM_FLIP_H,
      0xfe,     5,  0x39,   1|OAM_FLIP_H,
    128
};

const unsigned char const purple_hasee_walk0_left[] = {
      0xfe, 0xfd,   0x0A,   1,
      6,    0xfd,   0x0B,   1,
      0xfe,    5,   0x1A,   1,
      6,       5,   0x1B,   1,
    128
};

const unsigned char const purple_hasee_walk0_right[] = {
         6,  0xfd,  0x0A,   1|OAM_FLIP_H,
      0xfe,  0xfd,  0x0B,   1|OAM_FLIP_H,
         6,     5,  0x1A,   1|OAM_FLIP_H,
      0xfe,     5,  0x1B,   1|OAM_FLIP_H,
    128
};

const unsigned char const orange_hasee_walk0_left[] = {
      0xfe, 0xfd,   0x2A,   1,
      6,    0xfd,   0x2B,   1,
      0xfe,    5,   0x3A,   1,
      6,       5,   0x3B,   1,
    128
};

const unsigned char const orange_hasee_walk0_right[] = {
         6,  0xfd,  0x2A,   1|OAM_FLIP_H,
      0xfe,  0xfd,  0x2B,   1|OAM_FLIP_H,
         6,     5,  0x3A,   1|OAM_FLIP_H,
      0xfe,     5,  0x3B,   1|OAM_FLIP_H,
    128
};

const unsigned char const purple_hasee_walk1_left[] = {
      0xfe, 0xfd,   0x0C,   1,
      6,    0xfd,   0x0D,   1,
      0xfe,    5,   0x1C,   1,
      6,       5,   0x1D,   1,
    128
};

const unsigned char const purple_hasee_walk1_right[] = {
         6,  0xfd,  0x0C,   1|OAM_FLIP_H,
      0xfe,  0xfd,  0x0D,   1|OAM_FLIP_H,
         6,     5,  0x1C,   1|OAM_FLIP_H,
      0xfe,     5,  0x1D,   1|OAM_FLIP_H,
    128
};

const unsigned char const orange_hasee_walk1_left[] = {
      0xfe, 0xfd,   0x2C,   1,
      6,    0xfd,   0x2D,   1,
      0xfe,    5,   0x3C,   1,
      6,       5,   0x3D,   1,
    128
};

const unsigned char const orange_hasee_walk1_right[] = {
         6,  0xfd,  0x2C,   1|OAM_FLIP_H,
      0xfe,  0xfd,  0x2D,   1|OAM_FLIP_H,
         6,     5,  0x3C,   1|OAM_FLIP_H,
      0xfe,     5,  0x3D,   1|OAM_FLIP_H,
    128
};

const unsigned char const purple_hasee_squat_left[] = {
      0xfe, 0xfd,   0x0E,   1,
      6,    0xfd,   0x0F,   1,
      0xfe,    5,   0x1E,   1,
      6,       5,   0x1F,   1,
    128
};

const unsigned char const purple_hasee_squat_right[] = {
         6,  0xfd,  0x0E,   1|OAM_FLIP_H,
      0xfe,  0xfd,  0x0F,   1|OAM_FLIP_H,
         6,     5,  0x1E,   1|OAM_FLIP_H,
      0xfe,     5,  0x1F,   1|OAM_FLIP_H,
    128
};

const unsigned char const orange_hasee_squat_left[] = {
      0xfe, 0xfd,   0x2E,   1,
      6,    0xfd,   0x2F,   1,
      0xfe,    5,   0x3E,   1,
      6,       5,   0x3F,   1,
    128
};

const unsigned char const orange_hasee_squat_right[] = {
         6,  0xfd,  0x2E,   1|OAM_FLIP_H,
      0xfe,  0xfd,  0x2F,   1|OAM_FLIP_H,
         6,     5,  0x3E,   1|OAM_FLIP_H,
      0xfe,     5,  0x3F,   1|OAM_FLIP_H,
    128
};

const unsigned char const purple_hasee_idle_left_sick[] = {
      0xfe, 0xfd,   0x68,   1,
      6,    0xfd,   0x69,   1,
      0xfe,    5,   0x78,   1,
      6,       5,   0x79,   1,
    128
};

const unsigned char const purple_hasee_idle_right_sick[] = {
         6,  0xfd,  0x68,   1|OAM_FLIP_H,
      0xfe,  0xfd,  0x69,   1|OAM_FLIP_H,
         6,     5,  0x78,   1|OAM_FLIP_H,
      0xfe,     5,  0x79,   1|OAM_FLIP_H,
    128
};

const unsigned char const orange_hasee_idle_left_sick[] = {
      0xfe, 0xfd,   0x88,   1,
      6,    0xfd,   0x89,   1,
      0xfe,    5,   0x98,   1,
      6,       5,   0x99,   1,
    128
};

const unsigned char const orange_hasee_idle_right_sick[] = {
         6,  0xfd,  0x88,   1|OAM_FLIP_H,
      0xfe,  0xfd,  0x89,   1|OAM_FLIP_H,
         6,     5,  0x98,   1|OAM_FLIP_H,
      0xfe,     5,  0x99,   1|OAM_FLIP_H,
    128
};

const unsigned char const purple_hasee_walk0_left_sick[] = {
      0xfe, 0xfd,   0x6A,   1,
      6,    0xfd,   0x6B,   1,
      0xfe,    5,   0x7A,   1,
      6,       5,   0x7B,   1,
    128
};

const unsigned char const purple_hasee_walk0_right_sick[] = {
         6,  0xfd,  0x6A,   1|OAM_FLIP_H,
      0xfe,  0xfd,  0x6B,   1|OAM_FLIP_H,
         6,     5,  0x7A,   1|OAM_FLIP_H,
      0xfe,     5,  0x7B,   1|OAM_FLIP_H,
    128
};

const unsigned char const orange_hasee_walk0_left_sick[] = {
      0xfe, 0xfd,   0x8A,   1,
      6,    0xfd,   0x8B,   1,
      0xfe,    5,   0x9A,   1,
      6,       5,   0x9B,   1,
    128
};

const unsigned char const orange_hasee_walk0_right_sick[] = {
         6,  0xfd,  0x8A,   1|OAM_FLIP_H,
      0xfe,  0xfd,  0x8B,   1|OAM_FLIP_H,
         6,     5,  0x9A,   1|OAM_FLIP_H,
      0xfe,     5,  0x9B,   1|OAM_FLIP_H,
    128
};

const unsigned char const purple_hasee_walk1_left_sick[] = {
      0xfe, 0xfd,   0x6C,   1,
      6,    0xfd,   0x6D,   1,
      0xfe,    5,   0x7C,   1,
      6,       5,   0x7D,   1,
    128
};

const unsigned char const purple_hasee_walk1_right_sick[] = {
         6,  0xfd,  0x6C,   1|OAM_FLIP_H,
      0xfe,  0xfd,  0x6D,   1|OAM_FLIP_H,
         6,     5,  0x7C,   1|OAM_FLIP_H,
      0xfe,     5,  0x7D,   1|OAM_FLIP_H,
    128
};

const unsigned char const orange_hasee_walk1_left_sick[] = {
      0xfe, 0xfd,   0x8C,   1,
      6,    0xfd,   0x8D,   1,
      0xfe,    5,   0x9C,   1,
      6,       5,   0x9D,   1,
    128
};

const unsigned char const orange_hasee_walk1_right_sick[] = {
         6,  0xfd,  0x8C,   1|OAM_FLIP_H,
      0xfe,  0xfd,  0x8D,   1|OAM_FLIP_H,
         6,     5,  0x9C,   1|OAM_FLIP_H,
      0xfe,     5,  0x9D,   1|OAM_FLIP_H,
    128
};

const unsigned char const purple_hasee_squat_left_sick[] = {
      0xfe, 0xfd,   0x6E,   1,
      6,    0xfd,   0x6F,   1,
      0xfe,    5,   0x7E,   1,
      6,       5,   0x7F,   1,
    128
};

const unsigned char const purple_hasee_squat_right_sick[] = {
         6,  0xfd,  0x6E,   1|OAM_FLIP_H,
      0xfe,  0xfd,  0x6F,   1|OAM_FLIP_H,
         6,     5,  0x7E,   1|OAM_FLIP_H,
      0xfe,     5,  0x7F,   1|OAM_FLIP_H,
    128
};

const unsigned char const orange_hasee_squat_left_sick[] = {
      0xfe, 0xfd,   0x8E,   1,
      6,    0xfd,   0x8F,   1,
      0xfe,    5,   0x9E,   1,
      6,       5,   0x9F,   1,
    128
};

const unsigned char const orange_hasee_squat_right_sick[] = {
         6,  0xfd,  0x8E,   1|OAM_FLIP_H,
      0xfe,  0xfd,  0x8F,   1|OAM_FLIP_H,
         6,     5,  0x9E,   1|OAM_FLIP_H,
      0xfe,     5,  0x9F,   1|OAM_FLIP_H,
    128
};

const unsigned char * const purple_hasee_idle_animation[] = {
    purple_hasee_idle_left, purple_hasee_idle_right,
};

const unsigned char * const purple_hasee_idle_sick_animation[] = {
    purple_hasee_idle_left_sick, purple_hasee_idle_right_sick,
};

const unsigned char * const orange_hasee_idle_animation[] = {
    orange_hasee_idle_left, orange_hasee_idle_right,
};

const unsigned char * const orange_hasee_idle_sick_animation[] = {
    orange_hasee_idle_left_sick, orange_hasee_idle_right_sick,
};

const unsigned char * const purple_hasee_walk_animation[] = {
    purple_hasee_idle_left, purple_hasee_idle_right,
    purple_hasee_walk1_left, purple_hasee_walk1_right,
    purple_hasee_idle_left, purple_hasee_idle_right,
    purple_hasee_walk0_left, purple_hasee_walk0_right,
};

const unsigned char * const purple_hasee_walk_sick_animation[] = {
    purple_hasee_idle_left_sick, purple_hasee_idle_right_sick,
    purple_hasee_walk1_left_sick, purple_hasee_walk1_right_sick,
    purple_hasee_idle_left_sick, purple_hasee_idle_right_sick,
    purple_hasee_walk0_left_sick, purple_hasee_walk0_right_sick,
};

const unsigned char * const orange_hasee_walk_animation[] = {
    orange_hasee_idle_left, orange_hasee_idle_right,
    orange_hasee_walk1_left, orange_hasee_walk1_right,
    orange_hasee_idle_left, orange_hasee_idle_right,
    orange_hasee_walk0_left, orange_hasee_walk0_right,
};

const unsigned char * const orange_hasee_walk_sick_animation[] = {
    orange_hasee_idle_left_sick, orange_hasee_idle_right_sick,
    orange_hasee_walk1_left_sick, orange_hasee_walk1_right_sick,
    orange_hasee_idle_left_sick, orange_hasee_idle_right_sick,
    orange_hasee_walk0_left_sick, orange_hasee_walk0_right_sick,
};

const unsigned char * const purple_hasee_squat_animation[] = {
    purple_hasee_squat_left, purple_hasee_squat_right,
};

const unsigned char * const purple_hasee_squat_sick_animation[] = {
    purple_hasee_squat_left_sick, purple_hasee_squat_right_sick,
};

const unsigned char * const orange_hasee_squat_animation[] = {
    orange_hasee_squat_left, orange_hasee_squat_right,
};

const unsigned char * const orange_hasee_squat_sick_animation[] = {
    orange_hasee_squat_left_sick, orange_hasee_squat_right_sick,
}; 

const unsigned char const yellow_doughnutfruit[] = {
    0, 0, 0x00, 0,
    8, 0, 0x01, 0,
    0, 8, 0x11, 0|OAM_FLIP_H,
    8, 8, 0x11, 0,
    128
};

const unsigned char const blue_green_doughnutfruit[] = {
    0, 0, 0x00, 2,
    8, 0, 0x01, 2,
    0, 8, 0x11, 2|OAM_FLIP_H,
    8, 8, 0x11, 2,
    128
};

const unsigned char const grundoughnutfruit[] = {
    0, 0, 0x50, 2,
    8, 0, 0x51, 2,
    0, 8, 0x11, 2|OAM_FLIP_H,
    8, 8, 0x11, 2,
    128
};

const unsigned char const silver_gold_icy_doughnutfruit[] = {
    0, 0, 0x00, 3,
    8, 0, 0x01, 3,
    0, 8, 0x11, 3|OAM_FLIP_H,
    8, 8, 0x11, 3,
    128
};

const unsigned char const checkered_doughnutfruit[] = {
    0, 0, 0x42, 3,
    8, 0, 0x43, 3,
    0, 8, 0x52, 3,
    8, 8, 0x53, 3,
    128
};

const unsigned char const sponge_doughnutfruit[] = {
    0, 0, 0x44, 3,
    8, 0, 0x45, 3,
    0, 8, 0x54, 3,
    8, 8, 0x55, 3,
    128
};

const unsigned char const fiery_doughnutfruit[] = {
    0, 0, 0x46, 3,
    8, 0, 0x47, 3,
    0, 8, 0x56, 3,
    8, 8, 0x57, 3,
    128
};

const unsigned char const rainbow_doughnutfruit[] = {
    0, 0, 0x26, 3,
    8, 0, 0x27, 3,
    0, 8, 0x36, 3,
    8, 8, 0x37, 3,
    128
};

const unsigned char const fish_doughnutfruit[] = {
    0, 0, 0x40, 3,
    8, 0, 0x41, 3,
    0, 8, 0x11, 3|OAM_FLIP_H,
    8, 8, 0x11, 3,
    128
};

const unsigned char const purple_a_fruit[] = {
    0, 0, 0x02, 1,
    8, 0, 0x03, 1,
    0, 8, 0x12, 1,
    8, 8, 0x13, 1,
    128
};

const unsigned char const purple_e_fruit[] = {
    0, 0, 0x04, 1,
    8, 0, 0x05, 1,
    0, 8, 0x14, 1,
    8, 8, 0x15, 1,
    128
};

const unsigned char const purple_h_fruit[] = {
    0, 0, 0x06, 1,
    8, 0, 0x07, 1,
    0, 8, 0x16, 1,
    8, 8, 0x17, 1,
    128
};

const unsigned char const purple_s_fruit[] = {
    0, 0, 0x20, 1,
    8, 0, 0x21, 1,
    0, 8, 0x30, 1,
    8, 8, 0x31, 1,
    128
};

const unsigned char const orange_a_fruit[] = {
    0, 0, 0x60, 1,
    8, 0, 0x61, 1,
    0, 8, 0x70, 1,
    8, 8, 0x71, 1,
    128
};

const unsigned char const orange_e_fruit[] = {
    0, 0, 0x62, 1,
    8, 0, 0x63, 1,
    0, 8, 0x72, 1,
    8, 8, 0x73, 1,
    128
};

const unsigned char const orange_h_fruit[] = {
    0, 0, 0x64, 1,
    8, 0, 0x65, 1,
    0, 8, 0x74, 1,
    8, 8, 0x75, 1,
    128
};

const unsigned char const orange_s_fruit[] = {
    0, 0, 0x66, 1,
    8, 0, 0x67, 1,
    0, 8, 0x76, 1,
    8, 8, 0x77, 1,
    128
};

const unsigned char const maccy_pickup[] = {
    0, 0, 0x48, 3,
    8, 0, 0x49, 3,
    0, 8, 0x58, 3,
    8, 8, 0x59, 3,
    128
};

const unsigned char const gross_dung[] = {
    0, 0, 0x24, 1,
    8, 0, 0x25, 1,
    0, 8, 0x34, 1,
    8, 8, 0x35, 1,
    128
};

const unsigned char const gross_slime[] = {
    0, 0, 0x22, 0,
    8, 0, 0x23, 0,
    0, 8, 0x32, 0,
    8, 8, 0x33, 0,
    128
};

const unsigned char * const hasee_treat_metasprite_lut[] = {
    yellow_doughnutfruit,
    blue_green_doughnutfruit,
    blue_green_doughnutfruit,
    silver_gold_icy_doughnutfruit,
    silver_gold_icy_doughnutfruit,
    checkered_doughnutfruit,
    sponge_doughnutfruit,
    fiery_doughnutfruit,
    silver_gold_icy_doughnutfruit,
    rainbow_doughnutfruit,
    fish_doughnutfruit,
    grundoughnutfruit,
    maccy_pickup,
    gross_dung,
    gross_slime,
    purple_h_fruit,
    purple_a_fruit,
    purple_s_fruit,
    purple_e_fruit,
    orange_h_fruit,
    orange_a_fruit,
    orange_s_fruit,
    orange_e_fruit
};

// We have limited palettes so if a treat wants 2 or 3, we need to know which it need a "lock" on 

const unsigned char const hasee_treat_points[] = {
    // See hasee_constants.h for expected base indices of each item
    // Max of 8x multiplier; can't do more than 255 at once in unsigned char
     1,  2,  3,  4,  5,  6,  7,  8, // Yellow
     3,  6,  9, 12, 15, 18, 21, 24, // Blue
     4,  8, 12, 16, 20, 24, 28, 32, // Green
     5, 10, 15, 20, 25, 30, 35, 40, // Silver
    10, 20, 30, 40, 50, 60, 70, 80, // Golden
    12, 24, 36, 48, 60, 72, 84, 96, // Checkered
    15, 30, 45, 60, 75, 90,105,120, // Sponge
    18, 36, 54, 72, 90,108,126,144, // Fiery
    20, 40, 60, 80,100,120,140,160, // Icy
    40, 80,120,160,200,240,255,255, // Rainbow
    50,100,150,200,250,255,255,255, // Fish
     7, 14, 21, 28, 35, 42, 49, 56, // Grundoughnutfruit
    64,128,192,255,255,255,255,255, // Maccy

    // The below cases will be handled separately since they are redundant...
    // 0, // Gross - Dung
    // 0, // Gross - Slime
    // 2, // Purple H
    // 2, // Purple A
    // 2, // Purple S
    // 2, // Purple E
    // 2, // Orange H
    // 2, // Orange A
    // 2, // Orange S
    // 2, // Orange E
};

const unsigned char hasee_letter_points[] = {
    2, 4, 6, 8, 10, 12, 14, 16
};

// These are mostly here because it's more convenient to update screen gfx this way
const unsigned char const hasee_metatiles[]={
    0x84,0x85,0x94,0x95, 2, // Purple H
    0x80,0x81,0x90,0x91, 2, // Purple A
    0x86,0x87,0x96,0x97, 2, // Purple S
    0x82,0x83,0x92,0x93, 2, // Purple E
    0x8C,0x8D,0x9C,0x9D, 2, // Orange H
    0x88,0x89,0x98,0x99, 2, // Orange A
    0x8E,0x8F,0x9E,0x9F, 2, // Orange S
    0x8A,0x8B,0x9A,0x9B, 2, // Orange E
};

const unsigned char const hasee_paused_text[] = {
    0,  0, 0xF9, 1,
    8,  0, 0xFA, 1,
   16,  0, 0xFB, 1,
   24,  0, 0xFC, 1,
   32,  0, 0xFD, 1,
   40,  0, 0xFE, 1,
  128
};

const unsigned int const hasee_letter_hud_ntaddr_lut[] = {
    NTADR_A(12, 6),
    NTADR_A(14, 6),
    NTADR_A(16, 6),
    NTADR_A(18, 6),
    NTADR_A(20, 6),
    NTADR_A(12, 7),
};

const unsigned char const hasee_blank_letters_lut[] = {
    0xAC,0xAD,0xA8,0xA9,0xAE,0xAF,0xAA,0xAB,0xAA,0xAB,
    0xBC,0xBD,0xB8,0xB9,0xBE,0xBF,0xBA,0xBB,0xBA,0xBB,
};

const unsigned char const hasee_purple_filled_letters_lut[] = {
    0x84,0x85,0x80,0x81,0x86,0x87,0x82,0x83,0x82,0x83,
    0x94,0x95,0x90,0x91,0x96,0x97,0x92,0x93,0x92,0x93,
};

const unsigned char const hasee_leftright_movement_moving_lookup_table[] = {
    0xff, 0x01,
    0xfe, 0x02,
    0xfd, 0x03,
    0xfc, 0x04,
    0xfb, 0x05,
};

#pragma rodata-name(pop);

#pragma rodata-name(push, "BANK5")

// LibLZG'd data either need to be in Bank 5 or be copied to RAM because the LZG decode function is in bank 5
const unsigned char const hasee_game_screen[] = {
	76,90,71,0,0,4,0,0,0,1,29,72,71,97,208,1,
	1,2,3,4,177,177,177,178,0,4,23,176,177,4,1,2,
	28,24,4,4,48,48,48,4,201,2,16,24,6,7,8,4,
	194,22,23,24,2,29,88,2,28,24,4,1,172,173,168,169,
	174,175,170,171,170,171,2,20,24,188,189,184,185,190,191,186,
	187,186,187,2,12,24,160,4,2,161,162,163,2,8,80,164,
	165,166,167,4,2,2,30,152,2,31,24,2,30,24,4,2,
	195,196,197,4,65,2,10,24,2,5,7,195,4,164,177,177,
	179,3,13,197,3,96,2,5,24,2,5,7,177,177,196,4,
	1,3,215,3,4,4,98,2,7,24,179,196,181,3,142,3,
	2,180,4,228,3,132,2,6,24,3,107,3,217,3,245,2,
	6,34,2,5,24,3,205,3,89,3,194,179,180,4,2,3,
	1,3,216,2,6,44,179,180,3,155,3,146,4,4,176,177,
	226,4,28,208,209,210,210,211,212,213,213,214,215,2,24,28,
	227,228,229,229,230,231,208,208,182,183,208,3,39,211,3,39,
	214,2,19,39,198,199,3,166,3,39,230,3,39,2,22,24,
	4,28,4,5,0,255,4,3,51,4,225,175,175,239,4,225,
	240,240,255,255,243,240,48,2,7,16,95,95,95,87,85,95,
	19,0,85,4,3,17,3,207,255,255,15,4,5,
};

#pragma rodata-name(pop);