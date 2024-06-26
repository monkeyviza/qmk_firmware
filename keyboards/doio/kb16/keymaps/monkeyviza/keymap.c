/* Copyright 2022 DOIO
 * Copyright 2022 HorrorTroll <https://github.com/HorrorTroll>
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

// OLED animation
#include "lib/layer_status/layer_status.h"
//#include "lib/bongocat/bongocat.h"

// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.

enum layer_names {
    _BASE,
    _FN,
    _FN1,
    _FN2
};

enum layer_keycodes {
    KC_PASTE_WIN,
    KC_SAVE_WIN,
    KC_VDESK_L_WIN,
    KC_VDESK_R_WIN
 };

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case KC_PASTE_WIN:
            if (record->event.pressed) {
                SEND_STRING(SS_LCTL("v")); // paste for windows
            }
            return false;
        case KC_SAVE_WIN:
            if (record->event.pressed) {
                SEND_STRING(SS_LCTL("s")); // save for windows
            }
            return false;
        case KC_VDESK_L_WIN:
            if (record->event.pressed) {
                SEND_STRING(SS_LGUI(SS_LCTL(SS_TAP(X_LEFT)))); // Virtual Desktop left
            }
            return false;
        case KC_VDESK_R_WIN:
            if (record->event.pressed) {
                SEND_STRING(SS_LGUI(SS_LCTL(SS_TAP(X_RIGHT)))); // Virtual Desktop left
            }
            return false;
        default:
            return true; // Process all other keycodes normally
    }
};

// Tap Dance declarations
enum {
    TD_RESET = 0,
    CT_COPY_WIN,
    CT_CUT_WIN,
    CT_DO_ACTIONS,
    CT_VS_FMT_WIN,
    CT_VS_CSTM_WIN,
    TD_CLOSE_TAB_WIN,
    TD_VIRTUAL_DESK_WIN,
};

/**
 * This function performs a safe reset of the tap dance state and the keyboard.
 * It takes a tap_dance_state_t pointer and user_data as parameters.
 * If the state count is greater than or equal to 3, it performs a reset of the keyboard and the tap dance state.
 *
 * @param state The tap_dance_state_t pointer.
 * @param user_data A pointer to user-defined data (if any).
 */
void safe_reset(tap_dance_state_t *state, void *user_data) {
    // Check if the count of the tap dance state is greater than or equal to 3
    if (state->count >= 3) {
        // If the count is greater than or equal to 3, perform a reset of the keyboard
        reset_keyboard();

        // Reset the tap dance state
        reset_tap_dance(state);
    }
}

void ct_do_actions_win(tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) {
        SEND_STRING(SS_LCTL("z"));
        reset_tap_dance(state);
    } else {
        SEND_STRING(SS_LCTL("y"));
        reset_tap_dance(state);
    }
}

void ct_copy_win(tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) {
        SEND_STRING(SS_LCTL("c"));
        reset_tap_dance(state);
    } else {
        SEND_STRING(SS_LCTL("ac"));
        reset_tap_dance(state);
    }
}

void ct_cut_win(tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) {
        SEND_STRING(SS_LCTL("x"));
        reset_tap_dance(state);
    } else {
        SEND_STRING(SS_LCTL("ax"));
        reset_tap_dance(state);
    }
}

void ct_close_tab_win(tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) {
        SEND_STRING(SS_LCTL(SS_TAP(X_W)));
        reset_tap_dance(state);
    } else if (state->count == 2) {
        SEND_STRING(SS_LCTL(SS_LSFT(SS_TAP(X_T))));
        reset_tap_dance(state);
    } else {
        SEND_STRING(SS_LALT(SS_TAP(X_F4)));
        reset_tap_dance(state);
    }
}

void ct_virtual_desktops_win(tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) {
        SEND_STRING(SS_LGUI(SS_TAP(X_TAB)));
        reset_tap_dance(state);
    } else if (state->count == 2) {
        SEND_STRING(SS_LGUI(SS_LCTL(SS_TAP(X_D))));
        reset_tap_dance(state);
    } else {
        SEND_STRING(SS_LGUI(SS_LCTL(SS_TAP(X_F4))));
        reset_tap_dance(state);
    }
}


////////////////////
// VS Code commands
////////////////////

