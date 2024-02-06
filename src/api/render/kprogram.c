#include <render/kprogram.h>

#include <stdio.h>
#include <string.h>

#include <render/kgl.h>

b8 kRenderProgramCreate(kRenderProgram *self) {
    if (!self) { return kfalse; }
    // KGL(glGenVertexArrays(1, &self->vao));
    KGL(self->program = glCreateProgram());
    return ktrue;
}

b8 kRenderProgramDestroy(kRenderProgram *self) {
    if (!self) { return kfalse; }
    KGL(glDeleteProgram(self->program));
    self->program = 0;

    // KGL(glDeleteVertexArrays(1, &self->vao));
    // self->vao = 0;
    return kfalse;
}

b8 kRenderProgramBindUniformImpl(kRenderProgram *self, c_str name, void *data, enum kType type, int num_elems) {
    if (!self) { return kfalse; }

    GLint location = KGL(glGetUniformLocation(self->program, name));
    if (location == -1) { return kfalse; }
    self->uniforms[self->num_uniforms] = (kRenderProgramUniform) {
        .data = data,
        .type = type,
        .num_elements = num_elems,
        .location = location,
    };

    self->num_uniforms += 1;
    return ktrue;
}

b8 kRenderProgramUse(kRenderProgram *self) {
    if (!self) {
        KGL(glUseProgram(0));
        return ktrue;
    }

    KGL(glUseProgram(self->program));
    for (int i = 0; i < self->num_uniforms; ++i) {
        kRenderProgramUniform *u = &self->uniforms[i];
        switch (u->type) {
            case KTYPE_F32:
                KGL(glUniform1fv(u->location, u->num_elements, u->data));
                break;
            case KTYPE_VEC3F:
                KGL(glUniform3fv(u->location, u->num_elements, u->data));
                break;
            case KTYPE_MAT4F:
                KGL(glUniformMatrix4fv(u->location, u->num_elements, GL_FALSE, u->data));
                break;
        }
    }
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

unsigned k__glTypeSize(int type) {
    switch (type) {
        case GL_BYTE:                
            return sizeof(GLbyte); 
        case GL_UNSIGNED_BYTE:      
            return sizeof(GLubyte); 
        case GL_SHORT:              
            return sizeof(GLshort); 
        case GL_UNSIGNED_SHORT:    
            return sizeof(GLushort); 
        case GL_INT:                  
            return sizeof(GLint); 
        case GL_UNSIGNED_INT:        
            return sizeof(GLuint); 
        case GL_FLOAT:              
            return sizeof(GLfloat); 
        case GL_DOUBLE:            
            return sizeof(GLdouble); 
        default:
            return 0;
    }
}

// b8 kRenderProgramVertAttribImpl(kRenderProgram *self, int layout_location, b8 normalize, int vertex_sz, int attrib_type, int attrib_len, unsigned attrib_offset) {
//     if (!self || self->vao == 0) { return kfalse; }
//     KGL(glBindVertexArray(self->vao));
//     KGL(glEnableVertexAttribArray(layout_location));
//     KGL(glVertexAttribPointer(layout_location, attrib_len, attrib_type, normalize, vertex_sz, (void *) attrib_offset));

    // return ktrue;
// }
