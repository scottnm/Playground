//
// Examples of the MOV instruction.
//

.global _start // Provide program starting address

_start:
    // load x1,x2,x3 with 0x123456789012345678901234567890123456789012345678
    // x1 = 0x1234|5678|9012|3456
    mov  x1, #0x3456
    movk x1, #0x9012, LSL #16
    movk x1, #0x5678, LSL #32
    movk x1, #0x1234, LSL #48
    // x2 = 0x7890|1234|5678|9012
    mov  x2, #0x9012
    movk x2, #0x5678, LSL #16
    movk x2, #0x1234, LSL #32
    movk x2, #0x7890, LSL #48
    // x3 = 0x3456|7890|1234|5678
    mov  x3, #0x3456
    movk x3, #0x7890, LSL #16
    movk x3, #0x1234, LSL #32
    movk x3, #0x5678, LSL #48

    // load x4,x5,x6 with 0x781234567890123456789012345678901234567890123456
    // x4 = 0x7812|3456|7890|1234
    mov  x4, #0x1234
    movk x4, #0x7890, LSL #16
    movk x4, #0x3456, LSL #32
    movk x4, #0x7812, LSL #48
    // x5 = 0x5678|9012|3456|7890
    mov  x5, #0x7890
    movk x5, #0x3456, LSL #16
    movk x5, #0x9012, LSL #32
    movk x5, #0x5678, LSL #48
    // x6 = 0x1234|5678|9012|3456
    mov  x6, #0x3456
    movk x6, #0x9012, LSL #16
    movk x6, #0x5678, LSL #32
    movk x6, #0x1234, LSL #48

    add x1, x1, x4
    adc x2, x2, x5
    adc x3, x3, x6

    lsr x0, x3, #48
    lsr x0, x0, #8
    mov x8, #93
    svc 0
