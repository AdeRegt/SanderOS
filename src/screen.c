#include <system.h>
#include <stdarg.h>

char* videomemory = (char*) 0xb8000;
int videopointer = 0;
char backgroundcolor = 0x07;

void putc(const char a){
	videomemory[videopointer++] = a;
	videomemory[videopointer++] = backgroundcolor;
}

void printf(const char* format,...){
	va_list parameters;
	va_start(parameters, format);
	int xcount = 0;
	char deze;
	while((deze = format[xcount++])!='\0'){
		putc(deze);
	}
}
