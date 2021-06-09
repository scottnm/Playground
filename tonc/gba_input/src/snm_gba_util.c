#include "pch.h"

// libgba includes
#include <gba_console.h>
#include <gba_video.h>
#include <gba_interrupt.h>
#include <gba_systemcalls.h>
#include <gba_input.h>

#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 160
#define NUM_PIXELS (SCREEN_WIDTH * SCREEN_HEIGHT)

// GBA uses both 16-bit and 32-bit registers
typedef uint16_t register16_t;
typedef uint32_t register32_t;
static register32_t* s_display_control_register = (register32_t*)0x4000000; // REG_DISPCNT

static uint8_t*const s_mode4_front_screen_buffer = (uint8_t*)0x6000000;
static uint8_t*const s_mode4_back_screen_buffer  = (uint8_t*)0x600A000;
static uint8_t* s_mode4_current_screen_buffer = NULL;

static gba_interrupt_mask_t s_interrupt_mask = 0x0;

void
gba_init_interrupt_system()
{
    // defined in gba_interrupt.h
    irqInit();
}

void
gba_enable_interrupt(
    gba_interrupt_mask_t interrupt_mask)
{
    // defined in gba_interrupt.h
    s_interrupt_mask |= interrupt_mask;
    irqEnable(interrupt_mask);
}

// Gets the current interrupt mask
gba_interrupt_mask_t
gba_get_interrupt_mask()
{
    return s_interrupt_mask;
}

void
gba_wait_for_vblank_interrupt()
{
    // We must have previously set the vblank interrupt mask for this to be valid
    dbg_assert((s_interrupt_mask & GBA_INTERRUPT_MASK_ENABLE_VBLANK_INTERRUPTS) != 0);

    // defined in gba_systemcalls.h
    VBlankIntrWait();
}

void
gba_setup_simple_screen()
{
    // defined in gba_console.h
    consoleDemoInit();
}

void
gba_set_display_mode(
    gba_video_mode_t video_mode,
    gba_bg_mode_t bg_mode)
{
    dbg_assert(video_mode.value != VIDEO_MODE_UNSET);
    dbg_assert(bg_mode.value != GBA_BG_MODE_UNSET);

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

void
gba_flip_mode4_screen_buffer()
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
