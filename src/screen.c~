#include <system.h>
#include <stdarg.h>

#define SCREEN_MAX_X 80
#define SCREEN_MAX_Y 25

char* videomemory = (char*) 0xb8000;
int videopointer = 0;
char backgroundcolor = 0x07;

int curX = 0;
int curY = 0;

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
    do
    {
        // Modulo is negative for negative value. This trick makes abs() unnecessary.
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + value % base];
        value /= base;
    } while ( value );
    // Terminating the string.
    *ptr-- = '\0';
    // Invert the numbers.
    while ( low < ptr )
    {
        char tmp = *low;
        *low++ = *ptr;
        *ptr-- = tmp;
    }
    return rc;
}

void putc(const char a){
	if(a=='\n'){
		curY++;
		curX = 0;
	}else if(a=='\t'){
	
	}else if(a=='\r'){
	
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
}

void hexdumpN(char a){
	if(a==0x00){putc('0');return;}
	if(a==0x01){putc('1');return;}
	if(a==0x02){putc('2');return;}
	if(a==0x03){putc('3');return;}
	if(a==0x04){putc('4');return;}
	if(a==0x05){putc('5');return;}
	if(a==0x06){putc('6');return;}
	if(a==0x07){putc('7');return;}
	if(a==0x08){putc('8');return;}
	if(a==0x09){putc('9');return;}
	if(a==0x0A){putc('A');return;}
	if(a==0x0B){putc('B');return;}
	if(a==0x0C){putc('C');return;}
	if(a==0x0D){putc('D');return;}
	if(a==0x0E){putc('E');return;}
	if(a==0x0F){putc('F');return;}
}

void hexdump(long a){
	hexdumpN((char)((a << 0 ) & 0x0f);
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
				hexdump(c);
			}
		}else{
			putc(deze);
		}
	}
}
