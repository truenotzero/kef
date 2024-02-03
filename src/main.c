
#include <stdio.h>
#include <stdlib.h>
#include <dy/kdycode.h>
#include <render/kwindow.h>

kDylib dylib;

extern int GetLastError(void);

void kWindowUpdate(void) {
    if (!kDyReload(&dylib)) {
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

void work(void) {
    printf("Hello, World!\n");
    dylib = (kDylib) {0};

    if (kWindowCreate()) {
        kDyBindLib(&dylib, "dynamic");
        kDyBindFun(&dylib, "render", &dynamic_render);
        if (!kDyReload(&dylib)) {
            printf("Failed initial loading of library\n");
        }

        kWindowLoop();
    }

    //kDyUnload(&dylib);
    kWindowDestroy();

}

int main(int argc, char *argv[]) {
    (void) argc;
    (void) argv;
    
    work();

    return 0;
}
