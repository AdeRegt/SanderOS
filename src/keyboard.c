#include <system.h>

void keyboard_send_cmd(char val){
        outportb(0x60,val);
}

void keyboard_wait_for_ACK(){
	while(!(inportb(0x64) & 0b00000001));
        while(inportb(0x60)!=0xFA);
}

void keyboard_send_and_get_response_cmd(char val){
        keyboard_send_cmd(val);
        keyboard_wait_for_ACK();
}

void keyboard_int(){
	unsigned char deze = inportb(0x60);
	if(deze & 0x80){
		printf("YAY");
	}
}

extern void irq_keyboard();

void initialiseKeyboard(){
	setInterrupt(32+1, (unsigned long) &irq_keyboard);
	keyboard_send_cmd(0xF4);
	keyboard_wait_for_ACK();
}
