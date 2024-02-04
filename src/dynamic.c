#include <dy/kdylib.h>
#include <ktype.h>
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

static GLuint ebo;

struct vertex {
    float pos[2];
    float col[3];
};

typedef void (__stdcall *glDrawElements_t)(GLenum mode, GLsizei count, GLenum type, const void *indices);
glDrawElements_t _glDrawElements;

KDYFUN void dylib_setup(void) {
    struct vertex vertex_data[] = {
        [0] = {
            { -0.5f, 0.5f },
            { 1.0f, 0.0, 0.0f },
        },
        [1] = {
            { 0.5f, 0.5f },
            { 0.0f, 1.0, 0.0f },
        },
        [2] = {
            { 0.5f, -0.5f },
            { 0.0f, 0.0, 1.0f },
        },
        [3] = {
            { -0.5f, -0.5f },
            { 1.0f, 1.0, 0.0f },
        },
    };

    KGL(glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW));

    GLuint indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    KGL(glGenBuffers(1, &ebo));
    KGL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo));
    KGL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));
}

KDYFUN void dylib_cleanup(void) {
    KGL(glDeleteBuffers(1, &ebo));
}

KDYFUN void render(void) {
    KGL(glClearColor(0.2f, 0.2f, 0.2f, 1.0f));
    KGL(glClear(GL_COLOR_BUFFER_BIT));

    KGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *) 0));
    // KGL(glDrawArrays(GL_TRIANGLE_FAN, 0, 4));
}
