#include "matrix.h"
#include <stdio.h>

mat4_t mat4MakeScale(float sx, float sy, float sz) {
    mat4_t m = {{
        { sy, 0.0, 0.0, 0.0 },
        { 0.0, sx, 0.0, 0.0 },
        { 0.0, 0.0, sz, 0.0 },
        { 0.0, 0.0, 0.0, 1.0 },
    }};
    return m;
}

mat4_t mat4Identity(void) {
    return mat4MakeScale(1, 1, 1);
}

vec4_t mat4MulVec4(mat4_t m, vec4_t v) {
    float x = m.m[0][0] * v.x + m.m[0][1] * v.y + m.m[0][2] * v.z + m.m[0][3] * v.w;
    float y = m.m[1][0] * v.x + m.m[1][1] * v.y + m.m[1][2] * v.z + m.m[1][3] * v.w;
    float z = m.m[2][0] * v.x + m.m[2][1] * v.y + m.m[2][2] * v.z + m.m[2][3] * v.w;
    float w = m.m[3][0] * v.x + m.m[3][1] * v.y + m.m[3][2] * v.z + m.m[3][3] * v.w;
    vec4_t r = {.x = x, .y = y, .z = z, .w = w};
    return r;
}

mat4_t mat4MakeTranslate(float x, float y, float z) {
    mat4_t r = mat4Identity();
    r.m[0][3] = x;
    r.m[1][3] = y;
    r.m[2][3] = z;
    return r;
}
