#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N 5
#define LEFT (i + N - 1) % N
#define RIGHT (i + 1) % N

typedef enum { THINKING, HUNGRY, EATING } state_t;
state_t state[N];
sem_t mutex;
sem_t s[N];

void test(int i) {
    if (state[i] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING) {
        state[i] = EATING;
        printf("Filosofo %d pegou os garfos e esta COMENDO\n", i);
        sem_post(&s[i]);
    }
}

void take_forks(int i) {
    sem_wait(&mutex);
    state[i] = HUNGRY;
    printf("Filosofo %d esta faminto (HUNGRY)\n", i);
    test(i);
    sem_wait(&mutex);
    sem_wait(&s[i]);
}

void put_forks(int i) {
    sem_wait(&mutex);
    state[i] = THINKING;
    printf("Filosofo %d devolveu os garfos e voltou a PENSAR\n", i);
    test(LEFT);
    test(RIGHT);
    sem_post(&mutex);
}

pthread_mutex_t garfos[N];

void* filosofo(void* num) {
    int i = *(int*)num;
    while(1) {
        printf("Filosofo %d esta PENSANDO...\n", i);
        sleep(1);
        
        // Tenta pegar garfos (Solucao hierarquica para evitar Deadlock)
        if (i % 2 == 0) {
            pthread_mutex_lock(&garfos[LEFT]);
            pthread_mutex_lock(&garfos[RIGHT]);
        } else {
            pthread_mutex_lock(&garfos[RIGHT]);
            pthread_mutex_lock(&garfos[LEFT]);
        }
        
        printf("Filosofo %d esta COMENDO\n", i);
        sleep(1);
        
        pthread_mutex_unlock(&garfos[LEFT]);
        pthread_mutex_unlock(&garfos[RIGHT]);
    }
}

int main() {
    pthread_t thread_id[N];
    int ids[N];

    for (int i = 0; i < N; i++) {
        pthread_mutex_init(&garfos[i], NULL);
        ids[i] = i;
    }

    for (int i = 0; i < N; i++)
        pthread_create(&thread_id[i], NULL, filosofo, &ids[i]);

    for (int i = 0; i < N; i++)
        pthread_join(thread_id[i], NULL);

    return 0;
}
