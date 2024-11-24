#include <SDL2/SDL.h>
#include "array.h"
#include "display.h"
#include "vector.h"
#include "mesh.h"
#include "triangle.h"

//user options
bool DISPLAY_WIREFRAME = true;
bool DISPLAY_FACES = true;
bool DISPLAY_VERTICES = true;
bool ENABLE_CULLING = true;
//needs culling enabled to show those
bool ONLY_DISPLAY_CULLED_FACES = false;
bool DISPLAY_GRID = true;

//COLORS
uint32_t verticesColor = 0xFFFFDE21;
uint32_t wireframeColor = 0xFFFFFFFF;
uint32_t facesColor = 0xFF00FF00;


triangle_t *trianglesToRender = NULL;

vec3_t camPosition = {.x = 0, .y = 0, .z = 0 };
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
   
    //loadObj("assets/f22.obj");
    loadObj("assets/cube.obj");
    //loadObj("assets/cow.obj");
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
            SDL_Keycode keyPressed = event.key.keysym.sym;
            if (keyPressed == SDLK_ESCAPE) {
                isRunning = false;
            } else if (keyPressed == SDLK_1) {
                DISPLAY_WIREFRAME = true;
                DISPLAY_VERTICES = true;
                verticesColor = 0xFFFF0000;
                DISPLAY_FACES = false;
            } else if (keyPressed == SDLK_2) {
                DISPLAY_WIREFRAME = true;
                DISPLAY_VERTICES = false;
                DISPLAY_FACES = false;
            } else if (keyPressed == SDLK_3) {
                DISPLAY_WIREFRAME = false;
                DISPLAY_VERTICES = false;
                DISPLAY_FACES = true;
            } else if (keyPressed == SDLK_4) {
                DISPLAY_WIREFRAME = true;
                DISPLAY_FACES = true;
                DISPLAY_VERTICES = false;
            } else if (keyPressed == SDLK_c) {
                ENABLE_CULLING = true;
            } else if (keyPressed == SDLK_d) {
                ENABLE_CULLING = false;
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

        vec3_t transformedVertices[3];
        
        //TRANSFORMATION LOOP
        for (unsigned int j = 0; j < 3; ++j) {
            vec3_t curr = faceVertices[j];

            curr = rotateX(curr, mesh.rotation.x);
            curr = rotateY(curr, mesh.rotation.y);
            curr = rotateZ(curr, mesh.rotation.z);
            
            curr.z += 5;

            transformedVertices[j] = curr;
        }

       if (ENABLE_CULLING) { 
            //check if back is culled
            vec3_t AB = vec3Sub(transformedVertices[1], transformedVertices[0]);
            vec3_t AC = vec3Sub(transformedVertices[2], transformedVertices[0]);
            vec3_t camRay = vec3Sub(camPosition, transformedVertices[0]);
            vec3Normalize(&AB);
            vec3Normalize(&AC);
            vec3_t normal = vec3Cross(AB, AC); //if right handed, should be the opposite
            vec3Normalize(&normal);
            float dotProductResult = vec3Dot(normal, camRay);
            if ((dotProductResult < 0 && !ONLY_DISPLAY_CULLED_FACES) || (ONLY_DISPLAY_CULLED_FACES && dotProductResult >= 0)) {
                //face is culled, looking back, hidden to camera
                continue;
            }
        }

        triangle_t currentTriangle;
        for (unsigned int j = 0; j < 3; ++j) {
            vec2_t projectedPoint = project(transformedVertices[j]);
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
        if (DISPLAY_FACES) {
            //fill
            drawFilledTriangle(
                currentFace.points[0].x,
                currentFace.points[0].y,
                currentFace.points[1].x,
                currentFace.points[1].y,
                currentFace.points[2].x,
                currentFace.points[2].y,
                facesColor
            );
        }

        if (DISPLAY_WIREFRAME) {
            //outline
            drawTriangle(
                currentFace.points[0].x,
                currentFace.points[0].y,
                currentFace.points[1].x,
                currentFace.points[1].y,
                currentFace.points[2].x,
                currentFace.points[2].y,
                wireframeColor
            );
        }

        if (DISPLAY_VERTICES) {
            //points
            for (unsigned int j = 0; j < 3; ++j) {
                vec2_t current = currentFace.points[j];
                drawRectangle(current.x, current.y, 4, 4, verticesColor);
            }
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
