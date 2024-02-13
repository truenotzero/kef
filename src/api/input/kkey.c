#include <input/kkey.h>

static b8 keys[K_LAST_KEY];

#include <GLFW/glfw3.h>
#include <render/kwindow.h>
#include <stdio.h>

static u0 key_cb(GLFWwindow *wnd, int key, int sc, int action, int mods) {
    (u0) mods;

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(wnd, GLFW_TRUE);
    }

    if (sc < K_LAST_KEY) {
        keys[sc] = action;
    } else {
        printf("unknown key produced event: (scancode) %d\n", sc);
    }
}

static f32 cursor_x, cursor_y;

static u0 cursor_pos_cb(GLFWwindow *wnd, double x, double y) {
    (u0) wnd;
    cursor_x = (f32) x;
    cursor_y = (f32) y;
}

kVec2f kMousePos(u0) {
    return (kVec2f) {{ .x = cursor_x, .y = cursor_y }};
}

u0 kKeyboardEnable(u0) {
    GLFWwindow *w = k__windowRawHandle();
    glfwSetKeyCallback(w, &key_cb);
    glfwSetCursorPosCallback(w, &cursor_pos_cb);

    // prevents the cursor jumping randomly when you first move the mouse
    double x, y;
    glfwGetCursorPos(w, &x, &y);
    cursor_x = (f32) x;
    cursor_y = (f32) y;
}

b8 kKeyIsPressed(enum kKeyCode c) {
    return keys[c];
}
