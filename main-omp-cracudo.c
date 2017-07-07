/* SPMD Single Program Multiple Data
 * TransDNA com openMP pica das galáxias
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <omp.h>
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
#define NUM_THREADS 19 //o número de threads é setado por aqui.

int main(int argc, char** argv) {

    // Variáveis comuns a todas as threads
    double tIni, tFim, tExecucao;           // Controladoras de tempo
    int qtThreads = NUM_THREADS;  // Numero total de threads (definida pelo usuário no terminal)
    int qtCodons;                           // Quantidade de Codons que a cadeia de DNA original possui
    int codonsPorThread;                    // Quantidade de Codons que cada Thread tem direito
    char** codonsDNA;                       // Vetor de Codons do DNA
    char** codonsRNA;                       // Vetor de Codons do RNA
    char** aminoacidos;                     // Vetor de Aminoácidos
    omp_set_num_threads(NUM_THREADS);
    tIni = omp_get_wtime(); // Pega o tempo de início
    // Lê a cadeia do arquivo de entrada e encontra o ponto inicial para transcrição
    char *cadeiaDNAoriginal = ler("dnabigdemais.txt"); // TODO não esquecer de testar com demais arquivos de entrada
    //ERICK: explode com dnabig.txt. Amanhã eu vejo melhor
    printf("Processo: LEITURA NO ARQUIVO CONCLUIDA ");
    char *cadeiaDNA = getCistron(cadeiaDNAoriginal); // TODO alterar a função getCistron para printar no novo formato
    //ERICK: que novo formato?
    int tamanhoCadeiaDNA = strlen(cadeiaDNA);

    // TODO Verificar se compensa começar a área paralela a partir desse ponto
    //ERICK: testei e o parallel a partir daqui explode.
    // Particiona a cadeia original em codons (substrings de tamanho 3)
    codonsDNA = split(cadeiaDNA, TAMANHO_CODON);
    qtCodons = tamanhoCadeiaDNA / TAMANHO_CODON;
    // TODO Redistribuir a quantidade de codons entre threads caso a qtCodons não seja igual entre os processos

    codonsPorThread = qtCodons/qtThreads;
    printf("\nProcesso: TAMANHO DA CADEIA LIDA = %lu", strlen(cadeiaDNAoriginal));
    printf("\nProcesso: TAMANHO DO CISTRON = %i", tamanhoCadeiaDNA);
    printf("\nProcesso: TOTAL DE CODONS = %i", qtCodons);
    printf("\nProcesso: CODONS POR THREAD = %i", codonsPorThread);
    free(cadeiaDNAoriginal);
    free(cadeiaDNA);

    // Inicia o tamanho dos vetores que armazenarão os codonsRNA e aminoácidos
    codonsRNA = malloc(qtCodons * sizeof(char *));
    aminoacidos = malloc(qtCodons * sizeof(char *));

    // TODO Vale a pena colocar um timer somente para essa região paralela???
    // Início da seção paralela (threads)
    #pragma omp parallel
    {
        // Variáveis únicas de cada thread
        int idThread = omp_get_thread_num(); // ID da thread
        int i,x;                               // Iterador padrão
        int inicioAreaThread = codonsPorThread * idThread;
        int fimAreaThread = codonsPorThread * (idThread + 1);
        
        if((idThread == NUM_THREADS-1) && (qtCodons % NUM_THREADS != 0)){//número de threads não divide o número de códons	
        	fimAreaThread = fimAreaThread + (qtCodons % NUM_THREADS);       	
        }
        // Informa o usuário o estado da aplicação
        printf("\nThread %i: INICIOU ", idThread);
        printf("\nT #%i INI: %i", idThread, inicioAreaThread);
        printf("\nT #%i FIM: %i", idThread, fimAreaThread);

        // Executa a sua parte específica e escreve os resultados na variável compartilhada entre as threads
        for (i = inicioAreaThread; i < fimAreaThread; i++) {
            codonsRNA[i] = transcription(codonsDNA[i], TAMANHO_CODON);
        }
        for (i = inicioAreaThread; i < fimAreaThread; i++) {
            aminoacidos[i] = aminoacids(codonsRNA[i], TAMANHO_CODON);
        }

    }; // Fim da área paralela

    // Mostra os resultados para o usuário e prepara string final para escrever no arquivo de saída
    int i;
    char *resultadoArquivo = malloc(tamanhoCadeiaDNA * 7 * sizeof(char));
    strcat(resultadoArquivo, ".:RESULTADOS:.\nDNA   RNA  AMINO");
    printf(COR_AZUL "\n     .:RESULTADOS:. " COR_PADRAO);
    printf(COR_AZUL "\n   DNA   RNA  AMINO " COR_PADRAO);
    for (i = 0; i < qtCodons; i++) {
        printf(COR_AZUL "\n   %s   %s   %s " COR_PADRAO, codonsDNA[i], codonsRNA[i], aminoacidos[i]);
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
    escrever(resultadoArquivo, "resultados-omp.txt");
    free(codonsDNA);
    free(codonsRNA);
    free(aminoacidos);
    free(resultadoArquivo);
    printf(COR_VERDE "\nProcesso: ESCRITA DE RESULTADOS NO ARQUIVO CONCLUIDA " COR_PADRAO);

    // Informa o tempo total de execução
    tFim = omp_get_wtime(); // Pega o tempo de fim
    tExecucao = tFim - tIni;
    printf(COR_VERDE "\nTempo total: %fs\n" COR_PADRAO, tExecucao);


    return 0;
}
