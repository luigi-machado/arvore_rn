#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

typedef enum {VERMELHO, PRETO} cor_t;

typedef int item_t ;

typedef struct estrutura {
    cor_t cor;
    item_t dados;
    struct estrutura *direita;
    struct estrutura *esquerda;
    struct estrutura *pai;
} NO;

typedef struct arvore_rn {
    NO *raiz;
} arvore_rn;

cor_t corNO(NO *node);

bool trocaCor(NO *node);
