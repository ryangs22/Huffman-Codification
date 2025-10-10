#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>

/*
 ============================================================================
 PARTE 1: CRIAÇÃO DA LISTA ENCADEADA DE FREQUÊNCIA E ORDENAÇÃO CRESCENTE
 ============================================================================
*/
struct No {
    unsigned char simbolo;
    int frequencia;
    struct No* proximo;
    struct No* esquerdo;
    struct No* direito;
};

struct No* criarNo(unsigned char simbolo, int frequencia) {
    struct No* novoNo = (struct No*)malloc(sizeof(struct No));
    if (novoNo == NULL) {
        printf("Erro na alocação de memória.\n");
        exit(1);
    }
    novoNo->simbolo = simbolo;
    novoNo->frequencia = frequencia;
    novoNo->proximo = NULL;
    novoNo->esquerdo = NULL;
    novoNo->direito = NULL;
    return novoNo;
}

void contarFrequenciasString(const char* texto, int frequencias[256]) {
    for (int i = 0; i < 256; i++) {
        frequencias[i] = 0;
    }
    for (int i = 0; texto[i] != '\0'; i++) {
        frequencias[(unsigned char)texto[i]]++;
    }
}

struct No* inserirOrdenado(struct No* cabeca, struct No* novoNo) {
    if (cabeca == NULL || novoNo->frequencia < cabeca->frequencia) {
        novoNo->proximo = cabeca;
        return novoNo;
    }
    struct No* atual = cabeca;
    while (atual->proximo != NULL && atual->proximo->frequencia <= novoNo->frequencia) {
        atual = atual->proximo;
    }
    novoNo->proximo = atual->proximo;
    atual->proximo = novoNo;
    return cabeca;
}

struct No* construirListaFrequencia(int frequencias[256]) {
    struct No* cabeca = NULL;
    for (int i = 0; i < 256; i++) {
        if (frequencias[i] > 0) {
            struct No* novoNo = criarNo((unsigned char)i, frequencias[i]);
            cabeca = inserirOrdenado(cabeca, novoNo);
        }
    }
    return cabeca;
}

/*
 ============================================================================
 PARTE 2: CONSTRUÇÃO DA ÁRVORE DE HUFFMAN (CORRIGIDA)
 ============================================================================
*/

// ⭐ FUNÇÃO CORRIGIDA: usa ponteiro para ponteiro para atualizar a cabeça
struct No* removerPrimeiroNo(struct No** cabeca) {
    if (*cabeca == NULL) {
        return NULL;
    }
    struct No* primeiro = *cabeca;
    *cabeca = (*cabeca)->proximo;  // ⭐ ATUALIZA A CABEÇA ORIGINAL
    primeiro->proximo = NULL;
    return primeiro;
}

// ⭐ FUNÇÃO CORRIGIDA: usa a nova função de remoção
struct No* construirArvoreHuffman(struct No* cabeca) {
    if (cabeca == NULL) {
        printf("Erro: Lista de frequência vazia.\n");
        return NULL;
    }
    
    while (cabeca != NULL && cabeca->proximo != NULL) {
        // ⭐ CORREÇÃO: Passar &cabeca para atualizar a cabeça
        struct No* primeiro = removerPrimeiroNo(&cabeca);
        struct No* segundo = removerPrimeiroNo(&cabeca);
        
        if (primeiro == NULL || segundo == NULL) {
            printf("Erro: Não foi possível remover os dois primeiros nós.\n");
            break;
        }
        
        struct No* novoNo = criarNo('*', primeiro->frequencia + segundo->frequencia);
        novoNo->esquerdo = primeiro;
        novoNo->direito = segundo;
        
        cabeca = inserirOrdenado(cabeca, novoNo);
    }
    
    return cabeca;  // Retorna a raiz da árvore
}

void imprimirArvorePreOrdem(struct No* raiz, int altura_atual) {
    if (raiz == NULL) {
        return;
    }
    
    // Imprimir nó atual
    if (raiz->esquerdo == NULL && raiz->direito == NULL) {
        // É folha
        if (raiz->simbolo == '*' || raiz->simbolo == '\\') {
            printf("Folha: \\%c, Altura: %d\n", raiz->simbolo, altura_atual);
        } else if (raiz->simbolo >= 32 && raiz->simbolo <= 126) {
            printf("Folha: %c, Altura: %d\n", raiz->simbolo, altura_atual);
        } else {
            printf("Folha: \\x%02X, Altura: %d\n", raiz->simbolo, altura_atual);
        }
    } else {
        // É nó interno
        printf("Nó interno: *, Altura: %d\n", altura_atual);
    }
    
    // Recursão para subárvores
    imprimirArvorePreOrdem(raiz->esquerdo, altura_atual + 1);
    imprimirArvorePreOrdem(raiz->direito, altura_atual + 1);
}

void liberarArvore(struct No* raiz) {
    if (raiz == NULL) {
        return;
    }
    liberarArvore(raiz->esquerdo);
    liberarArvore(raiz->direito);
    free(raiz);
}

