#include <stdio.h>
#include "aluno.h"

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

void aluno_imprimir_linha(const Aluno *a){
    printf("%d %s %.2f %s\n",
           a->matricula,
           a->nome,
           aluno_media(a),
           situacao_texto(aluno_situacao(a)));
}
