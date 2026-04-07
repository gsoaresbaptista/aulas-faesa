#ifndef ALUNO_H
#define ALUNO_H

typedef enum {
    REPROVADO_NOTA,
    RECUPERACAO,
    APROVADO,
    REPROVADO_FALTA
} SituacaoAluno;

typedef struct {
    char cidade[30];
    char estado[3];
} Localidade;

typedef struct {
    char nome[50];
    int matricula;
    float notas[3];
    int faltas;
    Localidade local;
} Aluno;

int ler_aluno(Aluno *a);
float aluno_media(const Aluno *a);
SituacaoAluno aluno_situacao(const Aluno *a);
const char *situacao_texto(SituacaoAluno situacao);
void aluno_imprimir_linha(const Aluno *a);
Aluno *buscar_aluno_por_matricula(Aluno turma[], int n, int matricula);
void aplicar_bonus(Aluno *a, float bonus);

#endif
