//SPMD Single Program Multiple Data

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <mpi.h>
#include "transcription.h"

// Constantes
#define MESTRE 0
#define TAMANHO_CODON 3
#define CODONS 10
#define CADEIA_DNA 100
#define TRANSCRICAO 200
#define AMINOACIDOS 300

int main(int argc, char** argv) {

    // Variáveis comuns a todos os processos (cada processo possui uma instância para cada variável)
    double tIni, tFim, tExecucao; // Controladoras de tempo
    int i;                  // Iterador padrão
    int idProcesso;         // Identificador de cada processo
    int qtProcessos;        // Numero total de processos
    int qtCodons;           // Quantidade de Codons que a cadeia de DNA original possui
    char** codonsDNA;       // Vetor de Codons do DNA
    char** codonsRNA;       // Vetor de Codons do RNA
    char** aminoacidos;     // Vetor de Aminoácidos

    MPI_Status status;      // Variável de controle para MPI

    // Inicializa ambiente de execução MPI
    MPI_Init(&argc, &argv);
    tIni = MPI_Wtime(); // Pega o tempo de início

    // Sistema de comunicação global
    MPI_Comm_rank(MPI_COMM_WORLD, &idProcesso); // Identificador do processo
    MPI_Comm_size(MPI_COMM_WORLD, &qtProcessos); // Numero total de processos

    // Processo principal
    if(idProcesso == MESTRE) {
        // Informa ao usuário a etapa em que o programa se encontra
        printf("\nProcesso Mestre: INICIOU ");

        // printf("\nProcesso Mestre: LENDO CADEIA DE DNA NO ARQUIVO \n");
        // TODO Ler cadeia de DNA à partir do arquivo
        // TODO ler cadeia original até encontrar o ponto inicial para a transcrição
        printf("\nProcesso Mestre: LEITURA NO ARQUIVO CONCLUIDA ");

        // Início da cadeia de DNA. A partir desse ponto pode começar a transcrição
        char* cadeiaDNAoriginal = "AAAAACGGCGTAGCAAAAAACGGCGTAGCAAAAAACGGCGTAGCAAAAAACGGCGTAGCA";
        int tamanhoCadeiaDNA = strlen(cadeiaDNAoriginal);

        // Particiona a cadeia original em codons (substrings de tamanho 3)
        codonsDNA = split(cadeiaDNAoriginal);
        qtCodons = tamanhoCadeiaDNA/TAMANHO_CODON;

        // Inicia o tamanho dos vetores que aramzenarão os codonsRNA e aminoácidos
        codonsRNA = malloc(qtCodons * sizeof(char*));
        aminoacidos = malloc(qtCodons * sizeof(char*));

        // Divide o vetor entre os processos incluindo o mestre
        int idProc; // ID do processo destino
        for(i=qtCodons/qtProcessos, idProc = 1; i<qtCodons; i+=(qtCodons/qtProcessos), idProc++) {

            // Envia informação básica necessária para os demais processos (Quantidade de Codons)
            MPI_Send(&qtCodons, 1, MPI_INT, idProc, CODONS, MPI_COMM_WORLD);

            // MPI_Send(indiceInicialDoArrayDeCodons, quantidadeDeCodonsEnviados, tipoDeDadoEnviado, idProcessoParaEnvio, tag, comunicadorMPI);
            int codon;
            for(codon=(qtCodons/qtProcessos) * idProc; codon<((qtCodons/qtProcessos) * (idProc + 1)); codon++) {
                MPI_Send(&codonsDNA[((qtCodons/qtProcessos) * idProc) + codon], TAMANHO_CODON, MPI_CHAR, idProc, CADEIA_DNA, MPI_COMM_WORLD);
                // printf("\nCODON %i = %s", codon, codonsDNA[codon]);
            }
            printf("\nProcesso Mestre: ENVIOU CODONS DNA PARA PROCESSO #%i", idProc);
        }

        // Realiza a transcrição da sua parte(DNA -> RNA). Transcrição feita por cada codon
        for(i=0; i<qtCodons/qtProcessos; i++) {
            codonsRNA[i] = transcription(codonsDNA[i], TAMANHO_CODON);
        }
        printf("\nProcesso Mestre: TRANSCRICAO PARCIAL CONCLUIDA ");

        // Realiza a identificação de aminoácidos da sua parte (AUU -> Met)
        for(i=0; i<qtCodons/qtProcessos; i++) {
            aminoacidos[i] = aminoacids(codonsRNA[i], TAMANHO_CODON);
        }
        printf("\nProcesso Mestre: IDENTIFICACAO DE AMINOACIDOS PARCIAL CONCLUIDA ");

        // Recebe os codonsRNA dos demais processos
        for(i=1; i<qtProcessos; i++){
            MPI_Recv(&codonsRNA[(qtCodons/qtProcessos) * i], qtCodons/qtProcessos, MPI_CHAR, i, TRANSCRICAO, MPI_COMM_WORLD, &status);
        }
        printf("\nProcesso Mestre: TRANSCRICAO TOTAL CONCLUIDA ");

        // Recebe os aminoacidos dos demais processos
        for(i=1; i<qtProcessos; i++){
            MPI_Recv(&aminoacidos[(qtCodons/qtProcessos) * i], qtCodons/qtProcessos, MPI_CHAR, i, AMINOACIDOS, MPI_COMM_WORLD, &status);
        }
        printf("\nProcesso Mestre: IDENTIFICACAO DE AMINOACIDOS TOTAL CONCLUIDA \n");

        // Mostra os resultados para o usuário
        printf("\n     .:RESULTADOS:. ");
        printf("\n   DNA    RNA   AMINO");
        for(i=0; i<qtCodons; i++) {
            printf("\n   %s    %s   %s", codonsDNA[i], codonsRNA, aminoacidos[i]);
        }

        // TODO Escrever a cadeia original no arquivo de saida
        // TODO Escrever os codonsDNA no arquivo de saida
        // TODO Escrever os codonsRNA no arquivo de saida
        // TODO Escrever os nomes dos aminoacidos no arquivo de saida

    } else {
        printf("\n    Processo #%i: INICIOU ", idProcesso);

        // Recebe a quantidade de codons
        MPI_Recv(&qtCodons, 1, MPI_INT, MESTRE, CODONS, MPI_COMM_WORLD, &status);

        printf("\n    Processo #%i: QTCODONS = %i", idProcesso, qtCodons);
        printf("\n    Processo #%i: INICIO = %i", idProcesso, (qtCodons/qtProcessos) * idProcesso);

        // Inicializa os vetores
        codonsDNA = malloc(qtCodons/qtProcessos * sizeof(char*));
        codonsRNA = malloc(qtCodons/qtProcessos * sizeof(char*));
        aminoacidos = malloc(qtCodons/qtProcessos * sizeof(char*));

        printf("\n    Processo #%i: TESTE", idProcesso);

        // Recebe uma parte dos codons do DNA original para trabalhar
        for(i=0; i<qtCodons/qtProcessos; i++) {
            MPI_Recv(&codonsDNA[i], TAMANHO_CODON*4, MPI_CHAR, MESTRE, CADEIA_DNA, MPI_COMM_WORLD, &status);
            printf("\nCODON %i = %s", i, codonsDNA[i]);
        }

        // Realiza a transcrição da sua parte(DNA -> RNA). Transcrição feita por cada codon
        for(i=0; i<qtCodons/qtProcessos; i++) {
            codonsRNA[i] = transcription(codonsDNA[i], TAMANHO_CODON);
            printf("\n    Processo #%i: codonsDNA[%i] = %s", idProcesso, i, codonsDNA[i]);
        }
        printf("\n    Processo #%i: TRANSCRICAO PARCIAL CONCLUIDA ", idProcesso);

        // Realiza a identificação de aminoácidos da sua parte (AUU -> Met)
        for(i=0; i<qtCodons/qtProcessos; i++) {
            aminoacidos[i] = aminoacids(codonsRNA[i], TAMANHO_CODON);
        }
        printf("\n    Processo #%i: IDENTIFICACAO DE AMINOACIDOS PARCIAL CONCLUIDA", idProcesso);

        // Envia os resultados para o MESTRE
        MPI_Send(&codonsRNA[0], qtCodons/qtProcessos, MPI_CHAR, MESTRE, TRANSCRICAO, MPI_COMM_WORLD);
        MPI_Send(&aminoacidos[0], qtCodons/qtProcessos, MPI_CHAR, MESTRE, AMINOACIDOS, MPI_COMM_WORLD);
        printf("\n    Processo #%i: ENVIOU RESULTADOS PARA O PROCESSO MESTRE", idProcesso);
    }

    // Finaliza ambiente de comunicação MPI
    tFim = MPI_Wtime(); //Pega o tempo de finalização
    MPI_Finalize();
    tExecucao = tFim - tIni;
    if(idProcesso == MESTRE){
        printf("\nTempo total: %fs\n", tExecucao);
    }

    return 0;
}
