/* SPMD Single Program Multiple Data
 * TransDNA com MPI
*/

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
#define CODONS_POR_PROCESSO 20
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
    int codonsPorProcesso;  // Quantidade de codons para cada processo trabalhador
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
    if (idProcesso == MESTRE) {
        printf("\nProcesso Mestre: INICIOU ");

        // Le a cadeia do arquivo de entrada e encontra o ponto inicial para transcrição
        char *cadeiaDNAoriginal = ler("dna4.txt"); // TODO Alterar para ler o arquivo DNA desejado
        printf("\nProcesso Mestre: LEITURA NO ARQUIVO CONCLUIDA ");
        char *cadeiaDNA = getCistron(cadeiaDNAoriginal);
        int tamanhoCadeiaDNA = strlen(cadeiaDNA);

        // Particiona a cadeia original em codons (substrings de tamanho 3)
        codonsDNA = split(cadeiaDNA, TAMANHO_CODON);
        qtCodons = tamanhoCadeiaDNA / TAMANHO_CODON;
        printf("\nProcesso Mestre: TAMANHO DA CADEIA LIDA = %lu", strlen(cadeiaDNAoriginal));
        printf("\nProcesso Mestre: TAMANHO DO CISTRON = %i", tamanhoCadeiaDNA);
        printf("\nProcesso Mestre: TOTAL DE CODONS = %i", qtCodons);
        free(cadeiaDNAoriginal);
        free(cadeiaDNA);

        // Se a quantidade de codons por processo não for exata, a mestre deve ficar com a parte extra
        codonsPorProcesso = qtCodons/qtProcessos;
        int codonsPorMestre;
        if(qtCodons % qtProcessos != 0) {
            codonsPorMestre = qtCodons - (codonsPorProcesso * qtProcessos) + codonsPorProcesso;
            printf(COR_AMARELO "\nProcesso Mestre: %i CODONS PRO MESTRE" COR_PADRAO, codonsPorMestre);
            printf(COR_AMARELO "\nProcesso Mestre: %i CODONS POR PROCESSO" COR_PADRAO, codonsPorProcesso);
        } else {
            codonsPorMestre = codonsPorProcesso;
            printf("\nProcesso Mestre: %i CODONS POR PROCESSO", qtCodons/qtProcessos);
        }

        // Inicia o tamanho dos vetores que aramzenarão os codonsRNA e aminoácidos
        codonsRNA = malloc(qtCodons * sizeof(char *));
        aminoacidos = malloc(qtCodons * sizeof(char *));

        // Divide o vetor entre os processos incluindo o mestre
        int idProc; // ID do processo destino
        for (i = codonsPorMestre, idProc = 1; i < qtCodons; i += codonsPorProcesso, idProc++) {

            // Envia informações básicas necessária para os demais processos (Quantidade de Codons total e por processo)
            MPI_Send(&qtCodons, 1, MPI_INT, idProc, CODONS, MPI_COMM_WORLD);
            MPI_Send(&codonsPorProcesso, 1, MPI_INT, idProc, CODONS_POR_PROCESSO, MPI_COMM_WORLD);

            char *nucleotidios = malloc(codonsPorProcesso * TAMANHO_CODON * sizeof(char));
            initialize(nucleotidios, codonsPorProcesso * TAMANHO_CODON);

            int cont;
            int parada;
            for (cont = i, parada = 0; parada < codonsPorProcesso; cont++, parada++) {
                strcat(nucleotidios, codonsDNA[cont]);
            }

            // MPI_Send(indiceInicialDoArrayDeCodons, quantidadeDeCodonsEnviados, tipoDeDadoEnviado, idProcessoParaEnvio, tag, comunicadorMPI);
            int codon;
            for (codon = codonsPorProcesso * idProc; codon < (codonsPorProcesso * (idProc + 1)); codon++) {
                MPI_Send(nucleotidios, codonsPorProcesso * TAMANHO_CODON, MPI_CHAR, idProc, CADEIA_DNA,
                         MPI_COMM_WORLD);
            }
            free(nucleotidios);
            printf("\nProcesso Mestre: ENVIOU CODONS DNA PARA PROCESSO #%i", idProc);
        }

        // Realiza a transcrição da sua parte(DNA -> RNA). Transcrição feita por cada codon
        for (i = 0; i < codonsPorMestre; i++) {
            codonsRNA[i] = transcription(codonsDNA[i], TAMANHO_CODON);
        }
        printf("\nProcesso Mestre: TRANSCRICAO PARCIAL CONCLUIDA ");

        // Realiza a identificação de aminoácidos da sua parte (AUU -> Met)
        for (i = 0; i < codonsPorMestre; i++) {
            aminoacidos[i] = aminoacids(codonsRNA[i], TAMANHO_CODON);
        }
        printf("\nProcesso Mestre: IDENTIFICACAO DE AMINOACIDOS PARCIAL CONCLUIDA ");

        // Coloca a parte do mestre no resultado final de rna
        char *todosRNA = malloc(tamanhoCadeiaDNA * sizeof(char));
        initialize(todosRNA, tamanhoCadeiaDNA);
        for (i = 0; i < codonsPorMestre; i++) {
            strcat(todosRNA, codonsRNA[i]);
        }

        // Recebe os codonsRNA dos demais processos
        for (i = 1; i < qtProcessos; i++) {
            char *rnaFROMslave = malloc(codonsPorProcesso * TAMANHO_CODON * sizeof(char));
            initialize(rnaFROMslave, codonsPorProcesso * TAMANHO_CODON);
            MPI_Recv(rnaFROMslave, codonsPorProcesso * TAMANHO_CODON, MPI_CHAR, i, TRANSCRICAO,
                     MPI_COMM_WORLD, &status);
            strcat(todosRNA, rnaFROMslave);
            free(rnaFROMslave);
            printf("\nProcesso Mestre: RECEBEU RNAs DO PROCESSO #%i", i);
        }
        printf(COR_VERDE "\nProcesso Mestre: TRANSCRICAO TOTAL CONCLUIDA " COR_PADRAO);

        // Coloca a parte do mestre no resultado final de aminoacidos
        char *todosAminos = malloc(tamanhoCadeiaDNA * sizeof(char));
        initialize(todosAminos, tamanhoCadeiaDNA);
        for (i = 0; i < codonsPorMestre; i++) {
            strcat(todosAminos, aminoacidos[i]);
        }
        // Recebe os aminoacidos dos demais processos
        for (i = 1; i < qtProcessos; i++) {
            char *aminoFROMslave = malloc(codonsPorProcesso * TAMANHO_CODON * sizeof(char));
            initialize(aminoFROMslave, codonsPorProcesso * TAMANHO_CODON);
            MPI_Recv(aminoFROMslave, codonsPorProcesso * TAMANHO_CODON, MPI_CHAR, i, AMINOACIDOS,
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
        strcat(resultadoArquivo, ".:RESULTADOS:.\nDNA   RNA  AMINO");
        printf(COR_AZUL "\n     .:RESULTADOS:. " COR_PADRAO);
        printf(COR_AZUL "\n   DNA   RNA  AMINO" COR_PADRAO);
        for (i = 0; i < qtCodons; i++) {
            printf(COR_AZUL "\n   %s   %s   %s" COR_PADRAO, codonsDNA[i], codonsRNA[i], aminoacidos[i]);
            char *novaLinha = malloc(TAMANHO_CODON * 7 * sizeof(char));
            initialize(novaLinha, TAMANHO_CODON * 7);
            strcat(novaLinha, "\n");
            strcat(novaLinha, codonsDNA[i]);
            strcat(novaLinha, "   ");
            strcat(novaLinha, codonsRNA[i]);
            strcat(novaLinha, "   ");
            strcat(novaLinha, aminoacidos[i]);
            strcat(resultadoArquivo, novaLinha);
            free(novaLinha);
        }
        escrever(resultadoArquivo, "resultados-mpi.txt");
        free(codonsDNA);
        free(codonsRNA);
        free(aminoacidos);
        free(resultadoArquivo);
        printf(COR_VERDE "\nProcesso Mestre: ESCRITA DE RESULTADOS NO ARQUIVO CONCLUIDA " COR_PADRAO);

    } else {
        printf("\n    Processo #%i: INICIOU ", idProcesso);
        char *cadeia;

        // Recebe a quantidade de codons e codons por processo
        MPI_Recv(&qtCodons, 1, MPI_INT, MESTRE, CODONS, MPI_COMM_WORLD, &status);
        MPI_Recv(&codonsPorProcesso, 1, MPI_INT, MESTRE, CODONS_POR_PROCESSO, MPI_COMM_WORLD, &status);

        cadeia = malloc(codonsPorProcesso * TAMANHO_CODON * sizeof(char));
        MPI_Recv(cadeia, codonsPorProcesso * TAMANHO_CODON, MPI_CHAR, MESTRE, CADEIA_DNA, MPI_COMM_WORLD,
                 &status);
        printf("\n    Processo #%i: RECEBEU PARTE DA CADEIA DE DNA DO MESTRE ", idProcesso);

        // Inicializa os vetores
        codonsDNA = malloc(codonsPorProcesso * sizeof(char *));
        codonsRNA = malloc(codonsPorProcesso * sizeof(char *));
        aminoacidos = malloc(codonsPorProcesso * sizeof(char *));

        codonsDNA = split(cadeia, TAMANHO_CODON);
        free(cadeia);

        // Realiza a transcrição da sua parte(DNA -> RNA). Transcrição feita por cada codon
        for (i = 0; i < codonsPorProcesso; i++) {
            codonsRNA[i] = transcription(codonsDNA[i], TAMANHO_CODON);
        }
        printf("\n    Processo #%i: TRANSCRICAO PARCIAL CONCLUIDA ", idProcesso);

        // Realiza a identificação de aminoácidos da sua parte (AUU -> Met)
        for (i = 0; i < codonsPorProcesso; i++) {
            aminoacidos[i] = aminoacids(codonsRNA[i], TAMANHO_CODON);
        }
        printf("\n    Processo #%i: IDENTIFICACAO DE AMINOACIDOS PARCIAL CONCLUIDA", idProcesso);

        // Remonta uma única string para poder enviar de volta ao MESTRE
        char *rnaTOmaster = malloc(codonsPorProcesso * TAMANHO_CODON * sizeof(char));
        char *aminoTOmaster = malloc(codonsPorProcesso * TAMANHO_CODON * sizeof(char));
        initialize(rnaTOmaster, codonsPorProcesso * TAMANHO_CODON);
        initialize(aminoTOmaster, codonsPorProcesso * TAMANHO_CODON);

        for (i = 0; i < codonsPorProcesso; i++) {
            strcat(rnaTOmaster, codonsRNA[i]);
            strcat(aminoTOmaster, aminoacidos[i]);
        }

        // Envia os resultados para o MESTRE
        MPI_Send(rnaTOmaster, codonsPorProcesso * TAMANHO_CODON, MPI_CHAR, MESTRE, TRANSCRICAO,
                 MPI_COMM_WORLD);
        MPI_Send(aminoTOmaster, codonsPorProcesso * TAMANHO_CODON, MPI_CHAR, MESTRE, AMINOACIDOS,
                 MPI_COMM_WORLD);
        free(rnaTOmaster);
        free(aminoTOmaster);
        printf(COR_AZUL_E "\n    Processo #%i: ENVIOU RESULTADOS PARA O PROCESSO MESTRE" COR_PADRAO, idProcesso);
        printf("\n    Processo #%i: FINALIZOU", idProcesso);
    }

    // Finaliza ambiente de comunicação MPI
    tFim = MPI_Wtime(); //Pega o tempo de finalização
    MPI_Finalize();
    tExecucao = tFim - tIni;
    if (idProcesso == MESTRE) {
        printf(COR_VERDE "\nTempo total: %fs\n" COR_PADRAO, tExecucao);
    }

    return 0;
}
