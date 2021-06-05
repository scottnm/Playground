#include "pch.h"

#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 160
#define NUM_PIXELS (SCREEN_WIDTH * SCREEN_HEIGHT)

// GBA uses both 16-bit and 32-bit registers
typedef uint16_t register16_t;
typedef uint32_t register32_t;

static register32_t* s_display_control_register = (register32_t*)0x4000000; // REG_DISPCNT
static const register16_t* s_input_register = (register16_t*)0x04000130; // REG_KEY (read-only)
static volatile const register16_t* s_vcount_register = (register16_t*)0x4000006; // REG_VCOUNT

// In Mode4, GBA's VRAM is split into two buffers.
// The first buffer starts at 0x6000000
// The second buffer starts at 0x600A000
// This isn't just splitting the mode3 buffer in half. It's unclear why.
static uint8_t*const s_mode4_front_screen_buffer = (uint8_t*)0x6000000;
static uint8_t*const s_mode4_back_screen_buffer  = (uint8_t*)0x600A000;
static uint8_t* s_mode4_current_screen_buffer = NULL;

typedef struct display_mode_t {
    video_mode_t video_mode;
    bg_mode_t bg_mode;
} display_mode_t;

display_mode_t
get_gba_display_mode()
{
    const register32_t display_control = *s_display_control_register;

    // First 3 bits are the video mode
    const video_mode_t video_mode = { .value = display_control & 0x7 };
    // Bits 0x8->0xA are the bg_mode
    const bg_mode_t bg_mode = { .value = (display_control >> 0x8) & 0xF };

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
    const register32_t display_control = 0x0
        // the video mode is the first 3 bits
        | (0x7 & video_mode.value)
        // set the single bit represented by the bg mode
        | (0x1 << bg_mode.value);

    *s_display_control_register = display_control;

    // Ensure s_mode4_current_screen_buffer is setup (even if mode4 isn't used)
    s_mode4_current_screen_buffer = s_mode4_front_screen_buffer;
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

u16_span_t
get_gba_mode4_palette_buffer()
{
    dbg_assert(get_gba_display_mode().video_mode.value == VIDEO_MODE_MODE4_8BIT_PALETTE);

    // GBA's Mode4 palette memory starts at 0x5000000
    return (u16_span_t) {
        .data = (uint16_t*)0x5000000,
        .count = MODE4_PALETTE_MAX_SIZE,
    };
}

u8_span_t
get_gba_mode4_screen_buffer()
{
    dbg_assert(get_gba_display_mode().video_mode.value == VIDEO_MODE_MODE4_8BIT_PALETTE);

    return (u8_span_t) {
        .data = s_mode4_current_screen_buffer,
        .count = NUM_PIXELS,
    };
}

void
flip_gba_mode4_screen_buffer()
{
    dbg_assert(get_gba_display_mode().video_mode.value == VIDEO_MODE_MODE4_8BIT_PALETTE);

    // For full docs on how this register is set, see the REG_DISPCNT docs below
    register32_t display_control = *s_display_control_register;

    // The fourth bit controls whether we are using the backbuffer or not
    static const register32_t s_backbuffer_bit = (1 << 4); // 0x10

    // The backbuffer was being displayed, flip to the frontbuffer for rendering
    uint8_t* new_inactive_screen_buffer;
    if (display_control & s_backbuffer_bit)
    {
        display_control &= ~s_backbuffer_bit;
        new_inactive_screen_buffer = s_mode4_back_screen_buffer;
    }
    // The frontbuffer was being displayed, flip to the backbuffer for rendering
    else
    {
        display_control |= s_backbuffer_bit;
        new_inactive_screen_buffer = s_mode4_front_screen_buffer;
    }

    s_mode4_current_screen_buffer = new_inactive_screen_buffer;
    *s_display_control_register = display_control;
}

uint32_t
get_gba_pixel_index(
    uint8_t row,
    uint8_t col)
{
    return (row * SCREEN_WIDTH) + col;
}

input_t
poll_input()
{
    // For full docs on how this register is set, see the REG_KEY docs below
    register16_t input_bits = *s_input_register;
    return (input_t) {
        .a_pressed               = !(input_bits & 0x001),
        .b_pressed               = !(input_bits & 0x002),
        .select_pressed          = !(input_bits & 0x004),
        .start_pressed           = !(input_bits & 0x008),
        .dpad_right_pressed      = !(input_bits & 0x010),
        .dpad_left_pressed       = !(input_bits & 0x020),
        .dpad_up_pressed         = !(input_bits & 0x040),
        .dpad_down_pressed       = !(input_bits & 0x080),
        .right_shoulder_pressed  = !(input_bits & 0x100),
        .left_shoulder_pressed   = !(input_bits & 0x200),
    };
}

void
block_until_gba_vsync()
{
    // The vcount register writes 160 lines in VDraw mode (drawing) and then waits for 68 lines in VBlank mode
    while (*s_vcount_register < 160) {}
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

// READ-ONLY INPUT REGISTER (REG_KEY)
//
//              R R  R R R R  R R R R
// F E D C  B A 9 8  7 6 5 4  3 2 1 0
// X X X X  X X J I  D U L R  S E B A
// 0 (A) = A button
// 1 (B) = B button
// 2 (E) = Select button
// 3 (S) = Start button
// 4 (R) = D-pad Right
// 5 (L) = D-pad Left
// 6 (U) = D-pad Up
// 7 (D) = D-pad Down
// 8 (I) = Right shoulder button
// 9 (J) = Left shoulder button

// VCOUNT REGISTER (Read-only) (REG_VCOUNT)
// Address: 0x4000006 - LCY / REG_VCOUNT (Read Only)
//
// This location stores the current y location of the LCD hardware. It is incremented as the lines are drawn. The 160
// lines of display are followed by 68 lines of Vblank period, before the whole thing starts again for the next frame.
// Waiting for this register to reach 160 is one way to synchronize a program to 60Hz.
