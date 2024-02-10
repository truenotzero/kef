
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
    kMat4f model;
    kMat4f view;
    kMat4f projection;
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
f32 (*dynamic_scale)(u0) = 0;
kVec3f (*dynamic_translate)(u0) = 0;
kVec3f (*dynamic_rotate_axis)(u0) = 0;
f32 (*dynamic_rotate_angle)(u0) = 0;
kMat4f t;

float a = 0;
void kWindowRender(void) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (dynamic_render) {
        dynamic_render();
    }

    f32 scale = 0.5f;
    if (dynamic_scale) {
        scale = dynamic_scale();
    }
    kMat4f m_scale = kMatScale4f(scale, scale, scale);

    kVec3f translate = K_VEC3F_ZERO;
    if (dynamic_translate) {
        translate = dynamic_translate();
    }
    kMat4f m_translate = kMatTranslate4f(translate.x, translate.y, translate.z);

    kVec3f rotate_axis = K_VEC3F_FORWARD;
    if (dynamic_rotate_axis) {
        rotate_axis = dynamic_rotate_axis();
        rotate_axis = kVecNorm3f(rotate_axis);
    }
    f32 rotate_angle = 0.0f;
    if (dynamic_rotate_angle) {
        rotate_angle = dynamic_rotate_angle();
    }
    kMat4f m_rotate = kMatRotate4f(rotate_axis, rotate_angle);

    uniform.model = kMatMul4f(m_translate, m_scale, m_rotate);

    assert(kRenderProgramUse(&prog));
    kRenderMeshDraw(&mesh);
}

void work(void) {
    printf("Hello, World!\n");
    dylib = (kDylib) {0};

    if (kWindowCreate()) {
        kDyBindLib(&dylib, "dynamic");
        kDyBindFun(&dylib, "render", &dynamic_render);
        kDyBindFun(&dylib, "scale", (kDyfun *) &dynamic_scale);
        kDyBindFun(&dylib, "translate", (kDyfun *) &dynamic_translate);
        kDyBindFun(&dylib, "rotate_axis", (kDyfun *) &dynamic_rotate_axis);
        kDyBindFun(&dylib, "rotate_angle", (kDyfun *) &dynamic_rotate_angle);

        assert(kRenderProgramCreate(&prog));
        assert(kRenderProgramLoad(&prog, "shaders/base"));
        kRenderProgramBindUniform(&prog, "uModel", 1, &uniform.model);
        kRenderProgramBindUniform(&prog, "uView", 1, &uniform.view);
        kRenderProgramBindUniform(&prog, "uProjection", 1, &uniform.projection);

        uniform.view = kMatTranslate4f(0.0f, 0.0f, -3.0f);
        uniform.projection = kMatPerspective4f(kDegf(45.0f), 1.0f, 0.1f, 1000.0f);

        kRenderTexture tex;
        kRenderTextureCreate(&tex);
        kRenderTextureLoad(&tex, "res/tex/wall_arrow.png");
        kRenderTextureUse(&tex);

        assert(kRenderMeshCreate(&mesh));
        assert(kRenderMeshLoad(&mesh, "res/mesh/teapot_triangles.obj"));

        glEnable(GL_DEPTH_TEST);
        kWindowLoop();

        kRenderTextureDestroy(&tex);
        kRenderProgramDestroy(&prog);
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
