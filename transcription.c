#include <stdlib.h>
#include "transcription.h"

void initialize(char *array,int size){
	int i;
	for(i=0;i<size;i++)array[i] = '\0';
}

/**
* BIO: TRANSRIÇÂO [DNA -> RNA]
* Traduz uma sequencia em DNA para RNA.
* str - String recebida pela thread..
* Exemplo: AACCGCTCA -> AACCGCUCA
*/
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
/**
* AMINOACIDOS
* funcao que recebe uma string de tamanho 3
* e traduz para um nome.
* Exemplo: AAC CGC UCA -> Nome1 Nome2, Nome3
*/
const char* aminoacids(char *in,const int size){
	//não gosto de switch e case, dá no mesmo:
	if(in[0] == 'U' && in[1] == 'U'){
		if(in[2] == 'U' || in[2] == 'C') return "Phe";
		else return  "Leu";
	}
	else if(in[0] == 'C' && in[1] == 'U') return  "Leu";
	else if(in[0] == 'A' && in[1] == 'U'){
		if(in[2] == 'U' || in[2] == 'C') return  "Leu";
		else return  "Met";
	}
	else if(in[0] == 'G' && in[1] == 'U') return  "Val";
	//fim da primeira coluna
	else if(in[0] == 'U' && in[1] == 'C') return  "Ser";
	else if(in[0] == 'C' && in[1] == 'C') return  "Pro";
	else if(in[0] == 'A' && in[1] == 'C') return  "Thr";
	else if(in[0] == 'G' && in[1] == 'C') return  "Ala";
	//fim da segunda coluna
	else if(in[0] == 'U' && in[1] == 'A'){
		if(in[2] == 'U' || in[2] == 'C') return  "Tyr";
		//S\N: toda vez que isso aparecer significa que na tabela estava um nome sem sentido.
		else return  "S/N";
	}
	else if(in[0] == 'C' && in[1] == 'A'){
		if(in[2] == 'U' || in[2] == 'C') return  "His";
		else return  "GluN";
	}
	else if(in[0] == 'A' && in[1] == 'A'){
		if(in[2] == 'U' || in[2] == 'C') return  "AspN";
		else return  "Lys";
	}
	else if(in[0] == 'G' && in[1] == 'A'){
		if(in[2] == 'U' || in[2] == 'C') return  "Asp";
		else return  "Glu";
	}
	//fim da terceira coluna
	else if(in[0] == 'U' && in[1] == 'G'){
		if(in[2] == 'U' || in[2] == 'C') return  "Cys";
		if(in[2] == 'G') return "Tryp";
		else return "S/N";
	}
	else if(in[0] == 'C' && in[1] == 'G') return "Arg";
	else if(in[0] == 'A' && in[1] == 'G'){
		if(in[2] == 'U' || in[2] == 'C') return "Ser";
		else return "Arg";
	}
	else if(in[0] == 'G' && in[1] == 'G') return "Gly";
	//se não entrou em nenhum caso válido, o nome de retorno é sem sentido:
	return "S/N";
}


