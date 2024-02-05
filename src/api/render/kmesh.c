
#include "core/kutil.h"
#include <ktype.h>
#include <render/kgl.h>
#include <string.h>

typedef struct kMesh {
    unsigned vao;
    unsigned vertexdata_bo;
    unsigned indices_bo;
    int num_indices;
} kMesh;

b8 kRenderMeshCreate(kMesh *self) {
    if (!self) { return kfalse; }

    KGL(glGenVertexArrays(1, &self->vao));
    KGL(glGenBuffers(1, &self->vertexdata_bo));
    KGL(glGenBuffers(1, &self->indices_bo));

    return ktrue;
}

b8 kRenderMeshDestroy(kMesh *self) {
    if (!self) { return kfalse; }
    KGL(glDeleteBuffers(1, &self->indices_bo));
    self->indices_bo = 0;
    KGL(glDeleteBuffers(1, &self->vertexdata_bo));
    self->vertexdata_bo = 0;
    KGL(glDeleteVertexArrays(1, &self->vao));
    self->vao = 0;

    return ktrue;
}

b8 kRenderMeshLoad(kMesh *self, c_str mesh_filename) {
    char file_buf[8 * 1024];
    kUtilReadTextFile(mesh_filename, file_buf, sizeof(file_buf));
    // strntok

}

u0 kRenderMeshDraw(kMesh *self) {
    KGL(glBindVertexArray(self->vao));
    KGL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self->indices_bo));
    KGL(glDrawElements(GL_TRIANGLES, self->num_indices, GL_FLOAT, 0));
}
