#include <system.h>
#define ATAPI_SECTOR_SIZE 2048

//
// INIT ISO
//
//

typedef struct {
	unsigned short parrent;
	unsigned long lba;
	unsigned char name[10];
}ISOROOTTABLES;

 ISOROOTTABLES isoroot[100];
int isorootcnt = 0;

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


unsigned char* readCDROM(char* path){
	printf("CDROM: Now reading %s \n",path);
	// bestandspad opspliten
	unsigned int cunt = 1;
	unsigned char fsbuffer[10];
	unsigned short floor = 1;
	unsigned int stp = 1;
	unsigned int ttx = 0;
	unsigned char ftf = 0;
	while(1){
		int filler = 0;
		int end = 0;
		nextattempt:
		filler = 0;
		end = 0;
		ftf = 0;
		while(1){
			unsigned char deze = path[cunt];
			if(deze=='/'){
				cunt = cunt+1;
				goto ddf;
			}else if(deze==0x00){
				stp = 0;
				end = 1;
				goto ddf;
			}else{
				fsbuffer[filler++] = deze;
				ftf++;
			}
			cunt = cunt+1;
		}
		ddf:
		fsbuffer[filler] = 0x00;
		printf("> '%s' \n",fsbuffer);
		for(int g = 0 ; g < isorootcnt ; g++){
			if(isoroot[g].parrent==floor){
				int gevonden = 1;
//				printf(" | %s |",isoroot[g].name);
				for(int t = 0 ; t < filler ; t++){
					if(isoroot[g].name[t]!=fsbuffer[t]){
						gevonden = 0;
					}
				}
				if(gevonden){
					ttx = g;
					floor = g+1;
					if(end){goto sect_DIR;}
					goto nextattempt;
				}
			}
		}
		if(end==0){
			goto sect_FAL;
		}
		if(stp==0){
			break;
		}
	}
	printf("About to read %x\n",isoroot[ttx].lba);
	readRawCDROM(isoroot[ttx].lba,1,(unsigned char*)buffer);
	for(int i = 0 ; i < ATAPI_SECTOR_SIZE; i++){
		if(buffer[i]==';'){
			for(int z = 0 ; z < ftf ; z++){
				if(buffer[i-(ftf-z)]!=fsbuffer[z]){
					goto skipcontext;
				}
			}
			unsigned char tgy = i-ftf-1;
			if(buffer[i-ftf-1]==(ftf+2)){
				unsigned char txt = buffer[tgy-30];
				printf("YAY at %x \n",txt);
			}
			skipcontext:
			i++;
		}
	}
	sect_FAL:
	return "RETURN FAIL";
	sect_FIL:
	return "RETURN FIL";
	sect_DIR:
	return "RETURN DIR";
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
	readRawCDROM(dirtableloc,1,(unsigned char*)buffer);
	isorootcnt = 0;
	i = 0;
	while(1){
		unsigned char lengthofdirident = buffer[i];
		unsigned char extattrlength = buffer[i+1];
		isoroot[isorootcnt].lba 	= getB(i+2);
		isoroot[isorootcnt].parrent 	= getB(i+6);
		if(lengthofdirident==0){break;}
		for(int r = 0 ; r < lengthofdirident ; r++){
			isoroot[isorootcnt].name[r] = buffer[i+8+r];//printf("%c",buffer[i+8+r]);
		}//printf("'\n");
		printf("CDROM: detected %xth folder '%s' and parentdir is %x \n",isorootcnt,isoroot[isorootcnt].name,isoroot[isorootcnt].parrent);
		isorootcnt++;
		if(lengthofdirident % 2 ){
			i = i + 1;
		}
		i = i + 8 + lengthofdirident;
	}
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
//	printf("CDROM: Reading LBA=%x\n",lba);
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
