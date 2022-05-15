#include "pch.h"

#include <gba_console.h>
#include <gba_video.h>
#include <gba_interrupt.h>
#include <gba_systemcalls.h>
#include <gba_input.h>

static snmgba_interrupt_mask_t s_interrupt_mask = 0x0;

void
snmgba_init_interrupt_system()
{
    // defined in gba_interrupt.h
    irqInit();
}

void
snmgba_enable_interrupt(
    snmgba_interrupt_mask_t interrupt_mask)
{
    // defined in gba_interrupt.h
    s_interrupt_mask |= interrupt_mask;
    irqEnable(interrupt_mask);
}

// Gets the current interrupt mask
snmgba_interrupt_mask_t
snmgba_get_interrupt_mask()
{
    return s_interrupt_mask;
}

void
snmgba_wait_for_vblank_interrupt()
{
    // We must have previously set the vblank interrupt mask for this to be valid
    assert((s_interrupt_mask & SNMGBA_INTERRUPT_MASK_ENABLE_VBLANK_INTERRUPTS) != 0); // FIXME: bring in dbg_assert support

    // defined in gba_systemcalls.h
    VBlankIntrWait();
}

void
snmgba_setup_simple_screen()
{
    // defined in gba_console.h
    consoleDemoInit();
}
