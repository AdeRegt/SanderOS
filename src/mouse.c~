#include <system.h>

void mouselib_int_wait_1(){
	while(1){
		unsigned char dx = inportb(0x64);
		if((dx & 1)==1){}
	}
}
	
void mouse_init(){
	asm volatile("cli");
	mouselib_int_wait_1();
	outport(0x64,0xA8);
	asm volatile("sti");
	printf(">>READY<<");
}
