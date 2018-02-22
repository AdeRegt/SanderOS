#include <system.h>

typedef struct {
	unsigned int io_base;
	unsigned int control;
	unsigned int slave;
	unsigned long read;
	unsigned long write;
	unsigned long eject;
}ata_device;

ata_device ata_primairy_master  = {.io_base = 0x1F0, .control = 0x3F6, .slave = 0};
ata_device ata_primairy_slave   = {.io_base = 0x1F0, .control = 0x3F6, .slave = 1};
ata_device ata_secondary_master = {.io_base = 0x170, .control = 0x376, .slave = 0};
ata_device ata_secondary_slave  = {.io_base = 0x170, .control = 0x376, .slave = 1};

void ata_wait(ata_device dev){
        inportb(dev.control);
        inportb(dev.control);
        inportb(dev.control);
        inportb(dev.control);
}

void ata_wait2(ata_device dev){
        ata_wait(dev);
        char status = 0;
        while((status=inportb(dev.io_base+0x07))&0x80){}
}

int atapi_device_init(ata_device dev){
        outportb(dev.io_base+0x01,1);
        outportb(dev.control,0);
        outportb(dev.io_base+0x06,0xA0 | dev.slave <<4 );
        ata_wait(dev);
        outportb(dev.io_base+0x07,0xEC);
        ata_wait(dev);
        unsigned char cl = inportb(dev.io_base+0x04);
        unsigned char ch = inportb(dev.io_base+0x05);
        if(!(cl == 0x14 && ch == 0xEB) && !(cl == 0x69 && ch == 0x96)){
                return 0;
        }
        return 1;
}

int ata_device_init(ata_device dev){
        outportb(dev.io_base+0x01,1);
        outportb(dev.control,0);
        outportb(dev.io_base+0x06,0xA0 | dev.slave <<4 );
        ata_wait(dev);
        outportb(dev.io_base+0x07,0xEC);
        ata_wait(dev);
        int status = inportb(dev.io_base+0x07);
        if(status==0||(status & 0x01 )){
                return 0;
        }
        ata_wait2(dev);
        int i = 0;
        for(i = 0 ; i < 256 ; i++){
                inportb(dev.io_base);
        }
        outportb(dev.control,0);
        return 1;
}

void detectATAdevice(ata_device dev){
	// first, softreset
        outportb(dev.control,0x04);
        outportb(dev.control,0x00);

        outportb(dev.io_base + 0x06 , 0xA0 | dev.slave << 4 );

        ata_wait(dev);

        unsigned char cl = inportb(dev.io_base + 0x04);
        unsigned char ch = inportb(dev.io_base + 0x05);

        if(cl==0xFF&&ch==0xFF){
                //kernel_print(" NUL ");
        }else if(atapi_device_init(dev)){
//              kernel_print(" CDR ");
                printf(" CDROM ");
        }else if(ata_device_init(dev)){
                printf(" HDD ");
                //registerMount((char*)"HARDISK",dev,0,0,0);
        }else{
                //kernel_print(" ??? ");
        }
}

void detectATAPI(){
	detectATAdevice(ata_primairy_master);
        detectATAdevice(ata_primairy_slave);
        detectATAdevice(ata_secondary_master);
        detectATAdevice(ata_secondary_slave);
}
