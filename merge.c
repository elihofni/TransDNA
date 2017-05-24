//SPMD Single Program Multiple Data

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <mpi.h>
///swedrftgyhujiko
#define MASTER 0
#define INICIO 1
#define FIM 2

//Declarações das funções de sort
void mergeSort(int *vetor, int inicio, int fim);
void merge(int *vetor, int inicio, int meio, int fim);

void main(int argc, char** argv){
	int vetor[16] = {5, 8, 1, 14, 13, 3, 4, 7, 9, 2, 15, 6, 0, 11, 12, 10};
	int tamVetor = 16;

	double tIni, tFim, tExecucao; //Controladoras de tempo
	int i;
	int rank; //Rank/Identificador do processo
	int nPro; //numero de processos
	int source;
	int dest = MASTER;
	int tag = 200;
	MPI_Status status; 

	//Inicializa ambiente de execução MPI	
	MPI_Init(&argc, &argv);
	tIni = MPI_Wtime(); //Pega o tempo de início

	//Sistema de comunicação global
	MPI_Comm_rank(MPI_COMM_WORLD, &rank); //Identificador do processo
	MPI_Comm_size(MPI_COMM_WORLD, &nPro); //Numero total de processos

	if(rank == MASTER){
		printf("\n.:Processo Mestre - Iniciou:. \n");

		//Mostra o vetor DESordenado
		printf("\nVetor Original\n");
		for(i=0; i<tamVetor; i++){
			printf("%i, ",vetor[i]);
		}
		printf("\n");

		//Divide o vetor entre os processos
		for(i=1; i<nPro; i++){
			MPI_Send(&vetor[(tamVetor/nPro) * i], tamVetor/nPro, MPI_INT, i, INICIO, MPI_COMM_WORLD);
			printf("\nMestre enviando vetor para processo #%i\n", i);
		} 

		//Ordena a parte do master
		mergeSort(vetor, 0, (tamVetor/nPro)-1);

		//Mostra o vetor ordenado pelo master
		printf("\nParte ordenado pelo Master: ");
		for(i=0; i<(tamVetor/nPro); i++){
			printf("%i, ",vetor[i]);
		}

		//Recebe todas as partes já ordenadas
		for(i=1; i<nPro; i++){
			MPI_Recv(&vetor[(tamVetor/nPro) * i], tamVetor/nPro, MPI_INT, i, FIM, MPI_COMM_WORLD, &status);
		}

		//Junta todas as partes -- TODO desbugar esse merge final --
		//for(i=0; i<nPro/2; i++){
		//	int meio = (tamVetor/nPro) * (i+1);
		//	merge(vetor, nPro * i*2, meio-1, (tamVetor/nPro) * 2);
		//}
		int meio = 5;
		if(nPro == 4){
			merge(vetor, 0, meio-1, 8);
			meio = 12;
			merge(vetor, 8, meio-1, 16);
		}
			meio = 8;
			merge(vetor, 0, meio-1, 16);

		//Mostra o vetor ordenado
		printf("\n\nVetor Final\n");
		for(i=0; i<tamVetor; i++){
			printf("%i, ",vetor[i]);
		}
		printf("\n\n.:Processo Mestre - Finalizou:. \n");

	}else{
		printf("\n.:Processo Trabalhador #%i - Iniciou:.\n", rank);
		MPI_Recv(&vetor[(tamVetor/nPro) * rank], tamVetor/nPro, MPI_INT, MASTER, INICIO, MPI_COMM_WORLD, &status);

		//Ordena minha parte
		mergeSort(&vetor[(tamVetor/nPro) * rank], 0, (tamVetor/nPro) -1);

		//Mostra o vetor ordenado
		printf("\nParte ordenado pelo Processo #%i: ", rank);
		for(i=(tamVetor/nPro) * rank; i<((tamVetor/nPro) * (rank+1)); i++){
			printf("%i, ",vetor[i]);
		}

		//Envia a parte ordenada para o master
		MPI_Send(&vetor[(tamVetor/nPro) * rank], tamVetor/nPro, MPI_INT, MASTER, FIM, MPI_COMM_WORLD);

		printf("\n\n.:Processo Trabalhador #%i - Finalizou:. \n", rank);
	}

	//Finaliza ambiente de comunicação MPI
	tFim = MPI_Wtime(); //Pega o tempo de finalização
	MPI_Finalize();
	tExecucao = tFim - tIni;
	if(rank == MASTER){
		printf("Tempo total: %fs\n", tExecucao);
	}
	
}


void mergeSort(int *vetor, int inicio, int fim){
	int meio;
	if(inicio<fim){
		meio = (inicio+fim)/2;
		mergeSort(vetor, inicio, meio);
		mergeSort(vetor, meio+1, fim);
		merge(vetor, inicio, meio, fim);
	}
}

void merge(int *vetor, int inicio, int meio, int fim){
	int *temp, p1, p2, tamanho, i, j, k;
	int fim1 = 0, fim2 = 0;
	tamanho = fim-inicio+1;
	p1 = inicio;
	p2 = meio+1;
	temp = (int *)malloc(tamanho * sizeof(int));
	if(temp != NULL){
		for(i=0; i<tamanho; i++){
			if(!fim1 && !fim2){
				if(vetor[p1] < vetor[p2]){ //Combina ordenado
					temp[i] = vetor[p1++];
				}else{
					temp[i] = vetor[p2++];
				}
				if(p1>meio){ //Verifica se o vetor acabou
					fim1=1;
				}
				if(p2>fim){ //Verifica se o vetor acabou
					fim2=1;
				}
			}else{ //Copia o que sobrar
				if(!fim1){
					temp[i] = vetor[p1++];
				}else{
					temp[i] = vetor[p2++];
				}
			}
		}
		for(j=0, k=inicio; j<tamanho; j++, k++){ //Copiar do auxiliar para o principal
			vetor[k] = temp[j];
		}
	}
	free(temp);
}
