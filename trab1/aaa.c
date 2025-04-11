#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *file;
    const char *filename = "arquivo.bin"; // Nome do arquivo binário
    long fileSize;
    size_t numCount;

    // Abrir o arquivo no modo leitura binária
    file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        return 1;
    }

    // Ir para o final do arquivo para determinar o tamanho
    fseek(file, 0, SEEK_END);
    fileSize = ftell(file);
    rewind(file);

    if (fileSize <= 0) {
        printf("Arquivo vazio ou erro ao determinar tamanho.\n");
        fclose(file);
        return 1;
    }

    // Calcular o número de elementos no arquivo
    numCount = fileSize / sizeof(int);

    printf("O arquivo contém %zu números inteiros.\n", numCount);

    // Fechar o arquivo
    fclose(file);
    return 0;
}
