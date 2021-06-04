#ifndef __GBA_UTIL_H__
#define __GBA_UTIL_H__

// N.B. can't be `static const` values since C requires such values to known at compile time and C doesn't
// have a way to define compile-time, computed constants
#define COLOR_BLUE rgb16(0, 0, 31)
#define COLOR_WHITE rgb16(31, 31, 31)

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
    VIDEO_MODE_16BIT_LINEAR_BITMAP = 3, // Mode3
} video_mode_options_t;

typedef struct video_mode_t
{
    video_mode_options_t value;
} video_mode_t;

// FIXME: better name than bg?
typedef enum bg_mode_options_t
{
    BG_MODE_UNSET = 0x0,
    BG_MODE_ENABLE_BG2 = 0xA,
} bg_mode_options_t;

typedef struct bg_mode_t
{
    bg_mode_options_t value;
} bg_mode_t;

u16_span_t
get_gba_screen_buffer();

uint32_t
get_gba_pixel_index(
    uint8_t row,
    uint8_t col);

void
set_gba_display_mode(
    video_mode_t video_mode,
    bg_mode_t bg_mode);

#endif // __GBA_UTIL_H__

