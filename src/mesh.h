#ifndef MESH_H
#define MESH_H

#include "vector.h"
#include "triangle.h"

#define CUBE_VERTICES 8
#define CUBE_FACES (6 * 2) //faces of the cube * 2

extern vec3_t cubeVertices[CUBE_VERTICES];
extern face_t cubeFaces[CUBE_FACES];

typedef struct {
    vec3_t *vertices;
    face_t *faces;
    vec3_t rotation; //euler angle
} mesh_t;

extern mesh_t mesh;

void loadCube(void); 

#endif
