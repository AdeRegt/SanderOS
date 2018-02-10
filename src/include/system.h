void kernel_main();

// screen
void printf(const char* format,...);
void putc(const char a);
char * itoa( int value, char * str, int base );
void puts(const char* format);

void setupIDT();
