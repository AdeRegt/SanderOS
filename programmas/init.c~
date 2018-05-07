#include <system.h>

void main(){
	again:
	cls();
	printf("\[45mBeginscherm van het SanderOS bestuuringssysteem                    SanderOS v1.0");
	printf("\[40m");
	unsigned char* data = fopen("/isofs/");
	int innerpointer = 0;
	int selection = 1;
	int ont = 0;
	while(1){
		unsigned char deze = data[innerpointer++];
		if(deze==';'){
			printf("\n");
			ont++;
			if(ont==selection){printf("\[42m");}else{printf("\[40m");}
		}else if(deze==0x00){
			printf("\n");
			ont++;
			break;
		}else{
			printf("%c",deze);
		}
	}
	printf("\[45mSelecteer het gewenste programma met ENTER en beweeg met pijltjestoetse         ");
	for(;;);
}
