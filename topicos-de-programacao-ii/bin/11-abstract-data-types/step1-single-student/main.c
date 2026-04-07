#include <stdio.h>

float calcular_media(float nota1, float nota2, float nota3){
    return (nota1 + nota2 + nota3) / 3.0f;
}

const char *classificar_media(float media){
    if(media >= 7.0f)
        return "APROVADO";
    if(media >= 5.0f)
        return "RECUPERACAO";
    return "REPROVADO";
}

int main(void){
    char nome[50];
    int matricula;
    float nota1, nota2, nota3;
    float media;

    if(scanf("%49s %d %f %f %f", nome, &matricula, &nota1, &nota2, &nota3) != 5)
        return 1;

    media = calcular_media(nota1, nota2, nota3);

    printf("Nome: %s\n", nome);
    printf("Matricula: %d\n", matricula);
    printf("Media: %.2f\n", media);
    printf("Situacao: %s\n", classificar_media(media));

    return 0;
}
