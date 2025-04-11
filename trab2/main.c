#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void espera() {
    int tempo = rand() % 3 + 1;
    //printf("[TEMPO DE ESPERA] %d\n", tempo);
    sleep(tempo);
}

void produtor() {
    while (access("buffer.txt.lock", F_OK) == 0) continue;
    FILE *lock = fopen("buffer.txt.lock", "w");
    FILE *f = fopen("buffer.txt", "a");
    int numero = rand() % 100;
    fprintf(f, "%d\n", numero);
    printf("[Produtor] %d\n", numero);
    fclose(f);
    fclose(lock);
    remove("buffer.txt.lock");
    espera();
}

void consumidor() {
    while (access("buffer.txt.lock", F_OK) == 0) continue;
    FILE *lock = fopen("buffer.txt.lock", "w");
    FILE *f = fopen("buffer.txt", "r");
    FILE *fTemp = fopen("temp.txt", "a");
    char linha[4];

    if (fgets(linha, sizeof(linha), f) == NULL) {
        printf("[UNDERFLOW]\n");
        fclose(f);
        fclose(fTemp);
        fclose(lock);
        remove("temp.txt");
        remove("buffer.txt.lock");
        return;
    }

    printf("[Consumidor] %s", linha);

    while (fgets(linha, sizeof(linha), f)) {
        fputs(linha, fTemp);
    }
    remove("buffer.txt");
    rename("temp.txt", "buffer.txt");

    fclose(f);
    fclose(fTemp);
    remove("temp.txt");

    fclose(lock);
    remove("buffer.txt.lock");
    espera();
}

void inicia() {
    static int jaRodou = 0;
    if (jaRodou != 0) return;
    printf("[PROGRAMA INICIADO]\n");
    remove("buffer.txt.lock");
    srand(time(NULL));

    FILE *f = fopen("buffer.txt", "w");
    for (int i = 0; i < 10; i++) {
        int num = rand() % 100;
        fprintf(f, "%d\n", num);
    }
    fclose(f);
    jaRodou = 1;
}

int main() {
    inicia();
    
    pid_t pid = fork(); 

    if (pid > 0) {
        // Processo pai executa consumidor 
        while(1) consumidor();
    } else {
        // Processo filho executa produtor 
        while(1) produtor();
    }
    return 0;
}
