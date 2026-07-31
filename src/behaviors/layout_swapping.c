#include <stddef.h>
#include <stdint.h>
#include <dt-bindings/zmk/keys.h>

static uint32_t layout_qwerty[] = {
    HID_USAGE_KEY_KEYBOARD_A, 
    HID_USAGE_KEY_KEYBOARD_B, 
    HID_USAGE_KEY_KEYBOARD_C, 
    HID_USAGE_KEY_KEYBOARD_D, 
    HID_USAGE_KEY_KEYBOARD_E, 
    HID_USAGE_KEY_KEYBOARD_F, 
    HID_USAGE_KEY_KEYBOARD_G, 
    HID_USAGE_KEY_KEYBOARD_H, 
    HID_USAGE_KEY_KEYBOARD_I, 
    HID_USAGE_KEY_KEYBOARD_J, 
    HID_USAGE_KEY_KEYBOARD_K, 
    HID_USAGE_KEY_KEYBOARD_L, 
    HID_USAGE_KEY_KEYBOARD_M, 
    HID_USAGE_KEY_KEYBOARD_N, 
    HID_USAGE_KEY_KEYBOARD_O, 
    HID_USAGE_KEY_KEYBOARD_P, 
    HID_USAGE_KEY_KEYBOARD_Q, 
    HID_USAGE_KEY_KEYBOARD_R, 
    HID_USAGE_KEY_KEYBOARD_S, 
    HID_USAGE_KEY_KEYBOARD_T, 
    HID_USAGE_KEY_KEYBOARD_U, 
    HID_USAGE_KEY_KEYBOARD_V, 
    HID_USAGE_KEY_KEYBOARD_W, 
    HID_USAGE_KEY_KEYBOARD_X, 
    HID_USAGE_KEY_KEYBOARD_Y, 
    HID_USAGE_KEY_KEYBOARD_Z,
};

static uint32_t layout_dvorak[] = {
    HID_USAGE_KEY_KEYBOARD_A,
    HID_USAGE_KEY_KEYBOARD_N,
    HID_USAGE_KEY_KEYBOARD_I,
    HID_USAGE_KEY_KEYBOARD_H,
    HID_USAGE_KEY_KEYBOARD_D,
    HID_USAGE_KEY_KEYBOARD_Y,
    HID_USAGE_KEY_KEYBOARD_U,
    HID_USAGE_KEY_KEYBOARD_J,
    HID_USAGE_KEY_KEYBOARD_G,
    HID_USAGE_KEY_KEYBOARD_C,
    HID_USAGE_KEY_KEYBOARD_V,
    HID_USAGE_KEY_KEYBOARD_P,
    HID_USAGE_KEY_KEYBOARD_M,
    HID_USAGE_KEY_KEYBOARD_L,
    HID_USAGE_KEY_KEYBOARD_S,
    HID_USAGE_KEY_KEYBOARD_R,
    HID_USAGE_KEY_KEYBOARD_X,
    HID_USAGE_KEY_KEYBOARD_O,
    HID_USAGE_KEY_KEYBOARD_SEMICOLON_AND_COLON,
    HID_USAGE_KEY_KEYBOARD_K,
    HID_USAGE_KEY_KEYBOARD_F,
    HID_USAGE_KEY_KEYBOARD_PERIOD_AND_GREATER_THAN,
    HID_USAGE_KEY_KEYBOARD_COMMA_AND_LESS_THAN,
    HID_USAGE_KEY_KEYBOARD_B,
    HID_USAGE_KEY_KEYBOARD_T,
    HID_USAGE_KEY_KEYBOARD_SLASH_AND_QUESTION_MARK,
};

uint32_t layout_mapping(uint32_t from_layout[], uint32_t to_layout[], uint32_t hid_usage){
    for (size_t i = 0; i < 26 ; i++){
        if(from_layout[i] == hid_usage){
            return to_layout[i];
        };
    };
    return hid_usage;
};

uint32_t dvorak_to_qwerty(uint32_t hid_usage){
    return layout_mapping(layout_dvorak, layout_qwerty, hid_usage);
};

uint32_t qwerty_to_dvorak(uint32_t hid_usage){
    return layout_mapping(layout_qwerty, layout_dvorak, hid_usage);
};

