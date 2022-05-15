// Various macros to access the GPIO pins
// on the Raspberry Pi.
//
// X9 - file descriptor.
//

#include "fileio.S"

// Macro nanoSleep to sleep .1 second
// Calls Linux nanosleep entry point.
// Pass a reference to a timespec in both X0 and X1
// First is input time to sleep in seconds and nanoseconds.
// Second is time left to sleep if interrupted (which we ignore)
.macro  nanoSleep
        ldr         X0, =timespecsec
        ldr         X1, =timespecsec
        mov         x8, #__NR_nanosleep
        svc         0
.endm
.macro  GPIOExport  pin
        openFile    gpioexp, O_WRONLY
        mov         X9, X0      // save the file descriptor
        writeFile   X9, \pin, #2

        flushClose  X9
.endm
.macro  GPIODirectionOut   pin
        // copy pin into filename pattern
        ldr         X1, =\pin
        ldr         X2, =gpiopinfile
        add         X2, X2, #20
        ldrb        W3, [X1], #1 // load pin and post increment
        strb        W3, [X2], #1 // store to filename and post increment
        ldrb        W3, [X1]
        strb        W3, [X2]
        openFile    gpiopinfile, O_WRONLY
        mov         X9, X0      // save the file descriptor
        writeFile   X9, outstr, #3
        flushClose  X9
.endm
.macro  GPIOWrite   pin, value
        // copy pin into filename pattern
        ldr         X1, =\pin
        ldr         X2, =gpiovaluefile
        add         X2, X2, #20
        ldrb        W3, [X1], #1    // load pin and post increment
        strb        W3, [X2], #1    // store to filename and post increment
        ldrb        W3, [X1]
        strb        W3, [X2]
        openFile    gpiovaluefile, O_WRONLY
        mov         X9, X0      // save the file descriptor
        writeFile   X9, \value, #1
        flushClose  X9
.endm

.data
timespecsec:   .dword   0
timespecnano:  .dword   100000000
gpioexp:    .asciz  "/sys/class/gpio/export"
gpiopinfile: .asciz "/sys/class/gpio/gpioxx/direction"
gpiovaluefile: .asciz "/sys/class/gpio/gpioxx/value"
outstr:     .asciz  "out"
            .align  4          // save users of this file having to do this.
.text
