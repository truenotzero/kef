#include <dy/kdylib.h>
#include <ktype.h>
#include <stdio.h>

/*
before reloading:
rename the old dynamic lib
recompile source -> dylib
*/

#include <GL/glew.h>

static c_str VERTEX_SHADER =
"#version 450 core\n"
"layout(location=0) in vec4 aPos;\n"
"void main() {\n"
"   gl_Position = aPos;\n"
"}\n"
;

static c_str FRAGMENT_SHADER =
"#version 450 core\n"
"out vec3 col;\n"
"void main() {\n"
    "col = vec3(1.0f, 0.0f, 0.0f);\n"
"}\n"
;

static void check_shader(GLuint shader) {
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_TRUE) return;

    GLint type;
    glGetShaderiv(shader, GL_SHADER_TYPE, &type);
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
    glGetShaderInfoLog(shader, sizeof(buffer), 0, buffer);
    printf("Failed to compile %s shader: %s\n", shader_type_string, buffer);
}

static GLuint vao, vbo, program;

KDYFUN void dylib_setup(void) {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    float vertex_data[] = {
        0.0f, 0.75f,
        -0.75f, -0.75f,
        0.75f, -0.75f,
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *) 0);

    program = glCreateProgram();
    GLuint vert = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert, 1, &VERTEX_SHADER, NULL);
    glCompileShader(vert);
    check_shader(vert);
    glAttachShader(program, vert);

    GLuint frag = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag, 1, &FRAGMENT_SHADER, NULL);
    glCompileShader(frag);
    check_shader(frag);
    glAttachShader(program, frag);

    glLinkProgram(program);
    glDeleteShader(frag);
    glDeleteShader(vert);
    glUseProgram(program);
}

KDYFUN void dylib_cleanup(void) {
    glDeleteProgram(program);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}

KDYFUN void render(void) {
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(program);
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}
