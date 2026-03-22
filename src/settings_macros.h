// ===
// Constants + Macros for settings
#define SETTINGS_IS_DOWN_TO_REVIVE_ENABLED (settings_memory[0] & 0x01)
#define SETTINGS_TOGGLE_DOWN_TO_REVIVE() (settings_memory[0] ^= 0x01)

#define SETTINGS_ARE_SECRET_CHARACTERS_ENABLED (settings_memory[0] & 0b10)
#define SETTINGS_TOGGLE_ENABLE_SECRET_CHARACTERS() (settings_memory[0] ^= 0b10)
