#include <stdlib.h>
#include "transcription.h"

void initialize(char *array,int size){
	int i;
	for(i=0;i<size;i++)array[i] = '\0';
}


char* transcription(char *chain,int size){
	int i;
	char *rna = malloc(sizeof(char) * size),k;
	initialize(rna,size);
	for(i=0;i<size;i++){
		k = chain[i];
		if(k == 'A') rna[i] = 'U';
		else if(k == 'T') rna[i] = 'A';
		else if(k == 'C') rna[i] = 'G';
		else if(k == 'G') rna[i] = 'C';	
	}
	return rna;
}


