#include "io.h"

static int tamanho(FILE* f){
	fseek (f, 0, SEEK_END);
	int length = ftell (f);
	fseek (f, 0, SEEK_SET);

	return length;
}

char* ler(char* caminho){
	FILE* f = fopen(caminho, "r");

	char* string = calloc(tamanho(f),sizeof(char));
	
	fscanf(f,  "%s",	string);

	fclose(f);

	return string;
}

void escrever(char* str, char* caminho){
	FILE* f = fopen(caminho, "w");

	fprintf(f, "%s", str);

	fclose(f);
}

void escreverAppend(char* str, char* caminho){
	FILE* f = fopen(caminho, "a");

	fprintf(f, "%s", str);

	fclose(f);
}
