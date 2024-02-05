
#include <stdio.h>
#include <stdlib.h>
#include <dy/kdycode.h>
#include <render/kwindow.h>
#include <render/kgl.h>
#include <render/kprogram.h>


// TODO
// vertex attribute bindings too...

kDylib dylib;

extern int GetLastError(void);

void kWindowUpdate(void) {
    if (!kDyRequestReload(&dylib)) {
        int last_err = GetLastError();
        printf("Failed to dynamic reload: %d\n", last_err);
        exit(1);
    }
}

kDyfun dynamic_render = 0;

void kWindowRender(void) {
    if (dynamic_render) {
        dynamic_render();
    }
}

struct vertex {
    float pos[2];
    float col[3];
};
void work(void) {
    printf("Hello, World!\n");
    dylib = (kDylib) {0};

    if (kWindowCreate()) {
        kDyBindLib(&dylib, "dynamic");
        kDyBindFun(&dylib, "render", &dynamic_render);

        GLuint vbo;
        KGL(glGenBuffers(1, &vbo));
        KGL(glBindBuffer(GL_ARRAY_BUFFER, vbo));

        kRenderProgram prog;
        kRenderProgramCreate(&prog);
        kRenderProgramLoad(&prog, "shaders/base");
        kRenderProgramVertAttrib(&prog, 0, kfalse, struct vertex, pos);
        kRenderProgramVertAttrib(&prog, 1, kfalse, struct vertex, col);
        kRenderProgramUse(&prog);

        kWindowLoop();

        kRenderProgramDestroy(&prog);
        KGL(glDeleteBuffers(1, &vbo));
    }

    kDyCleanup(&dylib);
    kWindowDestroy();

}
int main(int argc, char *argv[]) {
    (void) argc;
    (void) argv;
    
   work();

    return 0;
}
