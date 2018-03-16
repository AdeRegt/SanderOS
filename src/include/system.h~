#define NULL 0

void kernel_main();

// screen
void printf(const char* format,...);
void putc(const char a);
char * itoa( int value, char * str, int base );
void puts(const char* format);

// IDT
void setupIDT();
void lidt(void* base, unsigned short size);
extern void irq_defaulte();
void setInterrupt(int entity,unsigned long location);

// ports
void outportb(unsigned short port, unsigned char val);
unsigned char inportb(unsigned short port);
void outportw(unsigned short port, unsigned short val);
unsigned short inportw(unsigned short port);
void outportl(unsigned short port, unsigned long val);
unsigned long inportl(unsigned short port);

void insw (unsigned short port, void *addr, unsigned long cnt);

// pci
void detectDevices();
unsigned long pciConfigReadWord (unsigned char bus, unsigned char slot, unsigned char func, unsigned char offset);
extern unsigned short getcs();

// keyboard
void initialiseKeyboard();

// power
void acpiPowerOff(void);
int acpiEnable();

typedef struct {
	unsigned int io_base;
	unsigned int control;
	unsigned int slave;
	unsigned long read;
	unsigned long write;
	unsigned long eject;
}ata_device;

// atapi
void detectATAPI();
void readRawCDROM(long lba,char count,char* locationx);

// ata
void detectHDDFilesystems(ata_device dev);

typedef struct {
	unsigned long read;
	unsigned long write;
	unsigned long eject;
	unsigned long identifier;
}Device;

typedef struct {
	unsigned long read;
	unsigned long write;
	unsigned long list;
	unsigned long remove;
	unsigned long device;
}Filesystem;
