#include "matrix.h"

mat4_t mat4MakeScale(float sx, float sy, float sz) {
    mat4_t m;
    m.m[0][0] = sx;
    m.m[1][1] = sx;
    m.m[2][2] = sx;
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
