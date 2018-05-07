#include <system.h>

unsigned char* filepath = "/isofs/\0\0\0                                                        ";
unsigned char* data     = "                                                                        ";
void main(){
	int selection = 1;
	again:
	cls();
	printf("\[44m\[37mBeginscherm van het SanderOS bestuuringssysteem                    SanderOS v1.0");
	printf("\[4em\[37m%s \[40m\[37m\n",filepath);
	data = (unsigned char*)fopen(filepath);
	int innerpointer = 0;
	int ont = 0;
	if(ont==selection){printf("\[47m\[30m");}else{printf("\[40m\[37m");}
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
	while(ont<22){
		ont++;
		printf("\n");
	}
	printf("\[44m\[37mSelecteer het gewenste programma met ENTER en beweeg met pijltjestoetsen        \[40m\[37m");
	unsigned char type = getc();
	if(type==2){
		selection--;
	}else if(type==1){
		selection++;
	}else if(type=='\n'){
		
		for(int i = 0 ; i < 50 ; i++){
			unsigned char e = filepath[i];
			if(e==0x00){
				int pntA = 0;
				int pntB = 0;
				int pntC = 0;
				filepath[i+(pntC++)] = '/';
				while(1){
					unsigned char deze = data[pntA++];
					if(deze==';'){
						pntB++;
					}else if(pntB==selection){
						filepath[i+(pntC++)] = deze;
					}else if(pntB==(selection+1)||deze==0x00){
						break;
					}
				}
				filepath[i+(pntC++)] = 0x00;
				break;
			}
		}
		
		selection = 0;
	}else if(type=='\t'){
		int cnt = 1;
		for(int i = 0 ; i < 50 ; i++){
			unsigned char e = filepath[50-i];
			if(e=='/'){
				filepath[50-i] = 0x00;
				break;
			}
		}
		selection = 1;
	}
	goto again;
	for(;;);
}
