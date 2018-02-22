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
ata_device cdromdevice;

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

extern void irq_hdd();

int atapi_device_init(ata_device dev){
	setInterrupt(32+14, (unsigned long) &irq_hdd);
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
                //printf(" NUL ");
        }else if(atapi_device_init(dev)){
//              printf(" CDR ");
		cdromdevice = dev;
                printf(" CDROM ");
        }else if(ata_device_init(dev)){
                printf(" HDD ");
                //registerMount((char*)"HARDISK",dev,0,0,0);
        }else{
                //printf(" ??? ");
        }
}

//
// C D R O M   M A N A G E R
//
//
#define ATAPI_SECTOR_SIZE 2048
char read_cmd[12] = {0xA8,0,0,0,0,0,0,0,0,0,0,0};
short* readw = (short*) &read_cmd;

void readRawCDROM(long lba,char count,char* locationx){//E
        outportb(cdromdevice.io_base+6,0xE0|(cdromdevice.slave <<4) | ((lba >> 24) & 0x0f));//cdromdevice.slave & ( 1 << 4 ));
        // wachtend
        //int u = 0;
        //for(u = 0 ; u < 5 ; u++){
        //      inportb(cdromdevice.io_base+0x206);
        //}
        outportb(cdromdevice.io_base+1,0x00);
        outportb(cdromdevice.io_base+4,ATAPI_SECTOR_SIZE & 0xFF );
        outportb(cdromdevice.io_base+5,ATAPI_SECTOR_SIZE >> 8);
        outportb(cdromdevice.io_base+7,0xA0);
        // POLL UNTILL UNBUSSY
        char status;
        while((status = inportb(cdromdevice.io_base+7)) & 0x80 ){
                if((status >> 0) & 1){printf("READERROR");for(;;);}
                asm volatile("pause");
        }
        if((status >> 0) & 1){printf("READERROR");for(;;);}
        while(!((status = inportb(cdromdevice.io_base+7))&0x8) && !(status & 0x1)){
                if((status >> 0) & 1){printf("READERROR");for(;;);}
                asm volatile("pause");
        }
        if((status >> 0) & 1){printf("READERROR");for(;;);}
        if(status & 0x1){
                printf("FATAL ERROR: status & 0x1 ");for(;;);
        }
        ata_int_ready();
        read_cmd[9] = count;
        read_cmd[2] = (lba >> 0x18) & 0xFF;
        read_cmd[3] = (lba >> 0x10) & 0xFF;
        read_cmd[4] = (lba >> 0x08) & 0xFF;
        read_cmd[5] = (lba >> 0x00) & 0xFF;
        int y = 0;
        for(y = 0 ; y < 6 ; y++){
                outportw(cdromdevice.io_base,readw[y]);
        }
        //int cnt = 0;
        ata_int_wait();
        int size = (((int)inportb(cdromdevice.io_base+5))<<8)|(int)(inportb(cdromdevice.io_base+4));
        if(size!=ATAPI_SECTOR_SIZE){printf("FATAL ERROR size!=ATAPI_SECTOR_SIZE");for(;;);}
        //short *readev = (short*) locationx;//0x2000;
        int i = 0;
        //ata_int_ready();
        int U = 0;
        while((status = inportb(cdromdevice.io_base+7)) & 0x80 ){
                if((status >> 0) & 1){printf("READERROR");for(;;);}
                asm volatile("pause");
        }

        for(i = 0 ; i < (ATAPI_SECTOR_SIZE/2)*count ; i++){
                short X = inportw(cdromdevice.io_base);
                char A = X;//(X >> 0x00) & 0xFF;
                char B = (X >> 0x08); //& 0xFF;
                locationx[U++] = A;
                locationx[U++] = B;
//              putc(A);putc(B);
        }
        //ata_int_wait();
        //while((status = inportb(cdromdevice.io_base+7)) & 0x88 ){asm volatile("pause"); }

}

void detectATAPI(){
	detectATAdevice(ata_primairy_master);
        detectATAdevice(ata_primairy_slave);
        detectATAdevice(ata_secondary_master);
        detectATAdevice(ata_secondary_slave);
}
