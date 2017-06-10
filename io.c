#include "io.h"

static int tamanho(FILE* f){
	fseek (f, 0, SEEK_END);
	int length = ftell (f);
	fseek (f, 0, SEEK_SET);

	return length;
}

char* ler(char* caminho){
	FILE* f = fopen(caminho, "r");
	if(!f)exit(1);

	char* string = calloc(tamanho(f),sizeof(char));
	
	fscanf(f,  "%s",	string);

	fclose(f);

	return string;
}

void escrever(char* str, char* caminho){
	FILE* f = fopen(caminho, "w");
	if(!f)exit(1);
	fprintf(f, "%s", str);

	fclose(f);
}

void escreverAppend(char* str, char* caminho){
	FILE* f = fopen(caminho, "a");
	if(!f)exit(1);
	fprintf(f, "%s", str);

	fclose(f);
}
