#include <stdio.h>
#include <ctype.h>
#include <string.h>

int busca_binaria_recursiva(char palavras[][32], char palavra[], int inicio, int fim) {
  if (fim < inicio) {
    return -1;
  }
  int meio = (inicio + fim) / 2;
  int comparacao = strcmp(palavras[meio], palavra);
  if (comparacao == 0) {
    return meio;
  } else if (comparacao > 0) {
    return busca_binaria_recursiva(palavras, palavra, inicio, meio - 1);
  } else {
    return busca_binaria_recursiva(palavras, palavra, meio + 1, fim);
  }
}

void inserir_palavra_recursiva(char palavras[][32], int numPalavras, char palavra[]) {
  if (numPalavras == 0 || strcmp(palavras[numPalavras - 1], palavra) <= 0) {
    strcpy(palavras[numPalavras], palavra);
    return;
  }
  strcpy(palavras[numPalavras], palavras[numPalavras - 1]);
  inserir_palavra_recursiva(palavras, numPalavras - 1, palavra);
}

void processar_token(char palavras[][32], int *numPalavras, char token[]) {
  int tamanho = strlen(token);
  if (tamanho > 0 && token[tamanho - 1] == '\n')
    token[tamanho - 1] = '\0';
  if (strcmp(token, ".") != 0) {
    for (int i = 0; i < tamanho; i++)
      token[i] = tolower(token[i]);
    printf("Palavra a ser adicionada: %s\n", token);
    int indice = busca_binaria_recursiva(palavras, token, 0, *numPalavras - 1);
    if (indice == -1) {
      if (*numPalavras < 1000) {
        inserir_palavra_recursiva(palavras, *numPalavras, token);
        (*numPalavras)++;
      } else {
        printf("Limite de palavras excedido.\n");
        return;
      }
    }
  }
}

void criar_dicionario(const char nomeArquivo[]) {
  char palavras[1000][32];
  int numPalavras = 0;
  FILE *arquivo = fopen(nomeArquivo, "r");
  if (arquivo == NULL) {
    printf("Erro ao abrir o arquivo.\n");
    return;
  }
  char linha[4096];
  int numeroLinha = 0;
  while (fgets(linha, sizeof(linha), arquivo)) {
    numeroLinha++;
    printf("Lendo linha %d: %s", numeroLinha, linha);
    char *token = strtok(linha, " ");
    while (token != NULL) {
      processar_token(palavras, &numPalavras, token);
      token = strtok(NULL, " ");
    }
  }
  fclose(arquivo);
  FILE *arquivoSaida = fopen("dicionario.txt", "w");
  if (arquivoSaida == NULL) {
    printf("Erro ao criar o arquivo de saída.\n");
    return;
  }
  for (int i = numPalavras - 1; i >= 0; i--)
    fprintf(arquivoSaida, "%s\n", palavras[i]);
  fprintf(arquivoSaida, "\nTotal de palavras diferentes: %d\n", numPalavras);
  fclose(arquivoSaida);
}

int main(void) {
  const char livro[] = "livro.txt";
  criar_dicionario(livro);
  return 0;
}
