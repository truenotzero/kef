
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <dy/kdycode.h>
#include <render/kwindow.h>
#include <render/kgl.h>
#include <render/kprogram.h>
#include <render/kmesh.h>
#include <render/ktexture.h>
#include <render/kcamera.h>
#include <input/kkey.h>
#include "core/kmath.h"

kDylib dylib;

struct {
    kMat4f view;
    kMat4f proj;
} common;

kRenderMesh tea_mesh;
kRenderProgram tea_prog = {0};
struct {
    kMat4f model;
    kVec3f global_light_col;
    kVec3f global_light_pos;
    kVec3f view_pos;
} tea_uniform;

kRenderMesh light_mesh;
kRenderProgram light_prog = {0};
struct {
    kMat4f model;
} light_uniform;

kCamera cam = {0};

kDyfun dynamic_render = 0;

// model
f32 (*dynamic_scale)(u0) = 0;
kVec3f (*dynamic_translate)(u0) = 0;
kVec3f (*dynamic_rotate_axis)(u0) = 0;
f32 (*dynamic_rotate_angle)(u0) = 0;

// view
f32 (*dynamic_move_speed)(u0) = 0;
f32 (*dynamic_sensitivity)(u0) = 0;

// global illumination
kVec3f (*dynamic_global_light)(u0) = 0;

extern int GetLastError(void);

// handles camera movement
static kVec3f cam_move(u0) {
    kVec3f moveDirection = {0};
    if (kKeyIsPressed(K_KEY_W)) {
        moveDirection = kVecSub3f(moveDirection, cam.r_forward);
    }
    if (kKeyIsPressed(K_KEY_S)) {
        moveDirection = kVecAdd3f(moveDirection, cam.r_forward);
    }
    if (kKeyIsPressed(K_KEY_A)) {
        moveDirection = kVecSub3f(moveDirection, cam.r_right);
    }
    if (kKeyIsPressed(K_KEY_D)) {
        moveDirection = kVecAdd3f(moveDirection, cam.r_right);
    } 
    if (kKeyIsPressed(K_KEY_SPACE)) {
        moveDirection = kVecAdd3f(moveDirection, cam.r_up);
    }
    if (kKeyIsPressed(K_KEY_LCTRL)) {
        moveDirection = kVecSub3f(moveDirection, cam.r_up);
    }

    if (!kVecIsZero3f(moveDirection)) {
        f32 moveSpeed = dynamic_move_speed();
        moveDirection = kVecScale3f(moveSpeed, kVecNorm3f(moveDirection));
    }
    return moveDirection;
}

void kWindowUpdate(void) {
    if (!kDyRequestReload(&dylib)) {
        int last_err = GetLastError();
        printf("Failed to dynamic reload: %d\n", last_err);
        exit(1);
    }

    // update camera movement
    cam.pos = kVecAdd3f(cam.pos, cam_move());

}

#include <math.h>

static u0 render_light(u0) {
    f32 scale = 0.15f;
    kMat4f m_scale = kMatScale(scale, scale, scale);

    kVec3f translate = K_VEC3F_UP;
    translate = kVecScale3f(10.0f, translate);
    static f32 a = 0.0f;
    f32 radius = 10.0f;
    translate.x = radius * sinf(a);
    translate.z = radius * cosf(a);
    a += 0.01f;
    kMat4f m_translate = kMatTranslate(translate.x, translate.y, translate.z);
    light_uniform.model = kMatMul4f(m_translate, m_scale);
    tea_uniform.global_light_pos = translate;

    assert(kRenderProgramUse(&light_prog));
    kRenderMeshDraw(&light_mesh);
}

static u0 render_tea(u0) {
    f32 scale = 1.0f;
    kMat4f m_scale = kMatScale(scale, scale, scale);

    kVec3f translate = K_VEC3F_ZERO;
    kMat4f m_translate = kMatTranslate(translate.x, translate.y, translate.z);
    tea_uniform.model = kMatMul4f(m_translate, m_scale);

    if (dynamic_global_light) {
        tea_uniform.global_light_col = dynamic_global_light();
    }

    assert(kRenderProgramUse(&tea_prog));
    kRenderMeshDraw(&tea_mesh);
}

