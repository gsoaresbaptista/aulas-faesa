#include <stdio.h>
#include <pthread.h>

long contador = 0;

void* incrementar(void* arg) {
    for (int i = 0; i < 1000000; i++) {
        contador++; // Operação não-atômica: reg=cont; reg++; cont=reg;
    }
    return NULL;
}

int main() {
    pthread_t t1, t2;
    pthread_create(&t1, NULL, incrementar, NULL);
    pthread_create(&t2, NULL, incrementar, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("Resultado final do contador: %ld (Esperado: 2000000)\n", contador);
    return 0;
}
