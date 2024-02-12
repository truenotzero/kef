
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

#include <GLFW/glfw3.h>

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

kCamera cam = {0};
extern GLFWwindow *window;

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

// model
f32 (*dynamic_scale)(u0) = 0;
kVec3f (*dynamic_translate)(u0) = 0;
kVec3f (*dynamic_rotate_axis)(u0) = 0;
f32 (*dynamic_rotate_angle)(u0) = 0;

// view
kVec3f (*dynamic_cam_pos)(u0) = 0;
kVec3f (*dynamic_cam_look_at)(u0) = 0;
f32 (*dynamic_sensitivity)(u0) = 0;

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
    kMat4f m_scale = kMatScale(scale, scale, scale);

    kVec3f translate = K_VEC3F_ZERO;
    if (dynamic_translate) {
        translate = dynamic_translate();
    }
    kMat4f m_translate = kMatTranslate(translate.x, translate.y, translate.z);

    kVec3f rotate_axis = K_VEC3F_FORWARD;
    if (dynamic_rotate_axis) {
        rotate_axis = dynamic_rotate_axis();
        rotate_axis = kVecNorm3f(rotate_axis);
    }
    f32 rotate_angle = 0.0f;
    if (dynamic_rotate_angle) {
        rotate_angle = dynamic_rotate_angle();
    }
    kMat4f m_rotate = kMatRotate(rotate_axis, rotate_angle);

    uniform.model = kMatMul4f(m_translate, m_scale, m_rotate);


    // kVec3f cam_pos = K_VEC3F_ZERO;
    // if (dynamic_cam_pos) {
    //     cam_pos = dynamic_cam_pos();
    // }
    // kVec3f cam_look_at = K_VEC3F_FORWARD;
    // if (dynamic_cam_look_at) {
    //     cam_look_at = dynamic_cam_look_at();
    // }
    // uniform.view = kMatLookAt(cam_pos, cam_look_at);
    const float yawSpeed = 0.01f;
    kVec3f moveDirection = {0};
    if (kKeyIsPressed(K_KEY_W)) {
        moveDirection = kVecSub3f(moveDirection, cam.r_forward);
    } else if (kKeyIsPressed(K_KEY_S)) {
        moveDirection = kVecAdd3f(moveDirection, cam.r_forward);
    } else if (kKeyIsPressed(K_KEY_A)) {
        moveDirection = kVecSub3f(moveDirection, cam.r_right);
        // cam = kCameraAddYawPitch(cam, +yawSpeed, 0.0f);
    } else if (kKeyIsPressed(K_KEY_D)) {
        moveDirection = kVecAdd3f(moveDirection, cam.r_right);
        // cam = kCameraAddYawPitch(cam, -yawSpeed, 0.0f);
    }
    static const f32 moveSpeed = 0.1f;
    if (!kVecIsZero3f(moveDirection)) {
        kVec3f deltaPos = kVecScale3f(moveSpeed, kVecNorm3f(moveDirection));
        cam.pos = kVecAdd3f(cam.pos, deltaPos);
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
    
    f32 yawDeg = cam.r_yaw * (180.0f / K_PIF);
    printf("x,y,z(%.2f,%.2f,%.2f) yaw(%.2f)\n", cam.pos.x, cam.pos.y, cam.pos.z, yawDeg);
    uniform.view = kCameraViewMat(cam);
    assert(kRenderProgramUse(&prog));
    kRenderMeshDraw(&mesh);
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

        kDyBindFun(&dylib, "cam_pos", (kDyfun *) &dynamic_cam_pos);
        kDyBindFun(&dylib, "cam_look_at", (kDyfun *) &dynamic_cam_look_at);
        kDyBindFun(&dylib, "sensitivity", (kDyfun *) &dynamic_sensitivity);

        assert(kRenderProgramCreate(&prog));
        assert(kRenderProgramLoad(&prog, "shaders/base"));
        kRenderProgramBindUniform(&prog, "uModel", 1, &uniform.model);
        kRenderProgramBindUniform(&prog, "uView", 1, &uniform.view);
        kRenderProgramBindUniform(&prog, "uProjection", 1, &uniform.projection);

        uniform.projection = kMatPerspective(kDegf(45.0f), 1.0f, 0.1f, 1000.0f);

        kRenderTexture tex;
        kRenderTextureCreate(&tex);
        kRenderTextureLoad(&tex, "res/tex/wall_arrow.png");
        kRenderTextureUse(&tex);

        assert(kRenderMeshCreate(&mesh));
        assert(kRenderMeshLoad(&mesh, "res/mesh/teapot_triangles.obj"));

        glEnable(GL_DEPTH_TEST);
        kWindowSetCursorVisible(kfalse);
        kKeyboardEnable();
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
