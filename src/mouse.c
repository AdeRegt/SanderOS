#include <system.h>
inline void mouse_wait(char a_type);

char mouselib_int_read();
void mouselib_int_write(char byte);
extern irq_mouse();
void mousehandler();

void setup_mouse(){
	mouse_wait(1);
	outportb(0x64,0xA8);
	mouse_wait(1);
	outportb(0x64,0x20);
	mouse_wait(0);
	char buff = inportb(0x60)|2;
	mouse_wait(1);
	outportb(0x64,0x60);
	mouse_wait(1);
	outportb(0x60,buff);
	mouselib_int_write(0xF6);
	mouselib_int_read();
	mouselib_int_write(0xF4);
	mouselib_int_read();
	setInterrupt(32+12, (unsigned long) &irq_mouse);
}

void mousehandler(){}

inline void mouse_wait(char a_type){
  short _time_out=100000; 
  if(a_type==0){
    while(_time_out--){
      if((inportb(0x64) & 1)==1){
        return;
      }
    }
    return;
  }else{
    while(_time_out--) {
      if((inportb(0x64) & 2)==0){
        return;
      }
    }
    return;
  }
}

void mouselib_int_write(char byte){
	mouse_wait(1);
	outportb(0x64,0xD4);
	mouse_wait(1);
	outportb(0x60,byte);
}


char mouselib_int_read(){
	mouse_wait(0);
	return inportb(0x60);
}

