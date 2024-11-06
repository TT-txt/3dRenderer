#include <SDL2/SDL.h>
#include "array.h"
#include "display.h"
#include "vector.h"
#include "mesh.h"
#include "triangle.h"

triangle_t *trianglesToRender;

vec3_t camPosition = {.x = 0, .y = 0, .z = -5 };
float fovFactor = 640;

bool isRunning = false;
int previousFrame = 0;

void setup(void) {
    colorBuffer = (uint32_t *) malloc(sizeof(uint32_t) * windowX * windowY);
    if (!colorBuffer) {
        fprintf(stderr, "Cannot allocate memory to colorbuffer.\n");
        isRunning = false;
    }

    colorBufferTexture = SDL_CreateTexture(
            renderer,
            SDL_PIXELFORMAT_ARGB8888,
            SDL_TEXTUREACCESS_STREAMING,
            windowX,
            windowY
    );
    if (!colorBufferTexture) {
        fprintf(stderr, "Cannot init the colorBufferTexture.\n");
        isRunning = false;
    }
   
    loadCube();

}

void processInputs(void) {
    SDL_Event event;
    SDL_PollEvent(&event);

    switch(event.type) {
        //cross to close the window
        case SDL_QUIT:
            isRunning = false;
            break;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                isRunning = false;
            }
            break;
    }
}

vec2_t project(vec3_t point) {
    vec2_t projected = {
        .x = (fovFactor * point.x) / point.z,
        .y = (fovFactor * point.y) / point.z
    };
    return projected;
}

void update(void) {
    //lock execution to reach the good framerate - comparing current time and old time
    //while (!SDL_TICKS_PASSED(SDL_GetTicks(), previousFrame + FRAME_TARGET_TIME));
    mesh.rotation.x += 0.01;
    mesh.rotation.y += 0.02;
    mesh.rotation.z += 0.03;

    int toWait = FRAME_TARGET_TIME - (SDL_GetTicks() - previousFrame);
    if (toWait > 0 && toWait <= FRAME_TARGET_TIME) {
        SDL_Delay(toWait);
    }
    previousFrame = SDL_GetTicks();

    trianglesToRender = NULL;
    int faces = arrayLength(mesh.faces);
    for (unsigned int i = 0; i < faces; ++i) {
        face_t currentFace = mesh.faces[i];
        vec3_t faceVertices[3];
        faceVertices[0] = mesh.vertices[currentFace.a - 1]; // compensating the init 1;
        faceVertices[1] = mesh.vertices[currentFace.b - 1];
        faceVertices[2] = mesh.vertices[currentFace.c - 1];
        triangle_t currentTriangle;

        for (unsigned int j = 0; j < 3; ++j) {
            vec3_t curr = faceVertices[j];

            curr = rotateX(curr, mesh.rotation.x);
            curr = rotateY(curr, mesh.rotation.y);
            curr = rotateZ(curr, mesh.rotation.z);
            
            curr.z -= camPosition.z;

            vec2_t projectedPoint = project(curr);
            projectedPoint.x += (windowX / 2);
            projectedPoint.y += (windowY / 2);
             
        currentTriangle.points[j] = projectedPoint;
        }
        arrayPush(trianglesToRender, currentTriangle);
    }
}

void render(void) {
    drawGrid();

    //loop all projected and draw rect at pos
    int trianglesNb = arrayLength(trianglesToRender);
    for (unsigned int i = 0; i < trianglesNb; ++i) {
        triangle_t currentFace = trianglesToRender[i];
        drawTriangle(
            currentFace.points[0].x,
            currentFace.points[0].y,
            currentFace.points[1].x,
            currentFace.points[1].y,
            currentFace.points[2].x,
            currentFace.points[2].y,
            0xFFABC000
        );
        for (unsigned int j = 0; j < 3; ++j) {
            vec2_t current = currentFace.points[j];
            drawRectangle(current.x, current.y, 4, 4, 0xFFFFFF00);
        }
    }
    arrayFree(trianglesToRender);

    renderColorBuffer();
    clearColorBuffer(0xFF000000);
    SDL_RenderPresent(renderer);
}

void freeResources(void) {
    arrayFree(mesh.faces);
    arrayFree(mesh.vertices);
    free(colorBuffer);
}

int main(void) {
    isRunning = initWindow(false, false);
    setup();
    while (isRunning) {
        processInputs();
        update();
        render();
    }

    destroyWindow();
    freeResources();
    return 0;
}
