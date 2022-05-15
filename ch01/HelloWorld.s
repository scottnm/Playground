//
// Assembler program to print "Hello World!" to stdout
//
// X0-X2 - parameters to Linux Function services
// X8    - Linux function number
// 

.global _start // Provide program start address

// Setup the parameters to print hello world and then call Linux system funcs to do it.
_start: mov X0, #1          // 1 = stdout
        ldr X1, =helloworld // string to print, 'helloworld' is a symbol for the string literal
        mov X2, #13         // length of our string
        mov X8, #64         // 64 is the Linux write system call I guess
        svc 0               // call Linux to write system call
        
        // setup the parameters to exit the program and then call Linux to do it
        mov X0, #0          // Use 0 return code
        mov X8, #93         // Service code '93' terminates
        svc 0               // Call Linux to terminate

.data
helloworld: .ascii "Hello World!\n"
