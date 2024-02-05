
#include <stdio.h>
#include <stdlib.h>
#include <dy/kdycode.h>
#include <render/kwindow.h>
#include <render/kgl.h>
#include <render/kprogram.h>
#include <render/kmesh.h>


// TODO
// vertex attribute bindings too...

kDylib dylib;
kRenderMesh mesh;

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
        // dynamic_render();
    }

    kRenderMeshDraw(&mesh);
}

void work(void) {
    printf("Hello, World!\n");
    dylib = (kDylib) {0};

    if (kWindowCreate()) {
        kDyBindLib(&dylib, "dynamic");
        kDyBindFun(&dylib, "render", &dynamic_render);

        kRenderProgram prog;
        kRenderProgramCreate(&prog);
        kRenderProgramLoad(&prog, "shaders/base");
        kRenderProgramUse(&prog);

        kRenderMeshCreate(&mesh);
        kRenderMeshLoad(&mesh, "res/mesh/square.obj");

        kWindowLoop();

        // kRenderProgramDestroy(&prog);
        kRenderMeshDestroy(&mesh);
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
