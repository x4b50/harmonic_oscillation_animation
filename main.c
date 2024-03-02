#include "raylib.h"
#include <math.h>

#define WINDOW_W 1280
#define WINDOW_H 720
#define UNIT_RADUIS 1/8
#define PADDING 1/30
#define SPRING_W 1/8

#define COLOR(c) *(Color*)&(c)
int BG_COLOR = 0xFF181818;

void draw_arrow(const int x, const int y, const int width, const int height, const double deg, const Color color) {
    Rectangle rec = {.x=x, .y=y, .width=width, .height=height-width*2.};
    DrawRectanglePro(rec, (Vector2){.x=width/2., .y=rec.height}, deg, color);
    double d = (deg-90)*PI/180;
    Vector2 v1 = {.x = x+cos(d)*height, .y = y+sin(d)*height};
    Vector2 v2 = {.x = v1.x+sin(d)*width-cos(d)*width*3, .y = v1.y-cos(d)*width-sin(d)*width*3};
    Vector2 v3 = {.x = v1.x-sin(d)*width-cos(d)*width*3, .y = v1.y+cos(d)*width-sin(d)*width*3};
    DrawTriangle(v1, v2, v3, color);
}

int main(void) {
    // anti-alyasing
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(WINDOW_W, WINDOW_H, "raylib [core] example - basic window");
    SetTargetFPS(120);

    double time = 0;
    double deg = 0;

    while (!WindowShouldClose()) {
        int window_w = GetScreenWidth();
        int window_h = GetScreenHeight();
        if (window_h<100) { window_h = 100;
            SetWindowSize(window_w, window_h);
        }
        if (window_w<200) { window_w = 200;
            SetWindowSize(window_w, window_h);
        }

        int padding = window_w*PADDING;
        int u_raduis = window_w*UNIT_RADUIS;
        if (u_raduis > window_h/2-padding)
            u_raduis = window_h/2-padding;
        int spring_w = window_w*SPRING_W;
        int rect_h = u_raduis*2;
        int rect_w = window_w -(u_raduis*2+spring_w+padding*4);

        time += GetFrameTime()*180/PI;
        deg = fmod(time, 360.)+90;

        BeginDrawing();
        ClearBackground(COLOR(BG_COLOR));
        DrawCircle(padding+u_raduis, window_h/2, u_raduis, LIGHTGRAY);
        draw_arrow(padding+u_raduis, window_h/2, u_raduis/15, u_raduis, deg-90., GREEN);
        draw_arrow(padding+u_raduis, window_h/2, u_raduis/15, u_raduis, deg, BLUE);
        draw_arrow(padding+u_raduis, window_h/2, u_raduis/15, u_raduis, deg-180., RED);
        DrawRectangleLines((padding+u_raduis)*2, window_h/2-rect_h/2-padding, spring_w, rect_h+2*padding, LIGHTGRAY);
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
