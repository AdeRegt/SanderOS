#include <system.h>

void kernel_main(){
	if(0){
		printf("\n                                                                       ");
		printf("\n                _/ _/ _/         _/ _/ _/          _/ _/ _/                 ");
		printf("\n             _/               _/         _/     _/                          ");
		printf("\n               _/                                 _/                        ");
		printf("\n                  _/        _/          _/           _/                     ");
		printf("\n                     _/                                 _/                  ");
		printf("\n                     _/    _/         _/                _/                  ");
		printf("\n                  _/        _/     _/                _/                     ");
		printf("\n           _/ _/               _/             _/ _/                         ");
		printf("\n                                                                       ");
		printf("                  S A N D E R S L A N D O  O P E R A T I N G   S Y S T E M   \n");
		printf("             An 32 bit operatingsystem created by Alexandros de Regt\n");
		printf("             Loading services ...\n");
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");
	}
	setupIDT();
	detectDevices();
	cls();
	insmod("/BOOT/TEST.O");
	insmod("/BOOT/TEST.O");
	insmod("/BOOT/TEST.O");
	malloc(10);
	initialiseKeyboard();
	//acpiEnable();
	//acpiPowerOff();
	for(;;);
}

void insmod(const char* path){
	printf("--INSMOD--\n");
	printf("  -> loading file\n");
	unsigned char* msx = readCDROM(path);
	printf("  -> parsing file\n");
	unsigned long location = elf_load_file(msx);
	printf("  -> calling file at %x \n",location);
	void (*foo)(void) = location;
	foo();
	printf("\n  -> insmod returned succesfully!\n");
}

int strlen(const char* str) {
	int len = 0;
	while (str[len])
		len++;
	return len;
}

int strcmp(char* A,char* B){
	int C = strlen(A);
	int D = strlen(B);
	if(C==D){
		for(int i = 0 ; i < D ; i++){
			if(A[i]!=B[i]){
				return 1;
			}
		}
		return 0;
	}else{
		return 1;
	}
}

void* memset(void* bufptr, int value, int size) {
	unsigned char* buf = (unsigned char*) bufptr;
	for (int i = 0; i < size; i++)
		buf[i] = (unsigned char) value;
	return bufptr;
}

void* mallocloc = (void*)0x1000;

void* malloc(unsigned long t){
	void* lx = mallocloc;
	mallocloc += t;
	return (void*)lx;
}
