#include <system.h>

unsigned char kbdus[128] ={
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',     /* 9 */
  '9', '0', '-', '=', '\b',     /* Backspace */
  '\t',                 /* Tab */
  'q', 'w', 'e', 'r',   /* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', /* Enter key */
    0,                  /* 29   - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',     /* 39 */
 '\'', '`',   0xbf,                /* Left shift */
 '\\', 'z', 'x', 'c', 'v', 'b', 'n',                    /* 49 */
  'm', ',', '.', '/',   0xbf,                              /* Right shift */
  '*',
    0,  /* Alt */
  ' ',  /* Space bar */
    0,  /* Caps lock */
    0xb0,  /* 59 - F1 key ... > */
    0xb1,   0xb2,   0xb3,   0xb4,   0xb5,   0xb6,   0xb7,   0xb8,
    0xb9,  /* < ... F10 */
    0,  /* 69 - Num lock*/
    0,  /* Scroll Lock */
    0,  /* Home key */
    2,  /* Up Arrow */
    0,  /* Page Up */
  '-',
    0,  /* Left Arrow */
    0,
    0,  /* Right Arrow */
  '+',
    0,  /* 79 - End key*/
    1,  /* Down Arrow */
    0,  /* Page Down */
    0,  /* Insert Key */
    0,  /* Delete Key */
    0,   0,   0,
    0xba,  /* F11 Key */
    0xbb,       /* F12 Key */
    0,  /* All other keys are undefined */
};

unsigned char kbshf[128] ={
    0,  27, '!', '@', '#', '$', '%', '^', '&', '*',     /* 9 */
  '(', ')', '_', '+', '\b',     /* Backspace */
  '\t',                 /* Tab */
  'Q', 'W', 'E', 'R',   /* 19 */
  'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', /* Enter key */
    0,                  /* 29   - Control */
  'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':',     /* 39 */
 '"', '`',   0xbf,                /* Left shift */
 '\\', 'Z', 'X', 'C', 'V', 'B', 'N',                    /* 49 */
  'M', ',', '.', '/',   0xbf,                              /* Right shift */
  '*',
    0,  /* Alt */
  ' ',  /* Space bar */
    0,  /* Caps lock */
    0,  /* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,  /* < ... F10 */
    0,  /* 69 - Num lock*/
    0,  /* Scroll Lock */
    0,  /* Home key */
    2,  /* Up Arrow */
    0,  /* Page Up */
  '-',
    0,  /* Left Arrow */
    0,
    0,  /* Right Arrow */
  '+',
    0,  /* 79 - End key*/
    1,  /* Down Arrow */
    0,  /* Page Down */
    0,  /* Insert Key */
    0,  /* Delete Key */
    0,   0,   0,
    0,  /* F11 Key */
    0xbe,       /* F12 Key */
    0,  /* All other keys are undefined */
};

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

char shiftisin = 0x00;
void keyboard_int(){
	unsigned char deze = inportb(0x60);
	if(deze & 0x80){
		unsigned char realchar = deze-0x80;
		unsigned char karakter = 0x00;
		if(shiftisin==0x01){
			karakter = kbshf[realchar];
		}else{
			karakter = kbdus[realchar];
		}
		if(karakter>=0xb0&&karakter<0xbe){
			unsigned char valve = karakter - 0xb0;
			printf("\nF%x press detected! Time to yield!!\n",valve+1);
			yield(valve);
		}else if(karakter==0xbf){
			shiftisin = 0x00;
		}else{ 
			//printf("%c",karakter);
			((unsigned char*)0x10000)[0] = karakter;
			//printf("%c",((unsigned char*)0x10000)[0]);
		}
	}else if(deze == 42 || deze == 54){
		shiftisin = 0x01;
	}
}

unsigned char getc(){
	int lapse = 0;
	while(1){
		unsigned char deze = ((unsigned char*)0x10000)[0];
		if(deze!=0x00){
			break;
		}
		((unsigned char*)0xb8000)[0] = deze;
	}
	return ((unsigned char*)0x10000)[0];
}

extern void irq_keyboard();

void initialiseKeyboard(){
	setInterrupt(32+1, (unsigned long) &irq_keyboard);
	keyboard_send_cmd(0xFF);
	while(inportb(0x60)!=0xAA);
	keyboard_send_cmd(0xF4);
	
}
