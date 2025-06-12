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

/* --- VEC 2 UTILS --- */
float vec2Length(vec2_t v) {
    float r = sqrt(pow(v.x, 2) + pow(v.y, 2));
    return r;
}

vec2_t vec2Add(vec2_t a, vec2_t b) {
    float x = a.x + b.x;
    float y = a.y + b.y;
    vec2_t r = {.x = x, .y = y};
    return r;
}

vec2_t vec2Sub(vec2_t a, vec2_t b) {
    float x = a.x - b.x;
    float y = a.y - b.y;
    vec2_t r = {.x = x, .y = y};
    return r;
}

vec2_t vec2Mul(vec2_t v, float sc) {
    v.x *= sc;
    v.y *= sc;
    return v;
}

vec2_t vec2Div(vec2_t v, float sc) {
    v.x /= sc;
    v.y /= sc;
    return v;
}

float vec2Dot(vec2_t a, vec2_t b) {
    return a.x * b.x + a.y + b.y; 
}

void vec2Normalize(vec2_t *a) {
    float length = vec2Length(*a);
    *a = vec2Div(*a, length);
}

/* --- VEC3 UTILS --- */
float vec3Length(vec3_t v) {
    float r = sqrt(pow(v.x, 2) + pow(v.y, 2) + pow(v.z, 2));
    return r;
}

vec3_t vec3Add(vec3_t a, vec3_t b) {
    float x = a.x + b.x;
    float y = a.y + b.y;
    float z = a.z + b.z;
    vec3_t r = {.x = x, .y = y, .z = z};
    return r;
}

vec3_t vec3Sub(vec3_t a, vec3_t b) {
    float x = a.x - b.x;
    float y = a.y - b.y;
    float z = a.z - b.z;
    vec3_t r = {.x = x, .y = y, .z = z};
    return r;
}

vec3_t vec3Mul(vec3_t v, float sc) {
    v.x *= sc;
    v.y *= sc;
    v.z *= sc;
    return v;
}

vec3_t vec3Div(vec3_t v, float sc) {
    v.x /= sc;
    v.y /= sc;
    v.z /= sc;
    return v;
}

vec3_t vec3Cross(vec3_t a, vec3_t b) {
    float x = a.y * b.z - a.z * b.y;
    float y = a.z * b.x - a.x * b.z;
    float z = a.x * b.y - a.y * b.x;
    vec3_t r = {.x = x, .y = y, .z = z};
    return r; 
}

float vec3Dot(vec3_t a, vec3_t b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

void vec3Normalize(vec3_t *a) {
    float length = vec3Length(*a);
    *a = vec3Div(*a, length);
}


//VEC 4
vec4_t vec3ToVec4(vec3_t v){
    vec4_t res = {
        .x = v.x,
        .y = v.y,
        .z = v.z,
        .w = 1.0
    };
    return res;
}


vec3_t vec4ToVec3(vec4_t v) {
    vec3_t res = {
        .x = v.x,
        .y = v.y,
        .z = v.z
    };
    return res;
}
