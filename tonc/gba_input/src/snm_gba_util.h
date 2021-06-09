//
// snm_gba_util.h
// This file provides nice interfaces for any gba utilities that I write or which are provided by 3rd party libraries
// but which I want to wrap because I don't like their interfaces e.g. libgba.
//

#pragma once

#include <gba_interrupt.h> // libgba; needed for IRQ_VBLANK

typedef enum gba_interrupt_mask_t {
    GBA_INTERRUPT_MASK_ENABLE_VBLANK_INTERRUPTS = IRQ_VBLANK,
} gba_interrupt_mask_t;

// Initialized the interrupt system
void
gba_init_interrupt_system();

// Enables the GBA system to fire interrupts specified by the interrupt mask.
void
gba_enable_interrupt(
    gba_interrupt_mask_t interrupt_mask);

// Gets the current interrupt mask
gba_interrupt_mask_t
gba_get_interrupt_mask();

void
gba_wait_for_vblank_interrupt();

// A helper function which bootstraps a simple gba screen with a blue background and white foreground text.
// Additionally, this seems to setup some console system to intercept printf calls to write text to the screen. (neat).
// Pretty much only useful for project setup.
void
gba_setup_simple_screen();

typedef enum gba_video_mode_options_t
{
    GBA_VIDEO_MODE_UNSET = 0,
    // Mode 3 - VRAM is read as a linear sequence of 16 bit values to form a bitmap
    GBA_VIDEO_MODE_MODE3_16BIT_BMP = 3,
    // Mode 4 - VRAM is read as a linear sequence of 8 bit values corresponding to a palette.
    // Since half the memory is required to set each pixel, VRAM supports double buffering
    GBA_VIDEO_MODE_MODE4_8BIT_PALETTE = 4,
} gba_video_mode_options_t;

typedef struct gba_video_mode_t
{
    gba_video_mode_options_t value;
} gba_video_mode_t;
ASSERT_STRONG_TYPEDEF_SIZE(gba_video_mode_t);

typedef enum gba_bg_mode_options_t
{
    GBA_BG_MODE_UNSET = 0x0,
    GBA_BG_MODE_ENABLE_BG2 = 0xA,
} gba_bg_mode_options_t;

typedef struct gba_bg_mode_t
{
    gba_bg_mode_options_t value;
} gba_bg_mode_t;
ASSERT_STRONG_TYPEDEF_SIZE(gba_bg_mode_t);

void
gba_set_display_mode(
    gba_video_mode_t video_mode,
    gba_bg_mode_t bg_mode);
