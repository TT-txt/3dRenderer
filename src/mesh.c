#include "mesh.h"
#include "array.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

mesh_t mesh = {
    .vertices = NULL,
    .faces = NULL,
    .rotation = { 0, 0, 0}
};

vec3_t cubeVertices[CUBE_VERTICES] = {
    { .x = -1, .y = -1, .z = -1 }, // 1
    { .x = -1, .y =  1, .z = -1 }, // 2
    { .x =  1, .y =  1, .z = -1 }, // 3
    { .x =  1, .y = -1, .z = -1 }, // 4
    { .x =  1, .y =  1, .z =  1 }, // 5
    { .x =  1, .y = -1, .z =  1 }, // 6
    { .x = -1, .y =  1, .z =  1 }, // 7
    { .x = -1, .y = -1, .z =  1 }  // 8
};

face_t cubeFaces[CUBE_FACES] = {
    // front
    { .a = 1, .b = 2, .c = 3 },
    { .a = 1, .b = 3, .c = 4 },
    // right
    { .a = 4, .b = 3, .c = 5 },
    { .a = 4, .b = 5, .c = 6 },
    // back
    { .a = 6, .b = 5, .c = 7 },
    { .a = 6, .b = 7, .c = 8 },
    // left
    { .a = 8, .b = 7, .c = 2 },
    { .a = 8, .b = 2, .c = 1 },
    // top
    { .a = 2, .b = 7, .c = 5 },
    { .a = 2, .b = 5, .c = 3 },
    // bottom
    { .a = 6, .b = 8, .c = 1 },
    { .a = 6, .b = 1, .c = 4 }
};

void loadCube(void) {
    for (int i = 0; i < CUBE_VERTICES; ++i) {
        vec3_t vertex = cubeVertices[i];
        arrayPush(mesh.vertices, vertex);
    }
    for (int i = 0; i < CUBE_FACES; ++i) {
        face_t face = cubeFaces[i];
        arrayPush(mesh.faces, face);
    }
}

/* --- IMPORTANT NOTICE ---
 * This is based on triangles faces.
 * If you want to use other models, as some software do
 * such as quads, you'll need to convert them into a soft like blender
*/
void loadObj(char *fileName) {
    FILE *fp;
    char line[1024];
    fp = fopen(fileName, "r");

    if (fp == NULL) {
        fprintf(stderr, "ERROR: cannot open obj file: %s \n", fileName);
        exit(EXIT_FAILURE);
    }

    while (fgets(line, 1024, fp)) {
        // Vertex information
        if (strncmp(line, "v ", 2) == 0) {
            vec3_t vertex;
            sscanf(line, "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);
            arrayPush(mesh.vertices, vertex);
        }

        // Face information
        if (strncmp(line, "f ", 2) == 0) {
            int vertexIndices[3];
            int textureIndices[3];
            int normalIndices[3];
            sscanf(
                line, "f %d/%d/%d %d/%d/%d %d/%d/%d",
                &vertexIndices[0], &textureIndices[0], &normalIndices[0], 
                &vertexIndices[1], &textureIndices[1], &normalIndices[1], 
                &vertexIndices[2], &textureIndices[2], &normalIndices[2]
            ); 
            face_t face = {
                .a = vertexIndices[0],
                .b = vertexIndices[1],
                .c = vertexIndices[2]
            };
            arrayPush(mesh.faces, face);
        }
    }
}