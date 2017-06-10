//SPMD Single Program Multiple Data

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <mpi.h>
#include "transcription.h"
#include "io.h"

// Constantes
#define MESTRE 0
#define TAMANHO_CODON 3
#define CODONS 10
#define CADEIA_DNA 100
#define TRANSCRICAO 200
#define AMINOACIDOS 300
#define MPI_IMPAR_ERRO 666

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

    if(qtProcessos%2 == 0 || qtProcessos == 1) {
        // Processo principal
        if (idProcesso == MESTRE) {
            // Informa ao usuário a etapa em que o programa se encontra
            printf("\nProcesso Mestre: INICIOU ");

            char *cadeiaDNAoriginal = ler("dna.txt");
            char *cadeiaDNA = getCistron(cadeiaDNAoriginal);
            int tamanhoCadeiaDNA = strlen(cadeiaDNA);
            printf("\nProcesso Mestre: LEITURA NO ARQUIVO CONCLUIDA ");

            // Particiona a cadeia original em codons (substrings de tamanho 3)
            codonsDNA = split(cadeiaDNA, TAMANHO_CODON);
            qtCodons = tamanhoCadeiaDNA / TAMANHO_CODON;
            printf(COR_AMARELO "\nProcesso Mestre: CODONS POR PROCESSO %.2f" COR_PADRAO, qtCodons/qtProcessos);
            free(cadeiaDNAoriginal);
            free(cadeiaDNA);

            // Inicia o tamanho dos vetores que aramzenarão os codonsRNA e aminoácidos
            codonsRNA = malloc(qtCodons * sizeof(char *));
            aminoacidos = malloc(qtCodons * sizeof(char *));

            // Divide o vetor entre os processos incluindo o mestre
            int idProc; // ID do processo destino
            for (i = qtCodons / qtProcessos, idProc = 1; i < qtCodons; i += (qtCodons / qtProcessos), idProc++) {

                // Envia informação básica necessária para os demais processos (Quantidade de Codons)
                MPI_Send(&qtCodons, 1, MPI_INT, idProc, CODONS, MPI_COMM_WORLD);

                char *nucleotidios = malloc((qtCodons / qtProcessos) * TAMANHO_CODON * sizeof(char));

                int w;
                for (w = i; w < (qtCodons / qtProcessos) * (idProc + 1); w++) {
                    strcat(nucleotidios, codonsDNA[w]);
                }

                // MPI_Send(indiceInicialDoArrayDeCodons, quantidadeDeCodonsEnviados, tipoDeDadoEnviado, idProcessoParaEnvio, tag, comunicadorMPI);
                int codon;
                for (codon = (qtCodons / qtProcessos) * idProc;
                     codon < ((qtCodons / qtProcessos) * (idProc + 1)); codon++) {
                    MPI_Send(nucleotidios, (qtCodons / qtProcessos) * TAMANHO_CODON, MPI_CHAR, idProc, CADEIA_DNA,
                             MPI_COMM_WORLD);
                }
                free(nucleotidios);
                printf("\nProcesso Mestre: ENVIOU CODONS DNA PARA PROCESSO #%i", idProc);
            }

            // Realiza a transcrição da sua parte(DNA -> RNA). Transcrição feita por cada codon
            for (i = 0; i < qtCodons / qtProcessos; i++) {
                codonsRNA[i] = transcription(codonsDNA[i], TAMANHO_CODON);
            }
            printf("\nProcesso Mestre: TRANSCRICAO PARCIAL CONCLUIDA ");

            // Realiza a identificação de aminoácidos da sua parte (AUU -> Met)
            for (i = 0; i < qtCodons / qtProcessos; i++) {
                aminoacidos[i] = aminoacids(codonsRNA[i], TAMANHO_CODON);
            }
            printf("\nProcesso Mestre: IDENTIFICACAO DE AMINOACIDOS PARCIAL CONCLUIDA ");

            // Recebe os codonsRNA dos demais processos
            char *todosRNA = malloc(tamanhoCadeiaDNA * sizeof(char));
            for (i = 0; i < qtCodons / qtProcessos; i++) {
                strcat(todosRNA, codonsRNA[i]);
            }
            for (i = 1; i < qtProcessos; i++) {
                char *rnaFROMslave = malloc((qtCodons / qtProcessos) * TAMANHO_CODON * sizeof(char));
                MPI_Recv(rnaFROMslave, (qtCodons / qtProcessos) * TAMANHO_CODON, MPI_CHAR, i, TRANSCRICAO,
                         MPI_COMM_WORLD, &status);
                strcat(todosRNA, rnaFROMslave);
                free(rnaFROMslave);
                printf("\nProcesso Mestre: RECEBEU RNAs DO PROCESSO #%i", i);
            }
            printf(COR_VERDE "\nProcesso Mestre: TRANSCRICAO TOTAL CONCLUIDA " COR_PADRAO);

            // Recebe os aminoacidos dos demais processos
            char *todosAminos = malloc(tamanhoCadeiaDNA * sizeof(char));
            for (i = 0; i < qtCodons / qtProcessos; i++) {
                strcat(todosAminos, aminoacidos[i]);
            }
            for (i = 1; i < qtProcessos; i++) {
                char *aminoFROMslave = malloc((qtCodons / qtProcessos) * TAMANHO_CODON * sizeof(char));
                MPI_Recv(aminoFROMslave, (qtCodons / qtProcessos) * TAMANHO_CODON, MPI_CHAR, i, AMINOACIDOS,
                         MPI_COMM_WORLD, &status);
                strcat(todosAminos, aminoFROMslave);
                free(aminoFROMslave);
                printf("\nProcesso Mestre: RECEBEU AMINOs DO PROCESSO #%i", i);
            }
            printf(COR_VERDE "\nProcesso Mestre: IDENTIFICACAO DE AMINOACIDOS TOTAL CONCLUIDA \n" COR_PADRAO);

            codonsRNA = split(todosRNA, TAMANHO_CODON);
            aminoacidos = split(todosAminos, TAMANHO_CODON);

            // Mostra os resultados para o usuário e prepara string final para escrever no arquivo de saída
            char *resultadoArquivo = malloc(tamanhoCadeiaDNA * 7 * sizeof(char));
            strcat(resultadoArquivo, ".:RESULTADOS:.\nDNA-RNA-AMINO");
            printf(COR_AZUL "\n     .:RESULTADOS:. " COR_PADRAO);
            printf(COR_AZUL "\n   DNA   RNA  AMINO" COR_PADRAO);
            for (i = 0; i < qtCodons; i++) {
                printf(COR_AZUL "\n   %s   %s   %s" COR_PADRAO, codonsDNA[i], codonsRNA[i], aminoacidos[i]);
                char *novaLinha = malloc(TAMANHO_CODON * 7 * sizeof(char));
                strcat(novaLinha, "\n");
                strcat(novaLinha, codonsDNA[i]);
                strcat(novaLinha, "-");
                strcat(novaLinha, codonsRNA[i]);
                strcat(novaLinha, "-");
                strcat(novaLinha, aminoacidos[i]);
                strcat(resultadoArquivo, novaLinha);
                free(novaLinha);
            }
            escrever(resultadoArquivo, "resultados.txt");
            free(codonsDNA);
            free(codonsRNA);
            free(aminoacidos);
            free(resultadoArquivo);
            printf(COR_VERDE "\nProcesso Mestre: ESCRITA DE RESULTADOS NO ARQUIVO CONCLUIDA " COR_PADRAO);

        } else {
            printf("\n    Processo #%i: INICIOU ", idProcesso);
            char *cadeia;

            // Recebe a quantidade de codons
            MPI_Recv(&qtCodons, 1, MPI_INT, MESTRE, CODONS, MPI_COMM_WORLD, &status);
            cadeia = malloc((qtCodons / qtProcessos) * TAMANHO_CODON * sizeof(char));
            MPI_Recv(cadeia, (qtCodons / qtProcessos) * TAMANHO_CODON, MPI_CHAR, MESTRE, CADEIA_DNA, MPI_COMM_WORLD,
                     &status);
            printf("\n    Processo #%i: RECEBEU PARTE DA CADEIA DE DNA DO MESTRE ", idProcesso);

            // Inicializa os vetores
            codonsDNA = malloc(qtCodons / qtProcessos * sizeof(char *));
            codonsRNA = malloc(qtCodons / qtProcessos * sizeof(char *));
            aminoacidos = malloc(qtCodons / qtProcessos * sizeof(char *));

            codonsDNA = split(cadeia, TAMANHO_CODON);
            free(cadeia);

            // Realiza a transcrição da sua parte(DNA -> RNA). Transcrição feita por cada codon
            for (i = 0; i < qtCodons / qtProcessos; i++) {
                codonsRNA[i] = transcription(codonsDNA[i], TAMANHO_CODON);
            }
            printf("\n    Processo #%i: TRANSCRICAO PARCIAL CONCLUIDA ", idProcesso);

            // Realiza a identificação de aminoácidos da sua parte (AUU -> Met)
            for (i = 0; i < qtCodons / qtProcessos; i++) {
                aminoacidos[i] = aminoacids(codonsRNA[i], TAMANHO_CODON);
            }
            printf("\n    Processo #%i: IDENTIFICACAO DE AMINOACIDOS PARCIAL CONCLUIDA", idProcesso);

            // Remonta uma única string para poder enviar de volta ao MESTRE
            char *rnaTOmaster = malloc((qtCodons / qtProcessos) * TAMANHO_CODON * sizeof(char));
            char *aminoTOmaster = malloc((qtCodons / qtProcessos) * TAMANHO_CODON * sizeof(char));

            for (i = 0; i < (qtCodons / qtProcessos); i++) {
                strcat(rnaTOmaster, codonsRNA[i]);
                strcat(aminoTOmaster, aminoacidos[i]);
            }

            // Envia os resultados para o MESTRE
            // printf("\n    Processo #%i: TAMANHO DO RNA %i", idProcesso, strlen(rnaTOmaster));
            // printf("\n    Processo #%i: TAMANHO DO AMINO %i", idProcesso, strlen(aminoTOmaster));
            MPI_Send(rnaTOmaster, qtCodons / qtProcessos * TAMANHO_CODON, MPI_CHAR, MESTRE, TRANSCRICAO,
                     MPI_COMM_WORLD);
            MPI_Send(aminoTOmaster, qtCodons / qtProcessos * TAMANHO_CODON, MPI_CHAR, MESTRE, AMINOACIDOS,
                     MPI_COMM_WORLD);
            free(rnaTOmaster);
            free(aminoTOmaster);
            printf("\n    Processo #%i: ENVIOU RESULTADOS PARA O PROCESSO MESTRE", idProcesso);
            printf("\n    Processo #%i: FINALIZOU", idProcesso);
        }

        // Finaliza ambiente de comunicação MPI
        tFim = MPI_Wtime(); //Pega o tempo de finalização
        MPI_Finalize();
        tExecucao = tFim - tIni;
        if (idProcesso == MESTRE) {
            printf(COR_VERDE "\nTempo total: %fs\n" COR_PADRAO, tExecucao);
        }
    }else {
        if (idProcesso == MESTRE) {
            printf(COR_VERMELHO "Quantidade de processos inválida =/\n" COR_PADRAO);
        }
        MPI_Finalize();
    }

    return 0;
}
