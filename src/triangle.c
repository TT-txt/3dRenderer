#include "triangle.h"

void fillFlatBottomTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color) {
    float slope1 = (float) (x1 - x0) / (y1 - y0);
    float slope2 = (float) (x2 - x0) / (y2 - y0);
    float start = x0;
    float end = x0;
    for (int y = y0; y <= y2; ++y) {
        drawLine(start, y, end, y, color);
        start += slope1;
        end += slope2;
    }
}

void fillFlatTopTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color) {
    float slope1 = (float) (x2 - x0) / (y2 - y0);
    float slope2 = (float) (x2 - x1) / (y2 - y1);
    float start = x2;
    float end = x2;
    for (int y = y2; y >= y0; --y) {
        drawLine(start, y, end, y, color);
        start -= slope1;
        end -= slope2;
    }
}


void intSwap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void drawFilledTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color) {
    int mx, my;
    //ordering points in function of the y position
    if (y0 > y1) {
        intSwap(&x0, &x1);
        intSwap(&y0, &y1);
    }
    if (y1 > y2) {
        intSwap(&x1, &x2);
        intSwap(&y1, &y2);
    }
    if (y0 > y1) {
        intSwap(&x0, &x1);
        intSwap(&y0, &y1);
    }
    //finding middle position
    my = y1;
    mx = ((float)((x2 - x0) * (y1 - y0)) / (float)(y2 - y0)) + x0; //Thales magic
    //drawing flat bottom
    fillFlatBottomTriangle(x0, y0, x1, y1, mx, my, color);
    fillFlatTopTriangle(x1, y1, mx, my, x2, y2, color);
}
