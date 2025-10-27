/*
 ============================================================================
 PARTE 8: SISTEMA PRINCIPAL COM MENU INTERATIVO
 ============================================================================
*/

// Função para comprimir arquivo seguindo o cabeçalho Huffman
void comprimirArquivo() {
    char nome_arquivo[256];
    char nome_saida[256];
    
    printf("\n=== COMPRESSÃO DE ARQUIVO ===\n");
    printf("Digite o nome do arquivo a ser comprimido: ");
    scanf("%255s", nome_arquivo);
    printf("Digite o nome do arquivo de saída (.huff): ");
    scanf("%255s", nome_saida);
    
    // Abrir arquivo em modo binário
    FILE* arquivo = fopen(nome_arquivo, "rb");
    if (arquivo == NULL) {
        printf("❌ Erro ao abrir arquivo: %s\n", nome_arquivo);
        return;
    }
    
    printf("📊 Analisando arquivo: %s\n", nome_arquivo);
    
    int frequencias[256];
    
    // PARTE 1: Contar frequências
    contarFrequenciasArquivo(arquivo, frequencias);
    
    // PARTE 2: Construir árvore de Huffman
    struct No* lista = construirListaFrequencia(frequencias);
    if (lista == NULL) {
        printf("❌ Arquivo vazio ou inválido!\n");
        fclose(arquivo);
        return;
    }
    
    struct No* raiz = construirArvoreHuffman(lista);
    if (raiz == NULL) {
        printf("❌ Erro ao construir árvore de Huffman!\n");
        liberarArvore(lista);
        fclose(arquivo);
        return;
    }
    
    // PARTE 3: Criar dicionário
    int altura;
    calcularAlturaArvore(raiz, &altura);
    int colunas = altura + 2;
    
    char* dicionario[256] = {NULL};
    alocarDicionario(dicionario, colunas);
    gerarDicionario(dicionario, raiz, colunas);
    
    // PARTE 4: Codificar arquivo
    codificarArquivo(arquivo, dicionario, "temp_codificado.bin");
    fclose(arquivo);
    
    // PARTE 6: Compactar com cabeçalho Huffman
    compactarComCabecalho("temp_codificado.bin", raiz, nome_saida);
    
    // Mostrar informações do cabeçalho
    printf("\n=== CABEÇALHO GERADO ===\n");
    mostrarCabecalhoCompactado(nome_saida);
    
    // Liberar memória
    liberarDicionario(dicionario);
    liberarArvore(raiz);
    
    // Remover arquivo temporário
    remove("temp_codificado.bin");
    
    printf("✅ Compressão concluída! Arquivo salvo como: %s\n", nome_saida);
}

// Função para descomprimir arquivo seguindo o cabeçalho Huffman
void descomprimirArquivo() {
    char nome_arquivo[256];
    char nome_saida[256];
    
    printf("\n=== DESCOMPRESSÃO DE ARQUIVO ===\n");
    printf("Digite o nome do arquivo .huff a ser descomprimido: ");
    scanf("%255s", nome_arquivo);
    printf("Digite o nome do arquivo de saída: ");
    scanf("%255s", nome_saida);
    
    // Verificar se é arquivo .huff
    if (strstr(nome_arquivo, ".huff") == NULL) {
        printf("⚠️  Aviso: O arquivo deve ter extensão .huff\n");
    }
    
    printf("📊 Lendo arquivo compactado: %s\n", nome_arquivo);
    
    // Para descompressão, precisamos primeiro ler o cabeçalho e reconstruir a árvore
    // Como não temos a árvore original, precisamos de uma abordagem diferente
    
    printf("❌ Funcionalidade de descompressão completa requer reconstrução da árvore do cabeçalho.\n");
    printf("   Esta versão assume que a árvore já está disponível (para demonstração).\n");
    
    // EM UMA IMPLEMENTAÇÃO COMPLETA, AQUI SERIA:
    // 1. Ler cabeçalho do arquivo .huff
    // 2. Extrair árvore da notação pré-ordem
    // 3. Reconstruir árvore Huffman
    // 4. Descompactar dados
}

// Função para mostrar informações do arquivo .huff
void mostrarInfoArquivo() {
    char nome_arquivo[256];
    
    printf("\n=== INFORMAÇÕES DO ARQUIVO .HUFF ===\n");
    printf("Digite o nome do arquivo .huff: ");
    scanf("%255s", nome_arquivo);
    
    FILE *arquivo = fopen(nome_arquivo, "rb");
    if (!arquivo) {
        printf("❌ Erro ao abrir arquivo: %s\n", nome_arquivo);
        return;
    }
    
    printf("\n📋 ESTRUTURA DO ARQUIVO %s:\n", nome_arquivo);
    mostrarCabecalhoCompactado(nome_arquivo);
    
    fclose(arquivo);
}

// Menu principal interativo
void menuPrincipal() {
    int opcao;
    
    printf("=== SISTEMA DE COMPRESSÃO HUFFMAN ===\n");
    printf("🔹 CÓDIGO 100%% GERAL - QUALQUER FORMATO DE ARQUIVO\n");
    printf("🔹 CABEÇALHO: 3 bits lixo + 13 bits árvore + Árvore Pré-Ordem\n");
    printf("🔹 SUPORTE: txt, jpg, png, mp3, mp4, exe, zip, etc.\n\n");
    
    do {
        printf("\n=== MENU PRINCIPAL ===\n");
        printf("1️⃣  - Comprimir arquivo\n");
        printf("2️⃣  - Descomprimir arquivo .huff\n");
        printf("3️⃣  - Mostrar informações do arquivo .huff\n");
        printf("0️⃣  - Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        
        switch(opcao) {
            case 1:
                comprimirArquivo();
                break;
            case 2:
                descomprimirArquivo();
                break;
            case 3:
                mostrarInfoArquivo();
                break;
            case 0:
                printf("👋 Saindo do sistema...\n");
                break;
            default:
                printf("❌ Opção inválida! Tente novamente.\n");
        }
    } while (opcao != 0);
}

// Função principal simplificada
int main() {
    setlocale(LC_ALL, "Portuguese");
    menuPrincipal();
    return 0;
}
