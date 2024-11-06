#include "vector.h"

vec3_t rotateX(vec3_t p, float a) {
    vec3_t r = {
        .x = p.x,
        .y = p.y * cos(a) - p.z * sin(a),
        .z = p.y * sin(a) + p.z * cos(a)
    };
    return r;
}

vec3_t rotateY(vec3_t p, float a) {
    vec3_t r = {
        .x = p.x * cos(a) - p.z * sin(a),
        .y = p.y,
        .z = p.x * sin(a) + p.z * cos(a)
    };
    return r;
}

vec3_t rotateZ(vec3_t p, float a) {
    vec3_t r = {
        .x = p.x * cos(a) - p.y * sin(a),
        .y = p.x * sin(a) + p.y * cos(a),
        .z = p.z
    };
    return r;
}
