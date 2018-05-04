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

unsigned char kbshf[128] ={
    0,  27, '!', '@', '#', '€', '%', '^', '&', '*',     /* 9 */
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
//		if(deze & 0x20){
//			printf("__SHIFT__");
//		}else{
			unsigned char realchar = deze-0x80;
			unsigned char karakter = 0x00;
			if(shiftisin==0x01){
				karakter = kbshf[realchar];
			}else{
				karakter = kbdus[realchar];
			}
			if(karakter==0xbe){
				printf("F12 press detected!\n");
			}else if(karakter==0xbf){
				shiftisin = 0x00;
			}else{ 
				printf("%c",karakter);
			}
//		}
	}else if(kbdus[deze]==0xbe){
		shiftisin = 0x01;
	}
	unsigned char thes = 0x00;
	((unsigned char*)0xb8000)[0]=((deze & 0b00000001)>0?'1':'0');
	((unsigned char*)0xb8002)[0]=((deze & 0b00000010)>0?'1':'0');
	((unsigned char*)0xb8004)[0]=((deze & 0b00000100)>0?'1':'0');
	((unsigned char*)0xb8006)[0]=((deze & 0b00001000)>0?'1':'0');
	((unsigned char*)0xb8008)[0]=((deze & 0b00010000)>0?'1':'0');
	((unsigned char*)0xb800a)[0]=((deze & 0b00100000)>0?'1':'0');
	((unsigned char*)0xb800c)[0]=((deze & 0b01000000)>0?'1':'0');
	((unsigned char*)0xb800e)[0]=((deze & 0b10000000)>0?'1':'0');
	((unsigned char*)0xb8010)[0]=((thes & 0b00000001)>0?'1':'0');
	((unsigned char*)0xb8012)[0]=((thes & 0b00000010)>0?'1':'0');
	((unsigned char*)0xb8014)[0]=((thes & 0b00000100)>0?'1':'0');
	((unsigned char*)0xb8016)[0]=((thes & 0b00001000)>0?'1':'0');
	((unsigned char*)0xb8018)[0]=((thes & 0b00010000)>0?'1':'0');
	((unsigned char*)0xb801a)[0]=((thes & 0b00100000)>0?'1':'0');
	((unsigned char*)0xb801c)[0]=((thes & 0b01000000)>0?'1':'0');
	((unsigned char*)0xb801e)[0]=((thes & 0b10000000)>0?'1':'0');
}

extern void irq_keyboard();

void initialiseKeyboard(){
	setInterrupt(32+1, (unsigned long) &irq_keyboard);
	keyboard_send_cmd(0xF4);
}
