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

/* --- VEC2 Utils --- */
float vec2Length(vec2_t v);
vec2_t vec2Add(vec2_t a, vec2_t b);
vec2_t vec2Sub(vec2_t a, vec2_t b);
vec2_t vec2Mul(vec2_t v, float sc);
vec2_t vec2Div(vec2_t v, float sc);
float vec2Dot(vec2_t a, vec2_t b);

/* --- VEC3 Utils  --- */
float vec3Length(vec3_t v);
vec3_t vec3Add(vec3_t a, vec3_t b);
vec3_t vec3Sub(vec3_t a, vec3_t b);
vec3_t vec3Mul(vec3_t v, float sc);
vec3_t vec3Div(vec3_t v, float sc);
vec3_t vec3Cross(vec3_t a, vec3_t b);
float vec3Dot(vec3_t a, vec3_t b);
#endif
