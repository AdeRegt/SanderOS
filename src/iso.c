#include <system.h>

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
unsigned char pathlist[500];
unsigned char* invalidpath = (unsigned char*)"ERROR";
unsigned char buffer[ATAPI_SECTOR_SIZE];
unsigned short dirtableloc;

unsigned char getB(int a){
	return buffer[a] & 0xff;
}

unsigned short getS(short a){
	return (unsigned short)((unsigned short*)buffer+a)[0];
}




unsigned char* readCDROM(unsigned char* path){
	//printf("CDROM: Now reading %s \n",path);
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
		//printf("CDROM: next /  '%s' \n",fsbuffer);
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
	//printf("CDROM: About to read %x\n",isoroot[ttx].lba);
	readRawCDROM(isoroot[ttx].lba,1,(unsigned char*)buffer);
	for(int i = 0 ; i < ATAPI_SECTOR_SIZE; i++){
		if(buffer[i]==';'){
			for(int z = 0 ; z < ftf ; z++){
				if(buffer[i-(ftf-z)]!=fsbuffer[z]){
					goto skipcontext;
				}
			}
			unsigned int tgy = (i-ftf-1)-30;
			unsigned int t0y = (i-ftf-1)-22;
			if(buffer[i-ftf-1]==(ftf+2)){
				unsigned short butt = ((unsigned short*)(buffer+tgy))[0];//(unsigned short)((unsigned short*)buffer[tgy])[0];
				unsigned short sutx = ((unsigned short*)(buffer+t0y))[0];
				unsigned short rdln = (sutx/ATAPI_SECTOR_SIZE)+1;
				void* loc = malloc(rdln*ATAPI_SECTOR_SIZE);
				//printf("CDROM: Found file and LBA= %x SIZE= %x SECT= %x \n",butt,sutx,rdln);
				readRawCDROM(butt,rdln,(unsigned char*)loc);
				return (unsigned char*)loc;
			}
			skipcontext:
			i++;
		}
	}
	sect_FAL:
	return invalidpath;
	sect_DIR:
	// eerst pathlist leegmaken
	for(int i = 0 ; i < 500 ; i++){
		pathlist[i] = 0x00;
	}
	// pathdata lezen
	int pointer = 0;
	for(int g = 0 ; g < isorootcnt ; g++){
		if(isoroot[g].parrent==floor){
			int ipointer = 0;
			char deze;
			while((deze=isoroot[g].name[ipointer++])!='\0'){
				pathlist[pointer++] = deze;
			}
			pathlist[pointer++] = ';';
		}
	}
	// data lezen
	//printf("CDROM: About to read %x\n",isoroot[ttx].lba);
	readRawCDROM(isoroot[ttx].lba,1,(unsigned char*)buffer);
	for(int i = 0 ; i < ATAPI_SECTOR_SIZE ; i++){
		if(buffer[i]==';'){
			unsigned int q = 1;
			unsigned char t = 2;
			while(1){
				unsigned char deze = buffer[i-q];
				if(deze==t){
					break;
				}
				q++;
				t++;
			}
			for(unsigned int r = 1 ; r < q ; r++){
				unsigned char A = buffer[(i-q)+r];
				pathlist[pointer++] = A;
			}
			pathlist[pointer++] = ';';
		}
	}
	if(pointer>0){
		pathlist[pointer-1] = 0x00;
	}
	return pathlist;
}

unsigned char* dvnme = (unsigned char*)"cdrom";
unsigned char* fsnme = (unsigned char*)"isofs";
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
//		unsigned char extattrlength = buffer[i+1];
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
	int dev = installBlockDevice((unsigned long)&readRawCDROM,NULL,ATAPI_SECTOR_SIZE,dvnme);
	installFileSystem((unsigned long)&readCDROM,NULL,dev,fsnme);
}
