
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
    kMat4f light;
} common;

kRenderProgram shadow_prog = {0};
unsigned shadow_fbo, shadow_map;

kRenderMesh tea_mesh, plane_mesh;
kRenderProgram obj_prog = {0};
struct {
    kMat4f model;
    kVec3f global_light_col;
    kVec3f global_light_pos;
    kVec3f view_pos;
} obj_uniform;

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
        if (kKeyIsPressed(K_KEY_LSHIFT)) {
            moveSpeed *= 3.0f;
        }
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

    // update camera view angle
    static kVec2f last_mouse_pos = {{-1, -1}};
    kVec2f mouse_pos = kMousePos();
    if (last_mouse_pos.x != -1 && last_mouse_pos.y != -1) {
        kVec2f delta_mouse = kVecSub(mouse_pos, last_mouse_pos);
        const f32 degsPerPixel = dynamic_sensitivity();
        kVec2f delta_view_angle = kVecScale(degsPerPixel, delta_mouse);
        cam = kCameraAddYawPitch(cam, -delta_view_angle.yaw, delta_view_angle.pitch);
    }
    last_mouse_pos = mouse_pos;
    obj_uniform.view_pos = cam.pos;
}

#include <math.h>

static u0 render_light(u0) {
    f32 scale = 0.15f;
    kMat4f m_scale = kMatScale(scale, scale, scale);

    kVec3f translate = K_VEC3F_UP;
    translate = kVecScale3f(6.6f, translate);
    static f32 a = 0.0f;
    f32 radius = 6.0f;
    translate.x = radius * sinf(a);
    // translate.z = 0.1f;
    translate.z = radius * cosf(a);
    a += 0.01f;
    kMat4f m_translate = kMatTranslate(translate.x, translate.y, translate.z);
    light_uniform.model = kMatMul4f(m_translate, m_scale);

    if (dynamic_global_light) {
        obj_uniform.global_light_col = dynamic_global_light();
    }

    obj_uniform.global_light_pos = translate;

    assert(kRenderProgramUse(&light_prog));
    kRenderMeshDraw(&light_mesh);
}

static u0 render_plane(kRenderProgram *prog) {
    f32 scale = 100.0f;
    kMat4f m_scale = kMatScale(scale, scale, scale);

    kVec3f translate = K_VEC3F_ZERO;
    kMat4f m_translate = kMatTranslate(translate.x, translate.y, translate.z);
    obj_uniform.model = kMatMul4f(m_translate, m_scale);

    assert(kRenderProgramUse(prog));
    kRenderMeshDraw(&plane_mesh);
}

static u0 render_tea(kRenderProgram *prog) {
    f32 scale = 1.0f;
    kMat4f m_scale = kMatScale(scale, scale, scale);

    kVec3f translate = K_VEC3F_UP;
    translate = kVecScale3f(1.0f, translate);
    kMat4f m_translate = kMatTranslate(translate.x, translate.y, translate.z);
    obj_uniform.model = kMatMul4f(m_translate, m_scale);

    assert(kRenderProgramUse(prog));
    kRenderMeshDraw(&tea_mesh);
}

int const shadow_size = 1024;
static u0 shadow_setup(unsigned *fbo, unsigned *map) {
    glGenFramebuffers(1, fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, *fbo);

    glGenTextures(1, map);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, *map);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadow_size, shadow_size, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, *map, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

static u0 shadow_cleanup(unsigned *fbo, unsigned *map) {
    glDeleteTextures(1, map);
    glDeleteFramebuffers(1, fbo);
}

static u0 draw_calls(kRenderProgram *prog) {
    render_tea(prog);
    render_plane(prog);
}

static kMat4f shadow_pass(unsigned fbo, int width, int height) {
    glViewport(0, 0, shadow_size, shadow_size);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glClear(GL_DEPTH_BUFFER_BIT);
    // do matrix stuff
    kMat4f proj = kMatPerspective(45.0f, 1.0f, 1.0f, 50.0f);
    kMat4f view = kMatLookAt(obj_uniform.global_light_pos, K_VEC3F_ZERO);
    common.light = kMatMul4f(proj, view);
    // render the scene from the light's perspective
    glCullFace(GL_FRONT);
    draw_calls(&shadow_prog);
    glCullFace(GL_BACK);

    // restore viewport
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, width, height);
    return common.light;
}

float a = 0;
void kWindowRender(void) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    common.view = kCameraViewMat(cam);
    common.proj = kMatPerspective(kDegf(45.0f), 1.0f, 0.1f, 1000.0f);
    render_light();

    shadow_pass(shadow_fbo, 1200, 1200);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, shadow_map);
    draw_calls(&obj_prog);
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

        assert(kRenderProgramCreate(&obj_prog));
        assert(kRenderProgramLoad(&obj_prog, "res/shader/phong"));
        kRenderProgramBindUniform(&obj_prog, "uModel", 1, &obj_uniform.model);
        kRenderProgramBindUniform(&obj_prog, "uGlobalLightCol", 1, &obj_uniform.global_light_col);
        kRenderProgramBindUniform(&obj_prog, "uGlobalLightPos", 1, &obj_uniform.global_light_pos);
        kRenderProgramBindUniform(&obj_prog, "uView", 1, &common.view);
        kRenderProgramBindUniform(&obj_prog, "uProj", 1, &common.proj);
        kRenderProgramBindUniform(&obj_prog, "uLight", 1, &common.light);

        assert(kRenderProgramCreate(&light_prog));
        assert(kRenderProgramLoad(&light_prog, "res/shader/point_light"));
        kRenderProgramBindUniform(&light_prog, "uModel", 1, &light_uniform.model);
        kRenderProgramBindUniform(&light_prog, "uView", 1, &common.view);
        kRenderProgramBindUniform(&light_prog, "uProj", 1, &common.proj);

        assert(kRenderProgramCreate(&shadow_prog));
        assert(kRenderProgramLoad(&shadow_prog, "res/shader/shadowmap"));
        kRenderProgramBindUniform(&shadow_prog, "uModel", 1, &obj_uniform.model);
        kRenderProgramBindUniform(&shadow_prog, "uLight", 1, &common.light);

        // kRenderTexture tex;
        // kRenderTextureCreate(&tex);
        // kRenderTextureLoad(&tex, "res/tex/wall_arrow.png");
        // kRenderTextureUse(&tex);

        assert(kRenderMeshCreate(&light_mesh));
        assert(kRenderMeshLoad(&light_mesh, "res/mesh/bulb.obj"));

        assert(kRenderMeshCreate(&tea_mesh));
        assert(kRenderMeshLoad(&tea_mesh, "res/mesh/cube.obj"));

        assert(kRenderMeshCreate(&plane_mesh));
        assert(kRenderMeshLoad(&plane_mesh, "res/mesh/plane.obj"));

        shadow_setup(&shadow_fbo, &shadow_map);

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glEnable(GL_FRAMEBUFFER_SRGB);
        glEnable(GL_MULTISAMPLE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        kWindowSetCursorVisible(kfalse);
        kKeyboardEnable();
        kWindowLoop();

        shadow_cleanup(&shadow_fbo, &shadow_map);

        // kRenderTextureDestroy(&tex);
        kRenderProgramDestroy(&shadow_prog);
        kRenderProgramDestroy(&obj_prog);
        kRenderProgramDestroy(&light_prog);
        kRenderMeshDestroy(&plane_mesh);
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
