#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "vector.h"
#include <stdint.h>
#include "display.h"

typedef struct {
    int a, b, c;
    uint32_t color;
} face_t;

typedef struct {
    vec2_t points[3];
    uint32_t color;
    float averageDepth;
} triangle_t;

void drawFilledTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);

#endif
