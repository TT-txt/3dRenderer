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


// color stuff
typedef enum {
    COLOR_RED       = 0xFF0000,
    COLOR_GREEN     = 0x00FF00,
    COLOR_BLUE      = 0x0000FF,
    COLOR_YELLOW    = 0xFFFF00,
    COLOR_CYAN      = 0x00FFFF,
    COLOR_MAGENTA   = 0xFF00FF,
    COLOR_ORANGE    = 0xFFA500,
    COLOR_PURPLE    = 0x800080,
    COLOR_WHITE     = 0xFFFFFF,
    COLOR_GRAY      = 0x808080,
    COLOR_COUNT     // Not a color; used to get the number of colors
} Color;

void loadObj(char *fileName);

#endif
