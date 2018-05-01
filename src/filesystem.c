#include <system.h>

BlockDevice blockdevices[10];
int blockdevicecount = 0;

FileSystem filesystem[10];
int filesystemcount = 0;

int installBlockDevice(unsigned long read,unsigned long write,unsigned long size,unsigned long name){
	blockdevices[blockdevicescount].read = read;
	blockdevices[blockdevicescount].write = write;
	blockdevices[blockdevicescount].size = size;
	blockdevices[blockdevicescount].name = name;
	blockdevicecount++;
	return blockdevicecount - 1;
}

int installFileSystem(unsigned long read,unsigned long write,unsigned int device,unsigned long name){
	filesystemcount++;
	return filesystemcount - 1;
}
