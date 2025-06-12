#include <grrlib.h>
#include <stdlib.h>
#include <wiiuse/wpad.h>
#define COLOR_COUNT 6
#define FADE_SPEED 0.005f

typedef struct {
    u8 r, g, b;
} color;

u8 lerp(u8 a, u8 b, float t) {
    return (u8)((1.0f - t) * a + t * b);
}

int main(int argc, char **argv) {
    GRRLIB_Init();
    WPAD_Init();

    color colors[COLOR_COUNT] = {
        {255, 0,   0},
        {255, 127, 0},
        {255, 255, 0},
        {0,   255, 0},
        {0,   0,   255},
        {127, 0,   255}
    };

    int current = 0;
    int next = 1;
    float t = 0.0f;
    bool fadeout = false;

    while (!fadeout) {
        WPAD_ScanPads();
        u32 pressed = WPAD_ButtonsDown(0);

        if (pressed & WPAD_BUTTON_HOME) {
            fadeout = true;
            break;
        }

        color c1 = colors[current];
        color c2 = colors[next];

        u8 r = lerp(c1.r, c2.r, t);
        u8 g = lerp(c1.g, c2.g, t);
        u8 b = lerp(c1.b, c2.b, t);

        GRRLIB_FillScreen(RGBA(r, g, b, 255));
        GRRLIB_Render();

        t += FADE_SPEED;

        if (t >= 1.0f) {
            t = 0.0f;
            current = next;
            next = (next + 1) % COLOR_COUNT;
        }
    }
    color c = colors[current];
    t = 0.0f;
    while (t <= 1.0f) {
        WPAD_ScanPads();

        u8 r = lerp(c.r, 0, t);
        u8 g = lerp(c.g, 0, t);
        u8 b = lerp(c.b, 0, t);

        GRRLIB_FillScreen(RGBA(r, g, b, 255));
        GRRLIB_Render();
        t += FADE_SPEED;
    }

    GRRLIB_Exit();
    exit(0);
}
