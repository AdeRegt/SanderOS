#include <system.h>

BlockDevice blockdevices[10];
int blockdevicescount = 0;

FileSystem filesystems[10];
int filesystemscount = 0;

int installBlockDevice(unsigned long read,unsigned long write,unsigned long size,unsigned long name){
	blockdevices[blockdevicescount].read 	= read;
	blockdevices[blockdevicescount].write 	= write;
	blockdevices[blockdevicescount].size 	= size;
	blockdevices[blockdevicescount].name 	= name;
	blockdevicescount++;
	return blockdevicescount - 1;
}

int installFileSystem(unsigned long read,unsigned long write,unsigned int device,unsigned long name){
	filesystems[filesystemscount].read 	= read;
	filesystems[filesystemscount].write 	= write;
	filesystems[filesystemscount].device 	= device;
	filesystems[filesystemscount].name 	= name;
	filesystemscount++;
	return filesystemscount - 1;
}

void devdump(){
	
	printf("Installed devices:\n");
	for(int i = 0 ; i < 10 ; i++){
		printf("Device %x named %s \n",i,(char*)blockdevices[i].name);
	}
}
