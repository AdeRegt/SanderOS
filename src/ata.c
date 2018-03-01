#include <system.h>

void loadHDDSector(ata_device dev,char* location,long LBA,long count){
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

void detectHDDFilesystems(ata_device dev){
	char* xXx = (char*) 0x2000;
	loadHDDSector(dev,xXx,1,1);
	for(int i = 0 ; i < 10 ; i++){printf("%c",xXx[i]);}
}
