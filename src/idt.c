#include <system.h>

void setupIDT(){
	
}

static inline void lidt(void* base, unsigned short size){   // This function works in 32 and 64bit mode
    struct {
        unsigned short length;
        void*    base;
    } __attribute__((packed)) IDTR = { size, base };
 
    asm ( "lidt %0" : : "m"(IDTR) );  // let the compiler choose an addressing mode
}
