#include <render/kmesh.h>

#include <render/kgl.h>
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

b8 kRenderMeshCreate(kRenderMesh *self) {
    if (!self) { return kfalse; }

    KGL(glGenVertexArrays(1, &self->vao));
    KGL(glGenBuffers(1, &self->vertexdata_bo));
    // KGL(glGenBuffers(1, &self->indices_bo));

    return ktrue;
}

b8 kRenderMeshDestroy(kRenderMesh *self) {
    if (!self) { return kfalse; }
    // KGL(glDeleteBuffers(1, &self->indices_bo));
    // self->indices_bo = 0;
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

    static int const max_definitions = 32 * 1024;
    int num_positions = 0;
    kVec3f *positions = malloc(max_definitions * sizeof(*positions));
    int num_tex_coords = 0;
    kVec2f *tex_coords = malloc(max_definitions * sizeof(*positions));
    int num_normals = 0;
    kVec3f *normals = malloc(max_definitions * sizeof(*normals));

    struct face {
        int pos_idx;
        int tex_coord_idx;
        int normal_idx;
    };

    struct face (*faces)[3] = malloc(max_definitions * sizeof(*faces));
    int num_faces = 0;

    int line_num = 0;
    while (fgets(line, line_sz, f)) {
        line_num += 1;
        int line_len = strlen(line);
        if (line_len < 1) { continue; }
        // commands to handle
        if (strstr(line, "v ") == line) {
            // v - vertex position definition
            assert(num_positions < max_definitions);
            kVec3f *pos = &positions[num_positions];
            sscanf(&line[2], "%f %f %f", &pos->x, &pos->y, &pos->z);
            num_positions += 1;
        } else if (strstr(line, "vt ") == line) {
            // vt - vertex uv texcoord definition
            assert(num_tex_coords < max_definitions);
            kVec2f *tex = &tex_coords[num_tex_coords];
            sscanf(&line[3], "%f %f", &tex->u, &tex->v);
            num_tex_coords += 1;
        } else if (strstr(line, "vn ") == line) {
            // vn - vertex normal definition
            assert(num_normals < max_definitions);
            kVec3f *norm = &normals[num_normals];
            sscanf(&line[3], "%f %f %f", &norm->x, &norm->y, &norm->z);
            num_normals += 1;
        } else if (strstr(line, "f ") == line) {
            // f - face (triangle) definition
            assert(num_faces < max_definitions);
            struct face (*face)[] = &faces[num_faces];

            sscanf(&line[2], "%d/%d/%d %d/%d/%d %d/%d/%d",
                &((*face)[0].pos_idx), &((*face)[0].tex_coord_idx), &((*face)[0].normal_idx),
                &((*face)[1].pos_idx), &((*face)[1].tex_coord_idx), &((*face)[1].normal_idx),
                &((*face)[2].pos_idx), &((*face)[2].tex_coord_idx), &((*face)[2].normal_idx)
            );
            // printf("loaded face:\n");
            // for (int i = 0; i < 3; ++i) {
            //     printf("vert: pos(%d) uv(%d) normal(%d)\n", (*face)[i].pos_idx, (*face)[i].tex_coord_idx, (*face)[i].normal_idx);
            // }

            num_faces += 1;
        } else if (strstr(line, "#") == line) {
            // # - comment (ignore)
            continue;
        } else {
            // printf("While loading %s:%d, unsupported command found\n", mesh_filename, line_num);
            // printf("%s\n\n", line);
            continue;
        }

    }
    fclose(f);

    // with all the data loaded from the model, build the vertex data buffer
    // TODO: implement element buffers.... (indexed drawing)
    struct vertex {
        kVec3f aPos;
        kVec2f aTexCoord;
        kVec3f aNormal;
    };
    int vertex_data_len = 3 * num_faces;
    struct vertex *vertex_data = malloc(vertex_data_len * sizeof(*vertex_data));

    for (int face_idx = 0; face_idx < num_faces; ++face_idx) {
        struct face (*face)[] = &faces[face_idx];
        for (int i = 0; i < 3; ++i) {
            // printf("Creating vertex:\n");
            struct face *f = &(*face)[i];
            if (f->pos_idx > 0) {
                vertex_data[3 * face_idx + i].aPos = positions[f->pos_idx - 1];
                // kVec3f aPos = vertex_data[3 * face_idx + i].aPos;
                // printf(" - aPos: %f %f %f\n", aPos.x, aPos.y, aPos.z);
            }
            if (f->tex_coord_idx > 0) {
                vertex_data[3 * face_idx + i].aTexCoord = tex_coords[f->tex_coord_idx - 1];
                // kVec2f aTexCoord = vertex_data[3 * face_idx + i].aTexCoord;
                // printf(" - aTexCoord: %f %f\n", aTexCoord.u, aTexCoord.v);
            }
            if (f->normal_idx > 0) {
                vertex_data[3 * face_idx + i] .aNormal = normals[f->normal_idx - 1];
                // kVec3f aNormal = vertex_data[3 * face_idx + i].aNormal;
                // printf(" - aNormal: %f %f %f\n", aNormal.x, aNormal.y, aNormal.z);
            }
        }
    }

    KGL(glBindVertexArray(self->vao));

    // set the vertex data
    KGL(glBindBuffer(GL_ARRAY_BUFFER, self->vertexdata_bo));
    KGL(glBufferData(GL_ARRAY_BUFFER, vertex_data_len * sizeof(*vertex_data), vertex_data, GL_STATIC_DRAW));

    // describe the vertex data
    // vertex position
    KGL(glEnableVertexAttribArray(0));
    KGL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(struct vertex), (void *) offsetof(struct vertex, aPos)));

    // vertex texture coords
    KGL(glEnableVertexAttribArray(1));
    KGL(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(struct vertex), (void *) offsetof(struct vertex, aTexCoord)));

    // vertex normal
    KGL(glEnableVertexAttribArray(2));
    KGL(glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(struct vertex), (void *) offsetof(struct vertex, aNormal)));

    self->num_indices = vertex_data_len;

    // set the index data
    // self->num_indices = num_indices;
    // KGL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self->indices_bo));
    // KGL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, num_indices * sizeof(*indices), indices, GL_STATIC_DRAW));

    free(faces);
    free(vertex_data);
    free(normals);
    free(tex_coords);
    free(positions);

    return ktrue;
}

u0 kRenderMeshDraw(kRenderMesh *self) {
    KGL(glBindVertexArray(self->vao));
    // KGL(glDrawElements(GL_TRIANGLES, self->num_indices, GL_UNSIGNED_INT, 0));
    KGL(glDrawArrays(GL_TRIANGLES, 0, self->num_indices));
}
