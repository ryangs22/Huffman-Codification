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
    novoNo->proximo = NULL;       // Próximo nó começa como NULL
    novoNo->esquerdo = NULL;      // Ponteiro esquerdo vazio (para uso futuro na árvore)
    novoNo->direito = NULL;       // Ponteiro direito vazio (para uso futuro na árvore)
    
    // 4. Retornar o novo nó criado
    return novoNo;
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
    
    // Encontrar a posição onde a frequência do próximo nó é maior ou igual
    while (atual->proximo != NULL && atual->proximo->frequencia <= novoNo->frequencia) {
        atual = atual->proximo;
    }
    
    // 3. Inserir o novo nó na posição encontrada
    novoNo->proximo = atual->proximo;
    atual->proximo = novoNo;
    
    return cabeca;
}

// Função para construir a lista de frequência a partir de um arquivo
struct No* construirListaFrequencia(FILE* arquivoEntrada) {
    // 1. Criar um array para contar frequências (256 possíveis valores de byte)
    int frequencias[256] = {0};
    unsigned char buffer;
    
    // 2. Ler o arquivo byte a byte e contar frequências
    while (fread(&buffer, sizeof(unsigned char), 1, arquivoEntrada) == 1) {
        frequencias[buffer]++;
    }
    
    // 3. Reposicionar o ponteiro do arquivo para o início
    rewind(arquivoEntrada);
    
    struct No* cabeca = NULL;
    
    // 4. Criar nós apenas para bytes que aparecem pelo menos uma vez
    for (int i = 0; i < 256; i++) {
        if (frequencias[i] > 0) {
            // 5. Criar novo nó para o símbolo com sua frequência
            struct No* novoNo = criarNo((unsigned char)i, frequencias[i]);
            
            // 6. Inserir na lista mantendo ordenação por frequência
            cabeca = inserirOrdenado(cabeca, novoNo);
        }
    }
    
    return cabeca;
}

// Função auxiliar para imprimir a lista de frequência (para debug)
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
