#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "io.h"
#include "transcription.h"
#define TAMANHO_CODON 3

char** sequencial(char* str, int size, char* fileName){
    char** finalArray = split(str, 3);

    int i;
	if(size%3){
		printf("Quantidade de bases nitrogenadas do DNA inválidas. O número deve ser múltiplo de 3 \n");
		exit(1);
	}
    for(i = 0; i < (size/3); i++){
        char* c = finalArray[i];
        char* a = transcription(c, 3);
        char* b = aminoacids(a, 3);
        
        char str[11];
		strcpy(str, a);
		strcat(str, " ");
		strcat(str, b);
		strcat(str, " ");
		strcat(str, c);
		strcat(str, "\n");
        
        escreverAppend(str, fileName);
    }

    return finalArray;
}

int main(){
	sequencial("AAAAACGGCGTAGCAAAAAACGGCGTAGCA", 30, "saida.txt");
	
	return 0;
}
