#include "transcription.h"

void initialize(char *array,int size){
    int i;
    for(i=0;i<size;i++)array[i] = '\0';
}

/**
* BIO: CODONS
* funcao que quebra uma string em substrings
* de tamanho 3.
* str - String recebida pela thread.
* Exemplo: AACCGCTCA -> AAC CGC UCA
*/
char** split(char* str, int subStringSize){
    int size = strlen(str);
    int finalArraySize = size / subStringSize;

    char temp[subStringSize + 1];
    temp[subStringSize] = '\0';


    /*
     * Array que armazena as substrigns.
     */
    char** finalArray = calloc(finalArraySize, sizeof(char*));
    int i = 0;
    for (i = 0; i < finalArraySize; i++) {
        finalArray[i] = calloc((subStringSize + 1), sizeof(char));
    }

    /*
     * Iterador do array final.
     */
    int j = 0;

    /*
     * Iterador da string tempor�ria.
     */
    i = 0;
    /*
     * Iterador do string de entrada.
     */
    int k = 0;

    for(i = 0; k < size; i++, k++){
        temp[i] = str[k];

        if(i == (subStringSize - 1)){
            //Reseto o iterador da string tempor�ria.
            i = -1;

            //atribuo a string de tamanho 3 para o vetor final.
            int l = 0;
            for(l = 0; l < subStringSize; l++){
                finalArray[j][l] = temp[l];
            }

            //Incremento o index atual do vetor final.
            j++;
        }
    }

    return finalArray;
}

/**
* BIO: TRANSRIÇÂO [DNA -> RNA]
* Traduz uma sequencia em DNA para RNA.
* str - String recebida pela thread..
* Exemplo: AACCGCTCA -> AACCGCUCA
*/
char* transcription(char *chain, int size){
    int i;
    char *rna = calloc((size+1), sizeof(char)),k;
    //initialize(rna,size);
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
char* aminoacids(char *in, int size){
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

char** translate(char* str, int size){
    char* transc = transcription(str, size);

    char** finalArray = split(transc, 3);
    
    int i;
    for(i = 0; i < (size/3); i++){
        char* c = finalArray[i];
        char* aminoacid = aminoacids(c, 3);

        int l = 0;
        for(l = 0; l < 3; l++){
            finalArray[i][l] = aminoacid[l];
        }
    }

    return finalArray;
}

/**
 * Procura pelo início de um Cístron (A partir desse ponto pode-se começar a transcrição)
 * @param cadeiaDNA Cadeia Original do arquivo
 * @return Cadeia de DNA pronta para transcrição
 */
char* getCistron(char* cadeiaDNA) {
    int tamanho = strlen(cadeiaDNA);
    int i;
    for(i=0;i<tamanho;i++){
        if(cadeiaDNA[i] == 'A') {
            if((cadeiaDNA[i+1] == 'T' && (cadeiaDNA[i+2] == 'T' || cadeiaDNA[i+2] == 'C')) || (cadeiaDNA[i+1] == 'C' && cadeiaDNA[i+2] == 'T')) {
                printf(COR_VERDE "\nProcesso Mestre: CISTRON INICIA NO INDICE %i" COR_PADRAO, i+2);
                char* cadeiaCistron = malloc((tamanho - (i+3)) * sizeof(char));
                strncpy(cadeiaCistron, cadeiaDNA + i+3, (tamanho - (i+3)));
                return cadeiaCistron;
            }
        }
    }
    return NULL;
}