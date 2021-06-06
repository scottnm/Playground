#include "pch.h"

int
main(
    void)
{
    // The vblank interrupt must be enabled before calling snmgba_wait_for_vblank_interrupt(..). Though we don't need to
    // set a handler for this interrupt since the default dispatcher handles the bios flags.
    snmgba_init_interrupt_system();
    snmgba_enable_interrupt(SNMGBA_INTERRUPT_MASK_ENABLE_VBLANK_INTERRUPTS);

    snmgba_setup_simple_screen(); // A basic blue screen

    // ansi escape sequence to set print co-ordinates
    // /x1b[line;columnH
    printf("\x1b[10;10HHello World!\n");

    while (true)
    {
        snmgba_wait_for_vblank_interrupt();
    }

    return 0;
}
