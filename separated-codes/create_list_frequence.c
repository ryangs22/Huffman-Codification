#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura do nó para lista encadeada e árvore
// Usando void* para o conteúdo como exigido
typedef struct Node {
    void* content;          // Byte (0-255) - para qualquer arquivo
    int frequency;          // Frequência do byte
    struct Node* next;      // Próximo na lista encadeada  
    struct Node* left;      // Filho esquerdo (árvore)
    struct Node* right;     // Filho direito (árvore)
} Node;

// Cria um novo nó com conteúdo e frequência
Node* createNode(void* content, int frequency) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->content = content;
    newNode->frequency = frequency;
    newNode->next = NULL;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

// Insere nó na lista encadeada mantendo ordenado por frequência
Node* insertSorted(Node* head, Node* newNode) {
    if (head == NULL || newNode->frequency < head->frequency) {
        newNode->next = head;
        return newNode;
    }
    
    Node* current = head;
    while (current->next != NULL && current->next->frequency <= newNode->frequency) {
        current = current->next;
    }
    
    newNode->next = current->next;
    current->next = newNode;
    return head;
}

// Conta frequências dos BYTES no arquivo (qualquer formato)
Node* countFrequencies(const unsigned char* data, size_t data_size) {
    int freq[256] = {0};
    
    // Contar frequências de BYTES (0-255)
    for (size_t i = 0; i < data_size; i++) {
        freq[data[i]]++;
    }
    
    // Criar lista de nós
    Node* head = NULL;
    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0) {
            unsigned char* content = (unsigned char*)malloc(sizeof(unsigned char));
            *content = (unsigned char)i;  // Armazena o byte (0-255)
            Node* newNode = createNode(content, freq[i]);
            head = insertSorted(head, newNode);
        }
    }
    
    return head;
}

// Remove o nó com menor frequência da lista
Node* removeMin(Node** head) {
    if (*head == NULL) return NULL;
    
    Node* minNode = *head;
    *head = (*head)->next;
    minNode->next = NULL;
    return minNode;
}

// Imprime a lista de frequências (para debug)
void printFrequencyList(Node* head) {
    printf("Lista de Frequências (BYTES):\n");
    Node* current = head;
    while (current != NULL) {
        unsigned char byte = *(unsigned char*)(current->content);
        printf("Byte: 0x%02X (%3d), Frequência: %d\n", 
               byte, byte, current->frequency);
        current = current->next;
    }
    printf("\n");
}

// FUNÇÃO MAIN - Ponto de entrada do programa
int main() {
    // Dados serão passados para as funções
    
    printf("=== COMPRESSOR HUFFMAN (QUALQUER FORMATO) ===\n");
    
    // 1. Contar frequências e criar lista ordenada
    // Node* frequencyList = countFrequencies(data, data_size);
    
    // 2. Construir árvore de Huffman (será implementada depois)
    // Node* huffmanTree = buildHuffmanTree(frequencyList);
    
    // 3. Gerar códigos (será implementada depois)
    // generateHuffmanCodes(huffmanTree);
    
    // 4. Codificar dados (será implementada depois)
    // unsigned char* encodedData = encodeData(data, data_size, huffmanTree);
    
    // 5. Salvar arquivo compactado (será implementada depois)
    // saveCompressedFile(encodedData, huffmanTree);
    
    printf("Processo concluído!\n");
    return 0;
}
