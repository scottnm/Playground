#ifndef __GBA_UTIL_H__
#define __GBA_UTIL_H__

uint16_t
rgb16(
    uint8_t r,
    uint8_t g,
    uint8_t b);

u16_span_t
get_gba_screen_buffer();

uint32_t
get_gba_pixel_index(
    uint8_t row,
    uint8_t col);

#define COLOR_BLUE rgb16(0, 0, 31)
#define COLOR_WHITE rgb16(31, 31, 31)

#endif // __GBA_UTIL_H__

