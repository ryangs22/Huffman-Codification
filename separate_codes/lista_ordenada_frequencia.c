#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

struct No {
    unsigned char simbolo;
    int frequencia;
    struct No* proximo;
    struct No* esquerdo;
    struct No* direito;
};

struct No* criarNo(unsigned char simbolo, int frequencia) {
    struct No* novoNo = (struct No*)malloc(sizeof(struct No));
    if (novoNo == NULL) exit(1);
    novoNo->simbolo = simbolo;
    novoNo->frequencia = frequencia;
    novoNo->proximo = NULL;
    novoNo->esquerdo = NULL;
    novoNo->direito = NULL;
    return novoNo;
}

// ✅ AGORA LÊ QUALQUER ARQUIVO BINÁRIO
void contarFrequenciasArquivo(FILE* arquivo, int frequencias[256]) {
    for (int i = 0; i < 256; i++) frequencias[i] = 0;
    
    unsigned char byte;
    // Lê até o final do arquivo, incluindo bytes 0x00
    while (fread(&byte, 1, 1, arquivo) == 1) {
        frequencias[byte]++;
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

// ✅ IMPRIME TODOS OS BYTES (0x00 a 0xFF)
void imprimirListaFrequencia(struct No* cabeca) {
    struct No* atual = cabeca;
    printf("Lista de Frequência (válida para qualquer formato):\n");
    while (atual != NULL) {
        if (atual->simbolo >= 32 && atual->simbolo <= 126) {
            printf("  '%c'  = %d\n", atual->simbolo, atual->frequencia);
        } else {
            printf("  0x%02X = %d\n", atual->simbolo, atual->frequencia);
        }
        atual = atual->proximo;
    }
}

void liberarLista(struct No* cabeca) {
    struct No* atual = cabeca;
    while (atual != NULL) {
        struct No* proximo = atual->proximo;
        free(atual);
        atual = proximo;
    }
}

// ✅ TESTE COM DIFERENTES FORMATOS
int main() {
    setlocale(LC_ALL, "Portuguese");
    
    // Para usar: passar nome do arquivo como argumento
    // Ex: ./huffman imagem.jpg
    //     ./huffman audio.mp3  
    //     ./huffman texto.txt
    
    char* nomeArquivo = "arquivo.bin"; // Ou qualquer formato
    
    FILE* arquivo = fopen(nomeArquivo, "rb"); // ✅ "rb" para binário
    if (arquivo == NULL) {
        printf("Erro ao abrir arquivo: %s\n", nomeArquivo);
        return 1;
    }
    
    int frequencias[256];
    contarFrequenciasArquivo(arquivo, frequencias);
    fclose(arquivo);
    
    struct No* lista = construirListaFrequencia(frequencias);
    imprimirListaFrequencia(lista);
    liberarLista(lista);
    
    return 0;
}
