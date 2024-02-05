#include "kmesh.h"

#include <render/kgl.h>
#include <stdio.h>
#include <string.h>

b8 kRenderMeshCreate(kRenderMesh *self) {
    if (!self) { return kfalse; }

    KGL(glGenVertexArrays(1, &self->vao));
    KGL(glGenBuffers(1, &self->vertexdata_bo));
    KGL(glGenBuffers(1, &self->indices_bo));

    return ktrue;
}

b8 kRenderMeshDestroy(kRenderMesh *self) {
    if (!self) { return kfalse; }
    KGL(glDeleteBuffers(1, &self->indices_bo));
    self->indices_bo = 0;
    KGL(glDeleteBuffers(1, &self->vertexdata_bo));
    self->vertexdata_bo = 0;
    KGL(glDeleteVertexArrays(1, &self->vao));
    self->vao = 0;

    return ktrue;
}

b8 kRenderMeshLoad(kRenderMesh *self, c_str mesh_filename) {
    unsigned const line_sz = 1024;
    char line[line_sz];
    FILE *f = fopen(mesh_filename, "r");
    if (!f) { return kfalse; }

    struct vertex {
        float x;
        float y;
        float z;
    } vertices[256];
    int const indices_per_face = 3;
    unsigned indices[3 * 1024];
    unsigned num_vertices = 0;
    unsigned num_indices = 0;

    while (fgets(line, line_sz, f)) {
        int line_len = strlen(line);
        if (line_len < 1) { continue; }

        switch (line[0]) {
            default:
                // fall through
            case '#':
                // comment found, skip
                continue;
            case 'v':
                // define vertex:
                struct vertex *v = &vertices[num_vertices];
                // line[1] is a space, line[2] is where data starts
                sscanf(&line[2], "%f %f %f", &v->x, &v->y, &v->z);
                num_vertices += 1;
                break;
            case 'f':
                // define face (triangle) by indices:
                unsigned *face = &indices[num_indices];
                // line[1] is a space, line[2] is where data starts
                sscanf(&line[2], "%u %u %u", &face[0], &face[1], &face[2]);
                // convert 1-based indices to 0-based indices
                for (unsigned i = 0; i < indices_per_face; ++i) {
                    face[i] -= 1;
                }
                num_indices += indices_per_face;
                break;
        }
    }
    fclose(f);

    KGL(glBindVertexArray(self->vao));

    // set the vertex data
    KGL(glBindBuffer(GL_ARRAY_BUFFER, self->vertexdata_bo));
    KGL(glBufferData(GL_ARRAY_BUFFER, num_vertices * sizeof(struct vertex), vertices, GL_STATIC_DRAW));

    // describe the vertex data
    KGL(glEnableVertexAttribArray(0));
    KGL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0));

    // set the index data
    self->num_indices = num_indices;
    KGL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self->indices_bo));
    KGL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, num_indices * sizeof(*indices), indices, GL_STATIC_DRAW));

    return ktrue;
}

u0 kRenderMeshDraw(kRenderMesh *self) {
    KGL(glBindVertexArray(self->vao));
    KGL(glDrawElements(GL_TRIANGLES, self->num_indices, GL_UNSIGNED_INT, 0));
}
