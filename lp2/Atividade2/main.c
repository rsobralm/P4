#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

#define THREAD_COUNT 10

pthread_barrier_t minhaBarreira;
int moda[THREAD_COUNT], frequencia[10][10] = {{0}}, soma[THREAD_COUNT], freq_geral[10], moda_numero[10] = {0};
double media[THREAD_COUNT];

void *leArquivo(void *parametro);

int main(int argc, char *argv[])
{
    pthread_t thrs[THREAD_COUNT];
    int ids[THREAD_COUNT];

    pthread_barrier_init(&minhaBarreira, NULL, THREAD_COUNT + 1);

    for (int i = 0; i < THREAD_COUNT; i++)
    {
        ids[i] = i;
        pthread_create(&thrs[i], NULL, leArquivo, &ids[i]);
    }

    pthread_barrier_wait(&minhaBarreira);


    int maior_freq = 0, mais_freq = 0;
    int soma_total = 0;
    for(int i = 0; i < THREAD_COUNT; i++)
    {
        printf("Grupo %c: Soma: %d, Media: %lf, Moda: %d\n", 97 + i, soma[i], media[i], moda[i]);
        soma_total += soma[i];
        moda_numero[moda[i]]++;
    }

    for(int i = 0; i < 10; i++)
    {
        if(moda_numero[i] > maior_freq)
        {
            maior_freq = moda_numero[i];
            mais_freq = i;
        }
    }

    printf("Total: Soma: %d, Media: %lf, Moda: %d\n", soma_total, soma_total/1000.0, mais_freq);

    // for(int i = 0; i < THREAD_COUNT; i++)
    // {
    //     for(int j = 0; j < 10; j++)
    //     {
    //         printf("%d ", frequencia[i][j]);
    //     }
    //     printf("\n");
    // }

    pthread_barrier_destroy(&minhaBarreira);
}

void *leArquivo(void *parametro)
{
    int thread_id = *(int *)parametro;

    // printf("%d\n", thread_id);

    char nome[] = "XX.txt";

    for (int j = 0; j < 10; j++)
    {
        nome[0] = 97 + thread_id;
        nome[1] = 48 + j;
        // printf("%s\n", nome);

        FILE *f;

        f = fopen(nome, "r");

        int num;

        while (fscanf(f, "%d", &num) != EOF)
        {
            soma[thread_id] += num;
            frequencia[thread_id][num]++;
        }
    }
    media[thread_id] = soma[thread_id] / 100.0;

    int mais_freq = 0, maior_freq = 0;
    for(int j = 0; j < 10; j++)
    {
        if(frequencia[thread_id][j] > maior_freq)
        {
            maior_freq = frequencia[thread_id][j];
            mais_freq = j;
        }
    }

    moda[thread_id] = mais_freq;

    // printf("%d\n", thread_id);

    pthread_barrier_wait(&minhaBarreira);
    return NULL;
}