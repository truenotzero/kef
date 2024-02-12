#include <render/kwindow.h>

#include <GL/glew.h>
#include <render/kgl.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

static GLFWwindow *window = 0;

static void framebuffer_size_cb(GLFWwindow *wnd, int width, int height) {
    (void) wnd;
    KGL(glViewport(0, 0, width, height));
}

b8 kWindowCreate(void) {
    if (glfwInit() != GL_TRUE) return kfalse;
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    window = glfwCreateWindow(1200, 1200, "TITLE", NULL, NULL);
    if (window == NULL) return kfalse;
    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, &framebuffer_size_cb);

    if (glewInit() != GLEW_OK) return kfalse;


    return ktrue;
}

void kWindowDestroy(void) {
    glfwDestroyWindow(window);
    glfwTerminate();
}

void kWindowLoop(void) {
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        kWindowUpdate();

        kWindowRender();
        glfwSwapBuffers(window);
    }

}

u0 kWindowSetCursorVisible(b8 is_visible) {
    if (!is_visible && glfwRawMouseMotionSupported()) {
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    }
    int cursor_mode = is_visible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED;
    glfwSetInputMode(window, GLFW_CURSOR, cursor_mode);
}

u0 *k__windowRawHandle(u0) {
    return window;
}

// impl for kgl.h
#include <render/kgl.h>
#include <stdio.h>
u0 k__renderCheckGlImpl(c_str file, int line) {
    char const *err_str;
    do {
        switch (glGetError()) {
            case GL_NO_ERROR:
                return;
            case GL_INVALID_ENUM:
                err_str = "GL_INVALID_ENUM";
                break;
            case GL_INVALID_VALUE:
                err_str = "GL_INVALID_VALUE";
                break;
            case GL_INVALID_OPERATION:
                err_str = "GL_INVALID_OPERATION";
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                err_str = "GL_INVALID_FRAMEBUFFER_OPERATION";
                break;
            case GL_OUT_OF_MEMORY:
                err_str = "GL_OUT_OF_MEMORY";
                break;
            case GL_STACK_OVERFLOW:
                err_str = "GL_STACK_OVERFLOW";
                break;
            case GL_STACK_UNDERFLOW:
                err_str = "GL_STACK_UNDERFLOW";
                break;
        }
        printf("[%s:%d] GL call error: %s\n", file, line, err_str);
    } while(1);
}
