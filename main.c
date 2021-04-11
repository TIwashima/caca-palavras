#include <stdio.h>
#include <stdlib.h>
#define MAX(x, y) (((x) > (y)) ? (x) : (y))

enum direcao { BAIXO, DIREITA };

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
 retorno - 0 caso a palavra seja encontrada, -1 se não for
 */
int procurar_direcao(char *palavra, int tamanhoPalavra, int n,int linhas, int colunas, char **matriz, int direcao, int i, int j, int pos[]) {
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
    pos[2*n] = i;
    pos[2*n+1] = j;
  } else if (newI == linhas || newJ == colunas) {
    return 0; 
  } else {
    return 0;
  }
  
  if (tamanhoPalavra == n+1) {
    return 1;
  } else {
    return procurar_direcao(palavra, tamanhoPalavra, n+1, linhas, colunas, matriz, direcao, newI, newJ, pos);
  }
}

/* Busca uma palavra dentro da matriz e retorna um vetor de coordenadas

  Parametros:

  palavra - a palavra a ser procurada
  matriz - a matriz onde buscamos a palavra
  retorno - vetor com as coordenadas das letras da palavra, formato [x0, y0, x1, y1, ...]
*/
int procurar_palavra(char *palavra, int tamanhoPalavra, int linhas, int colunas, char **matriz,int * pos) {
  int i, j, acha_palavra;
  
  for (i = 0; i < linhas; i++) {
    for (j = 0; j < colunas; j++) {
      if (matriz[i][j] == palavra[0]) {
        acha_palavra = procurar_direcao(palavra, tamanhoPalavra, 0, linhas, colunas, matriz, DIREITA, i, j, pos);
        if (acha_palavra) {
          return 1;
        }
        acha_palavra = procurar_direcao(palavra, tamanhoPalavra, 0, linhas, colunas, matriz, BAIXO, i, j, pos);
        if (acha_palavra) {
          return 1;
        }
      }
    }
  }
  return 0;
}

void le_matriz(char **matriz, int linhas, int colunas) {
  int l, c;
  for (l = 0; l < linhas; l++) {
    for (c = 0; c < colunas; c++) {
      scanf(" %c", &matriz[l][c]);
    }
  }
}

void le_palavras(char **palavras, int qtd_palavras) {
  int i;
  for (i = 0; i < qtd_palavras; i++) {
    scanf("%s", palavras[i]);
  }
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
  int linhas, colunas, tamanhoPalavra, acha_palavra,;
  scanf("%d", &linhas);
  scanf("%d", &colunas);
  int pos[MAX(linhas,colunas) * 2];

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
  le_palavras(palavras, qtd_palavras);
  
  // A partir daqui é testando a busca
  for (i = 0; i < qtd_palavras; i++) {
    // Isso aqui conta o tamanho da palavra
    for (j = 0; palavras[i][j] != '\0'; j++);
    tamanhoPalavra = j;
    // Realmente fazendo a busca
    acha_palavra = procurar_palavra(palavras[i], tamanhoPalavra, linhas, colunas, matriz, pos);
    if (acha_palavra) {
        // Por enquanto só printa o vetor de posições, de cada palavra, é preciso integrar melhor
        // com o print. Talvez fazendo um vetor de vetores de posição?
        for (j = 0; j < tamanhoPalavra * 2; j++) {
            printf("%d ", pos[j]);
        }
    } else {
        printf("Palavra nao encontrada");
    }
  }
    
  for (i = 0; i < 3; i++)
      free(matriz[i]);
  free(matriz);

  return 0;
}
