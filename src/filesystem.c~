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

unsigned char* FAILSTRING = "FAILED";

unsigned char* fopen(unsigned char* path){
	int i = 0;
	for(i = 0 ; i < filesystemscount ; i++){
		for(int g = 0 ; g < 5 ; g++){
			if(path[1+g]!=filesystems[i].name[g]){
				goto endtx;
			}
			goto cltx;
			endtx:
			i++;
			i--;
		}
	}
	return FAILSTRING;
	cltx:
	printf("X");
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
