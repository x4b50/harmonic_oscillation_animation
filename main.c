#include "raylib.h"

#define WINDOW_W 1280
#define WINDOW_H 720
#define UNIT_RADUIS 1/8
#define PADDING 1/30
#define SPRING_W 1/8

#define COLOR(c) *(Color*)&(c)
int BG_COLOR = 0xFF181818;

int main(void)
{
    // anti-alyasing
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(WINDOW_W, WINDOW_H, "raylib [core] example - basic window");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        int window_w = GetScreenWidth();
        int window_h = GetScreenHeight();

        int padding = window_w*PADDING;
        int u_raduis = window_w*UNIT_RADUIS;
        if (u_raduis > (window_h-padding)/2)
            u_raduis = (window_h-padding)/2;
        int spring_w = window_w*SPRING_W;
        int rect_h = u_raduis*2;
        int rect_w = window_w -(u_raduis*2+spring_w+padding*4);

        BeginDrawing();
        ClearBackground(COLOR(BG_COLOR));
        DrawCircle(padding+u_raduis, window_h/2, u_raduis, LIGHTGRAY);
        DrawRectangleLines((padding+u_raduis)*2, padding, spring_w, window_h-2*padding, LIGHTGRAY);
        DrawRectangle(
                u_raduis*2+spring_w+padding*3,
                window_h/2-rect_h/2,
                rect_w, rect_h,
                LIGHTGRAY);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}


/*
    o | []
*/
