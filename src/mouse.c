#include <system.h>
inline void mouse_wait(char a_type);

void setup_mouse(){
	mouse_wait(1);
	outportb(0x64,0xA8);
	mouse_wait(1);
	outportb(0x64,0x20);
	mouse_wait(0);
}

inline void mouse_wait(char a_type){
  short _time_out=100000; 
  if(a_type==0)
  {
    while(_time_out--) //Data
    {
      if((inportb(0x64) & 1)==1)
      {
        return;
      }
    }
    return;
  }
  else
  {
    while(_time_out--) //Signal
    {
      if((inportb(0x64) & 2)==0)
      {
        return;
      }
    }
    return;
  }
}
