# TransDNA
Transcrição DNA e identificação de aminoácidos

### Processo Mestre

- Lê o arquivo de entrada com o dna
- Encontra ponto inicial da transcrição (início do cístron)
- Separa a string entre os processos (ela inclusive)
- Realiza a transcrição, e identifica os aminoacidos de sua parte
- Recebe as partes dos demais processos
- Escreve no arquivo de saida


### Processos Trabalhadores 

```C
/*
* BIO: TRANSCRIÇÂO [DNA -> RNA]
* Traduz uma sequencia em DNA para RNA.
* str - String recebida pela thread.
* Exemplo: AACCGCTCA -> AACCGCUCA 
*/
char* func(char* str);
```

 
```C
/*
* BIO: CODONS
* funcao que quebra uma string em substrings
* de tamanho 3.
* str - String recebida pela thread.
* Exemplo: AACCGCTCA -> AAC CGC UCA 
*/
char** func(char* str);
```

```C
/*
* AMINOACIDOS
* funcao que recebe uma string de tamanho 3
* e traduz para um nome.
* Exemplo: AAC CGC UCA -> Nome1 Nome2, Nome3 
*/
char* func(char* strAraay);
```

### Exemplo de arquivos
Entrada
-
    DNA: AAAAACGGCGTAGCA

Saida
-      
     CODONS DNA: AAA AAC GGC GTA GCA                        
     CODONS RNA: UUU UUG CCG CAU CGU
    AMINOACIDOS: Phe Leu Pro His Arg

### Compilando e executando
- Ir até a raíz do projeto via terminal
- Paralelo

        - Compilar `mpicc main.c -o dna transcription.c io.c`
        - Executar(2 processos) `mpirun -np 2 dna`

- Sequencial

        - Compilar(sequencial) `gcc sequencial.c -o sequencial transcription.c io.c`
        - Executar `./sequencial`
