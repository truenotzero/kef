
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include <dy/kdycode.h>
#include <render/kwindow.h>
#include <render/kgl.h>
#include <render/kprogram.h>
#include <render/kmesh.h>
#include "core/kmath.h"


// TODO
// vertex attribute bindings too...

kDylib dylib;
kRenderMesh mesh;
kMat4f rot;
kRenderProgram prog = {0};
struct {
    kMat4f mat;
} uniform;

extern int GetLastError(void);

void kWindowUpdate(void) {
    if (!kDyRequestReload(&dylib)) {
        int last_err = GetLastError();
        printf("Failed to dynamic reload: %d\n", last_err);
        exit(1);
    }
}

kDyfun dynamic_render = 0;

float a = 0;
void kWindowRender(void) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    if (dynamic_render) {
        // dynamic_render();
    }

    uniform.mat = kMatMul4f(uniform.mat, rot);
    assert(kRenderProgramUse(&prog));
    kRenderMeshDraw(&mesh);
}

#include "core/stb_image.h"
u0 texture_test(u0) {
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(ktrue);
    void *data = stbi_load("res/tex/wall_arrow.png", &width, &height, &nrChannels, 0);

    unsigned texobj;
    glGenTextures(1, &texobj);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texobj);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
}

void work(void) {
    printf("Hello, World!\n");
    dylib = (kDylib) {0};

    if (kWindowCreate()) {
        kDyBindLib(&dylib, "dynamic");
        kDyBindFun(&dylib, "render", &dynamic_render);

        assert(kRenderProgramCreate(&prog));
        assert(kRenderProgramLoad(&prog, "shaders/base"));
        // assert(kRenderProgramBindUniform(&prog, "brightness", 1, &uniform.brightness));
        kRenderProgramBindUniform(&prog, "uMat", 1, &uniform.mat);
        texture_test();

        uniform.mat = kMatIdentity4f();
        kVec3f axis = {0};
        axis.x = 1.0f;
        axis.y = 1.0f;
        axis.z = 1.0f;
        axis = kVecNorm3f(axis);
        rot = kMatRotate4f(axis, 0.0025f);
        // kMatPrint4f(rot);

        assert(kRenderMeshCreate(&mesh));
        assert(kRenderMeshLoad(&mesh, "res/mesh/square.obj"));

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