/**
 * This function is a Tap Dance action for formatting code in VS Code on Windows.
 * It takes a tap_dance_state_t pointer and user_data as parameters.
 * If the state count is 1, it sends the shortcut for "Shift + Alt + F" to format the entire file in VS Code, and resets the tap dance state.
 * If the state count is any other value, it sends the shortcut for "Ctrl + K" followed by "Ctrl + F" to format the selected code in VS Code, and resets the tap dance state.
 *
 * @param state The tap_dance_state_t pointer.
 * @param user_data A pointer to user-defined data (if any).
 */
void ct_vsc_fmt_win(tap_dance_state_t *state, void *user_data) {
    // Check the count of the tap dance state
    if (state->count == 1) {
        // If count is 1, send the shortcut for "Shift + Alt + F" to format the entire file in VS Code
        SEND_STRING(SS_RSFT(SS_LALT("f")));

        // Reset the tap dance state
        reset_tap_dance(state);
    } else {
        // For any other count value, send the shortcut for "Ctrl + K" followed by "Ctrl + F" to format the selected code in VS Code
        SEND_STRING(SS_LCTL("k") SS_LCTL("f"));

        // Reset the tap dance state
        reset_tap_dance(state);
    }
}

/**
 * This function is a Tap Dance action for VS Code commands.
 * It takes a tap_dance_state_t pointer and user_data as parameters.
 * If the state count is 1, it sends the shortcut for "Shift + Alt + Down Arrow" and resets the tap dance state.
 * If the state count is 2, it sends the shortcut for "Shift + Ctrl + K" and resets the tap dance state.
 * If the state count is any other value, it sends the shortcut for "Alt + Down Arrow" and resets the tap dance state.
 *
 * @param state The tap_dance_state_t pointer.
 * @param user_data A pointer to user-defined data (if any).
 */
void ct_vsc_custom_mv(tap_dance_state_t *state, void *user_data) {
    // Check the count of the tap dance state
    if (state->count == 1) {
        // If count is 1, send the shortcut for "Shift + Alt + Down Arrow"
        SEND_STRING(SS_RSFT(SS_LALT(SS_TAP(X_DOWN))));

        // Reset the tap dance state
        reset_tap_dance(state);
    } else if (state->count == 2) {
        // If count is 2, send the shortcut for "Shift + Ctrl + K"
        SEND_STRING(SS_RSFT(SS_LCTL(SS_TAP(X_K))));

        // Reset the tap dance state
        reset_tap_dance(state);
    } else {
        // For any other count value, send the shortcut for "Alt + Down Arrow"
        SEND_STRING(SS_LALT(SS_TAP(X_DOWN)));

        // Reset the tap dance state
        reset_tap_dance(state);
    }
}

tap_dance_action_t tap_dance_actions[] = {
  [CT_COPY_WIN] = ACTION_TAP_DANCE_FN(ct_copy_win),
  [CT_CUT_WIN] = ACTION_TAP_DANCE_FN(ct_cut_win),
  [CT_DO_ACTIONS] = ACTION_TAP_DANCE_FN(ct_do_actions_win),
  [CT_VS_FMT_WIN] = ACTION_TAP_DANCE_FN(ct_vsc_fmt_win),
  [CT_VS_CSTM_WIN] = ACTION_TAP_DANCE_FN(ct_vsc_custom_mv),
  [TD_CLOSE_TAB_WIN] = ACTION_TAP_DANCE_FN(ct_close_tab_win),
  [TD_VIRTUAL_DESK_WIN] = ACTION_TAP_DANCE_FN(ct_virtual_desktops_win),
  [TD_RESET] = ACTION_TAP_DANCE_FN(safe_reset)
};



