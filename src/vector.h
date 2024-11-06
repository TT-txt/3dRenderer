#ifndef VECTOR_H
#define VECTOR_H

#include <math.h>

typedef struct {
    float x;
    float y;
} vec2_t;

typedef struct {
    float x;
    float y;
    float z;
} vec3_t;

vec3_t rotateX(vec3_t p, float a);
vec3_t rotateY(vec3_t p, float a);
vec3_t rotateZ(vec3_t p, float a);

#endif
