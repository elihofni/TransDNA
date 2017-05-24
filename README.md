# TransDNA
Transcrição DNA Paralela e identificação de aminoácidos

### Thread Main

- Ler o arquivo de entrada com o dna
- Separar a string entre processos
- Merge final de rna, codons, aminoacidos
- Escrever no arquivo de saida


### Thread

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

### Exemplo d arquivos
Entrada
- DNA: aaaaacggcgtagca

Saida
-    DNA: aaaaacggcgtagca
-    RNA: uuuuugccgcaucgu
- CODONS: uuu uug ccg cau cgu
-  AMINO: nome nome nome nome nome