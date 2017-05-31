#include "transcription.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//COMPILAÇÃO: gcc -c transcription.c
//	      gcc -o main main.c *.o
int main(){
	
	char *anal,*lana;
	scanf("%s",anal);
	int t = strlen(anal);
	printf("%d",t);
	lana = transcription(anal,strlen(anal));
	printf("%s \n",anal);
	printf("%s \n",lana);
	return 0;

}