const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/*
       ┌───┬───┬───┬───┐   ┌───┐ ┌───┐
       │ 1 │ 2 │ 3 │ 4 │   │Ply│ │TO1│
       ├───┼───┼───┼───┤   └───┘ └───┘
       │ 5 │ 6 │ 7 │ 8 │
       ├───┼───┼───┼───┤
       │ 9 │ 0 │ ↑ │Ent│      ┌───┐
       ├───┼───┼───┼───┤      │Mut│
       │Fn2│ ← │ ↓ │ → │      └───┘
       └───┴───┴───┴───┘
       ┌───┬───┬───┬───┐   ┌───┐ ┌───┐
       │ ! │ @ │ # │ $ │   │   │ │   │
       ├───┼───┼───┼───┤   └───┘ └───┘
       │ % │ ^ │ & │ * │
       ├───┼───┼───┼───┤
       │ ( │ ) │   │   │      ┌───┐
       ├───┼───┼───┼───┤      │   │
       │   │   │   │   │      └───┘
       └───┴───┴───┴───┘
*/
    /*  Row:    0         1        2        3         4      */
    [_BASE] = LAYOUT(
                TD(CT_COPY_WIN),        KC_PASTE_WIN,        TD(CT_CUT_WIN),    TD(CT_VS_FMT_WIN),          TO(_FN2),
                KC_SAVE_WIN,            TD(CT_DO_ACTIONS),   KC_7,              KC_8,                       TO(_FN),
                TD(CT_VS_CSTM_WIN),     KC_VDESK_L_WIN,      KC_VDESK_R_WIN,    TD(TD_VIRTUAL_DESK_WIN),    KC_MPLY,
                MO(_FN2),               TD(TD_CLOSE_TAB_WIN),             KC_DOWN,           KC_RIGHT
            ),

/*
       ┌───┬───┬───┬───┐   ┌───┐ ┌───┐
       │   │   │   │   │   │   │ │   │
       ├───┼───┼───┼───┤   └───┘ └───┘
       │   │   │   │   │
       ├───┼───┼───┼───┤
       │   │   │   │   │      ┌───┐
       ├───┼───┼───┼───┤      │   │
       │   │   │   │   │      └───┘
       └───┴───┴───┴───┘
*/
    /*  Row:    0        1        2        3        4       */
    [_FN] = LAYOUT(
                _______, _______, _______, _______, TO(_BASE),
                _______, _______, _______, _______, TO(_FN1),
                _______, _______, _______, _______, _______,
                _______, _______, _______, _______
            ),

/*
       ┌───┬───┬───┬───┐   ┌───┐ ┌───┐
       │   │   │   │   │   │   │ │   │
       ├───┼───┼───┼───┤   └───┘ └───┘
       │   │   │   │   │
       ├───┼───┼───┼───┤
       │   │   │   │   │      ┌───┐
       ├───┼───┼───┼───┤      │   │
       │   │   │   │   │      └───┘
       └───┴───┴───┴───┘
*/
    /*  Row:    0        1        2        3        4       */
    [_FN1] = LAYOUT(
                _______, _______, _______, _______, TO(_FN),
                _______, _______, _______, _______, TO(_FN2),
                _______, _______, _______, _______, _______,
                _______, _______, _______, _______
            ),

/*
       ┌───┬───┬───┬───┐   ┌───┐ ┌───┐
       │Spi│Spd│   │   │   │   │ │TO0│
       ├───┼───┼───┼───┤   └───┘ └───┘
       │Sai│Sad│   │   │
       ├───┼───┼───┼───┤
       │Tog│Mod│Hui│   │      ┌───┐
       ├───┼───┼───┼───┤      │   │
       │   │Vai│Hud│Vad│      └───┘
       └───┴───┴───┴───┘
*/
    /*  Row:    0        1        2        3        4        */
    [_FN2] = LAYOUT(
                RGB_SPI, RGB_SPD, _______, TD(TD_RESET), TO(_FN1),
                RGB_SAI, RGB_SAD, _______, _______, TO(_BASE),
                RGB_TOG, RGB_MOD, RGB_HUI, _______, _______,
                _______, RGB_VAI, RGB_HUD, RGB_VAD
            ),
};

#ifdef OLED_ENABLE
bool oled_task_user(void) {
    render_layer_status();
    // render_bongocat();

    return true;
}
#endif

#ifdef ENCODER_MAP_ENABLE
    const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
        [_BASE] = {ENCODER_CCW_CW(KC_WH_L, KC_WH_R), ENCODER_CCW_CW(KC_WH_D, KC_WH_U), ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
        [_FN]   = {ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_TRNS, KC_TRNS)},
        [_FN1]  = {ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_TRNS, KC_TRNS)},
        [_FN2]  = {ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_TRNS, KC_TRNS)},
    };
#endif
