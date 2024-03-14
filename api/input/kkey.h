#ifndef K_KEY_H_
#define K_KEY_H_

#include <ktype.h>

/// Enum representing different key scancodes
enum kKeyCode {
    K_KEY_W = 17,
    K_KEY_LCTRL = 29,
    K_KEY_A = 30,
    K_KEY_S = 31,
    K_KEY_D = 32,
    K_KEY_LSHIFT = 42,
    K_KEY_SPACE = 57,
    K_LAST_KEY,
};

/// Gets the mouse's position in window coordinates
kVec2f kMousePos(u0);

/// Enable keyboard input
u0 kKeyboardEnable(u0);

/// Checks if a certain key is pressed
/// True if it's currently being held down, false otherwise
b8 kKeyIsPressed(enum kKeyCode c);

#endif // K_KEY_H_
