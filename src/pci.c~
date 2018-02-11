#include <system.h>

void detectDevices(){
	int i = 0;
	unsigned char bus;
	unsigned char device;
	for(bus = 0; bus < 250; bus++) {
         	for(device = 0; device < 32; device++) {
         		unsigned short vendor;
             		if ((vendor = pciConfigReadWord(bus,device,0,0)) != 0xFFFF) {
             			i++;
             		}
         	}
     	}
     	printf("PCI declares %i devices!\n",i);
}

unsigned short pciConfigReadWord (unsigned char bus, unsigned char slot, unsigned char func, unsigned char offset){
    unsigned long address;
    unsigned long lbus  = (unsigned long)bus;
    unsigned long lslot = (unsigned long)slot;
    unsigned long lfunc = (unsigned long)func;
    unsigned short tmp = 0;
 
    /* create configuration address as per Figure 1 */
    address = (unsigned long)((lbus << 16) | (lslot << 11) |
              (lfunc << 8) | (offset & 0xfc) | ((unsigned long)0x80000000));
 
    /* write out the address */
    outportl (0xCF8, address);
    /* read in the data */
    /* (offset & 2) * 8) = 0 will choose the first word of the 32 bits register */
    tmp = (unsigned short)((inportl (0xCFC) >> ((offset & 2) * 8)) & 0xffff);
    return (tmp);
 }
