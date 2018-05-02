#include <system.h>
void loadHDDSector(char* location,long LBA,long count);
ata_device dev;

void main(ata_device *dxv){
	dev.io_base = dxv->io_base;
	dev.slave   = dxv->slave;
	printf("Hello small world at IOBase %x!\n",dev.io_base);
	void* mx = malloc(512);
	loadHDDSector(mx,0,1);
	printf("Partition1: %x \n",mx+0x01BE+8);
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
