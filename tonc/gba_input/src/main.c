#include "pch.h"

int
main(
    void)
{
    // The vblank interrupt must be enabled before calling gba_wait_for_vblank_interrupt(..). Though we don't need to
    // set a handler for this interrupt since the default dispatcher handles the bios flags.
    gba_init_interrupt_system();
    gba_enable_interrupt(GBA_INTERRUPT_MASK_ENABLE_VBLANK_INTERRUPTS);

    gba_set_display_mode(
        (gba_video_mode_t) { .value = GBA_VIDEO_MODE_MODE4_8BIT_PALETTE },
        (gba_bg_mode_t) { .value = GBA_BG_MODE_ENABLE_BG2 });

    #error TODOs
    /*
     * 1. set the palette mem (only need like 13 values, 3 values for each of 4 quadrants + 1 value for black)
     *      - each of the 4 quadrants can be in one of the 3 states (on, off, held)
     * 2. set the video memory with the constant video data (we'll only update the palette mem to flip colors)
     * 3. poll for input (bring it in from my last util but update it to include new curr vs prev functionality)
     */
    while (true)
    {
        gba_wait_for_vblank_interrupt();
    }

    return 0;
}