void imprimirListaFrequencia(struct No* cabeca) {
    struct No* atual = cabeca;
    printf("Lista de Frequência (válida para qualquer formato):\n");
    while (atual != NULL) {
        if (atual->simbolo >= 32 && atual->simbolo <= 126) {
            printf(" '%c' = %d\n", atual->simbolo, atual->frequencia);
        } else {
            printf(" 0x%02X = %d\n", atual->simbolo, atual->frequencia);
        }
        atual = atual->proximo;
    }
}

/*
 ============================================================================
 PARTE 3: CRIAÇÃO DO DICIONÁRIO DE CÓDIGOS HUFFMAN
 ============================================================================
*/
void calcularAlturaArvore(struct No* raiz, int* altura) {
    if (raiz == NULL) {
        *altura = -1;
        return;
    }
    int alturaEsquerda, alturaDireita;
    calcularAlturaArvore(raiz->esquerdo, &alturaEsquerda);
    calcularAlturaArvore(raiz->direito, &alturaDireita);
    if (alturaEsquerda > alturaDireita) {
        *altura = alturaEsquerda + 1;
    } else {
        *altura = alturaDireita + 1;
    }
}

void alocarDicionario(char* dicionario[256], int colunas) {
    for (int i = 0; i < 256; i++) {
        dicionario[i] = (char*)malloc(colunas * sizeof(char));
        if (dicionario[i] != NULL) {
            dicionario[i][0] = '\0';
        }
    }
}

void gerarDicionarioRecursivo(char* dicionario[256], struct No* no, char concatenacao[], int profundidade, int colunas) {
    if (no == NULL) {
        return;
    }
    if (profundidade >= colunas - 1) {
        printf("Erro: Profundidade excede o tamanho do dicionário.\n");
        return;
    }
    if (no->esquerdo == NULL && no->direito == NULL) {
        concatenacao[profundidade] = '\0';
        strncpy(dicionario[no->simbolo], concatenacao, colunas - 1);
        dicionario[no->simbolo][colunas - 1] = '\0';
        return;
    }
    if (no->esquerdo != NULL) {
        concatenacao[profundidade] = '0';
        gerarDicionarioRecursivo(dicionario, no->esquerdo, concatenacao, profundidade + 1, colunas);
    }
    if (no->direito != NULL) {
        concatenacao[profundidade] = '1';
        gerarDicionarioRecursivo(dicionario, no->direito, concatenacao, profundidade + 1, colunas);
    }
}

void gerarDicionario(char* dicionario[256], struct No* raiz, int colunas) {
    char concatenacao[colunas];
    gerarDicionarioRecursivo(dicionario, raiz, concatenacao, 0, colunas);
}

void liberarDicionario(char* dicionario[256]) {
    for (int i = 0; i < 256; i++) {
        if (dicionario[i] != NULL) {
            free(dicionario[i]);
            dicionario[i] = NULL;
        }
    }
}

void imprimirDicionario(char* dicionario[256]) {
    printf("=== DICIONÁRIO DE CÓDIGOS HUFFMAN ===\n");
    printf("Símbolo | Código\n");
    printf("--------|-------\n");
    for (int i = 0; i < 256; i++) {
        if (dicionario[i] != NULL && dicionario[i][0] != '\0') {
            if (i >= 32 && i <= 126) {
                printf(" '%c' | %s\n", i, dicionario[i]);
            } else {
                printf(" 0x%02X | %s\n", i, dicionario[i]);
            }
        }
    }
}

int main() {
    setlocale(LC_ALL, "Portuguese");
   
    const char* texto = "Vamos aprender a programa";
   
    int frequencias[256];
    contarFrequenciasString(texto, frequencias);
   
    struct No* lista = construirListaFrequencia(frequencias);
   
    printf("=== LISTA DE FREQUÊNCIA ===\n");
    if (lista == NULL) {
        printf("Lista de frequência vazia!\n");
    } else {
        imprimirListaFrequencia(lista);
    }
   
    printf("\n=== CONSTRUINDO ÁRVORE DE HUFFMAN ===\n");
    struct No* raiz = construirArvoreHuffman(lista);
   
    if (raiz == NULL) {
        printf("Erro: Árvore não foi construída.\n");
    } else {
        printf("\n=== ÁRVORE EM PRÉ-ORDEM (Folha: Símbolo, Altura: Altura da Folha) ===\n");
        imprimirArvorePreOrdem(raiz, 0);
        printf("\n");
    }
   
    printf("\n=== CRIANDO DICIONÁRIO ===\n");
    int altura;
    calcularAlturaArvore(raiz, &altura);
    int colunas = altura + 2;
    printf("Altura da árvore: %d | Colunas do dicionário: %d\n", altura, colunas);
   
    char* dicionario[256];
    alocarDicionario(dicionario, colunas);
    gerarDicionario(dicionario, raiz, colunas);
    imprimirDicionario(dicionario);
   
    liberarDicionario(dicionario);
    liberarArvore(raiz);
   
    return 0;
}
