#include <SDL2/SDL.h>
#include "array.h"
#include "display.h"
#include "vector.h"
#include "mesh.h"
#include "triangle.h"
#include "matrix.h"

#ifndef M_PI
#define M_PI 3.1415926535
#endif

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

triangle_t *trianglesToRender = NULL;

vec3_t camPosition = {.x = 0, .y = 0, .z = 0 };

bool isRunning = false;
int previousFrame = 0;

mat4_t projMatrix; 

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

    float fov = M_PI / 3.0; // 60 deg
    float aspect = (float) windowY / (float) windowX;
    float zNear = 0.1;
    float zFar = 100.0;
    projMatrix = mat4MakePerspective(fov, aspect, zNear, zFar);
   
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

void update(void) {
    //lock execution to reach the good framerate - comparing current time and old time
    //while (!SDL_TICKS_PASSED(SDL_GetTicks(), previousFrame + FRAME_TARGET_TIME));
    mesh.rotation.x += 0.01;
    mesh.rotation.y += 0.02;
    mesh.rotation.z += 0.03;

    //mesh.scale.y += 0.003;
    //mesh.scale.x += 0.002;
    
    //mesh.translation.x += 0.1;
    //mesh.translation.y += 0.05;
    mesh.translation.z = 5.0;

    mat4_t scaleMatrix = mat4MakeScale(mesh.scale.x,mesh.scale.y,mesh.scale.z);
    mat4_t xRotationMatrix = mat4MakeXRotation(mesh.rotation.x);
    mat4_t yRotationMatrix = mat4MakeYRotation(mesh.rotation.y);
    mat4_t zRotationMatrix = mat4MakeZRotation(mesh.rotation.z);
    mat4_t translateMatrix = mat4MakeTranslate(mesh.translation.x, mesh.translation.y, mesh.translation.z); 

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

        vec4_t transformedVertices[3];
        
        //TRANSFORMATION LOOP
        for (unsigned int j = 0; j < 3; ++j) {
            vec4_t curr = vec3ToVec4(faceVertices[j]);
            mat4_t worldMatrix = mat4Identity();
            worldMatrix = mat4MulMat4(scaleMatrix, worldMatrix);
            worldMatrix = mat4MulMat4(zRotationMatrix, worldMatrix);
            worldMatrix = mat4MulMat4(yRotationMatrix, worldMatrix);
            worldMatrix = mat4MulMat4(xRotationMatrix, worldMatrix);
            worldMatrix = mat4MulMat4(translateMatrix, worldMatrix);

            curr = mat4MulVec4(worldMatrix, curr);

            transformedVertices[j] = curr;
        }

       if (ENABLE_CULLING) { 
            //check if back is culled
            vec3_t AB = vec3Sub(vec4ToVec3(transformedVertices[1]), vec4ToVec3(transformedVertices[0]));
            vec3_t AC = vec3Sub(vec4ToVec3(transformedVertices[2]), vec4ToVec3(transformedVertices[0]));
            vec3_t camRay = vec3Sub(camPosition, vec4ToVec3(transformedVertices[0]));
            vec3Normalize(&AB);
            vec3Normalize(&AC);
            vec3_t normal = vec3Cross(AB, AC); //if right handed, should be the opposite
            vec3Normalize(&normal);
            float dotProductResult = vec3Dot(normal, camRay);
            if ((dotProductResult < 0 && !ONLY_DISPLAY_CULLED_FACES) ||
                    (ONLY_DISPLAY_CULLED_FACES && dotProductResult >= 0)) {
                //face is culled, looking back, hidden to camera
                continue;
            }
        }

        triangle_t currentTriangle;
        currentTriangle.averageDepth = 0;
        for (unsigned int j = 0; j < 3; ++j) {
            vec4_t projectedPoint = mat4MulVec4Project(projMatrix, transformedVertices[j]);

            projectedPoint.x *= (windowX / 2.0);
            projectedPoint.y *= (windowY / 2.0);
            projectedPoint.x += (windowX / 2.0);
            projectedPoint.y += (windowY / 2.0);

            vec2_t toAdd = {
                .x = projectedPoint.x,
                .y = projectedPoint.y
            };
            currentTriangle.points[j] = toAdd;
            currentTriangle.averageDepth += transformedVertices[j].z;
        }
        currentTriangle.averageDepth /= 3.0;
        currentTriangle.color = currentFace.color;
        arrayPush(trianglesToRender, currentTriangle);
    }

    int size = arrayLength(trianglesToRender);
    bool sorted = false;
    for (int j = size - 1; j > 0 && !sorted; --j) {
        sorted = true;
        for (int k = 0; k < j; ++k) {
            if (trianglesToRender[k].averageDepth < trianglesToRender[k + 1].averageDepth) {
                triangle_t temp = trianglesToRender[k];
                trianglesToRender[k] = trianglesToRender[k + 1];
                trianglesToRender[k + 1] = temp;

                sorted = false;
            }
        }
    }
}

void render(void) {
    drawGrid();

    //loop all projected and draw rect at pos
    int trianglesNb = arrayLength(trianglesToRender);
    //we need to sort the faces by z value, painter's algo
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
                currentFace.color
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
