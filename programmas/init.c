#include <system.h>

void main(){
	int selection = 1;
	again:
	cls();
	printf("\[44mBeginscherm van het SanderOS bestuuringssysteem                    SanderOS v1.0");
	printf("\[40m");
	unsigned char* data = fopen("/isofs/");
	int innerpointer = 0;
	int ont = 0;
	while(1){
		unsigned char deze = data[innerpointer++];
		if(deze==';'){
			printf("\n");
			ont++;
			if(ont==selection){printf("\[47m\[30m");}else{printf("\[40m\[37m");}
		}else if(deze==0x00){
			printf("\n");
			ont++;
			break;
		}else{
			printf("%c",deze);
		}
	}
	printf("\[44mSelecteer het gewenste programma met ENTER en beweeg met pijltjestoetsen        ");
	unsigned char type = getc();
	if(type==2){
		selection--;
	}else if(type==1){
		selection++;
	}
	goto again;
	for(;;);
}
