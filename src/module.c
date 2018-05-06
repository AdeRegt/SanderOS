#include <system.h>


void insmod(const char* path,void* arguments){
	printf("--INSMOD--\n");
	printf("  -> loading file\n");
	unsigned char* msx = readCDROM(path);
	if(strcmp(msx,(unsigned char*)"ERROR")==0){
		printf(" -> Unable to locate kernelmodule\n");
	}else{
		printf("  -> parsing file\n");
		void* location = elf_load_file(msx);
		if(location==NULL){
			printf("\n  -> insmod returned error!\n");
		}else{
			printf("  -> calling file at %x \n",location);
			void (*foo)(void*) = (void*)location;
			foo(arguments);
			printf("\n  -> insmod returned succesfully!\n");
		}
	}
}
