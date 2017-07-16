# TransDNA
Implementação da transcrição de DNA e identificação de aminoácidos usando paradigmas paralelos

## MPI
    Processo Mestre

    - Lê o arquivo de entrada com o dna
    - Encontra ponto inicial da transcrição (início do cístron)
    - Separa a string entre os processos (ela inclusive)
    - Realiza a transcrição e identifica os aminoacidos de sua parte
    - Recebe as partes dos demais processos
    - Escreve no arquivo de saida


    Processos Trabalhadores
     
    - Recebe parte do cístron do processo mestre
    - Realiza a transcrição e identifica os aminoacidos de sua parte
    - Envia codons RNA e aminoácidos para o processo mestre

## openMP
    Processo
    
    - Lê o arquivo de entrada com o dna
    - Encontra o ponto inicial da transcrição (início do cístron)
    - Inicia o ambiente openMP
    - Escreve no arquivo de saída
    
    
    Threads
    
    - Realizam a transcrição e identificação dos aminoacidos de sua parte

## Funções auxiliares implementadas

```C
/*
* Funcao que quebra uma string em substrings
* @param str - String que sofrerá operação de quebra
* @param substringSize - Tamanho desejado para cada partição (OBS: Precisa ser um numero multiplo do tamanho de str) 
* @return Vetor com as substrings
* Exemplo: split("AACCGCTCA", 3) -> ["AAC", "CGC", "UCA"]
*/
char** split(char* str, int subStringSize);
```

```C
/*
* BIOLOGIA: TRANSCRIÇÂO [DNA -> RNA]
* Traduz uma sequencia em DNA para RNA.
* @param chain - String que sofrerá transcrição
* @param size - Tamanho da string passada strlen(chain)
* @return String
* Exemplo: transcription("AACCGCTCA", 9)  -> "UUGGCGAGU" 
*/
char* transcription(char *chain, int size);
```

```C
/*
* BIOLOGIA: AMINOACIDOS [AAC -> Asp]
* Funcao que recebe uma string de tamanho 3 e retorna o nome do aminoácido correspondente
* @param in - String que sofrerá identificação do aminoácido
* @param size - Tamanho da string (o padrão é tamanho 3)
* Exemplo: aminoacids("AAC", 3) -> "Asp"
*/
char* aminoacids(char *in, int size);
```

## Exemplos de arquivos
#### Entrada
É preciso passar uma das hélices do DNA contendo o início de um cístron

OBS: A leitura procurará pelo início de um cístron, identificado após os codons ATT, ACT ou ATC 

OBS: A cadeia pode ter qualquer tamanho, porém o cístron precisa ser múltiplo de 3

OBS: Toda a cadeia antes do cístron é ignorada visto que poderá gerar uma proteína inválida

Exemplos reais de DNA _http://www.cbs.dtu.dk/services/NetGene2/fasta.php_
 
    GTAGTAGTAATTAAAAACGGCGTAGCA
    
Nesse exemplo, a substring do índice 0 ao 8 é descartada; a substring do índice 9 ao 11 também é descartada pois indica o fim do cístron. Desse modo apenas a substring a partir do índice 12 é considerada para a aplicação

   _GTAGTAGTA_ __ATT__ AAAAACGGCGTAGCA


#### Saida
O arquivo de saída é organizado em três colunas que facilitam a apuração e identificação de erros

        .:RESULTADOS:.
      DNA       RNA     AMINO
      AAA       UUU      Phe
      AAC       UUG      Leu
      GCG       CCG      Pro
      GTA       CAU      His
      GCA       CGU      Arg


## Compilar e executar
Ir até a raíz do projeto via terminal

    Paralelo MPI

        - Compilar `mpicc main-mpi.c transcription.c io.c -o dna-mpi`
        - Executar(2 processos) `mpirun -np 2 dna-mpi`

    Paralelo OpenMP

        - Compilar `gcc -fopenmp main-omp.c transcription.c io.c -o dna-omp`
        - Definir Quantidade de Threads Padrão `export OMP_NUM_THREADS=4`
        - Executar `./dna-omp`
        
    Sequencial

        - Compilar `gcc main-seq.c transcription.c io.c -o dna-seq`
        - Executar `./dna-seq`
