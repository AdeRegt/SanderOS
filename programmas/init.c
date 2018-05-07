#include <system.h>

void main(){
	again:
	cls();
	printf("\[45mBeginscherm van het SanderOS bestuuringssysteem                   SanderOS v1.0");
	printf("\[40m");
	unsigned char* data = fopen("/isofs/");
	int innerpointer = 0;
	while(1){
		unsigned char deze = data[innerpointer++];
		if(deze==';'){
			printf("\n");
		}else if(deze==0x00){
			printf("\n");
			break;
		}else{
			printf("%c",deze);
		}
	}
	printf("\[45mSelecteer het gewenste programma met ENTER en beweeg met pijltjestoetse         ");
	for(;;);
}
