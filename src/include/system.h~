void kernel_main();

// screen
void printf(const char* format,...);
void putc(const char a);
char * itoa( int value, char * str, int base );
void puts(const char* format);

// IDT
void setupIDT();
void lidt(void* base, unsigned short size);

// ports
void outportb(unsigned short port, unsigned char val);
unsigned char inportb(unsigned short port);
void outportw(unsigned short port, unsigned short val);
unsigned short inportw(unsigned short port);
void outportl(unsigned short port, unsigned long val);
unsigned long inportl(unsigned short port);
