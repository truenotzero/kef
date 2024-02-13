#ifndef K_KEY_H_
#define K_KEY_H_

#include <ktype.h>

enum kKeyCode {
    K_KEY_W = 17,
    K_KEY_LCTRL = 29,
    K_KEY_A = 30,
    K_KEY_S = 31,
    K_KEY_D = 32,
    K_KEY_SPACE = 57,
    K_LAST_KEY,
};

kVec2f kMousePos(u0);

u0 kKeyboardEnable(u0);
b8 kKeyIsPressed(enum kKeyCode c);

#endif // K_KEY_H_
