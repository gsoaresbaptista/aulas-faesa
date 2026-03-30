#include <stdio.h>
#include <pthread.h>

int vez;
int interesse[2] = {0, 0};
long contador = 0;

void enter_region(int process) {
    int outro = 1 - process;
    interesse[process] = 1;
    vez = outro;
    while (interesse[outro] && vez == outro);
}

void leave_region(int process) {
    interesse[process] = 0;
}

void* worker(void* arg) {
    int id = *(int*)arg;
    for (int i = 0; i < 1000000; i++) {
        enter_region(id);
        contador++;
        leave_region(id);
    }
    return NULL;
}

int main() {
    pthread_t t1, t2;
    int id0 = 0, id1 = 1;
    
    pthread_create(&t1, NULL, worker, &id0);
    pthread_create(&t2, NULL, worker, &id1);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    
    printf("Resultado com Peterson: %ld\n", contador);
    return 0;
}
