/* vim: tabstop=4 shiftwidth=4 noexpandtab
 * Mouse driver
 */
#include <system.h>

//Mouse.inc by SANiK
//License: Use as you wish, except to cause damage
unsigned char mouse_cycle=0;     //unsigned char
signed char mouse_char[3];    //signed char
signed char mouse_x=0;         //signed char
signed char mouse_y=0;         //signed char

//Mouse functions
void mouse_handler() //struct regs *a_r (not used but just there)
{
  switch(mouse_cycle)
  {
    case 0:
      mouse_char[0]=inportb(0x60);
      mouse_cycle++;
      break;
    case 1:
      mouse_char[1]=inportb(0x60);
      mouse_cycle++;
      break;
    case 2:
      mouse_char[2]=inportb(0x60);
      mouse_x=mouse_char[1];
      mouse_y=mouse_char[2];
      mouse_cycle=0;
      break;
  }
  printf("YAY?");
}

inline void mouse_wait(unsigned char a_type) //unsigned char
{
  unsigned long _time_out=500000; //unsigned int
  if(a_type==0)
  {
    while(_time_out--) //Data
    {
      if((inportb(0x64) & 1)==1)
      {
        return;
      }
    }
    printf("TIMEOUT: A\n");
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
    printf("TIMEOUT: B\n");
    return;
  }
}

inline void mouse_write(unsigned char a_write) //unsigned char
{
  //Wait to be able to send a command
  mouse_wait(1);
  //Tell the mouse we are sending a command
  outportb(0x64, 0xD4);
  //Wait for the final part
  mouse_wait(1);
  //Finally write
  outportb(0x60, a_write);
}

unsigned char mouse_read()
{
  //Get's response from mouse
  mouse_wait(0);
  return inportb(0x60);
}


extern void irq_mouse();

void mouse_install()
{
  unsigned char _status;  //unsigned char

  //Enable the auxiliary mouse device
  mouse_wait(1);
  outportb(0x64, 0xA8);
 
  //Enable the interrupts
  mouse_wait(1);
  outportb(0x64, 0x20);
  mouse_wait(0);
  _status=(inportb(0x60) | 2);
  mouse_wait(1);
  outportb(0x64, 0x60);
  mouse_wait(1);
  outportb(0x60, _status);
 
  //Tell the mouse to use default settings
  mouse_write(0xF6);
  mouse_read();  //Acknowledge
 
  //Enable the mouse
  mouse_write(0xF4);
  mouse_read();  //Acknowledge

  //Setup the mouse handler
  setInterrupt(32+12, irq_mouse);
}




