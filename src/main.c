
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include <dy/kdycode.h>
#include <render/kwindow.h>
#include <render/kgl.h>
#include <render/kprogram.h>
#include <render/kmesh.h>
#include <render/ktexture.h>
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

#include <Windows.h>
//extern int GetLastError(void);

void kWindowUpdate(void) {
    if (!kDyRequestReload(&dylib)) {
        int last_err = GetLastError();
        printf("Failed to dynamic reload: %d\n", last_err);
        exit(1);
    }
}

kDyfun dynamic_render = 0;
kVec3f(*dynamic_translate)(u0) = 0;
kMat4f t;

float a = 0;
void kWindowRender(void) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    if (dynamic_render) {
        dynamic_render();
    }

    // if (dynamic_transform) {
    //     t = dynamic_transform();
    // } else {
    //     // t = kMatIdentity4f();
    //     kVec3f axis = {0};
    //     axis.x = 1.0f;
    //     t = kMatMul4f(t, kMatRotate4f(axis, 0.001f));
    // }
    f32 scale = 0.3f;
    kVec3f trans = { 0.0f, 0.0f, 0.0f };
    if (dynamic_translate) {
        trans = dynamic_translate();
    }
    kVec3f x_axis = {1.0f, 0.0f, 0.0f};
    kVec3f y_axis = {0.0f, 1.0f, 0.0f};
    kVec3f z_axis = {0.0f, 0.0f, 1.0f};
    kMat4f m_rotate = kMatRotate4f(x_axis, kDegf(30.0f));
    kMat4f m_trans = kMatTrans4f(trans.x, trans.y, trans.z);
    kMat4f m_scale = kMatScale4f(scale, scale, scale);
    static f32 a = 0.0f;
    kMat4f m_spin = kMatRotate4f(y_axis, a);
    a += 0.01f;
    kMat4f m_perspective = kMatFrustum4f(1.0f, 1.0f, 1.0f, 100.0f);
    t = kMatMul4f(kMatMul4f(kMatMul4f(kMatMul4f(m_perspective,m_scale), m_trans), m_rotate), m_spin);
    uniform.mat = t;
    assert(kRenderProgramUse(&prog));
    kRenderMeshDraw(&mesh);
}

void work(void) {
    printf("Hello, World!\n");
    dylib = (kDylib) {0};


    if (kWindowCreate()) {
        kDyBindLib(&dylib, "dynamic");
        kDyBindFun(&dylib, "render", &dynamic_render);
        kDyBindFun(&dylib, "translate", (kDyfun *) &dynamic_translate);

        assert(kRenderProgramCreate(&prog));
        assert(kRenderProgramLoad(&prog, "shaders/base"));
        // assert(kRenderProgramBindUniform(&prog, "brightness", 1, &uniform.brightness));
        kRenderProgramBindUniform(&prog, "uMat", 1, &uniform.mat);

        uniform.mat = kMatIdentity4f();
        kVec3f axis = {0};
        axis.y = 1.0f;
        axis = kVecNorm3f(axis);
        rot = kMatRotate4f(axis, 0.0025f);
        // kMatPrint4f(rot);

        kRenderTexture tex;
        kRenderTextureCreate(&tex);
        kRenderTextureLoad(&tex, "res/tex/wall_arrow.png");
        kRenderTextureUse(&tex);

        assert(kRenderMeshCreate(&mesh));
        assert(kRenderMeshLoad(&mesh, "res/mesh/teapot_triangles.obj"));

        kWindowLoop();

        kRenderTextureDestroy(&tex);
        kRenderProgramDestroy(&prog);
        kRenderMeshDestroy(&mesh);
    }

    kDyCleanup(&dylib);
    kWindowDestroy();

}

__declspec(dllexport) void success(void) {
    printf("eureka!");
}


int main(int argc, char *argv[]) {
    (void) argc;
    (void) argv;

    work();

    // typedef void (*fptr)(void);
    // fptr pSuccess = (fptr) GetProcAddress(GetModuleHandle(NULL), "success");
    // pSuccess();

    return 0;
}
