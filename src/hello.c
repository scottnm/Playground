#include "pch.h"

typedef enum HELLO_MODE4_PALETTE
{
    HELLO_MODE4_PALETTE_WHITE = 0,
    HELLO_MODE4_PALETTE_BLUE,
    HELLO_MODE4_PALETTE_NUM_ENTRIES,
} HELLO_MODE4_PALETTE;

int
main(
    void)
{
    set_gba_display_mode(
        (video_mode_t) { .value = VIDEO_MODE_MODE4_8BIT_PALETTE },
        (bg_mode_t) { .value = BG_MODE_ENABLE_BG2 });

    u16_span_t palette = get_gba_mode4_palette_buffer();
    palette.data[HELLO_MODE4_PALETTE_WHITE] = COLOR_WHITE;
    palette.data[HELLO_MODE4_PALETTE_BLUE] = COLOR_BLUE;

    u8_span_t screen_memory = get_gba_mode4_screen_buffer();

    // clear screen, and draw a blue background
    for (uint32_t pixel = 0; pixel < screen_memory.count; pixel += 1)
    {
        screen_memory.data[pixel] = HELLO_MODE4_PALETTE_BLUE;
    }

    // draw a white HI on the background
    {
        // Draw the 3 vertical lines for the H and I
        for (uint32_t vert_line = 0; vert_line < 3; vert_line += 1)
        {
            uint32_t col = vert_line * 15 + 20;
            for (uint32_t row = 30; row < 50; row += 1)
            {
                uint32_t pixel_index = get_gba_pixel_index(row, col);
                screen_memory.data[pixel_index] = HELLO_MODE4_PALETTE_WHITE;
            }
        }

        // draw the horizontal bar for the H
        uint32_t h_bar_offset = get_gba_pixel_index(40, 20);
        for (uint32_t i = 0; i < 15; i += 1)
        {
            screen_memory.data[h_bar_offset + i] = HELLO_MODE4_PALETTE_WHITE;
        }
    }

    while (true)
    {
        // Loop forever
    }
    return 0;
}
