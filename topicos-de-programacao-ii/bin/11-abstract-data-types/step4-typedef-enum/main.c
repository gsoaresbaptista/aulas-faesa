#include <stdio.h>

#define MAX_ALUNOS 100

typedef enum {
    REPROVADO_NOTA,
    RECUPERACAO,
    APROVADO,
    REPROVADO_FALTA
} SituacaoAluno;

typedef struct {
    char nome[50];
    int matricula;
    float notas[3];
    int faltas;
} Aluno;

int ler_aluno(Aluno *a){
    return scanf("%49s %d %f %f %f %d",
                 a->nome,
                 &a->matricula,
                 &a->notas[0],
                 &a->notas[1],
                 &a->notas[2],
                 &a->faltas) == 6;
}

float aluno_media(const Aluno *a){
    return (a->notas[0] + a->notas[1] + a->notas[2]) / 3.0f;
}

SituacaoAluno aluno_situacao(const Aluno *a){
    float media = aluno_media(a);

    if(a->faltas > 18)
        return REPROVADO_FALTA;
    if(media >= 7.0f)
        return APROVADO;
    if(media >= 5.0f)
        return RECUPERACAO;
    return REPROVADO_NOTA;
}

const char *situacao_texto(SituacaoAluno situacao){
    switch(situacao){
        case REPROVADO_NOTA:
            return "REPROVADO_NOTA";
        case RECUPERACAO:
            return "RECUPERACAO";
        case APROVADO:
            return "APROVADO";
        case REPROVADO_FALTA:
            return "REPROVADO_FALTA";
    }

    return "DESCONHECIDA";
}

int indice_maior_media(const Aluno turma[], int n){
    int indice = 0;
    float maior = aluno_media(&turma[0]);

    for(int i = 1; i < n; i++){
        float atual = aluno_media(&turma[i]);
        if(atual > maior){
            maior = atual;
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
    int reprovados_nota = 0;
    int reprovados_falta = 0;

    if(scanf("%d", &n) != 1 || n < 1 || n > MAX_ALUNOS)
        return 1;

    for(int i = 0; i < n; i++){
        if(!ler_aluno(&turma[i]))
            return 1;
    }

    printf("RELATORIO COMPLETO\n");
    for(int i = 0; i < n; i++){
        float media = aluno_media(&turma[i]);
        SituacaoAluno situacao = aluno_situacao(&turma[i]);

        printf("%d %s %.2f %s\n",
               turma[i].matricula,
               turma[i].nome,
               media,
               situacao_texto(situacao));

        switch(situacao){
            case APROVADO:
                aprovados++;
                break;
            case RECUPERACAO:
                recuperacao++;
                break;
            case REPROVADO_NOTA:
                reprovados_nota++;
                break;
            case REPROVADO_FALTA:
                reprovados_falta++;
                break;
        }
    }

    {
        int melhor = indice_maior_media(turma, n);
        printf("Aprovados: %d\n", aprovados);
        printf("Recuperacao: %d\n", recuperacao);
        printf("Reprovados por nota: %d\n", reprovados_nota);
        printf("Reprovados por falta: %d\n", reprovados_falta);
        printf("Maior media geral: %s %d %.2f\n",
               turma[melhor].nome,
               turma[melhor].matricula,
               aluno_media(&turma[melhor]));
    }

    return 0;
}
