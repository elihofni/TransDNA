#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "io.h"
#include "transcription.h"
#define TAMANHO_CODON 3

int main(){
	char nome[200];
	printf("Insira o diretório do arquivo de entrada: \n");
	scanf("%s",nome);
	char* dnaOriginal = ler(nome);
	int tam =strlen(dnaOriginal),i,j;
	//char *rnaTraduzido = transcription(dnaOriginal,strlen(dnaOriginal));//traduz de DNA para RNA
        // Inicia o tamanho dos vetores que aramzenarão os codonsRNA e aminoácidos
	char **result = translate(dnaOriginal,tam);
	for(i = 0; i < tam; i++){
		printf("%s \n", &result[i]);
		
	}
	//printf("%s",rnaTraduzido);
	
	return 0;
}
