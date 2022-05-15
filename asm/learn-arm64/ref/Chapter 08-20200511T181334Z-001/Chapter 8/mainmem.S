//
// Assembler program to flash three LEDs connnected to the
// Raspberry Pi GPIO port using direct memory access.
//
// W6 - loop variable to flash lights 10 times
//

#include "gpiomem.S"

.global _start	            // Provide program starting address
_start: mapMem
        nanoSleep

        GPIODirectionOut pin17
        GPIODirectionOut pin27
        GPIODirectionOut pin22
        // setup a loop counter for 10 iterations
        mov         W6, #10

loop:   GPIOTurnOn   pin17
        nanoSleep
        GPIOTurnOff   pin17
        GPIOTurnOn    pin27
        nanoSleep
        GPIOTurnOff   pin27
        GPIOTurnOn    pin22
        nanoSleep
brk1:
        GPIOTurnOff   pin22
        //decrement loop counter and see if we loop
        subs    W6, W6, #1      // Subtract 1 from loop register setting status register
        b.ne     loop        // If we haven't counted down to 0 then loop

_end:   mov     X0, #0      // Use 0 return code
        mov     X8, #__NR_exit
        svc     0           // Linus command to terminate program
