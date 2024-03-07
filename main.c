#include "raylib.h"
#include <math.h>

#define WINDOW_W 1280
#define WINDOW_H 720
#define UNIT_RADUIS 1/8
#define PADDING 1/30
#define SPRING_W 1/8
#define SLIDERS_H 1/25
#define SLIDERS_W PADDING/3

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
    // main vector
    _draw_vec(x, y, width, height, deg, color1, true);
}

void draw_anim_vecs(const int x, const int y, const int width, const int height, const double deg, const Color color) {
    double d = -(deg+90)*PI/180;
    Vector2 v1 = {.x = x+cos(d)*height, .y = y+sin(d)*height};

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

    bool stopped = false;
    double ampl = 0.5;
    double period = 1;
    double time = 0;
    double a_deg = 0;
    double deg = 0;

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_ESCAPE)) WindowShouldClose();
        if (IsKeyPressed(KEY_SPACE)) stopped = !stopped;

        int window_w = GetScreenWidth();
        int window_h = GetScreenHeight();
        if (window_h<100) { window_h = 100;
            SetWindowSize(window_w, window_h);
        }
        if (window_w<200) { window_w = 200;
            SetWindowSize(window_w, window_h);
        }

        int sliders_h = window_h*SLIDERS_H;
        int sliders_w = window_h*SLIDERS_W;
        int padding = window_w*PADDING;
        int u_raduis = window_w*UNIT_RADUIS;
        if (u_raduis > window_h/2-padding-sliders_h)
            u_raduis = window_h/2-padding-sliders_h;

        int spring_w = window_w*SPRING_W;
        int spring_h = 2*(u_raduis+padding);
        int spring_x = (padding+u_raduis)*2;
        int spring_y = window_h/2-u_raduis-padding;

        int rect_h = u_raduis*2;
        int rect_x0 = u_raduis*2+spring_w+padding*3+sliders_w;
        int rect_w = window_w -(rect_x0+padding);

        if (!stopped) {
            time += GetFrameTime()*180/PI;
            a_deg = fmod(time, 360.);
            deg = a_deg/period-90;
        }

        BeginDrawing();
        ClearBackground(COLOR(BG_COLOR));
        DrawText("amplituda predkosc przyspieszenie", padding/2, window_h/2-u_raduis-padding, padding/2, RED);
        DrawText("amplituda predkosc", padding/2, window_h/2-u_raduis-padding, padding/2, GREEN);
        DrawText("amplituda", padding/2, window_h/2-u_raduis-padding, padding/2, BLUE);
        // unit circle
        DrawCircle(padding+u_raduis, window_h/2, u_raduis, LIGHTGRAY);
        DrawRectangle(padding, window_h/2-u_raduis/90, u_raduis*2, u_raduis/45, GRAY);
        DrawRectangle(padding+u_raduis-u_raduis/90, window_h/2-u_raduis, u_raduis/45, u_raduis*2, GRAY);
        draw_unit_projections(padding+u_raduis, window_h/2, u_raduis/15, u_raduis, deg, BLUE, GREEN, RED);

        // spring
        DrawRectangleLines(spring_x, spring_y, spring_w, spring_h, LIGHTGRAY);
        {
            float offset = (float)padding/(float)spring_h;
            #define DIF (spring_h-padding*2)/(float)n*((1+offset-ampl*sin(a_deg*PI/180./period))/(2+2*offset))
            int n = 9;
            int spring_mid = spring_x+spring_w/2;
            int px = spring_mid-spring_w/4;
            int py = spring_y+DIF;
                
            DrawLine(spring_mid, spring_y, px, py, LIGHTGRAY);
            for (int i=0; i<n-1; i++) {
                int x = px + (1-2*(i%2))*spring_w/2;
                DrawLine(px, py+i*DIF, x, py+(i+1)*DIF, LIGHTGRAY);
                px = x;
            }
            DrawLine(px, py+(n-1)*DIF, spring_mid, py+n*DIF, LIGHTGRAY);
            DrawRectangle(spring_mid-spring_w/4, py+n*DIF, spring_w/2, padding/2, BLUE);
            draw_anim_vecs(spring_mid, py+n*DIF+(float)padding/4, u_raduis/15, ampl*u_raduis*(1/period), deg+90., GREEN);
            draw_anim_vecs(spring_mid, py+n*DIF+(float)padding/4, u_raduis/15, ampl*u_raduis*(1/period)*(1/period), deg-180., RED);
        }

        // sin wave
        DrawRectangle(rect_x0, window_h/2-rect_h/2, rect_w, rect_h, LIGHTGRAY);
        {
            int px = 0, py = 0;
            for (double i=1; i<=PI*200; i++) {
                int x = i*rect_w/(PI*200);
                int y = ampl*sin(i/100/period)*u_raduis;
                DrawLine(rect_x0+px, window_h/2-py, rect_x0+x, window_h/2-y, BLACK);
                px = x; py = y;
            }
        }

        // periods
        int p_offs = rect_w*period;
        while (p_offs<rect_w) {
            DrawRectangle(rect_x0+p_offs-1, window_h/2-rect_h/2, 2, rect_h, BLACK);
            p_offs += rect_w*period;
        }

        // sliders
        // DrawRectangleLines(0, window_h-sliders_h, window_w, sliders_h, LIGHTGRAY);
        DrawRectangle(rect_x0, (window_h+rect_h+padding)/2, rect_w, sliders_h/4, LIGHTGRAY);
        DrawCircle(rect_x0+rect_w*period/2, (window_h+rect_h+padding+sliders_h/4)/2, u_raduis/20., BLUE);
        int msx = GetMouseX();
        int msy = GetMouseY();
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && msx-rect_x0 > rect_w/4) {
            period = ((float)(msx-rect_x0))/(float)rect_w*2;
        }
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && rect_x0+rect_w-msx < 0) {period = 2;}

        DrawRectangle(rect_x0-padding/2-sliders_w, (window_h-rect_h)/2, sliders_w/2, u_raduis, LIGHTGRAY);
        DrawCircle(rect_x0-padding/2-sliders_w+sliders_w/4, window_h/2.-ampl*u_raduis, u_raduis/20., BLUE);
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)
                && rect_x0-padding/2-sliders_w*2 - u_raduis/20 < msx
                && rect_x0-padding/2+sliders_w + u_raduis/20 > msx
                && window_h/2 > msy && window_h/2-rect_h/2-u_raduis/5 < msy
                ) {
            ampl = (window_h/2.-msy)/u_raduis;
            if (ampl > 1) {ampl=1;}
        }

        // moving dot
        int cx = rect_x0+(a_deg*rect_w/360);
        int cy = window_h/2.-(ampl*sin(a_deg*PI/180./period))*u_raduis;
        DrawCircle(cx, cy, u_raduis/15., BLUE);
        draw_anim_vecs(cx, cy, u_raduis/15, ampl*u_raduis*(1/period), deg+90., GREEN);
        draw_anim_vecs(cx, cy, u_raduis/15, ampl*u_raduis*(1/period)*(1/period), deg-180., RED);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
