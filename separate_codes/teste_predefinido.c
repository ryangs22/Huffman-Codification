#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>

/*
 ============================================================================
 PARTE 1: CRIAÇÃO DA LISTA ENCADEADA DE FREQUÊNCIA E ORDENAÇÃO CRESCENTE
 ============================================================================
*/

// Estrutura do nó da lista de frequência
struct No {
    unsigned char simbolo;        // Caractere/símbolo (0x00 a 0xFF)
    int frequencia;              // Frequência do símbolo
    struct No* proximo;          // Ponteiro para próximo nó na lista
    struct No* esquerdo;         // Ponteiro esquerdo (para árvore binária)
    struct No* direito;          // Ponteiro direito (para árvore binária)
};

// Função para criar um novo nó da lista de frequência
struct No* criarNo(unsigned char simbolo, int frequencia) {
    // Alocar memória para o novo nó
    struct No* novoNo = (struct No*)malloc(sizeof(struct No));
    if (novoNo == NULL) {
        printf("Erro na alocação de memória.\n");
        exit(1);
    }
    
    // Inicializar os campos do nó
    novoNo->simbolo = simbolo;
    novoNo->frequencia = frequencia;
    novoNo->proximo = NULL;
    novoNo->esquerdo = NULL;
    novoNo->direito = NULL;
    return novoNo;
}

