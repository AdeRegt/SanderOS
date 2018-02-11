#include <system.h>

void detectDevices(){
	unsigned int i = 0;
	for(unsigned char busses = 0 ; busses < 250 ; busses++){
		for(unsigned char slots = 0 ; slots < 32 ; slots++){
			for(unsigned char functions = 0 ; functions < 8 ; functions++){
				if(pciConfigReadWord(busses,slots,functions,0)!=0xFFFF){
					i = i + 1;
				}
			}
		}
	}
	printf("PCI count: %i",i);
}

unsigned long pciConfigReadWord (unsigned char bus, unsigned char slot, unsigned char func, unsigned char offset){
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
    tmp = (unsigned long)((inportl (0xCFC) >> ((offset & 2) * 8)) & 0xffff);
    return (tmp);
 }
