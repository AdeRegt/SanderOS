#include <system.h>

void mouselib_int_wait_1(){
	while(1){
		unsigned char dx = inportb(0x64);
		if((dx & 1)==0){
			break;
		}
	}
}


mouselib_int_wait_0:
	mov cx, 65000
	mov dx, 0x64
.wait:
	in al, dx
	bt ax, 0
	jc .okay
	loop .wait
.okay:
	ret

void mouselib_int_wait_0(){
	while(1){
		unsigned char dx = inportb(0x64);
		if((dx & 0)>0){
			break;
		}
	}
}
	
void mouse_init(){
	printf(">>STARTING<<");
	asm volatile("cli");
	mouselib_int_wait_1();
	outportb(0x64,0xA8);
	mouselib_int_wait_1();
	outportb(0x64,0x20);
	mouselib_int_wait_0();
	unsigned char x = inportb(0x60) | 0x02;
	mouselib_int_wait_1();
	outportb(0x64,0x60);
	mouselib_int_wait_1();
	outportb(0x60,x);
	asm volatile("sti");
	printf(">>READY<<");
}