// Procedimento para contar frequências de QUALQUER arquivo binário
void contarFrequenciasArquivo(FILE* arquivo, int frequencias[256]) {
    // Inicializar todo o array com zeros
    for (int i = 0; i < 256; i++) {
        frequencias[i] = 0;
    }
    
    unsigned char byte;
    // Ler cada byte do arquivo até o final (incluindo bytes 0x00)
    while (fread(&byte, 1, 1, arquivo) == 1) {
        frequencias[byte]++;  // Incrementa a frequência do byte lido
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
    
    // Inserir o novo nó na posição encontrada
    novoNo->proximo = atual->proximo;
    atual->proximo = novoNo;
    return cabeca;
}

// Função para construir a lista de frequência a partir do array de frequências
struct No* construirListaFrequencia(int frequencias[256]) {
    struct No* cabeca = NULL;
    
    // Percorrer todos os 256 valores possíveis de byte
    for (int i = 0; i < 256; i++) {
        // Verificar se o byte aparece pelo menos uma vez
        if (frequencias[i] > 0) {
            // Criar novo nó para o símbolo com sua frequência
            struct No* novoNo = criarNo((unsigned char)i, frequencias[i]);
            
            // Inserir na lista mantendo ordenação por frequência
            cabeca = inserirOrdenado(cabeca, novoNo);
        }
    }
    return cabeca;
}

/*
 ============================================================================
 PARTE 2: CONSTRUÇÃO DA ÁRVORE DE HUFFMAN
 ============================================================================
*/

// Função para remover o primeiro nó da lista (CORRIGIDA)
struct No* removerPrimeiroNo(struct No** cabeca) {
    if (*cabeca == NULL) {
        return NULL;
    }
    
    struct No* primeiro = *cabeca;      // Primeiro nó a ser removido
    *cabeca = (*cabeca)->proximo;       // Atualiza cabeça para o próximo
    primeiro->proximo = NULL;           // Isola o nó removido
    
    return primeiro;
}

// Procedimento para construir a árvore de Huffman (CORRIGIDA)
struct No* construirArvoreHuffman(struct No* cabeca) {
    if (cabeca == NULL) {
        printf("Erro: Lista de frequência vazia.\n");
        return NULL;
    }
    
    // Continuar enquanto houver mais de 1 nó na lista
    while (cabeca != NULL && cabeca->proximo != NULL) {
        
        // Passo 1: Remover os dois primeiros nós da lista
        struct No* primeiro = removerPrimeiroNo(&cabeca);
        struct No* segundo = removerPrimeiroNo(&cabeca);
        
        // Verificar se ambos os nós foram removidos com sucesso
        if (primeiro == NULL || segundo == NULL) {
            printf("Erro: Não foi possível remover os dois primeiros nós.\n");
            if (primeiro != NULL) free(primeiro);
            if (segundo != NULL) free(segundo);
            break;
        }
        
        // Passo 2: Criar novo nó interno com '*' (nó da árvore)
        struct No* novoNo = criarNo('*', primeiro->frequencia + segundo->frequencia);
        
        // Passo 3: Configurar os ponteiros esquerdo e direito
        novoNo->esquerdo = primeiro;
        novoNo->direito = segundo;
        
        // Passo 4: Inserir o novo nó de volta na lista ordenadamente
        cabeca = inserirOrdenado(cabeca, novoNo);
    }
    
    // Retornar a nova cabeça (que será a raiz da árvore)
    return cabeca;
}

// Procedimento para imprimir a árvore em PRÉ-ORDEM (com caractere de escape '\')
void imprimirArvorePreOrdem(struct No* raiz) {
    if (raiz == NULL) {
        return;
    }
    
    // Se for nó interno (*) ou caractere especial, usar escape
    if (raiz->simbolo == '*' || raiz->simbolo == '\\') {
        printf("\\%c", raiz->simbolo);
    }
    // Se for caractere imprimível normal
    else if (raiz->simbolo >= 32 && raiz->simbolo <= 126) {
        printf("%c", raiz->simbolo);
    }
    // Se for caractere não imprimível, mostrar com escape hexadecimal
    else {
        printf("\\x%02X", raiz->simbolo);
    }
    
    // Recursão para esquerdo e direito
    imprimirArvorePreOrdem(raiz->esquerdo);
    imprimirArvorePreOrdem(raiz->direito);
}

// Procedimento para liberar a árvore inteira (pós-ordem)
void liberarArvore(struct No* raiz) {
    if (raiz == NULL) {
        return;
    }
    
    // Liberar subárvores primeiro
    liberarArvore(raiz->esquerdo);
    liberarArvore(raiz->direito);
    
    // Liberar o nó atual
    free(raiz);
}

// Função para imprimir a lista de frequência
void imprimirListaFrequencia(struct No* cabeca) {
    struct No* atual = cabeca;
    printf("Lista de Frequência (válida para qualquer formato):\n");
    
    while (atual != NULL) {
        // Verificar se é caractere ASCII imprimível
        if (atual->simbolo >= 32 && atual->simbolo <= 126) {
            printf("  '%c'  = %d\n", atual->simbolo, atual->frequencia);
        } else {
            // Mostrar em hexadecimal para bytes não-printáveis
            printf("  0x%02X = %d\n", atual->simbolo, atual->frequencia);
        }
        atual = atual->proximo;
    }
}

/*
 ============================================================================
 PARTE 3: CRIAÇÃO DO DICIONÁRIO DE CÓDIGOS HUFFMAN
 ============================================================================
*/

// Função para calcular altura máxima da árvore
void calcularAlturaArvore(struct No* raiz, int* altura) {
    if (raiz == NULL) {
        *altura = -1;
        return;
    }
    
    int alturaEsquerda, alturaDireita;
    
    // Calcular altura da subárvore esquerda
    calcularAlturaArvore(raiz->esquerdo, &alturaEsquerda);
    
    // Calcular altura da subárvore direita  
    calcularAlturaArvore(raiz->direito, &alturaDireita);
    
    // Determinar a maior altura
    if (alturaEsquerda > alturaDireita) {
        *altura = alturaEsquerda + 1;
    } else {
        *altura = alturaDireita + 1;
    }
}

// Função para alocar a matriz dicionário
void alocarDicionario(char* dicionario[256], int colunas) {
    for (int i = 0; i < 256; i++) {
        // Alocar cada linha com número específico de colunas
        dicionario[i] = (char*)malloc(colunas * sizeof(char));
        
        // Verificar se a alocação foi bem-sucedida
        if (dicionario[i] == NULL) {
            printf("Erro: Falha ao alocar dicionario[%d]\n", i);
            // Liberar alocações anteriores em caso de erro
            for (int j = 0; j < i; j++) {
                free(dicionario[j]);
            }
            exit(1);
        }
        
        // Inicializar com string vazia
        dicionario[i][0] = '\0';
    }
}

// Função recursiva para gerar os códigos Huffman (COM SEGURANÇA)
void gerarDicionarioRecursivo(char* dicionario[256], struct No* no, char concatenacao[], int profundidade, int colunas) {
    if (no == NULL) {
        return;
    }
    
    // Verificar limite de profundidade para evitar estouro
    if (profundidade >= colunas - 1) {
        printf("Aviso: Profundidade máxima atingida para o símbolo 0x%02X\n", no->simbolo);
        return;
    }
    
    // Se é nó folha, salvar o código no dicionário
    if (no->esquerdo == NULL && no->direito == NULL) {
        // Finalizar a string do código
        concatenacao[profundidade] = '\0';
        
        // Copiar o código para a posição correspondente no dicionário
        strcpy(dicionario[no->simbolo], concatenacao);
        return;
    }
    
    // Explorar subárvore esquerda (adiciona '0')
    if (no->esquerdo != NULL) {
        concatenacao[profundidade] = '0';
        gerarDicionarioRecursivo(dicionario, no->esquerdo, concatenacao, profundidade + 1, colunas);
    }
    
    // Explorar subárvore direita (adiciona '1')
    if (no->direito != NULL) {
        concatenacao[profundidade] = '1';
        gerarDicionarioRecursivo(dicionario, no->direito, concatenacao, profundidade + 1, colunas);
    }
}

// Função para gerar o dicionário completo
void gerarDicionario(char* dicionario[256], struct No* raiz, int colunas) {
    // Criar array temporário para construção dos códigos
    char concatenacao[colunas];
    
    // Iniciar a geração recursiva
    gerarDicionarioRecursivo(dicionario, raiz, concatenacao, 0, colunas);
}

// Função para liberar a memória do dicionário
void liberarDicionario(char* dicionario[256]) {
    for (int i = 0; i < 256; i++) {
        if (dicionario[i] != NULL) {
            free(dicionario[i]);
            dicionario[i] = NULL;
        }
    }
}

// Função para imprimir o dicionário
void imprimirDicionario(char* dicionario[256]) {
    printf("=== DICIONÁRIO DE CÓDIGOS HUFFMAN ===\n");
    printf("Símbolo | Código\n");
    printf("--------|-------\n");
    
    int count = 0;
    for (int i = 0; i < 256; i++) {
        if (dicionario[i] != NULL && dicionario[i][0] != '\0') {
            if (i >= 32 && i <= 126) {
                printf("  '%c'   |  %s\n", i, dicionario[i]);
            } else {
                printf("  0x%02X |  %s\n", i, dicionario[i]);
            }
            count++;
        }
    }
    printf("Total de símbolos no dicionário: %d\n", count);
}

// Função principal
int main() {
    setlocale(LC_ALL, "Portuguese");
    
    // TESTE COM STRING SIMPLES PARA VERIFICAÇÃO
    const char* texto_teste = "ABRACADABRA";
    
    // Criar arquivo temporário para teste
    FILE* arquivo_teste = fopen("teste.txt", "wb");
    if (arquivo_teste != NULL) {
        fwrite(texto_teste, sizeof(char), strlen(texto_teste), arquivo_teste);
        fclose(arquivo_teste);
    }
    
    // Abrir arquivo em modo binário
    char* nomeArquivo = "teste.txt";
    FILE* arquivo = fopen(nomeArquivo, "rb");
    
    if (arquivo == NULL) {
        printf("Erro ao abrir arquivo: %s\n", nomeArquivo);
        return 1;
    }
    
    int frequencias[256];
    
    // PARTE 1.1: Contar frequências de todos os bytes do arquivo
    contarFrequenciasArquivo(arquivo, frequencias);
    fclose(arquivo);
    
    // PARTE 1.2: Construir lista ordenada de frequências
    struct No* lista = construirListaFrequencia(frequencias);
    
    printf("=== LISTA DE FREQUÊNCIA ===\n");
    if (lista == NULL) {
        printf("Lista vazia - nenhum símbolo encontrado!\n");
        return 1;
    }
    imprimirListaFrequencia(lista);
    
    // PARTE 2: Construir árvore de Huffman
    printf("\n=== CONSTRUINDO ÁRVORE DE HUFFMAN ===\n");
    struct No* raiz = construirArvoreHuffman(lista);
    
    if (raiz == NULL) {
        printf("ERRO: Árvore não foi construída!\n");
        liberarArvore(lista); // Liberar lista original em caso de erro
        return 1;
    }
    
    printf("\n=== ÁRVORE EM PRÉ-ORDEM ===\n");
    imprimirArvorePreOrdem(raiz);
    printf("\n");
    
    // PARTE 3: Criar dicionário de códigos Huffman
    printf("\n=== CRIANDO DICIONÁRIO ===\n");
    
    // 3.1 Calcular altura da árvore
    int altura;
    calcularAlturaArvore(raiz, &altura);
    int colunas = altura + 2; // +1 para profundidade, +1 para '\0'
    
    printf("Altura da árvore: %d | Colunas do dicionário: %d\n", altura, colunas);
    
    // 3.2 Alocar dicionário
    char* dicionario[256] = {NULL}; // Inicializar tudo como NULL
    alocarDicionario(dicionario, colunas);
    
    // 3.3 Gerar códigos
    gerarDicionario(dicionario, raiz, colunas);
    
    // 3.4 Imprimir dicionário
    imprimirDicionario(dicionario);
    
    // Liberar memória
    liberarDicionario(dicionario);
    liberarArvore(raiz);
    
    // Remover arquivo temporário
    remove("teste.txt");
    
    printf("\n✅ PROCESSO CONCLUÍDO COM SUCESSO!\n");
    
    return 0;
}
