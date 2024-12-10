/*
...........................Grupo.................................................................................
		- Gabriel
		- Gustavo
		- Kamille
		- Victor
*/
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

//#define TAM 160000000
#define TAM 1600000


void merge(int *vet, int esq, int meio, int dir) {
    int n1 = meio - esq + 1;
    int n2 = dir - meio;

    static int *E = NULL, *D = NULL;  
    static int size = 0; 

    if (size < n1 || size < n2) {
        size = (n1 > n2) ? n1 : n2;
        E = realloc(E, size * sizeof(int));
        D = realloc(D, size * sizeof(int));
    }

    for (int i = 0; i < n1; i++)
        E[i] = vet[esq + i];
    for (int i = 0; i < n2; i++)
        D[i] = vet[meio + 1 + i];
    
    int i = 0, j = 0, k = esq;
    while (i < n1 && j < n2) {
        if (E[i] <= D[j]) {
            vet[k] = E[i];
            i++;
        } else {
            vet[k] = D[j];
            j++;
        }
        k++;
    }
    
    while (i < n1) {
        vet[k] = E[i];
        i++;
        k++;
    }
    
    while (j < n2) {
        vet[k] = D[j];
        j++;
        k++;
    }
}

void merge_sort(int *vet, int n) {
    int tam_sublista, esq, meio, dir;
    
    for (tam_sublista = 1; tam_sublista < n; tam_sublista *= 2) {
        for (esq = 0; esq < n - tam_sublista; esq += 2 * tam_sublista) {
            meio = esq + tam_sublista - 1;
            dir = (esq + 2 * tam_sublista - 1 < n) ? esq + 2 * tam_sublista - 1 : n - 1;
            merge(vet, esq, meio, dir);
        }
    }
}

int main(int argc, char *argv[]) {
    int rank, size;    
    clock_t inicio, fim;    
    double tempo_execucao;
    
    inicio = clock();

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    int *vet = NULL;
    
    if (rank == 0) {
        vet = (int *)malloc(TAM * sizeof(int));
        for (int i = 0; i < TAM; i++) {
            vet[i] = rand() % 100; 
        }
    }
    
    int local_tam = TAM / size;
    int *local_vet = (int *)malloc(local_tam * sizeof(int));
    
    MPI_Scatter(vet, local_tam, MPI_INT, local_vet, local_tam, MPI_INT, 0, MPI_COMM_WORLD);
    
    merge_sort(local_vet, local_tam);

    MPI_Gather(local_vet, local_tam, MPI_INT, vet, local_tam, MPI_INT, 0, MPI_COMM_WORLD);
    
    if (rank == 0) {
        for (int i = 1; i < size; i++) {
            int ini = i * local_tam;
            merge(vet, 0, ini - 1, ini + local_tam - 1);
        }
        
        free(vet);
        fim = clock();
        tempo_execucao = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
        printf("Tempo de execução: %f segundos\n", tempo_execucao);
    }
    
    free(local_vet);
    MPI_Finalize();
    return 0;
}

