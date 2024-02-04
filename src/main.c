
#include <stdio.h>
#include <stdlib.h>
#include <dy/kdycode.h>
#include <render/kwindow.h>
#include <render/kgl.h>

// uses stdio.h
#include <string.h>
typedef struct kRenderProgram {
    GLuint vao;
    GLuint program;
} kRenderProgram;

b8 kRenderProgramCreate(kRenderProgram *self) {
    if (!self) { return kfalse; }
    KGL(glGenVertexArrays(1, &self->vao));
    KGL(self->program = glCreateProgram());
    return ktrue;
}

b8 kRenderProgramDestroy(kRenderProgram *self) {
    if (!self) { return kfalse; }
    KGL(glDeleteProgram(self->program));
    self->program = 0;

    KGL(glDeleteVertexArrays(1, &self->vao));
    self->vao = 0;
    return kfalse;
}

b8 kRenderProgramUse(kRenderProgram *self) {
    GLuint vao, program;
    if (!self) {
        vao = 0;
        program = 0;
    } else {
        if (self->vao == 0 || self->program == 0) { return kfalse; }
        vao = self->vao;
        program = self->program;
    }

    KGL(glBindVertexArray(vao));
    KGL(glUseProgram(program));
    return ktrue;
}

static b8 check_shader(GLuint shader) {
    GLint status;
    KGL(glGetShaderiv(shader, GL_COMPILE_STATUS, &status));
    if (status == GL_TRUE) { return ktrue; }

    GLint type;
    KGL(glGetShaderiv(shader, GL_SHADER_TYPE, &type));
    c_str shader_type_string;
    switch (type) {
        case GL_VERTEX_SHADER:
            shader_type_string = "vertex";
            break;
        case GL_FRAGMENT_SHADER:
            shader_type_string = "fragment";
            break;
        default:
            shader_type_string = "unknown";
            break;
    }
    char buffer[1024];
    KGL(glGetShaderInfoLog(shader, sizeof(buffer), 0, buffer));
    printf("Failed to compile %s shader: %s\n", shader_type_string, buffer);
    return kfalse;
}

static b8 check_program(GLuint program) {
    GLint status;
    KGL(glGetProgramiv(program, GL_LINK_STATUS, &status));
    if (status == GL_TRUE) { return ktrue; }

    char buffer[1024];
    KGL(glGetProgramInfoLog(program, sizeof(buffer), 0, buffer));
    printf("Failed to link program: %s\n", buffer);
    return kfalse;
}

// loads, compiles and links a shader program
// a shader is a directory with each shader called:
// | my-shader
// |- vertex.glsl
// |- fragment.glsl
// and so on
b8 kRenderProgramLoad(kRenderProgram *self, c_str program_name) {
    if (!self) { return kfalse; }
    // for now support vertex & fragment shaders only

    GLenum supported_shader_types[] = { GL_VERTEX_SHADER, GL_FRAGMENT_SHADER };
    c_str shader_filenames[] = { "vertex.glsl", "fragment.glsl" };

    for (unsigned i = 0; i < sizeof(supported_shader_types) / sizeof(*supported_shader_types); ++i) {
        GLenum shader_type = supported_shader_types[i];
        c_str shader_filename = shader_filenames[i];

        int const filename_buffer_sz = 256;
        char filename_buffer[filename_buffer_sz];
        // check that the directory name + '/' + filename do not overrun the buffer
        // also consider that one element is reserved for the null terminator
        // hence >= and not >
        if (strlen(program_name) + 1 + strlen(shader_filename) >= filename_buffer_sz) { return kfalse; }
        strncpy(filename_buffer, program_name, sizeof(filename_buffer));
        strcat(filename_buffer, "/");
        strcat(filename_buffer, shader_filename);

        FILE *f = fopen(filename_buffer, "r");
        // file not found, skip this shader
        if (!f) {
            printf("KGL: For program: %s, %s not found, skipping...\n", program_name, shader_filename);
            continue;
        }

        int const shader_src_sz = 8 * 1024;
        char shader_src[shader_src_sz];
        unsigned shader_src_len = fread(shader_src, sizeof(*shader_src), shader_src_sz, f);
        if (shader_src_len == shader_src_sz) {
            printf("KGL: For shader: %s, read buffer full, shader possibly truncated\n", filename_buffer);
        }
        fclose(f);


        GLuint shader = KGL(glCreateShader(shader_type));
        c_str sources[] = { shader_src };
        GLint lengths[] = { shader_src_len };
        KGL(glShaderSource(shader, 1, sources, lengths));
        KGL(glCompileShader(shader));
        if (!check_shader(shader)) {
            printf("KGL: For shader: %s, compilation failed!\n", filename_buffer);
            KGL(glDeleteShader(shader_type));
            return kfalse;
        }
        KGL(glAttachShader(self->program, shader));
        KGL(glDeleteShader(shader));
    }

    KGL(glLinkProgram(self->program));
    if (!check_program(self->program)) {
        printf("KGL: For program: %s, linking failed!\n", program_name);
        return kfalse;
    }

    return ktrue;
}

// TODO
// vertex attribute bindings too...

kDylib dylib;

extern int GetLastError(void);

void kWindowUpdate(void) {
    if (!kDyRequestReload(&dylib)) {
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

struct vertex {
    float pos[2];
    float col[3];
};
void work(void) {
    printf("Hello, World!\n");
    dylib = (kDylib) {0};

    if (kWindowCreate()) {
        kDyBindLib(&dylib, "dynamic");
        kDyBindFun(&dylib, "render", &dynamic_render);

        GLuint vbo;
        KGL(glGenBuffers(1, &vbo));
        KGL(glBindBuffer(GL_ARRAY_BUFFER, vbo));

        kRenderProgram prog;
        kRenderProgramCreate(&prog);
        kRenderProgramLoad(&prog, "shaders/base");
        kRenderProgramUse(&prog);

        KGL(glEnableVertexAttribArray(0));
        KGL(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(struct vertex), (void *) offsetof(struct vertex, pos)));
        KGL(glEnableVertexAttribArray(1));
        KGL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(struct vertex), (void *) offsetof(struct vertex, col)));

        kWindowLoop();

        kRenderProgramDestroy(&prog);
        KGL(glDeleteBuffers(1, &vbo));
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
