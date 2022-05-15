//
// Examples of the MOV instruction.
//

.global _start // Provide program starting address

_start:
    // Load x2 with 0x1234fedc4f5d6e3a using MOV and MOVK
    MOV     x2, #0x6e3a	
    MOVK    x2, #0x4f5d, LSL #16 
    MOVK    x2, #0xfedc, LSL #32 
    MOVK    x2, #0x1234, LSL #48 

    // Just move W2 into W1
    MOV     W1, W2

    // Now let's see all the shift versions of MOV
    MOV x1, x2, LSL #1      // Logical shift left
    MOV x1, x2, LSR #1      // Logical shift right
    MOV x1, x2, ASR #1      // Arithmetic shift left
    MOV x1, x2, ROR #1      // Rotation right

    // Repeat the above shifts using mnemonics
    LSL x1, x2, #1 // LSL
    LSR x1, x2, #1 // LSL
    ASR x1, x2, #1 // LSL
    ROR x1, x2, #1 // LSL

    // Example that works with 8 bit immediate and shift
    MOV x1, #0xAB000000 // Too big for #imm6

    // Example that can't be represented and reuslts in an error
    // MOV x1, #ABCDEF11 // Too big and can't be represented

    // Example of MOVN
    MOVN w1, #45

    // Example of a MOV that the assembler changes to MOVN
    mov W1, #0xFFFFFFFE // (-2)

    // Setup the exit
    MOV x0, #0
    MOV x8, #93
    SVC 0


