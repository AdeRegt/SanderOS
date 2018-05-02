#include <system.h>


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

void* memcpy(void* restrict dstptr, const void* restrict srcptr, int size) {
	unsigned char* dst = (unsigned char*) dstptr;
	const unsigned char* src = (const unsigned char*) srcptr;
	for (int i = 0; i < size; i++)
		dst[i] = src[i];
	return dstptr;
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
