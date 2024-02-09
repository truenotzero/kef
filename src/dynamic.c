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
#include <assert.h>
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

KDYFUN kMat4f transform(u0) {
    return identity();
}
