#include <render/kwindow.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

static GLFWwindow *window = 0;

static void cb(GLFWwindow *wnd, int key, int sc, int action, int mods) {
    (void) sc;
    (void) mods;
    
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(wnd, GLFW_TRUE);
    }
}

b8 kWindowCreate(void) {
    if (glfwInit() != GL_TRUE) return kfalse;
    window = glfwCreateWindow(1200, 1200, "TITLE", NULL, NULL);
    if (window == NULL) return kfalse;
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) return kfalse;

    glfwSetKeyCallback(window, &cb);

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
