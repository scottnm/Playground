#include "pch.h"

#include "in_memory_sample_pic.h"

#define PALETTE_LIME_GREEN 75

int
main(
    void)
{
    set_gba_display_mode(
        (video_mode_t) { .value = VIDEO_MODE_MODE4_8BIT_PALETTE },
        (bg_mode_t) { .value = BG_MODE_ENABLE_BG2 });

    {
        u16_span_t system_palette = get_gba_mode4_palette_buffer();
        cu16_span_t source_palette = as_span(cu16_span_t, g_picPalette);
        memcpy_u16(system_palette, source_palette);
    }

    u8_span_t screen_memory = get_gba_mode4_screen_buffer();

    cu8_span_t picture_bytes = {
        .data = (const uint8_t*)g_picData,
        .count = ARRAYSIZE(g_picData) * sizeof(g_picData[0])
    };

    typedef struct u8vec2_t
    {
        uint8_t x;
        uint8_t y;
    } u8vec2_t;

    u8vec2_t block_position = { .x = 50, .y = 50 };
    const u8vec2_t block_size = { .x = 10, .y = 10 };

    vsync_state_t vsync = {0};
    while (true)
    {
        // Lock our input polling and drawing to a 60hz frame rate.
        vsync = vsync_poll(vsync);
        if (!vsync_is_new_frame(vsync))
        {
            continue;
        }

        // FIXME: Figure out how to account for delta time here
        input_t input = poll_input();
        if (input.dpad_up_pressed)
        {
            block_position.y -= 1;
        }
        if (input.dpad_down_pressed)
        {
            block_position.y += 1;
        }
        if (input.dpad_left_pressed)
        {
            block_position.x -= 1;
        }
        if (input.dpad_right_pressed)
        {
            block_position.x += 1;
        }

        // Draw the picture
        memcpy_u8(screen_memory, picture_bytes);

        // Draw the green square
        for (uint8_t block_row = 0; block_row < block_size.y; block_row += 1)
        {
            uint8_t block_width = block_size.x;
            uint8_t col_start = block_position.x;
            uint8_t row = block_row + block_position.y;
            uint32_t pixel_offset = get_gba_pixel_index(row, col_start);
            memset_u8(
                (u8_span_t) { .data = screen_memory.data + pixel_offset, .count = block_width },
                PALETTE_LIME_GREEN);
        }
    }

    return 0;
}
