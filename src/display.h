#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>
#include <SDL2/SDL.h>
#include <stdbool.h>

#define FPS 30
#define FRAME_TARGET_TIME (1000 / FPS)

extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern SDL_Texture *colorBufferTexture;

extern uint32_t *colorBuffer;
extern int windowX;
extern int windowY;

bool initWindow(bool fullscreen, bool fullSize);
void destroyWindow(void);

void drawGrid(void);
void drawRectangle(int x, int y, int width, int height, uint32_t color);
void drawPixel(int x, int y, uint32_t color);
void drawLine(int x0, int y0, int x1, int y1, uint32_t color);
void drawTriangle(int x0, int x1, int x2, int y0, int y1, int y2, uint32_t color);

void clearColorBuffer(uint32_t color);
void renderColorBuffer(void);

#endif
