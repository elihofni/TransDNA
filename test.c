#include "transcription.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//COMPILAÇÃO: gcc -c transcription.c
//	      gcc -o main main.c *.o
//a função de aminoacids tá programada para receber apenas uma string de 3 elementos de cada vez!
int main(){
	
	char *anal,*lana;
	const char *boom = malloc(sizeof(char) * 3);
	scanf("%s",anal);
	int t = strlen(anal);
	printf("%d \n",t);
	lana = transcription(anal,strlen(anal));
	boom = aminoacids(lana,3);
	printf("%s \n",anal);
	printf("%s \n",lana);
	printf("%s \n",boom);
	free(lana);
	return 0;

}
