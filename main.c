#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define MAX(x, y) (((x) > (y)) ? (x) : (y))

enum direcao { BAIXO, DIREITA };

struct args {
  char *palavra;
  int tamanhoPalavra;
  int linhas;
  int colunas;
  char **matriz;
  int *pos; 
  int posicao;
};

int strLen(char *palavra) {
    int i;
    for (i=0;palavra[i] != '\0';i++);
    return i;
}

/* Procura uma palavra na matriz em uma determinada direção
 A busca se faz letra a letra, se uma letra for encontrada,
 adicionamos suas coordenadas no vetor pos e olhamos a próxima
 letra nessa direção até chegarmos ao fim.

 Parametros:

 Palavra - palavra sendo procurada, a cada letra que avançamos,
 tiramos uma letra de palavra
 n - a atual posição no vetor de coordenadas
 matriz - a matriz em que buscamos as palavras
 direcao - a direção a ser seguida
 x, y - as atuais coordenadas em que estamos buscando a letra
 pos - vetor de coordenadas das letras da palavra
 posicao - numero equivalente ao total de letras encontradas entre todas as
    palavras buscadas. Como salvamos todas as posicoes com letras em um vetor,
    esse parametro serve para que encontremos em quais posicoes do vetor
    colocar as coordenadas de cada letra.
 retorno - 0 caso a palavra seja encontrada, -1 se não for
 */
int procurar_direcao(char *palavra, int tamanhoPalavra, int n,int linhas, 
    int colunas, char **matriz, int direcao, int i, int j, int pos[], 
    int posicao) {
  int newI, newJ;
  if (direcao == BAIXO) {
    newI = i+1;
    newJ = j;
  }
  
  if (direcao == DIREITA) {
    newI = i;
    newJ = j+1;
  }

  if (matriz[i][j] == palavra[n]) {
    pos[2 * (posicao + n)] = i;
    pos[2 * (posicao + n) + 1] = j;
  } else if (newI == linhas || newJ == colunas) {
    return 0; 
  } else {
    return 0;
  }
  
  if (tamanhoPalavra == n+1) {
    return 1;
  } else {
    return procurar_direcao(palavra, tamanhoPalavra, n+1, linhas, colunas, 
        matriz, direcao, newI, newJ, pos, posicao);
  }
}

/* Busca uma palavra dentro da matriz e retorna um vetor de coordenadas

  Parametros:

  palavra - a palavra a ser procurada
  matriz - a matriz onde buscamos a palavra
  retorno - vetor com as coordenadas das letras da palavra, formato [x0, y0, x1, y1, ...]
*/
void* procurar_palavra(void *structPalavra) {
  int i, j, acha_palavra;
  char *palavra = ((struct args*)structPalavra)->palavra;
  int tamanhoPalavra = ((struct args*)structPalavra)->tamanhoPalavra;
  int linhas = ((struct args*)structPalavra)->linhas ;
  int colunas = ((struct args*)structPalavra)->colunas;
  char **matriz = ((struct args*)structPalavra)->matriz;
  int *pos = ((struct args*)structPalavra)->pos; 
  int posicao = ((struct args*)structPalavra)->posicao;
  for (i = 0; i < linhas; i++) {
    for (j = 0; j < colunas; j++) {
      if (matriz[i][j] == palavra[0]) {
        acha_palavra = procurar_direcao(palavra, tamanhoPalavra, 0, linhas, 
            colunas, matriz, DIREITA, i, j, pos, posicao);
        if (acha_palavra) {
          return NULL;
        }
        acha_palavra = procurar_direcao(palavra, tamanhoPalavra, 0, linhas, 
            colunas, matriz, BAIXO, i, j, pos, posicao);
        if (acha_palavra) {
          return NULL;
        }
      }
    }
  }
}

void le_matriz(char **matriz, int linhas, int colunas) {
  int l, c;
  for (l = 0; l < linhas; l++) {
    for (c = 0; c < colunas; c++) {
      scanf(" %c", &matriz[l][c]);
    }
  }
}

