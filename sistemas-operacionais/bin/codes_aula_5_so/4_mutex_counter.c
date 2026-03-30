#include <stdio.h>
#include <pthread.h>

long contador = 0;
pthread_mutex_t trava;

void* incrementar(void* arg) {
    for (int i = 0; i < 1000000; i++) {
        pthread_mutex_lock(&trava);
        contador++; // Regiao Critica Protegida
        pthread_mutex_unlock(&trava);
    }
    return NULL;
}

int main() {
    pthread_t t1, t2;
    pthread_mutex_init(&trava, NULL);

    pthread_create(&t1, NULL, incrementar, NULL);
    pthread_create(&t2, NULL, incrementar, NULL);
    
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    pthread_mutex_destroy(&trava);

    printf("Resultado com Mutex: %ld (Esperado: 2000000)\n", contador);
    return 0;
}
