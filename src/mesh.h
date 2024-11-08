#ifndef MESH_H
#define MESH_H

#include "vector.h"
#include "triangle.h"

typedef struct {
    vec3_t *vertices;
    face_t *faces;
    vec3_t rotation; //euler angle
} mesh_t;

extern mesh_t mesh;

void loadObj(char *fileName);

#endif
