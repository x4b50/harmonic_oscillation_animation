#include "raylib.h"
#include <math.h>

#define WINDOW_W 1280
#define WINDOW_H 720
#define UNIT_RADUIS 1/8
#define PADDING 1/30
#define SPRING_W 1/8

#define COLOR(c) *(Color*)&(c)
int BG_COLOR = 0xFF181818;

void _draw_vec(const int x, const int y, const int width, const int height, const double deg, const Color color, bool project) {
    if (height < width*2) return;
    Rectangle rec = {.x=x, .y=y, .width=width, .height=height-width*2.};
    DrawRectanglePro(rec, (Vector2){.x=width/2., .y=rec.height}, -deg, color);
    double d = -(deg+90)*PI/180;
    Vector2 v1 = {.x = x+cos(d)*height, .y = y+sin(d)*height};
    Vector2 v2 = {.x = v1.x+sin(d)*width-cos(d)*width*3, .y = v1.y-cos(d)*width-sin(d)*width*3};
    Vector2 v3 = {.x = v1.x-sin(d)*width-cos(d)*width*3, .y = v1.y+cos(d)*width-sin(d)*width*3};
    DrawTriangle(v1, v2, v3, color);
}

void draw_unit_projections (
    const int x, const int y, const int width, const int height, const double deg,
    const Color color1, const Color color2, const Color color3
 ) {
    _draw_vec(x, y, width, height, deg, color1, true);
    double d = -(deg+90)*PI/180;
    Vector2 v = {.x = x+cos(d)*height, .y = y+sin(d)*height};

    int h1, d1;
    if (v.y-y > 0) {h1 = v.y-y; d1=180;}
    else {h1 = y-v.y; d1=0;}

    int h2, d2;
    if (v.x-x > 0) {h2 = v.x-x; d2=-90;}
    else {h2 = x-v.x; d2=90;}

    _draw_vec(x, y, width/2, h1, d1, color1, true);
    _draw_vec(x, y, width/2, h2, d2, color2, true);
    _draw_vec(x, y, width/2, h1, d1-180, color3, true);
}

void draw_anim_vecs(const int x, const int y, const int width, const int height, const double deg, const Color color) {
    double d = -(deg+90)*PI/180;
    Vector2 v1 = {.x = x+cos(d)*height, .y = y+sin(d)*height};
    int lx;
    if (v1.x>x) {lx=v1.x;}
    else {lx = x+(x-v1.x);}

    if (v1.y-y > 0) {
        _draw_vec(x, y, width/2, v1.y-y, 180, color, false);
    } else {
        _draw_vec(x, y, width/2, y-v1.y, 0, color, false);
    }
}

int main(void) {
    // anti-alyasing
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(WINDOW_W, WINDOW_H, "raylib [core] example - basic window");
    SetTargetFPS(120);

    double time = 0;
    double a_deg = 0;
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
        int spring_h = 2*(u_raduis+padding);
        int spring_x = (padding+u_raduis)*2;
        int spring_y = window_h/2-u_raduis-padding;

        int rect_h = u_raduis*2;
        int rect_w = window_w -(u_raduis*2+spring_w+padding*4);
        int rect_x0 = u_raduis*2+spring_w+padding*3;

        time += GetFrameTime()*180/PI;
        a_deg = fmod(time, 360.);
        deg = a_deg-90;

        BeginDrawing();
        ClearBackground(COLOR(BG_COLOR));
        DrawCircle(padding+u_raduis, window_h/2, u_raduis, LIGHTGRAY);
        DrawRectangle(padding, window_h/2-u_raduis/90, u_raduis*2, u_raduis/45, GRAY);
        DrawRectangle(padding+u_raduis-u_raduis/90, window_h/2-u_raduis, u_raduis/45, u_raduis*2, GRAY);
        draw_unit_projections(padding+u_raduis, window_h/2, u_raduis/15, u_raduis, deg, BLUE, GREEN, RED);

        DrawRectangleLines(spring_x, spring_y, spring_w, spring_h, LIGHTGRAY);
        {
            float offset = (float)padding/(float)spring_h;
            #define DIF (spring_h-padding*2)/(float)n*((1+offset-sin(a_deg*PI/180.))/(2+2*offset))
            int n = 6;
            int spring_mid = spring_x+spring_w/2;
            int px = spring_mid-spring_w/4;
            int py = spring_y+DIF;
                
            DrawLine(spring_mid, spring_y, px, py, LIGHTGRAY);
            for (int i=0; i<n-1; i++) {
                int x = px + (1-2*(i%2))*spring_w/2;
                int y = py + DIF;
                DrawLine(px, py, x, y, LIGHTGRAY);
                px = x; py = y;
            }
            DrawLine(px, py, spring_mid, py+DIF, LIGHTGRAY);
            DrawRectangle(spring_mid-spring_w/4, py+DIF, spring_w/2, padding/2, LIGHTGRAY);
        }

        DrawRectangle(
                rect_x0,
                window_h/2-rect_h/2,
                rect_w, rect_h,
                LIGHTGRAY);
        {
            int px = 0, py = 0;
            for (double i=1; i<=PI*200; i++) {
                int x = i*rect_w/(PI*200);
                int y = sin(i/100)*u_raduis;
                DrawLine(rect_x0+px, window_h/2-py, rect_x0+x, window_h/2-y, GRAY);
                px = x; py = y;
            }
        }

        int cx = rect_x0+(a_deg*rect_w/360);
        int cy = window_h/2.-(sin(a_deg*PI/180.))*u_raduis;
        DrawCircle(cx, cy, 15, BLUE);
        draw_anim_vecs(cx, cy, 15, u_raduis, deg+90., GREEN);
        draw_anim_vecs(cx, cy, 15, u_raduis, deg-180., RED);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
