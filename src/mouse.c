#include <system.h>

void mouselib_int_wait_1(){
	while(1){
		unsigned char dx = inportb(0x64);
		if((dx & 1)==0){
			break;
		}
	}
}
	
void mouse_init(){
	printf(">>STARTING<<");
	asm volatile("cli");
	mouselib_int_wait_1();
	outportb(0x64,0xA8);
	asm volatile("sti");
	printf(">>READY<<");
}
