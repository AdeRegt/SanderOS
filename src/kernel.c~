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
	initialiseKeyboard();
	printf("FINISHED!\n");
	printf("Installed devices:\n");
	for(int i = 0 ; i < 10 ; i++){
		printf("Device %x named %s \n",i,(char*)blockdevices[i].name);
	}
	//acpiEnable();
	//acpiPowerOff();
	for(;;);
}

