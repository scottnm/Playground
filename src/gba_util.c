#include "pch.h"

#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 160
#define NUM_PIXELS (SCREEN_WIDTH * SCREEN_HEIGHT)

uint16_t
rgb16(
    uint8_t r,
    uint8_t g,
    uint8_t b)
{
    assert(r < 32);
    assert(g < 32);
    assert(b < 32);
    return r + (g<<5) + (b<<10);
}

u16_span_t
get_gba_screen_buffer()
{
    // GBA's VRAM(?) starts at 0x6000000
    return (u16_span_t) {
        .data = (uint16_t*)0x6000000,
        .count = NUM_PIXELS,
    };
}

uint32_t
get_gba_pixel_index(
    uint8_t row,
    uint8_t col)
{
    return (row * SCREEN_WIDTH) + col;
}
