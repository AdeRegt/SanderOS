#include <system.h>
#include <stdarg.h>

#define SCREEN_MAX_X 80
#define SCREEN_MAX_Y 25

char* videomemory = (char*) 0xb8000;
int videopointer = 0;
char backgroundcolor = 0x07;

int curX = 0;
int curY = 0;

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
	if(curY>=(SCREEN_MAX_Y-1)){cls();}
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
