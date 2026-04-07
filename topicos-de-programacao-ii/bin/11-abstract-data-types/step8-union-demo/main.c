#include <stdio.h>

#define MAX_REGISTROS 100

typedef enum {
    TIPO_INT,
    TIPO_FLOAT,
    TIPO_TEXTO
} TipoRegistro;

typedef union {
    int inteiro;
    float real;
    char texto[40];
} Dado;

typedef struct {
    char nome[30];
    TipoRegistro tipo;
    Dado valor;
} Registro;

int ler_registro(Registro *r){
    char tipo_char;

    if(scanf(" %c", &tipo_char) != 1)
        return 0;

    if(tipo_char == 'I'){
        r->tipo = TIPO_INT;
        return scanf("%29s %d", r->nome, &r->valor.inteiro) == 2;
    }

    if(tipo_char == 'F'){
        r->tipo = TIPO_FLOAT;
        return scanf("%29s %f", r->nome, &r->valor.real) == 2;
    }

    if(tipo_char == 'T'){
        r->tipo = TIPO_TEXTO;
        return scanf("%29s %39s", r->nome, r->valor.texto) == 2;
    }

    return 0;
}

void imprimir_registro(const Registro *r){
    switch(r->tipo){
        case TIPO_INT:
            printf("%s I %d\n", r->nome, r->valor.inteiro);
            break;
        case TIPO_FLOAT:
            printf("%s F %.2f\n", r->nome, r->valor.real);
            break;
        case TIPO_TEXTO:
            printf("%s T %s\n", r->nome, r->valor.texto);
            break;
    }
}

int main(void){
    Registro registros[MAX_REGISTROS];
    int n;
    int qtd_int = 0;
    int qtd_float = 0;
    int qtd_texto = 0;

    if(scanf("%d", &n) != 1 || n < 1 || n > MAX_REGISTROS)
        return 1;

    for(int i = 0; i < n; i++){
        if(!ler_registro(&registros[i]))
            return 1;
    }

    printf("LISTA DE REGISTROS\n");
    for(int i = 0; i < n; i++){
        imprimir_registro(&registros[i]);
        switch(registros[i].tipo){
            case TIPO_INT:
                qtd_int++;
                break;
            case TIPO_FLOAT:
                qtd_float++;
                break;
            case TIPO_TEXTO:
                qtd_texto++;
                break;
        }
    }

    printf("Inteiros: %d\n", qtd_int);
    printf("Floats: %d\n", qtd_float);
    printf("Textos: %d\n", qtd_texto);

    return 0;
}
