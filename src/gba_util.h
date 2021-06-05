#ifndef __GBA_UTIL_H__
#define __GBA_UTIL_H__

#define MODE4_PALETTE_MAX_SIZE 256

// N.B. can't be `static const` values since C requires such values to known at compile time and C doesn't
// have a way to define compile-time, computed constants
#define COLOR_BLUE rgb16(0, 0, 31)
#define COLOR_WHITE rgb16(31, 31, 31)

// Bug #2: add color16 and palette8 typedefs for better type safety
static
inline
uint16_t
rgb16(
    uint8_t r,
    uint8_t g,
    uint8_t b)
{
    dbg_assert(r < 32);
    dbg_assert(g < 32);
    dbg_assert(b < 32);
    return r + (g<<5) + (b<<10);
}

typedef enum video_mode_options_t
{
    VIDEO_MODE_UNSET = 0,
    // Mode 3 - VRAM is read as a linear sequence of 16 bit values to form a bitmap
    VIDEO_MODE_MODE3_16BIT_BMP = 3,
    // Mode 4 - VRAM is read as a linear sequence of 8 bit values corresponding to a palette.
    // Since half the memory is required to set each pixel, VRAM supports double buffering
    VIDEO_MODE_MODE4_8BIT_PALETTE = 4,
} video_mode_options_t;

typedef struct video_mode_t
{
    video_mode_options_t value;
} video_mode_t;
ASSERT_STRONG_TYPEDEF_SIZE(video_mode_t);

typedef enum bg_mode_options_t
{
    BG_MODE_UNSET = 0x0,
    BG_MODE_ENABLE_BG2 = 0xA,
} bg_mode_options_t;

typedef struct bg_mode_t
{
    bg_mode_options_t value;
} bg_mode_t;
ASSERT_STRONG_TYPEDEF_SIZE(bg_mode_t);

void
set_gba_display_mode(
    video_mode_t video_mode,
    bg_mode_t bg_mode);

u16_span_t
get_gba_mode3_screen_buffer();

u16_span_t
get_gba_mode4_palette_buffer();

u8_span_t
get_gba_mode4_screen_buffer();

void
flip_gba_mode4_screen_buffer();

uint32_t
get_gba_pixel_index(
    uint8_t row,
    uint8_t col);

typedef struct input_t {
    bool a_pressed;
    bool b_pressed;
    bool select_pressed;
    bool start_pressed;
    bool dpad_right_pressed;
    bool dpad_left_pressed;
    bool dpad_up_pressed;
    bool dpad_down_pressed;
    bool right_shoulder_pressed;
    bool left_shoulder_pressed;
} input_t;

input_t
poll_input();

void
block_until_gba_vsync();

#endif // __GBA_UTIL_H__

