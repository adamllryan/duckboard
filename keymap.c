/* Copyright 2020-2021 doodboard
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include QMK_KEYBOARD_H

#define KC_COPY LCTL(KC_C)
#define KC_PASTE LCTL(KC_V)
#define KC_HL LCTL(KC_A)


enum my_keycodes {
    KC_TGSCROLL = SAFE_RANGE,
    KC_GAMEMODE_ON,
    KC_GAMEMODE_OFF, 
    COPY,
    PASTE,
    CUT, 
    TOGGLE_OLED
};

short scroll_mode = 0;
short scroll_mode_non_override = 0;


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT( //base
                 TG(2),   KC_F24, KC_F20, KC_F16,
                 TG(1),    KC_F23,    KC_F19,    KC_F15,
                 TO(0),    KC_F22,    KC_F18,    KC_F14,
        KC_GAMEMODE_ON, KC_TGSCROLL,    KC_F21,    KC_F17,    KC_F13,
        TOGGLE_OLED, KC_HL,    KC_COPY,    KC_PASTE,  LT(4,KC_MPLY)),
    [1] = LAYOUT( //macro
                 KC_TRNS, PROGRAMMABLE_BUTTON_12, PROGRAMMABLE_BUTTON_8, PROGRAMMABLE_BUTTON_4,
                 KC_TRNS, PROGRAMMABLE_BUTTON_11, PROGRAMMABLE_BUTTON_7, PROGRAMMABLE_BUTTON_3,
                 KC_TRNS, PROGRAMMABLE_BUTTON_10, PROGRAMMABLE_BUTTON_6, PROGRAMMABLE_BUTTON_2,
        KC_TRNS,   KC_TRNS, PROGRAMMABLE_BUTTON_9, PROGRAMMABLE_BUTTON_5, PROGRAMMABLE_BUTTON_1,
        KC_TRNS,   KC_TRNS,   KC_TRNS, KC_TRNS, KC_TRNS),
    [2] = LAYOUT( //func
                 KC_TRNS,   KC_F12, KC_F8, KC_F4,
                 KC_TRNS, KC_F11,   KC_F7, KC_F3,
                 KC_TRNS, KC_F10, KC_F6, KC_F2,
        KC_TRNS, KC_TRNS,  KC_F9, KC_F5, KC_F1,
        KC_TRNS,   KC_TRNS, KC_TRNS,  KC_TRNS,  KC_TRNS),
    [3] = LAYOUT( //game
                 KC_LCTL,   KC_LSFT, KC_TAB, KC_ESC,
                 KC_Z, KC_A,   KC_Q, KC_1,
                 KC_X, KC_S, KC_W, KC_2,
        KC_GAMEMODE_OFF, KC_C,  KC_D, KC_E, KC_3,
        KC_SPACE,   KC_V, KC_F,  KC_R,  KC_4),
    [4] = LAYOUT(
                 KC_TRNS, RGB_TOG, RGB_MOD, KC_TRNS,
                 RGB_HUI, RGB_SAI, RGB_VAI, KC_TRNS,
                 RGB_HUD, RGB_SAD, RGB_VAD, KC_TRNS,
        KC_TRNS,   KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        QK_BOOT,   KC_TRNS,   KC_TRNS, KC_TRNS, KC_TRNS),
};


bool screen_status = false;
uint32_t screen_off_timer = 0;
bool screen_force = false;

void screen_off(void) {
        screen_off_timer = 0;
        screen_status = false;
        oled_off();
}
void screen_on(void) {
    screen_status = true;
    if (!screen_force)
        screen_off_timer = timer_read32();
    oled_on();
}

bool process_record_user(uint16_t kc, keyrecord_t *record) {
    if (record->event.pressed) { //only on key down
        switch (kc) {
        case KC_TGSCROLL:
            scroll_mode++;
            if (scroll_mode == 5) scroll_mode = 0;
            break;
        case KC_GAMEMODE_ON:
            scroll_mode_non_override = scroll_mode;
            scroll_mode = 4;
            layer_clear();
            layer_on(3);
            break;
        case KC_GAMEMODE_OFF:
            scroll_mode = scroll_mode_non_override;
            layer_clear();
            layer_on(0);
            break;
        case COPY:
            tap_code16(QK_LCTL | KC_C);
            break;
        case CUT:
            tap_code16(QK_LCTL | KC_X);
            break;
        case PASTE:
            tap_code16(QK_LCTL | KC_V);
            break;
        case TOGGLE_OLED:
            if (screen_status) {
                screen_force = false;
                screen_off();
            } else {
                screen_force = true;
                screen_on();
            }
            return true;
        default:
            break;
        }
        if (!screen_force) screen_on();
    }
    return true;
}

layer_state_t layer_state_set_user(layer_state_t state) {
    switch (get_highest_layer(state)) {
    case 0:
        rgblight_setrgb (0x00,  0x00, 0xFF);
        break;
    case 1:
        rgblight_setrgb (0xFF,  0x00, 0x00);
        break;
    case 2:
        rgblight_setrgb (0x00,  0xFF, 0x00);
        break;
    case 3:
        rgblight_setrgb (0x7A,  0x00, 0xFF);
        break;
    default: //  for any other layers, or the default layer
        rgblight_setrgb (0x00,  0xFF, 0xFF);
        break;
    }
  return state;
}
//ms before screen turns off
const short idle_time = 5000;
bool is_alt_tab_active = false;
uint16_t alt_tab_timer = 0;
bool encoder_update_user(uint8_t index, bool clockwise) {
    switch (scroll_mode) {
        case 0: /* Alt-tab functionality */
            if (clockwise) {
            if (!is_alt_tab_active) {
                is_alt_tab_active = true;
                register_code(KC_LALT);
            }
            alt_tab_timer = timer_read();
            tap_code16(KC_TAB);
            } else {
            if (!is_alt_tab_active) {
                is_alt_tab_active = true;
                register_code(KC_LALT);
            }
            alt_tab_timer = timer_read();
            tap_code16(S(KC_TAB));
            }
            break;
        case 1: /* Scrub history functionality*/
            if (index == 0) { /* First encoder */
                tap_code16(QK_LCTL | clockwise ? KC_Y : KC_Z);
            }
            break;
        case 2: /* webpage scroll functionality*/ 
            if (index == 0) { 
                tap_code(clockwise ? KC_PGDN : KC_PGUP);
            }
            break;
        case 3: /* Volume scroll functionality*/
            if (index == 0) {
                tap_code(clockwise ? KC_VOLU : KC_VOLD);
            }
            break;
        case 4:
            tap_code(clockwise ? KC_F22 : KC_F21);
            break;
        default:
            break;
    }
    return true;
}



