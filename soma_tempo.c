#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    FILE *file;
    char line[256];
    double totalTime = 0.0;
    int testCount = 0;

    file = fopen("resultados.txt", "r");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        return 1;
    }

    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, "Tempo de execução:") != NULL) {
            double time;
            if (sscanf(line, " Tempo de execução: %lf segundos", &time) == 1) {
                totalTime += time;
                testCount++;
                printf("Tempo de execução encontrado: %lf segundos\n", time);
            }
        }
    }

    fclose(file);

    if (testCount > 0) {
        printf("\nTotal de %d testes executados.\n", testCount);
        printf("Soma total dos tempos de execução: %lf segundos\n", totalTime);
        printf("Média dos tempos de execução: %lf segundos\n", totalTime / testCount);
    } else {
        printf("Nenhum tempo de execução encontrado no arquivo.\n");
    }

    return 0;
}

