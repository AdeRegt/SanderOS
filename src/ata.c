#include <system.h>

void loadHDDSector(ata_device dev,char* location,long LBA,long count){
	outportb(0x1F6, 0xE0 | (dev.slave << 4) | ((LBA >> 24) & 0x0F));
	outportb(0x1F1, 0x00);
	outportb(0x1F2, (unsigned char) count);
	outportb(0x1F3, (unsigned char) LBA);
	outportb(0x1F4, (unsigned char)(LBA >> 8));
	outportb(0x1F5, (unsigned char)(LBA >> 16));
	outportb(0x1F7, 0x20);
	insw(0x1F0, location,count);
}

void detectHDDFilesystems(ata_device dev){
	printf(">>OKE<<");
}
