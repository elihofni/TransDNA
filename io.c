#include <stdlib.h>
#include <stdio.h>
#include "io.h"

int tamanho(FILE* f){
	fseek (f, 0, SEEK_END);
	int length = ftell (f);
	fseek (f, 0, SEEK_SET);

	return length;
}

char* ler(char* caminho){
	FILE* f = fopen(caminho, "r");

	char* string = malloc(sizeof(char) * tamanho(f));
	
	fscanf(f,  "%s",	string);

	fclose(f);

	return string;
}

void escrever(char* str, char* caminho){
	FILE* f = fopen(caminho, "w");

	fprintf(f, "%s", str);

	fclose(f);
}
