#include <system.h>
#define IDT_MAX 256

typedef struct {
    unsigned short base_lo;
    unsigned short sel; /* Our kernel segment goes here! */
    unsigned char always0; /* This will ALWAYS be set to 0! */
    unsigned char flags; /* Set using the above table! */
    unsigned short base_hi;
} IDTEntry;

IDTEntry idttable[IDT_MAX];

void setupIDT(){

	// settingup PIC
	outportb(0x20, 0x11);
    	outportb(0xA0, 0x11);
    	outportb(0x21, 0x20);
    	outportb(0xA1, 0x28);
    	outportb(0x21, 0x04);
    	outportb(0xA1, 0x02);
    	outportb(0x21, 0x01);
    	outportb(0xA1, 0x01);
    	outportb(0x21, 0x00);
    	outportb(0xA1, 0x00);
    	
    	// install everything
    	for(int i = 0 ; i < IDT_MAX ; i++){
    		installInterrupt(i,(unsigned long)&irq_defaulte);
    	}
    	
    	// settingup idt
    	lidt(idttable,IDT_MAX);
    	
    	// activating idt
    	asm volatile("sti");
}

void installInterrupt(int entity,unsigned long location){
	idttable[entity].base_lo = (location & 0xFFFF);
    	idttable[entity].base_hi = (location >> 16) & 0xFFFF;
    	idttable[entity].sel = 0x10;getcs();
    	idttable[entity].always0 = 0;
    	idttable[entity].flags = 0x8E;
}

void lidt(void* base, unsigned short size){   // This function works in 32 and 64bit mode
    struct {
        unsigned short length;
        void*    base;
    } __attribute__((packed)) IDTR = { size, base };
 
    asm ( "lidt %0" : : "m"(IDTR) );  // let the compiler choose an addressing mode
}
