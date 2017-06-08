#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "io.h"
#include "transcription.h"
#define TAMANHO_CODON 3

char** sequencial(char* str, int size, char* fileName){
    char** finalArray = split(str, 3);

    int i;
    for(i = 0; i < (size/3); i++){
        char* c = finalArray[i];
        char* a = transcription(c, 3);
        char* b = aminoacids(a, 3);
        
        printf("%s %s %s\n", a, b, c);
        
        
        /*char str[11];
		strcpy(str, a);
		strcat(str, " ");
		strcat(str, b);
		strcat(str, " ");
		strcat(str, c);
		strcat(str, "\n");
        
        escrever(str, fileName);*/
    }

    return finalArray;
}

int main(){
	sequencial("AAAAACGGCGTAGCA", 15, "teste.txt");
	
	return 0;
}
