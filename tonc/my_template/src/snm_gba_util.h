//
// snm_gba_util.h
// This file provides nice interfaces for any gba utilities that I write or which are provided by 3rd party libraries e.g. libgba.
//

#pragma once

#include <gba_interrupt.h> // needed for IRQ_VBLANK

typedef int interrupt_mask_t;
typedef enum snmgba_interrupt_mask_t {
    SNMGBA_INTERRUPT_MASK_ENABLE_VBLANK_INTERRUPTS = IRQ_VBLANK,
} snmgba_interrupt_mask_t;

// Initialized the interrupt system
void
snmgba_init_interrupt_system();

// Enables the GBA system to fire interrupts specified by the interrupt mask.
void
snmgba_enable_interrupt(
    snmgba_interrupt_mask_t interrupt_mask);

// Gets the current interrupt mask
snmgba_interrupt_mask_t
snmgba_get_interrupt_mask();

void
snmgba_wait_for_vblank_interrupt();

// A helper function which bootstraps a simple gba screen with a blue background and white foreground text
// Pretty much only useful for project setup.
void
snmgba_setup_simple_screen();
