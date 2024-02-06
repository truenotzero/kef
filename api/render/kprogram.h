#ifndef KPROGRAM_H_
#define KPROGRAM_H_

#include <ktype.h>

#define KUNIFORM_MAX 16

typedef struct kRenderProgramUniform {
    int location;
    int num_elements;
    enum kType type;
    void *data;
} kRenderProgramUniform;

typedef struct kRenderProgram {
    // unsigned vao;
    unsigned program;
    int num_uniforms;
    kRenderProgramUniform uniforms[KUNIFORM_MAX];
} kRenderProgram;

b8 kRenderProgramCreate(kRenderProgram *self);
b8 kRenderProgramDestroy(kRenderProgram *self);
b8 kRenderProgramLoad(kRenderProgram *self, c_str program_name);
b8 kRenderProgramBindUniformImpl(kRenderProgram *self, c_str name, void *data, enum kType type, int num_elems);
#define kRenderProgramBindUniform(self, name, num_elems, bound_var_ptr) kRenderProgramBindUniformImpl(self, name, bound_var_ptr, kGetType(*bound_var_ptr), num_elems)
b8 kRenderProgramUse(kRenderProgram *self);

// #define accessStructElem(strukt, elem) (((strukt *) 0)->elem)
// #define toGLType(var)               \
//     _Generic( (var),                \
//     GLbyte: GL_BYTE,                \
//     GLubyte: GL_UNSIGNED_BYTE,      \
//     GLshort: GL_SHORT,              \
//     GLushort: GL_UNSIGNED_SHORT,    \
//     GLint: GL_INT,                  \
//     GLuint: GL_UNSIGNED_INT,        \
//     GLfloat: GL_FLOAT,              \
//     GLdouble: GL_DOUBLE,            \
//     GLbyte *: GL_BYTE,              \
//     GLubyte *: GL_UNSIGNED_BYTE,    \
//     GLshort *: GL_SHORT,            \
//     GLushort *: GL_UNSIGNED_SHORT,  \
//     GLint *: GL_INT,                \
//     GLuint *: GL_UNSIGNED_INT,      \
//     GLfloat *: GL_FLOAT,            \
//     GLdouble *: GL_DOUBLE,          \
//     default: GL_INVALID_ENUM        \
//     )

// unsigned k__glTypeSize(int type);

// b8 kRenderProgramVertAttribImpl(kRenderProgram *self, int layout_location, b8 normalize, int vertex_sz, int attrib_type, int attrib_len, unsigned attrib_offset);
// #define kRenderProgramVertAttrib(self, layout_location, normalize, vertex_struct, attrib_elem) \
//     kRenderProgramVertAttribImpl(self, layout_location, normalize, sizeof(vertex_struct), toGLType(accessStructElem(vertex_struct, attrib_elem)), sizeof(accessStructElem(vertex_struct, attrib_elem)) / k__glTypeSize(toGLType(accessStructElem(vertex_struct, attrib_elem))), offsetof(vertex_struct, attrib_elem))


#endif // KPROGRAM_H_
