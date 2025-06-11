#ifndef MATRIX_H
#define MATRIX_H
#include "vector.h"

typedef struct {
   float m[4][4]; 
} mat4_t;

mat4_t mat4Identity(void); //sometime just called the I
mat4_t mat4MakeScale(float, float, float);
vec4_t mat4MulVec4(mat4_t, vec4_t);

#endif
