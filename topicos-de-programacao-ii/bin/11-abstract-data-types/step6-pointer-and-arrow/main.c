#include <stdio.h>
#include "aluno.h"

#define MAX_ALUNOS 100

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
    int m;
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

    if(scanf("%d", &m) != 1 || m < 0)
        return 1;

    printf("OPERACOES\n");
    for(int i = 0; i < m; i++){
        int matricula;
        float bonus;
        Aluno *aluno;

        if(scanf("%d %f", &matricula, &bonus) != 2)
            return 1;

        aluno = buscar_aluno_por_matricula(turma, n, matricula);
        if(aluno != NULL){
            aplicar_bonus(aluno, bonus);
            printf("APLICADO %d %.2f\n", matricula, bonus);
        }else{
            printf("NAO_ENCONTRADO %d\n", matricula);
        }
    }

    printf("RELATORIO FINAL\n");
    for(int i = 0; i < n; i++){
        SituacaoAluno situacao = aluno_situacao(&turma[i]);
        aluno_imprimir_linha(&turma[i]);

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
