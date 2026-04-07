#include <stdio.h>

#define MAX_ALUNOS 100

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

int indice_melhor_media(const Aluno turma[], int n){
    int indice = 0;
    float melhor = aluno_media(&turma[0]);

    for(int i = 1; i < n; i++){
        float media_atual = aluno_media(&turma[i]);
        if(media_atual > melhor){
            melhor = media_atual;
            indice = i;
        }
    }

    return indice;
}

int main(void){
    Aluno turma[MAX_ALUNOS];
    int n;
    int aprovados = 0;
    int recuperacao = 0;
    int reprovados = 0;

    if(scanf("%d", &n) != 1 || n < 1 || n > MAX_ALUNOS)
        return 1;

    for(int i = 0; i < n; i++){
        if(!ler_aluno(&turma[i]))
            return 1;
    }

    printf("RELATORIO DA TURMA\n");
    for(int i = 0; i < n; i++){
        float media = aluno_media(&turma[i]);
        const char *situacao = classificar_media(media);

        printf("%d %s %.2f %s\n", turma[i].matricula, turma[i].nome, media, situacao);

        if(media >= 7.0f)
            aprovados++;
        else if(media >= 5.0f)
            recuperacao++;
        else
            reprovados++;
    }

    {
        int melhor = indice_melhor_media(turma, n);
        printf("Aprovados: %d\n", aprovados);
        printf("Recuperacao: %d\n", recuperacao);
        printf("Reprovados: %d\n", reprovados);
        printf("Melhor media: %s %d %.2f\n",
               turma[melhor].nome,
               turma[melhor].matricula,
               aluno_media(&turma[melhor]));
    }

    return 0;
}
