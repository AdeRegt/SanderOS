#include <system.h>
void mouselib_int_wait_0();
void mouselib_int_wait_1();

void setup_mouse(){
	mouselib_int_wait_1();
	outportb(0x64,0xA8);
	mouselib_int_wait_1();
	outportb(0x64,0xA8);
	mouselib_int_wait_0();
}

void mouselib_int_wait_0(){
	while(1){
		if(inportb(0x64)&0){
			break;
		}
	}
}

void mouselib_int_wait_1(){
	while(1){
		if(!(inportb(0x64)&1)){
			break;
		}
	}
}
