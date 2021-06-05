#include "pch.h"

#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 160
#define NUM_PIXELS (SCREEN_WIDTH * SCREEN_HEIGHT)

// GBA uses both 16-bit and 32-bit registers
typedef uint16_t register16_t;
typedef uint32_t register32_t;

static register32_t* s_display_control_register = (register32_t*)0x4000000; // REG_DISPCNT
static const register16_t* s_input_register = (const register16_t*)0x04000130; // REG_KEY (read-only)
static const register16_t* s_display_status_register = (const register16_t*)0x4000004; // REG_DISPSTAT

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
    // Bug #3: add support for mode4 double buffering
    dbg_assert(get_gba_display_mode().video_mode.value == VIDEO_MODE_MODE4_8BIT_PALETTE);

    // GBA's VRAM starts at 0x6000000
    return (u8_span_t) {
        .data = (uint8_t*)0x6000000,
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

typedef union vsync_state_internal_t {
    struct {
        bool is_vblank;
        bool has_changed;
    } internal_view;
    vsync_state_t external_view;
} vsync_state_internal_t;
// The sizes of the internal union and external struct must match. This guarantees that the external struct is at least
// as large as the internal struct so that all writes to the internal struct are captured in the external struct.
static_assert(sizeof(vsync_state_t) == sizeof(vsync_state_internal_t), VSync_struct_sizes_must_match);

vsync_state_t
vsync_poll(
    vsync_state_t vsync)
{
    // For full docs on how this register is set, see the REG_DISPSTAT docs below
    const register16_t display_status = *s_display_status_register;

    // the first bit of the display status is the vrefresh status
    bool vrefresh_status_is_vblank = (display_status & 0x1) == 1;

    const vsync_state_internal_t* vsync_data = (const vsync_state_internal_t*)&vsync;
    bool vrefresh_status_changed = (vsync_data->internal_view.is_vblank != vrefresh_status_is_vblank);

    const vsync_state_internal_t new_state = {
        .internal_view = {
            .is_vblank = vrefresh_status_is_vblank,
            .has_changed = vrefresh_status_changed,
        },
    };

    return new_state.external_view;
}

bool
vsync_is_new_frame(
    vsync_state_t vsync)
{
    const vsync_state_internal_t* vsync_data = (const vsync_state_internal_t*)&vsync;
    return vsync_data->internal_view.has_changed && vsync_data->internal_view.is_vblank;
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

// DISPLAY_STATUS REGISTER (REG_DISPSTAT)
//
//                              R R R
// F E D C  B A 9 8  7 6 5 4  3 2 1 0
// T T T T  T T T T  X X Y H  V Z G W
// 0   (W) = V Refresh status. This will be 0 during VDraw, and 1 during VBlank.
//           VDraw lasts for 160 scanlines; VBlank follows after that and lasts 68
//           scanlines. Checking this is one alternative to checking REG_VCOUNT.
//
// 1   (G) = H Refresh status. This will be 0 during HDraw, and 1 during HBlank HDraw
//           lasts for approximately 1004 cycles; HBlank follows, and lasts
//           approximately 228 cycles, though the time and length of HBlank may in
//           fact vary based on the number of sprites and on rotation/scaling/blending
//           effects being performed on the current line.
//
// 2   (Z) = VCount Triggered Status. Gets set to 1 when a Y trigger interrupt occurs.
// 3   (V) = Enables LCD's VBlank IRQ. This interrupt goes off at the start of VBlank.
// 4   (H) = Enables LCD's HBlank IRQ. This interrupt goes off at the start of HBlank.
// 5   (Y) = Enable VCount trigger IRQ. Goes off when VCount line trigger is reached.
// 8-F (T) = Vcount line trigger. Set this to the VCount value you wish to trigger an
//           interrupt.
