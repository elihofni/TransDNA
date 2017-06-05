# TransDNA
Transcrição DNA Paralela e identificação de aminoácidos

### Processo Main

- Ler o arquivo de entrada com o dna
- Separar a string entre os processos (ela inclusive)
- Realizar a transcrição, e identificacao de aminoacidos de sua parte
- Receber as partes dos demais processos
- Escrever no arquivo de saida


### Processos Trabalhadores 

```C
/*
* BIO: TRANSRIÇÂO [DNA -> RNA]
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
- DNA: aaaaacggcgtagca

Saida
-         DNA: aaaaacggcgtagca
-  CODONS DNA: aaa aac ggc gta gca
-         RNA: uuuuugccgcaucgu
-  CODONS RNA: uuu uug ccg cau cgu
- AMINOACIDOS: nome nome nome nome nome

### Compilando e executando
- Ir até a raíz do projeto via terminal
- Compilar `mpicc main.c -o dna transcription.c`
- Executar `mpirun -np 2 dna` (`-np <quantidade_de_procesos>`)