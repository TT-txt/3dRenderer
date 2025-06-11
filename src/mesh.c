#include "mesh.h"
#include "array.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

mesh_t mesh = {
    .vertices = NULL,
    .faces = NULL,
    .rotation = { 0, 0, 0},
    .scale= { 1.0, 1.0, 1.0},
    .translation = { 0, 0, 0}
};

Color getRandomColor() {
    // Seed once at program start if needed
    static int seeded = 0;
    if (!seeded) {
        srand((unsigned int)time(NULL));
        seeded = 1;
    }
    return (Color)(rand() % COLOR_COUNT);
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
                .c = vertexIndices[2],
                .color = getRandomColor()
            };
            arrayPush(mesh.faces, face);
        }
    }
}