float a = 0;
void kWindowRender(void) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (dynamic_render) {
        dynamic_render();
    }


    static kVec2f last_mouse_pos = {{-1, -1}};
    kVec2f mouse_pos = kMousePos();
    if (last_mouse_pos.x != -1 && last_mouse_pos.y != -1) {
        kVec2f delta_mouse = kVecSub(mouse_pos, last_mouse_pos);
        const f32 degsPerPixel = dynamic_sensitivity();
        kVec2f delta_view_angle = kVecScale(degsPerPixel, delta_mouse);
        cam = kCameraAddYawPitch(cam, -delta_view_angle.yaw, delta_view_angle.pitch);
    }
    last_mouse_pos = mouse_pos;
    
    tea_uniform.view_pos = cam.pos;
    common.view = kCameraViewMat(cam);
    common.proj = kMatPerspective(kDegf(45.0f), 1.0f, 0.1f, 1000.0f);
    render_light();
    render_tea();
}

void work(void) {
    printf("Hello, World!\n");
    dylib = (kDylib) {0};

    cam.pos.x = 0.0f;
    cam.pos.y = 0.0f;
    cam.pos.z = 3.0f;

    // cam = kCameraSetYawPitch(cam, kDegf(-90.0f), 0.0f);

    if (kWindowCreate()) {
        kDyBindLib(&dylib, "dynamic");
        kDyBindFun(&dylib, "render", &dynamic_render);

        kDyBindFun(&dylib, "scale", (kDyfun *) &dynamic_scale);
        kDyBindFun(&dylib, "translate", (kDyfun *) &dynamic_translate);
        kDyBindFun(&dylib, "rotate_axis", (kDyfun *) &dynamic_rotate_axis);
        kDyBindFun(&dylib, "rotate_angle", (kDyfun *) &dynamic_rotate_angle);

        kDyBindFun(&dylib, "move_speed", (kDyfun *) &dynamic_move_speed);
        kDyBindFun(&dylib, "sensitivity", (kDyfun *) &dynamic_sensitivity);

        kDyBindFun(&dylib, "global_light", (kDyfun *) &dynamic_global_light);

        assert(kRenderProgramCreate(&tea_prog));
        assert(kRenderProgramLoad(&tea_prog, "res/shader/base"));
        kRenderProgramBindUniform(&tea_prog, "uModel", 1, &tea_uniform.model);
        kRenderProgramBindUniform(&tea_prog, "uGlobalLightCol", 1, &tea_uniform.global_light_col);
        kRenderProgramBindUniform(&tea_prog, "uGlobalLightPos", 1, &tea_uniform.global_light_pos);
        kRenderProgramBindUniform(&tea_prog, "uView", 1, &common.view);
        kRenderProgramBindUniform(&tea_prog, "uProj", 1, &common.proj);

        assert(kRenderProgramCreate(&light_prog));
        assert(kRenderProgramLoad(&light_prog, "res/shader/light"));
        kRenderProgramBindUniform(&light_prog, "uModel", 1, &light_uniform.model);
        kRenderProgramBindUniform(&light_prog, "uView", 1, &common.view);
        kRenderProgramBindUniform(&light_prog, "uProj", 1, &common.proj);

        // kRenderTexture tex;
        // kRenderTextureCreate(&tex);
        // kRenderTextureLoad(&tex, "res/tex/wall_arrow.png");
        // kRenderTextureUse(&tex);

        assert(kRenderMeshCreate(&light_mesh));
        assert(kRenderMeshLoad(&light_mesh, "res/mesh/bulb.obj"));

        assert(kRenderMeshCreate(&tea_mesh));
        assert(kRenderMeshLoad(&tea_mesh, "res/mesh/teapot_smooth.obj"));

        glEnable(GL_DEPTH_TEST);
        kWindowSetCursorVisible(kfalse);
        kKeyboardEnable();
        kWindowLoop();

        // kRenderTextureDestroy(&tex);
        kRenderProgramDestroy(&tea_prog);
        kRenderProgramDestroy(&light_prog);
        kRenderMeshDestroy(&tea_mesh);
        kRenderMeshDestroy(&light_mesh);
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
