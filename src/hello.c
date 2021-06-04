#include "pch.h"

#include "in_memory_sample_pic.h"

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

    {
        u16_span_t system_palette = get_gba_mode4_palette_buffer();
        cu16_span_t source_palette = as_span(cu16_span_t, g_picPalette);
        memcpy_u16(system_palette, source_palette);
    }

    u8_span_t screen_memory = get_gba_mode4_screen_buffer();

    // Draw the picture
    cu8_span_t picture_bytes = {
        .data = (const uint8_t*)g_picData,
        .count = ARRAYSIZE(g_picData) * sizeof(g_picData[0])
    };

    memcpy_u8(screen_memory, picture_bytes);

    while (true)
    {
        // Loop forever
    }
    return 0;
}
