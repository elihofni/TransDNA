#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Cores para saída de texto
#define COR_PADRAO "\x1b[0m"
#define COR_VERDE "\x1b[32;1m"
#define COR_VERMELHO "\x1b[31;1m"
#define COR_AMARELO "\x1b[33;1m"
#define COR_AZUL_E "\x1b[34;1m"
#define COR_AZUL "\x1b[36;1m"
#define COR_ROSA "\x1b[35;1m"

char* getCistron(char* cadeiaDNA);
char* transcription(char *chain,int size);
void initialize(char *array,int size);
char* aminoacids(char *in,const int size);
char** split(char* str, int subStringSize);
