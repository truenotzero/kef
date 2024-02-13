#include <dy/kdylib.h>
#include <ktype.h>
#include <core/kmath.h>
#include <stdio.h>

/*
before reloading:
rename the old dynamic lib
recompile source -> dylib
*/

#include <render/kgl.h>

u0 k__renderCheckGlImpl(c_str file, int line) {
    char const *err_str;
    switch (glGetError()) {
        case GL_NO_ERROR:
            return;
        case GL_INVALID_ENUM:
            err_str = "GL_INVALID_ENUM";
            break;
        case GL_INVALID_VALUE:
            err_str = "GL_INVALID_VALUE";
            break;
        case GL_INVALID_OPERATION:
            err_str = "GL_INVALID_OPERATION";
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            err_str = "GL_INVALID_FRAMEBUFFER_OPERATION";
            break;
        case GL_OUT_OF_MEMORY:
            err_str = "GL_OUT_OF_MEMORY";
            break;
        case GL_STACK_OVERFLOW:
            err_str = "GL_STACK_OVERFLOW";
            break;
        case GL_STACK_UNDERFLOW:
            err_str = "GL_STACK_UNDERFLOW";
            break;
    }
    printf("[%s:%d] GL call error: %s\n", file, line, err_str);
}

struct vertex {
    float pos[2];
    float col[3];
};

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
typedef kMat4f (*kMatIdentity4f_t)(u0);
kMatIdentity4f_t identity = 0;

KDYFUN void dylib_setup(void) {
        // KGL(glDisable(GL_CULL_FACE));
        // identity = (kMatIdentity4f_t) GetProcAddress(GetModuleHandle("main.exe"), "kMatIdentity4f");
        // assert(identity);
        // KGL(glEnable(GL_CULL_FACE));
}

KDYFUN void dylib_cleanup(void) {
}

KDYFUN void render(void) {
}

#include <math.h>

KDYFUN f32 scale(u0) {
    return 0.25f;
}

KDYFUN kVec3f translate(u0) {
    f32 x = 0.0f;
    f32 y = -0.5f;
    f32 z = 0.0f;
    return (kVec3f) {{ x, y, z }};
}

#include <math.h>
KDYFUN kVec3f rotate_axis(u0) {
    kVec3f v = K_VEC3F_FORWARD;
    return v;
}

KDYFUN f32 rotate_angle(u0) {
    static f32 a = 0;
    a -= 0.01f;
    return a;
}

KDYFUN kVec3f cam_pos(u0) {
    static f32 a = 0.0f;

    f32 spinRadius = 0.0f;
    f32 x = 0.0f + spinRadius * sinf(a);
    f32 y = 0.0f;
    f32 z = -3.0f + spinRadius * cosf(a);

    a += 0.003f;

    return (kVec3f) {{
        .x = x,
        .y = y,
        .z = z,
    }};
}

KDYFUN f32 move_speed(u0) {
    return 0.05f;
}

KDYFUN f32 sensitivity(u0) {
    return 0.0006f;
}

KDYFUN kVec3f global_light(u0) {
    f32 grayscale = 1.0f;
    kVec3f light;
    light.r = grayscale; 
    light.g = grayscale; 
    light.b = grayscale; 
    return light;
}
