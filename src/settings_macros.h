// ===
// Constants + Macros for settings
#define SETTINGS_IS_DOWN_TO_REVIVE_ENABLED (settings_memory[0] & 0x01)
#define SETTINGS_TOGGLE_DOWN_TO_REVIVE() (settings_memory[0] ^= 0x01)

const unsigned char const settings_bitmask_lookup_table[] = {
    0x01,
    0x02,
    0x04,
    0x08,
    0x10,
    0x20,
    0x40,
    0x80,
};