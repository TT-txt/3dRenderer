#include "display.h"

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture *colorBufferTexture = NULL;

uint32_t *colorBuffer = NULL;
int windowX = 800;
int windowY = 600;

bool initWindow(bool fullscreen, bool fullSize) {
    //Inits SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "Cannot init SDL2.\n");
        return false;
    }

    if (fullSize) {
        SDL_DisplayMode displayMode;
        SDL_GetCurrentDisplayMode(0, &displayMode);
        windowX = displayMode.w;
        windowY = displayMode.h;
    } else {
        windowX = 800;
        windowY = 600;
    }

    //Creates the SDL Window
    window = SDL_CreateWindow(
            NULL,
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            windowX,
            windowY,
            SDL_WINDOW_BORDERLESS
    );
    if (!window) {
        fprintf(stderr, "Cannot create the SDL Window.\n");
        return false;
    }
    //Create the renderer
    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        fprintf(stderr, "Cannot create the renderer.\n");
        return false;
    }

    if (fullscreen) {
        SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
    }

    return true;
}

void destroyWindow(void) {
    SDL_DestroyTexture(colorBufferTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void drawGrid(void) {
    for (unsigned int y = 0; y < windowY; y+=10) {
        for (unsigned int x = 0; x < windowX; x+=10) {
            colorBuffer[y * windowX + x] = 0xFFFFFFFF;
        }
    }
}

void drawRectangle(int x, int y, int width, int height, uint32_t color) {
    for (unsigned int i = 0; i < height; ++i) {
        for (unsigned int j = 0; j < width; ++j) {
            drawPixel(x + j, i + y, color);
        }
    }
}

void drawPixel(int x, int y, uint32_t color) {
    if (x >= 0 && x < windowX && y >= 0 && y < windowY) {
        colorBuffer[x + y * windowX] = color;
    }
}

void drawLine(int x0, int y0, int x1, int y1, uint32_t color) {
    int dX = x1 - x0;
    int dY = y1 - y0;
    int l = abs(dX) >= abs(dY) ? abs(dX) : abs(dY);
    float xInc = dX / (float) l;
    float yInc = dY / (float) l;
    float currX = x0;
    float currY = y0;
    for (int i = 0; i < l; ++i) {
        drawPixel(round(currX), round(currY), color);
        currX += xInc;
        currY += yInc;
    }
}

void drawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color) {
    drawLine(x0, y0, x1, y1, color);
    drawLine(x1, y1, x2, y2, color);
    drawLine(x2, y2, x0, y0, color);
}
void clearColorBuffer(uint32_t color) {
    unsigned int screenSize = windowX * windowY;
    for (unsigned int i = 0; i < screenSize; ++i) {
        colorBuffer[i] = color;
    }
}

void renderColorBuffer(void) {
    SDL_UpdateTexture(
            colorBufferTexture,
            NULL,
            colorBuffer,
            (int) (windowX * sizeof(uint32_t))
    );
    SDL_RenderCopy(renderer, colorBufferTexture, NULL, NULL);

}

