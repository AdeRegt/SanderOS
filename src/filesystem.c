#include <system.h>

BlockDevice blockdevices[10];
int blockdevicescount = 0;

FileSystem filesystems[10];
int filesystemscount = 0;

int installBlockDevice(unsigned long read,unsigned long write,unsigned long size,unsigned char name[5]){
	blockdevices[blockdevicescount].read 	= read;
	blockdevices[blockdevicescount].write 	= write;
	blockdevices[blockdevicescount].size 	= size;
	for(int i = 0 ; i < 5 ; i++){
		blockdevices[blockdevicescount].name[i] 	= name[i];
	}
	blockdevicescount++;
	return blockdevicescount - 1;
}

int installFileSystem(unsigned long read,unsigned long write,unsigned int device,unsigned char name[5]){
	filesystems[filesystemscount].read 	= read;
	filesystems[filesystemscount].write 	= write;
	filesystems[filesystemscount].device 	= device;
	for(int i = 0 ; i < 5 ; i++){
		filesystems[filesystemscount].name[i] 	= name[i];
	}
	filesystemscount++;
	return filesystemscount - 1;
}

unsigned char* FAILSTRING = (unsigned char*)"FAILED";

unsigned char* fopen(unsigned char* path){
	int i = 0;
	unsigned char ddx = 1;
	for(i = 0 ; i < filesystemscount ; i++){
		int targz = 1;
		for(int g = 0 ; g < 5 ; g++){
			unsigned char A = path[1+g];
			unsigned char B = filesystems[i].name[g];
			//printf("%c - %c \n",A,B);
			if(A!=B){
				targz = 0;
			}
		}
		if(targz==1){
			ddx = 0;
			break;
		}
	}
	if(ddx==1){
		return FAILSTRING;
	}else{
		void* (*foo)(void*) = (void*)filesystems[i].read;
		return foo((unsigned char*)&path[6]);
	}
}

void devdump(){
	
	printf("Installed devices:\n");
	for(int i = 0 ; i < 10 ; i++){
		if(blockdevices[i].name[0]!=0x00){
			printf("Device %x named %s \n",i,(unsigned char*)blockdevices[i].name);
		}
	}
	printf("\nInstalled filesystems:\n");
	for(int i = 0 ; i < 10 ; i++){
		if(filesystems[i].name[0]!=0x00){
		printf("Device %x named %s \n",i,(unsigned char*)filesystems[i].name);
		}
	}
}
