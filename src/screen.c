#include <system.h>
#include <stdarg.h>

#define SCREEN_MAX_X 80
#define SCREEN_MAX_Y 25

char* videomemory = (char*) 0xb8000;
int videopointer = 0;
char backgroundcolor = 0x07;

int curX = 0;
int curY = 0;

int getCurX(){return curX;}
int getCurY(){return curY;}
void setCurX(int a){curX = a;}
void setCurY(int a){curY = a;}

void cls(){
	curX = 0;
	curY = 0;
	int x = 0;
	int y = 0;
	for(y = 0 ; y < SCREEN_MAX_Y ; y++){
		for(x = 0 ; x < SCREEN_MAX_X ; x++){
			putc(' ');
		}
	}
	curX = 0;
	curY = 0;
}

void puts(const char* format){
	int xcount = 0;
	char deze;
	while((deze = format[xcount++])!='\0'){
		putc(deze);
	}
}

char * itoa( int value, char * str, int base ){
    char * rc;
    char * ptr;
    char * low;
    // Check for supported base.
    if ( base < 2 || base > 36 )
    {
        *str = '\0';
        return str;
    }
    rc = ptr = str;
    // Set '-' for negative decimals.
    if ( value < 0 && base == 10 )
    {
        *ptr++ = '-';
    }
    // Remember where the numbers start.
    low = ptr;
    // The actual conversion.
    do{
        // Modulo is negative for negative value. This trick makes abs() unnecessary.
        *ptr++ = "ZYXWVUTSRQPONMLKJIHGFEDCBA9876543210123456789ABCDEFGHIJKLMNODPQRSTUVWXYZ"[35 + value % base];
        value /= base;
    } while ( value );
    // Terminating the string.
    *ptr-- = '\0';
    // Invert the numbers.
    while ( low < ptr ){
        char tmp = *low;
        *low++ = *ptr;
        *ptr-- = tmp;
    }
    return rc;
}

void putc(const char a){
	if(curY>=SCREEN_MAX_Y){
		for(int i = (SCREEN_MAX_X*2) ; i < (SCREEN_MAX_X*2)*SCREEN_MAX_Y ; i++){
			videomemory[i-(SCREEN_MAX_X*2)] = videomemory[i];
			videomemory[i] = 0x00; 
		}
		curY = 24;
		curX = 0;
	}
	if(a=='\n'){
		curY++;
		curX = 0;
	}else if(a=='\t'){
	
	}else if(a=='\r'){
	
	}else if(a=='\b'){
		videopointer--;
		videopointer--;
		curX--;
	}else{
		videopointer = ((curY*SCREEN_MAX_X)+(curX))*2;
		videomemory[videopointer++] = a;
		videomemory[videopointer++] = backgroundcolor;
		curX++;
		if(curX==SCREEN_MAX_X){
			curY++;
			curX = 0;
		}
	}
	update_cursor(curX,curY);
}

char nib2col(char a){
	if(a=='0'){return 0x00;}
	if(a=='1'){return 0x01;}
	if(a=='2'){return 0x02;}
	if(a=='3'){return 0x03;}
	if(a=='4'){return 0x04;}
	if(a=='5'){return 0x05;}
	if(a=='6'){return 0x06;}
	if(a=='7'){return 0x07;}
	if(a=='8'){return 0x08;}
	if(a=='9'){return 0x09;}
	return 0x00;
}

