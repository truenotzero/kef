#ifndef KMESH_H_
#define KMESH_H_

#include <ktype.h>

typedef struct kRenderMesh {
    unsigned vao;
    unsigned vertexdata_bo;
    unsigned indices_bo;
    int num_indices;
} kRenderMesh;

b8 kRenderMeshCreate(kRenderMesh *self);
b8 kRenderMeshDestroy(kRenderMesh *self);
b8 kRenderMeshLoad(kRenderMesh *self, c_str mesh_filename);
u0 kRenderMeshDraw(kRenderMesh *self);

#endif // KMESH_H_
