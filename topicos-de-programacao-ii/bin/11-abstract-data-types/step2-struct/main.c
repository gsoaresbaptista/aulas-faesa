#include <stdio.h>

typedef struct {
    char nome[50];
    int matricula;
    float notas[3];
} Aluno;

int ler_aluno(Aluno *a){
    return scanf("%49s %d %f %f %f",
                 a->nome,
                 &a->matricula,
                 &a->notas[0],
                 &a->notas[1],
                 &a->notas[2]) == 5;
}

float aluno_media(const Aluno *a){
    return (a->notas[0] + a->notas[1] + a->notas[2]) / 3.0f;
}

const char *classificar_media(float media){
    if(media >= 7.0f)
        return "APROVADO";
    if(media >= 5.0f)
        return "RECUPERACAO";
    return "REPROVADO";
}

int main(void){
    Aluno aluno;
    float media;

    if(!ler_aluno(&aluno))
        return 1;

    media = aluno_media(&aluno);

    printf("Nome: %s\n", aluno.nome);
    printf("Matricula: %d\n", aluno.matricula);
    printf("Notas: %.2f %.2f %.2f\n", aluno.notas[0], aluno.notas[1], aluno.notas[2]);
    printf("Media: %.2f\n", media);
    printf("Situacao: %s\n", classificar_media(media));

    return 0;
}