void printf(const char* format,...){
	va_list parameters;
	va_start(parameters, format);
	int xcount = 0;
	char deze;
	while((deze = format[xcount++])!='\0'){
		if(deze=='%'){
			deze = format[xcount++];
			if(deze=='c'){
				char c = (char) va_arg(parameters, int /* char promotes to int */);
				putc(c);
			}else if(deze=='i'){
				long c = (long) va_arg(parameters, long);
				char* x = "       ";
				char* y = itoa(c,x,10);
				puts(y);
			}else if(deze=='s'){
				char* c = (char*) va_arg(parameters,char*);
				puts(c);
			}else if(deze=='x'){
				long c = (long) va_arg(parameters, long);
				char* x = {0};
				char* y = itoa(c,x,16);
				puts("0x");
				puts(y);
			}else if(deze=='d'){
				long deze = (long) va_arg(parameters,long);
				putc((deze & 0b10000000000000000000000000000000)>0?'1':'0');
				putc((deze & 0b01000000000000000000000000000000)>0?'1':'0');
				putc((deze & 0b00100000000000000000000000000000)>0?'1':'0');
				putc((deze & 0b00010000000000000000000000000000)>0?'1':'0');
				putc((deze & 0b00001000000000000000000000000000)>0?'1':'0');
				putc((deze & 0b00000100000000000000000000000000)>0?'1':'0');
				putc((deze & 0b00000010000000000000000000000000)>0?'1':'0');
				putc((deze & 0b00000001000000000000000000000000)>0?'1':'0');
				putc((deze & 0b00000000100000000000000000000000)>0?'1':'0');
				putc((deze & 0b00000000010000000000000000000000)>0?'1':'0');
				putc((deze & 0b00000000001000000000000000000000)>0?'1':'0');
				putc((deze & 0b00000000000100000000000000000000)>0?'1':'0');
				putc((deze & 0b00000000000010000000000000000000)>0?'1':'0');
				putc((deze & 0b00000000000001000000000000000000)>0?'1':'0');
				putc((deze & 0b00000000000000100000000000000000)>0?'1':'0');
				putc((deze & 0b00000000000000010000000000000000)>0?'1':'0');
				putc((deze & 0b00000000000000001000000000000000)>0?'1':'0');
				putc((deze & 0b00000000000000000100000000000000)>0?'1':'0');
				putc((deze & 0b00000000000000000010000000000000)>0?'1':'0');
				putc((deze & 0b00000000000000000001000000000000)>0?'1':'0');
				putc((deze & 0b00000000000000000000100000000000)>0?'1':'0');
				putc((deze & 0b00000000000000000000010000000000)>0?'1':'0');
				putc((deze & 0b00000000000000000000001000000000)>0?'1':'0');
				putc((deze & 0b00000000000000000000000100000000)>0?'1':'0');
				putc((deze & 0b00000000000000000000000010000000)>0?'1':'0');
				putc((deze & 0b00000000000000000000000001000000)>0?'1':'0');
				putc((deze & 0b00000000000000000000000000100000)>0?'1':'0');
				putc((deze & 0b00000000000000000000000000010000)>0?'1':'0');
				putc((deze & 0b00000000000000000000000000001000)>0?'1':'0');
				putc((deze & 0b00000000000000000000000000000100)>0?'1':'0');
				putc((deze & 0b00000000000000000000000000000010)>0?'1':'0');
				putc((deze & 0b00000000000000000000000000000001)>0?'1':'0');
			}
		}else{
			if(deze=='\['){
				deze = format[xcount++];
				if(deze=='3'){
					deze = format[xcount++];
					char nibble1 = nib2col(deze);
					backgroundcolor = (backgroundcolor & 0xF0) | (nibble1 & 0xF);;
					deze = format[xcount++];
					if(deze=='m'){
						continue;
					}
				}else if(deze=='4'){
					deze = format[xcount++];
					char nibble1 = nib2col(deze);
					backgroundcolor = (backgroundcolor & 0x0F) | ((nibble1 & 0xF) << 4);
					deze = format[xcount++];
					if(deze=='m'){
						continue;
					}
				}
			}
			putc(deze);
		}
	}
}

void enable_cursor(unsigned char cursor_start, unsigned char cursor_end){
	outportb(0x3D4, 0x0A);
	outportb(0x3D5, (inportb(0x3D5) & 0xC0) | cursor_start);
	outportb(0x3D4, 0x0B);
	outportb(0x3D5, (inportb(0x3E0) & 0xE0) | cursor_end);
}

void disable_cursor(){
	outportb(0x3D4, 0x0A);
	outportb(0x3D5, 0x20);
}

void update_cursor(unsigned int x, unsigned int y){
	unsigned short pos = y * SCREEN_MAX_X + x;
 
	outportb(0x3D4, 0x0F);
	outportb(0x3D5, (unsigned short) (pos & 0xFF));
	outportb(0x3D4, 0x0E);
	outportb(0x3D5, (unsigned short) ((pos >> 8) & 0xFF));
}
