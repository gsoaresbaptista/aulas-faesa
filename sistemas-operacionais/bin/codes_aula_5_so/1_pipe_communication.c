#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main() {
    int fd[2]; // fd[0]: leitura, fd[1]: escrita
    pid_t pid;
    
    if (pipe(fd) == -1) return 1;
    
    pid = fork();
    
    if (pid > 0) { // Processo PAI
        close(fd[0]); // Fecha o lado de leitura pois o pai vai escrever
        char msg[] = "Olá do processo Pai!";
        write(fd[1], msg, strlen(msg) + 1);
        close(fd[1]);
    } else { // Processo FILHO
        close(fd[1]); // Fecha o lado de escrita pois o filho vai ler
        char buffer[100];
        read(fd[0], buffer, sizeof(buffer));
        printf("Filho recebeu via pipe: %s\n", buffer);
        close(fd[0]);
    }
    return 0;
}
