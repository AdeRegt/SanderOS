#include <system.h>
#define ATAPI_SECTOR_SIZE 2048

//
// INIT ISO
//
//

unsigned char buffer[ATAPI_SECTOR_SIZE];
unsigned short dirtableloc;

unsigned char getB(int a){
	return buffer[a] & 0xff;
}

unsigned short getS(short a){
	return (unsigned short)((unsigned short*)buffer+a)[0];
}


unsigned long getL(long a){
	return ((unsigned long)((unsigned long*)buffer[a])[0]);
}

int strcmp(unsigned char* A,unsigned char* B,unsigned int C){
	for(unsigned int cursor = 0 ; cursor < C ; cursor++){
		unsigned char C = A[cursor];
		unsigned char D = B[cursor];
		printf("%c - %c | ",C,D);
		if(C!=D){return 0;}
	}
	return 1;
}

unsigned char* readCDROM(char* path){
	printf("CDROM: Now reading %s \n",path);
	if(path[0]=='/'){
		readRawCDROM(dirtableloc,1,(unsigned char*) buffer);
		unsigned char lbaroot = buffer[2];
		char nmebffr[50];
		int here = 1;
		char type = 0;// 0=file 1=dire
		char deze;
		readRawCDROM(lbaroot,1,(unsigned char*) buffer);
		while(type!=1){
			// grab next token
			int y = 0;
			while(1){
				deze = path[here++];
				if(deze=='/'){
					type=0;
					break;
				}else if(deze=='\0'){
					type=1;
					break;
				}else{
					nmebffr[y++] = deze;
				}
			}
			nmebffr[y] = 0x00;
			printf("CDROM - trv : %s [ %x ]\n",nmebffr,y);
			// lookup in level
			unsigned char i = 0;
			while(1){
				if(strcmp((unsigned char*)&nmebffr,(unsigned char*)&buffer[i+33],y)==1){
					printf("_A_");
				}
				i += buffer[i];
			}
		}
	}
	exception:
	return "<<FILE NOT FOUND EXCEPTION>>";
}


//void readRawCDROM(long lba,char count,char* locationx)
void initCDROM(){
	readRawCDROM(0,1,(unsigned char*)buffer);
	printf("CDROM: Disk %s bootable!\n",(buffer[510]==0x55&&buffer[511]==0xAA)?"is":"isnot");
	unsigned long pvd = 0;
	int i = 0;
	for(i = 0 ; i < 10 ; i++){
		readRawCDROM(0x10+i,1,(unsigned char*)buffer);
		if(buffer[1]=='C'&&buffer[2]=='D'&&buffer[3]=='0'&&buffer[4]=='0'&&buffer[5]=='1'){
			if(buffer[0]==1){
				pvd = 0x10+i;
				break;
			}
		}
	}
	printf("CDROM: Primairy Volume Descriptor is at %x \n",pvd);
	readRawCDROM(pvd,1,(unsigned char*) buffer);
	unsigned short sectorsize = getS(128);
	dirtableloc = getB(140);
	printf("CDROM: Sectorsize is %s \n",(sectorsize!=ATAPI_SECTOR_SIZE)?"valid":"invalid");
	printf("CDROM: Directorytable is at %x \n",dirtableloc);
}

//
// INIT DEVICES
//
//

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

void ata_int_ready(){
        ((char*)0x1000)[0]=0x00;
}

void ata_int_wait(){
        int i = 0;
        while(((char*)0x1000)[0]!='X'){
                i++;
        }
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
                initCDROM();
        }else if(ata_device_init(dev)){
                printf(" HDD ");
                detectHDDFilesystems(dev);
        }else{
                //printf(" ??? ");
        }
}

//
// C D R O M   M A N A G E R
//
//
char read_cmd[12] = {0xA8,0,0,0,0,0,0,0,0,0,0,0};
short* readw = (short*) &read_cmd;

void readRawCDROM(unsigned long lba,unsigned char count,unsigned char* locationx){//E
	printf("CDROM: Reading LBA=%x\n",lba);
        outportb(cdromdevice.io_base+6,0xE0|(cdromdevice.slave <<4) | ((lba >> 24) & 0x0f));//cdromdevice.slave & ( 1 << 4 ));
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
        int i = 0;
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
        }
}

void detectATAPI(){
	detectATAdevice(ata_primairy_master);
        detectATAdevice(ata_primairy_slave);
        detectATAdevice(ata_secondary_master);
        detectATAdevice(ata_secondary_slave);
}
