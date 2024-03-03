#include <stdlib.h>
#include <stdbool.h>

#ifndef ARVORE_RN
#define ARVORE_RN

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

static bool ehFolha(NO* node);

NO* sucessorImediato(NO* node);

void inicializar(arvore_rn *arvore);

NO* encontrarNO(arvore_rn *arvore, item_t chave);

// Inserção padrão de arvore binária de busca;
// FUNÇÃO ANTIGA
static NO* old_inserir_bin(NO *raiz, item_t chave);

// Inserção padrão de arvore binária de busca
static bool inserir_bin(arvore_rn *arvore, item_t chave);

void inserir(arvore_rn *arvore, item_t chave);

static void trocarChave(NO* atual, NO* novo);

// Remoção padrao de arvore binaria de busca
bool remover_bin(arvore_rn *arvore, item_t chave);

void remover(arvore_rn *arvore, item_t chave);

void balancearArvore(arvore_rn *arvore);

#endif // ARVORE_RN 