void matrix_scan_user(void) {
  if (is_alt_tab_active) {
    if (timer_elapsed(alt_tab_timer) > 1000) {
      unregister_code(KC_LALT);
      is_alt_tab_active = false;
    }
  }
  if (!screen_force && screen_status) {
    if (timer_elapsed(screen_off_timer) > 10000) {
        screen_off();
    }
  }
}

#ifdef OLED_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) { return OLED_ROTATION_270; }


// WPM-responsive animation stuff here
#define IDLE_FRAMES 2
#define IDLE_SPEED 40 // below this wpm value your animation will idle

#define ANIM_FRAME_DURATION 200 // how long each frame lasts in ms
// #define SLEEP_TIMER 60000 // should sleep after this period of 0 wpm, needs fixing
#define ANIM_SIZE 636 // number of bytes in array, minimize for adequate firmware size, max is 1024

uint32_t anim_timer = 0;
uint32_t anim_sleep = 0;
uint8_t current_idle_frame = 0;

// Credit to u/Pop-X- for the initial code. You can find his commit here https://github.com/qmk/qmk_firmware/pull/9264/files#diff-303f6e3a7a5ee54be0a9a13630842956R196-R333.
static void render_anim(void) {
    static const char PROGMEM idle[IDLE_FRAMES][ANIM_SIZE] = {
        {
        0,  0,192,192,192,192,192,192,192,248,248, 30, 30,254,254,248,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  3,  3,  3,  3,  3,255,255,255,255,255,255,255,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,127,127,255,255,255,255,255,159,159,135,135,129,129,129, 97, 97, 25, 25,  7,  7,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1, 97, 97,127,  1,  1, 97, 97,127,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0
        },
        {
        0,  0,128,128,128,128,128,128,128,240,240, 60, 60,252,252,240,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  7,  7,  7,  7,  7,255,255,254,254,255,255,255,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,255,255,255,255,255,255,255, 63, 63, 15, 15,  3,  3,  3,195,195, 51, 51, 15, 15,  3,  3,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  3,  3, 99, 99,127,  3,  3, 99, 99,127,  3,  3,  0,  0,  0,  0,  0,  0,  0,  0,  0
        }
    };

    //assumes 1 frame prep stage
    void animation_phase(void) {
            current_idle_frame = (current_idle_frame + 1) % IDLE_FRAMES;
            oled_write_raw_P(idle[abs((IDLE_FRAMES-1)-current_idle_frame)], ANIM_SIZE);
    }

        if(timer_elapsed32(anim_timer) > ANIM_FRAME_DURATION) {
            anim_timer = timer_read32();
            animation_phase();
        }
    }

bool oled_task_user(void) {
    if (screen_status) {
        render_anim();
        oled_set_cursor(0,5);
        oled_write_P(PSTR("duck+board\n>---<\n"), false);
        // // Host Keyboard Layer Status
        // oled_write_P(PSTR("mode<\n"), false);
        // oled_write_P(PSTR(""), false);

        switch (get_highest_layer(layer_state)) {
            case 0:
                oled_write_P(PSTR("base<\n\n>---<\n"), false);
                break;
            case 1:
                    oled_write_P(PSTR("base macr<\n>---<\n"), false);
                break;
            case 2:
                if (IS_LAYER_ON(1))
                    oled_write_P(PSTR("base macrofunc<\n\n"), false);
                else
                    oled_write_P(PSTR("base func<\n>---<\n"), false);
                break;
            case 3:
                oled_write_P(PSTR("game<\n\n>---<\n"), false);
            case 4:
                oled_write_P(PSTR("settings<\n\n>---<\n"), false);
        }
        switch (scroll_mode) {
            case 0:
                oled_write_P(PSTR("windw"), false);
                break;
            case 1:
                oled_write_P(PSTR("histo"), false);
                break;
            case 2:
                oled_write_P(PSTR("page"), false);
                break;
            case 3:
                oled_write_P(PSTR("vol+-"), false);
                break;
            case 4:
                oled_write_P(PSTR("macro"), false);
                break;
        }
    } else {
        oled_off();
    }
    return false;
}
#endif

void keyboard_post_init_user(void) {
  //Customise these values to debug
  //debug_enable=true;
  //debug_matrix=true;
  //debug_keyboard=true;
  //debug_mouse=true;
  screen_on();
}
