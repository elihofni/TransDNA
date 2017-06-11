# TransDNA
Implementação da transcrição de DNA e identificação de aminoácidos utilizando MPI

### Processo Mestre

- Lê o arquivo de entrada com o dna
- Encontra ponto inicial da transcrição (início do cístron)
- Separa a string entre os processos (ela inclusive)
- Realiza a transcrição e identifica os aminoacidos de sua parte
- Recebe as partes dos demais processos
- Escreve no arquivo de saida


### Processos Trabalhadores 
- Recebe parte do cístron do processo mestre
- Realiza a transcrição e identifica os aminoacidos de sua parte
- Envia codons RNA e aminoácidos para o processo mestre

### Funções

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

### Exemplo de arquivos
Entrada
-
    DNA: ATTAAAAACGGCGTAGCA
    DNA: GTAGTAGTAATTAAAAACGGCGTAGCA
OBS: A cadeia pode ter qualquer tamanho, porém o cístron precisa ser múltiplo de 3
OBS: A leitura procurará pelo início de um cístron, identificado pelos codons ATT, ACT e ATC. Toda a cadeia antes do cístron é ignorada visto que poderá gerar uma proteína inválida

Saida
-      
     CODONS DNA: AAA AAC GGC GTA GCA                        
     CODONS RNA: UUU UUG CCG CAU CGU
    AMINOACIDOS: Phe Leu Pro His Arg

### Compilar e executar
- Ir até a raíz do projeto via terminal
- Paralelo

        - Compilar `mpicc main.c -o dna transcription.c io.c`
        - Executar(2 processos) `mpirun -np 2 dna`

- Sequencial

        - Compilar(sequencial) `gcc sequencial.c -o sequencial transcription.c io.c`
        - Executar `./sequencial`
