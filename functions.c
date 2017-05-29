#include <stdio.h>
#include <stdlib.h>
#include <mem.h>

char* TERMINATOR = "fim";

/**
* BIO: TRANSRIÇÂO [DNA -> RNA]
* Traduz uma sequencia em DNA para RNA.
* str - String recebida pela thread..
* Exemplo: AACCGCTCA -> AACCGCUCA
*/
char* dnaToRna(char* str){
    return 0;
}

/**
* BIO: CODONS
* funcao que quebra uma string em substrings
* de tamanho 3.
* str - String recebida pela thread.
* Exemplo: AACCGCTCA -> AAC CGC UCA
*/
char** split(const char* str){
    int size = strlen(str);
    const int subStringSize = 3;
    const int finalArraySize = size / subStringSize;

    char temp[subStringSize + 1];
    temp[subStringSize] = '\0';


    /*
     * Array que armazena as substrigns.
     */
    char* finalArray[finalArraySize];
    for (int i = 0; i < finalArraySize; i++) {
        finalArray[i] = malloc((subStringSize + 1) * sizeof(char));
    }

    //Terminador do array.
    finalArray[finalArraySize] = TERMINATOR;

    /*
     * Iterador do array final.
     */
    int j = 0;

    /*
     * Iterador da string temporária.
     */
    int i = 0;

    /*
     * Iterador do string de entrada.
     */
    int k = 0;

    for(i = 0; k < size; i++, k++){
        temp[i] = str[k];

        if(i == (subStringSize - 1)){
            //Reseto o iterador da string temporária.
            i = -1;

            //atribuo a string de tamanho 3 para o vetor final.
            finalArray[j] = temp;

            //Incremento o index atual do vetor final.
            j++;
        }
    }

    return finalArray;
}

/**
* AMINOACIDOS
* funcao que recebe uma string de tamanho 3
* e traduz para um nome.
* Exemplo: AAC CGC UCA -> Nome1 Nome2, Nome3
*/
char* toAmin(char* strAraay){
    return 0;
}

int main() {

    char** hehehe = split("VAITOMACU");

    int i = 0;
    char* str = "";
    while(str != TERMINATOR){
        str = hehehe[i];
        printf("%s", str);
    }

    return 0;
}