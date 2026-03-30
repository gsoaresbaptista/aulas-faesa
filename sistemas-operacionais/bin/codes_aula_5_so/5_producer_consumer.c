#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 5
int buffer[BUFFER_SIZE];
int count = 0;

sem_t empty, full;
pthread_mutex_t mutex;

void* producer(void* arg) {
    for (int i = 0; i < 10; i++) {
        sem_wait(&empty); // Espera vaga no buffer
        pthread_mutex_lock(&mutex);
        
        buffer[count++] = i;
        printf("Produtor: Inseriu item %d na posicao %d\n", i, count-1);
        
        pthread_mutex_unlock(&mutex);
        sem_post(&full); // Avisa que tem novo item
        sleep(1);
    }
    return NULL;
}

void* consumer(void* arg) {
    for (int i = 0; i < 10; i++) {
        sem_wait(&full); // Espera item no buffer
        pthread_mutex_lock(&mutex);
        
        int item = buffer[--count];
        printf("Consumidor: Removeu item %d da posicao %d\n", item, count);
        
        pthread_mutex_unlock(&mutex);
        sem_post(&empty); // Libera vaga no buffer
        sleep(2);
    }
    return NULL;
}

int main() {
    pthread_t p, c;
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
    pthread_mutex_init(&mutex, NULL);

    pthread_create(&p, NULL, producer, NULL);
    pthread_create(&c, NULL, consumer, NULL);
    pthread_join(p, NULL);
    pthread_join(c, NULL);

    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);

    return 0;
}
