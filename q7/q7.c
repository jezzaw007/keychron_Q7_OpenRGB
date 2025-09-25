#include "quantum.h"
#include "rgb_matrix.h"
#include "eeprom.h"

// Tracks current protocol mode
static bool openrgb_mode = false;

// EEPROM pointer for RGB mode
#define EECONFIG_RGB_MODE ((uint16_t *)(EECONFIG_USER + 1))

// Matrix mask for ISO layout
const matrix_row_t matrix_mask[] = {
    0b1111111111111111,
    0b1111111111111111,
    0b1111111111111111,
    0b1111111111111111,
    0b1111111111101111,
};

// Optional: DIP switch layer toggle
#ifdef DIP_SWITCH_ENABLE
bool dip_switch_update_kb(uint8_t index, bool active) {
    if (!dip_switch_update_user(index, active)) return false;
    if (index == 0) {
        default_layer_set(1UL << (active ? 1 : 0));
    }
    return true;
}
#endif

// OpenRGB fallback color buffer
RGB g_openrgb_direct_mode_colors[RGB_MATRIX_LED_COUNT];

__attribute__((weak)) void openrgb_direct_mode_init(void) {
    for (int i = 0; i < RGB_MATRIX_LED_COUNT; i++) {
        g_openrgb_direct_mode_colors[i] = (RGB){255, 255, 255}; // fallback white
    }
}

// Runtime protocol mode helpers
bool is_openrgb_mode(void) {
    return openrgb_mode;
}

void set_openrgb_mode(void) {
    openrgb_mode = true;
    openrgb_direct_mode_init();
    eeprom_update_word(EECONFIG_RGB_MODE, 0x0173);
}

void set_via_mode(void) {
    openrgb_mode = false;
    eeprom_update_word(EECONFIG_RGB_MODE, 0x0172);
}

// Load mode from EEPROM at startup
void keyboard_pre_init_kb(void) {
    uint16_t stored_pid = eeprom_read_word(EECONFIG_RGB_MODE);
    if (stored_pid == 0x0173) {
        set_openrgb_mode();
    } else {
        set_via_mode();
    }
    keyboard_pre_init_user();
}

#if defined(RGB_MATRIX_ENABLE) && defined(CAPS_LOCK_LED_INDEX)
bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
    if (!process_record_user(keycode, record)) return false;

    switch (keycode) {
        case RGB_TOG:
            if (record->event.pressed) {
                switch (rgb_matrix_get_flags()) {
                    case LED_FLAG_ALL:
                        rgb_matrix_set_flags(LED_FLAG_NONE);
                        rgb_matrix_set_color_all(0, 0, 0);
                        break;
                    default:
                        rgb_matrix_set_flags(LED_FLAG_ALL);
                        break;
                }
            }
            if (!rgb_matrix_is_enabled()) {
                rgb_matrix_set_flags(LED_FLAG_ALL);
                rgb_matrix_enable();
            }
            return false;
    }
    return true;
}

bool rgb_matrix_indicators_advanced_kb(uint8_t led_min, uint8_t led_max) {
    if (!rgb_matrix_indicators_advanced_user(led_min, led_max)) return false;

    if (host_keyboard_led_state().caps_lock) {
        RGB_MATRIX_INDICATOR_SET_COLOR(CAPS_LOCK_LED_INDEX, 255, 255, 255);
    } else {
        if (!rgb_matrix_get_flags()) {
            RGB_MATRIX_INDICATOR_SET_COLOR(CAPS_LOCK_LED_INDEX, 0, 0, 0);
        }
    }
    return true;
}
#endif
