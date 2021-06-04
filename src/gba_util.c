#include "pch.h"

#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 160
#define NUM_PIXELS (SCREEN_WIDTH * SCREEN_HEIGHT)

// GBA uses a 32-bit arch with 32-bit registers
typedef uint32_t register_t;

static register_t* s_display_control_register = (register_t*)0x4000000; // REG_DISPCNT

typedef struct display_mode_t {
    video_mode_t video_mode;
    bg_mode_t bg_mode;
} display_mode_t;

display_mode_t
get_gba_display_mode()
{
    const register_t display_control = *s_display_control_register;

    // First 3 bits are the video mode
    const video_mode_options_t video_mode = display_control & 0x7;
    // Bits 0x8->0xA are the bg_mode
    const bg_mode_options_t bg_mode = (display_control >> 0x8) & 0xF;

    return (display_mode_t) {
        .video_mode = video_mode,
        .bg_mode = bg_mode,
    };
}

void
set_gba_display_mode(
    video_mode_t video_mode,
    bg_mode_t bg_mode)
{
    dbg_assert(video_mode.value != VIDEO_MODE_UNSET);
    dbg_assert(bg_mode.value != BG_MODE_UNSET);

    // For full docs on how this register is set, see the REG_DISPCNT docs below
    const register_t display_control = 0x0
        // the video mode is the first 3 bits
        | (0x7 & video_mode.value)
        // set the single bit represented by the bg mode
        | (0x1 << bg_mode.value);

    *s_display_control_register = display_control;
}

u16_span_t
get_gba_mode3_screen_buffer()
{
    dbg_assert(get_gba_display_mode().video_mode.value == VIDEO_MODE_MODE3_16BIT_BMP);
    // GBA's VRAM starts at 0x6000000
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

//
// AUX HARDWARE DOCS
//

// DISPLAY CONTROL REGISTER (REG_DISPCNT)
//                           R
// F E D C  B A 9 8  7 6 5 4  3 2 1 0
// W V U S  L K J I  F D B A  C M M M
// 0-2 (M) = The video mode. See video modes list above.
// 3   (C) = Game Boy Color mode. Read only - should stay at 0.
// 4   (A) = This bit controls the starting address of the bitmap in bitmapped modes
//           and is used for page flipping. See the description of the specific
//           video mode for details.
// 5   (B) = Force processing during hblank. Setting this causes the display
//           controller to process data earlier and longer, beginning from the end of
//           the previous scanline up to the end of the current one. This added
//           processing time can help prevent flickering when there are too many
//           sprites on a scanline.
// 6   (D) = Sets whether sprites stored in VRAM use 1 dimension or 2.
//           1 - 1d: tiles are are stored sequentially
//           0 - 2d: each row of tiles is stored 32 x 64 bytes in from the start of the
//           previous row.
// 7   (F) = Force the display to go blank when set. This can be used to save power
//           when the display isn't needed, or to blank the screen when it is being
//           built up (such as in mode 3, which has only one framebuffer). On the SNES,
//           transfers rates to VRAM were improved during a forced blank; it is logical
//           to assume that this would also hold true on the GBA.
//
// 8   (I) = If set, enable display of BG0.
// 9   (J) = If set, enable display of BG1.
// A   (K) = If set, enable display of BG2.
// B   (L) = If set, enable display of BG3.
// C   (S) = If set, enable display of OAM (sprites).
// D   (U) = Enable Window 0
// E   (V) = Enable Window 1
// F   (W) = Enable Sprite Windows
