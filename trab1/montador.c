#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura para mapear instruções para opcodes
typedef struct {
    char instrucao[10];
    unsigned char opcode;
} TabelaOp;

// Tabela de opcodes
TabelaOp tabela[] = {
    {"LOAD$", 0x00},
    {"LOAD", 0x01},
    {"STORE", 0x02},
    {"ADD", 0x03},
    {"SUB", 0x04},
    {"MUL", 0x05},
    {"DIV", 0x06},
    {"INC", 0x07},
    {"DEC", 0x08},
    {"AND", 0x09},
    {"OR", 0x0a},
    {"NOT", 0x0b},
    {"JMP", 0x0c},
    {"JZ", 0x0d},
    {"JNZ", 0x0e},
    {"JG", 0x0f},
    {"JL", 0x10},
    {"JGE", 0x11},
    {"JLE", 0x12},
    {"HLT", 0x13}
};

// Função para obter o opcode de uma instrução
unsigned char get_opcode(const char *instrucao) {
    for (int i = 0; i < sizeof(tabela) / sizeof(TabelaOp); i++) {
        if (strcmp(tabela[i].instrucao, instrucao) == 0) {
            return tabela[i].opcode;
        }
    }
    return 0xFF; // Retorna 0xFF para instrução inválida
}

// Função para converter operandos com prefixo `$`
int parse_operando(const char *operando_str) {
    if (operando_str[0] == '$') {
        // Remove o `$` e converte o restante
        return (int)strtol(operando_str + 1, NULL, 0); // Base 0 permite detectar hexadecimal automaticamente
    }
    return atoi(operando_str); // Converte diretamente se não houver `$`
}

int main() {
    FILE *arquivo_asm = fopen("programa.asm", "r");
    FILE *arquivo_bin = fopen("programa.bin", "wb"); // Modo binário para salvar os dados

    if (!arquivo_asm || !arquivo_bin) {
        perror("Erro ao abrir arquivos");
        return 1;
    }

    char linha[100];
    while (fgets(linha, sizeof(linha), arquivo_asm)) {
        char instrucao[10], operando_str[10];
        int operando = 0; // Operando padrão é 0
        int elementos_lidos;

        // Analisa a linha (com ou sem operando)
        elementos_lidos = sscanf(linha, "%s %s", instrucao, operando_str);
        if (elementos_lidos == 1) {
            // Somente o opcode foi lido
            operando = 0; // Operando padrão para instruções sem operando
        } else if (elementos_lidos == 2) {
            // Converte o operando se ele existir
            operando = parse_operando(operando_str);
        }

        // Diferenciação de LOAD$ e LOAD
        if (strcmp(instrucao, "LOAD") == 0) {
            if (operando_str[0] == '$') {
                strcpy(instrucao, "LOAD$"); // Troca para LOAD$ se for de memória
            }
        }

        unsigned char opcode = get_opcode(instrucao);

        if (opcode == 0xFF) {
            fprintf(stderr, "Instrução inválida: %s\n", instrucao);
            continue;
        }

        // Escrevendo no arquivo binário (em hexadecimal)
        fwrite(&opcode, sizeof(unsigned char), 1, arquivo_bin);
        fwrite(&operando, sizeof(unsigned char), 1, arquivo_bin);
    }

    fclose(arquivo_asm);
    fclose(arquivo_bin);
    return 0;
}
