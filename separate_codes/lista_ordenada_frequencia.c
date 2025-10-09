#include <stdio.h>
#include <stdlib.h>

// Estrutura do nó da lista de frequência
struct No {
    unsigned char simbolo;        // Caractere/símbolo
    int frequencia;              // Frequência do símbolo
    struct No* proximo;          // Ponteiro para próximo nó na lista
    struct No* esquerdo;         // Ponteiro esquerdo (para árvore binária)
    struct No* direito;          // Ponteiro direito (para árvore binária)
};

// Função para criar um novo nó da lista de frequência
struct No* criarNo(unsigned char simbolo, int frequencia) {
    // 1. Alocar memória para o novo nó
    struct No* novoNo = (struct No*)malloc(sizeof(struct No));
    
    // 2. Verificar se a alocação foi bem-sucedida
    if (novoNo == NULL) {
        printf("Erro: Falha na alocação de memória.\n");
        exit(1);
    }
    
    // 3. Inicializar os campos do nó
    novoNo->simbolo = simbolo;
    novoNo->frequencia = frequencia;
    novoNo->proximo = NULL;
    novoNo->esquerdo = NULL;
    novoNo->direito = NULL;
    
    return novoNo;
}

// Procedimento para contar frequências dos caracteres no arquivo
void contarFrequenciasArquivo(FILE* arquivoEntrada, int frequencias[256]) {
    int caractere;
    
    // 1. Inicializar todo o array com zeros
    for (int i = 0; i < 256; i++) {
        frequencias[i] = 0;
    }
    
    // 2. Ler cada caractere do arquivo até o final
    while ((caractere = fgetc(arquivoEntrada)) != EOF) {
        // 3. Incrementar a frequência do caractere lido
        frequencias[caractere]++;
    }
}

// Função para inserir nó na lista mantendo a ordenação por frequência
struct No* inserirOrdenado(struct No* cabeca, struct No* novoNo) {
    // Caso 1: Lista vazia ou novo nó deve ser a nova cabeça
    if (cabeca == NULL || novoNo->frequencia < cabeca->frequencia) {
        novoNo->proximo = cabeca;
        return novoNo;
    }
    
    // Caso 2: Percorrer a lista para encontrar posição correta
    struct No* atual = cabeca;
    
    while (atual->proximo != NULL && atual->proximo->frequencia <= novoNo->frequencia) {
        atual = atual->proximo;
    }
    
    // 3. Inserir o novo nó na posição encontrada
    novoNo->proximo = atual->proximo;
    atual->proximo = novoNo;
    
    return cabeca;
}

// Função para construir a lista de frequência a partir do array de frequências
struct No* construirListaFrequencia(int frequencias[256]) {
    struct No* cabeca = NULL;
    
    // 1. Percorrer todos os 256 valores possíveis de byte
    for (int i = 0; i < 256; i++) {
        // 2. Verificar se o caractere aparece pelo menos uma vez
        if (frequencias[i] > 0) {
            // 3. Criar novo nó para o símbolo com sua frequência
            struct No* novoNo = criarNo((unsigned char)i, frequencias[i]);
            
            // 4. Inserir na lista mantendo ordenação por frequência
            cabeca = inserirOrdenado(cabeca, novoNo);
        }
    }
    
    return cabeca;
}

// Função auxiliar para imprimir a lista de frequência
void imprimirListaFrequencia(struct No* cabeca) {
    struct No* atual = cabeca;
    printf("Lista de Frequência Ordenada:\n");
    printf("Símbolo | Frequência\n");
    printf("--------|-----------\n");
    
    while (atual != NULL) {
        if (atual->simbolo >= 32 && atual->simbolo <= 126) {
            printf("   '%c'  |    %d\n", atual->simbolo, atual->frequencia);
        } else {
            printf("  0x%02X |    %d\n", atual->simbolo, atual->frequencia);
        }
        atual = atual->proximo;
    }
}
