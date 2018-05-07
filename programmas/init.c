#include <system.h>

void main(){
	again:
	cls();
	printf("\[45mSelecteer het gewenste programma - SanderOS         ");
	unsigned char* data = fopen("/isofs/");
	int innerpointer = 0;
	while(1){
		unsigned char deze = data[innerpointer++];
		if(deze==';'){
			printf("\n");
		}else if(deze==0x00){
			break;
		}else{
			printf("%c",deze);
		}
	}
	for(;;);
}
