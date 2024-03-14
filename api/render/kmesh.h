#ifndef KMESH_H_
#define KMESH_H_

#include <ktype.h>

/// A mesh
typedef struct kRenderMesh {
    unsigned vao;
    unsigned vertexdata_bo;
    // unsigned indices_bo;
    int num_indices;
} kRenderMesh;

/// Create a new mesh
b8 kRenderMeshCreate(kRenderMesh *self);
/// Clean up a mesh
b8 kRenderMeshDestroy(kRenderMesh *self);
/// Load a mesh from file, partial support for .obj
b8 kRenderMeshLoad(kRenderMesh *self, c_str mesh_filename);
/// Draw a mesh to the screen
u0 kRenderMeshDraw(kRenderMesh *self);

#endif // KMESH_H_
