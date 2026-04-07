#include <stdio.h>
#include "aluno.h"

int ler_aluno(Aluno *a){
    return scanf("%49s %d %f %f %f %d %29s %2s",
                 a->nome,
                 &a->matricula,
                 &a->notas[0],
                 &a->notas[1],
                 &a->notas[2],
                 &a->faltas,
                 a->local.cidade,
                 a->local.estado) == 8;
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

void aluno_imprimir_linha(const Aluno *a){
    printf("%d %s %s %s %.2f %s\n",
           a->matricula,
           a->nome,
           a->local.cidade,
           a->local.estado,
           aluno_media(a),
           situacao_texto(aluno_situacao(a)));
}

Aluno *buscar_aluno_por_matricula(Aluno turma[], int n, int matricula){
    for(int i = 0; i < n; i++){
        if(turma[i].matricula == matricula)
            return &turma[i];
    }

    return NULL;
}

static int indice_menor_nota(const Aluno *a){
    int indice = 0;

    for(int i = 1; i < 3; i++){
        if(a->notas[i] < a->notas[indice])
            indice = i;
    }

    return indice;
}

void aplicar_bonus(Aluno *a, float bonus){
    int indice;

    if(a == NULL)
        return;

    indice = indice_menor_nota(a);
    a->notas[indice] += bonus;
    if(a->notas[indice] > 10.0f)
        a->notas[indice] = 10.0f;
}