int le_palavras(char **palavras, int qtd_palavras) {
  int i, tamanhoPos = 0;
  for (i = 0; i < qtd_palavras; i++) {
    scanf("%s", palavras[i]);
    tamanhoPos = tamanhoPos + strLen(palavras[i]);
  }
  
  return tamanhoPos;
}

/* função printaMartiz: imprime o caça palavras apenas com as palavras encotradas

  Parametros:

  pos[] - vetor com as coordenadas das letras da palavra, formato [x0, y0, x1, y1, ...]
  tamanho - tamanho do vetor pos[]
  matriz - a matriz que contém o caça palavras
  linhas - número de linhas da matriz
  colunas - número de colunas da matriz
*/

void printaMatriz(int pos[], int tamanho, char **matriz, int linhas, int colunas){
    char **aux;

    aux = malloc(linhas*sizeof (char*));
    for (int i=0; i < linhas; i++)
        aux[i] = malloc (colunas * sizeof (char)) ;
    for (int i = 0; i < linhas; ++i) {
        for (int j = 0; j < colunas; ++j) {
            aux[i][j]='*';
        }
    }
    for (int i = 0; i < tamanho; i=i+2) {
        int a = pos[i];
        int b = pos[i+1];
        aux[a][b]=matriz[a][b];
    }
    for (int i = 0; i < linhas; ++i) {
        for (int j = 0; j < colunas; ++j) {
            printf("%c ", aux[i][j]);
        }
        printf("\n");
    }

    for (int i=0; i < linhas; i++)
        free(aux[i]);
    free(aux) ;
}

int main() {

  // Recebe o caça-palavras, que é armazenado em uma matriz de char
  int linhas, colunas, tamanhoPalavra, acha_palavra, tamanhoPos, posicao = 0;
  scanf("%d", &linhas);
  scanf("%d", &colunas);

  char **matriz;
  int i, j;
  matriz = malloc(linhas * sizeof(char *));
  for (i = 0; i < linhas; i++) {
    matriz[i] = malloc(colunas * sizeof(char));
  }
  le_matriz(matriz, linhas, colunas);
  // Recebe as palavras que serão encontradas e armazena em um vetor
  int qtd_palavras;
  scanf("%d", &qtd_palavras);
  char **palavras;
  palavras = malloc(qtd_palavras * sizeof(char *));
  for (i = 0; i < qtd_palavras; i++) {
    palavras[i] = calloc(MAX(linhas, colunas), sizeof(char));
  }
  tamanhoPos = le_palavras(palavras, qtd_palavras);
  int pos[tamanhoPos * 2];
  
  /* // A partir daqui é testando a busca
  for (i = 0; i < qtd_palavras; i++) {
    // Isso aqui conta o tamanho da palavra
    tamanhoPalavra = strLen(palavras[i]);
    posicao = procurar_palavra(palavras[i], tamanhoPalavra, linhas, colunas,
        matriz, pos, posicao);
  } */
  pthread_t thr[qtd_palavras];
  struct args structPalavras[qtd_palavras];
  
  //structPalavras = (struct args *)malloc(qtd_palavras * sizeof(struct args));
  for(i = 0; i < qtd_palavras; i++){
    tamanhoPalavra = strLen(palavras[i]);
    structPalavras[i].palavra = palavras[i];
    structPalavras[i].tamanhoPalavra = tamanhoPalavra;
    structPalavras[i].linhas = linhas;
    structPalavras[i].colunas = colunas;
    structPalavras[i].matriz = matriz;
    structPalavras[i].pos = pos;
    structPalavras[i].posicao = posicao;
    pthread_create(&thr[i], NULL, procurar_palavra, (void *)(&(structPalavras[i])));
    posicao = posicao + tamanhoPalavra;
  }

  for (i = 0; i < qtd_palavras; i++) 
    pthread_join(thr[i], NULL); 

  printaMatriz(pos, tamanhoPos * 2, matriz, linhas, colunas);
  
  for (i = 0; i < 3; i++)
      free(matriz[i]);
  free(matriz);

  //free(structPalavras);

  return 0;
}
