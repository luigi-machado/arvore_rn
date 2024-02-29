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

// Retorna a cor do nó
cor_t corDoNO(NO *node); 

// Dado um nó, realiza a mudança de cor.
// Caso seja Preto altera para vermelho,
// caso seja vermelho altera para preto
bool trocaCor(NO *node); 

// Aloca um novo nó a ser inserido
NO* criarNo(item_t dados, NO* pai);

// Realiza a inserção do nó
NO* inserir(NO *raiz, item_t chave);

void balancearArvore(arvore_rn *arvore);

void adicionarNo(arvore_rn *arvore, item_t chave);
