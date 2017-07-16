/* SPMD Single Program Multiple Data
 * TransDNA sequencial
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
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
	clock_t start, end;
	double cpu_time_used;
	start = clock();
	char *chain;
	chain = ler("dna7.txt"); // Alterar para ler o arquivo DNA desejado
	chain = getCistron(chain);
	int length = strlen(chain);
	printf("\nTAMANHO DO CISTRON %d \n\n",length);
	sequencial(chain,length,"resultados-seq.txt");
	end = clock();
	cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	printf(COR_VERDE "Tempo Total: %.6fs \n" COR_PADRAO, cpu_time_used);

	return 0;
}
