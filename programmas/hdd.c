#include <system.h>
void loadHDDSector(char* location,long LBA,long count);
ata_device dev;

typedef struct{
	unsigned char jmper[3];
	unsigned char oemident[8];
	unsigned short bytespersector;
	unsigned char sectorspercluster;
	unsigned short reservedsectors;
	unsigned char numberoffat;
	unsigned short direntries;
	unsigned short totalsectors;
	unsigned char mediadesc;
	unsigned short numberperfat;
	unsigned short sectorspertrac;
	unsigned short heads;
	unsigned long hidden;
	unsigned long sectormedia;
	unsigned char drivenum;
	unsigned char flags;
	unsigned char signature;
	unsigned long volumeid;
	unsigned char volumelabelstring[11];
	unsigned char systemidentstring[8];
	unsigned char bootcode[448];
	unsigned char bootcodeA;
	unsigned char bootcodeB;
}FAT16Boot;
FAT16Boot block;

void main(ata_device *dxv){
	dev.io_base = dxv->io_base;
	dev.slave   = dxv->slave;
	printf("Hello small world at IOBase %x!\n",dev.io_base);
	void* mx = malloc(512);
	loadHDDSector(mx,0,1);
	unsigned long X = ((unsigned long*)(mx+8+0x01BE))[0];
	printf("Partition1: %x \n",X);
	loadHDDSector(mx,X,1);
	FAT16Boot *boot = (FAT16Boot *)mx;
	printf("oemident: %s \n",boot->oemident);
	printf("volumelabelstring: %s \n",boot->volumelabelstring);
	printf("sizeofstruct: %i \n",sizeof(FAT16Boot));
}


void loadHDDSector(char* location,long LBA,long count){
	outportb(dev.io_base+6, 0xE0 | (dev.slave << 4) | ((LBA >> 24) & 0x0F));
	outportb(dev.io_base+1, 0x00);
	outportb(dev.io_base+2, (unsigned char) count);
	outportb(dev.io_base+3, (unsigned char) LBA);
	outportb(dev.io_base+4, (unsigned char)(LBA >> 8));
	outportb(dev.io_base+5, (unsigned char)(LBA >> 16));
	outportb(dev.io_base+7, 0x20);
	int status;
	while((status = inportb(dev.io_base+7)) & 0x80 ){}
	insw(dev.io_base+0, location,count*512);
}
