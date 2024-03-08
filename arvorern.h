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
static bool trocaCor(NO *node); 

// Aloca um novo nó a ser inserido
static NO* criarNo(item_t dados, NO* pai);

// Retorna true se o nó não tiver nenhum filho, caso contrario retorna false.
bool ehFolha(NO* node);

// Busca o menor nó a direita 
NO* sucessorImediato(NO* node);

// Deixa a arvore pronta para uso;
void inicializar(arvore_rn *arvore);

// Busca um nó na arvore e retorna um ponteiro para ele.
// Caso não encontre retorna NULL
NO* encontrarNO(arvore_rn *arvore, item_t chave);

static void rotacaoDireita(arvore_rn *arvore, NO* pivo);

static void rotacaoEsquerda(arvore_rn *arvore, NO* pivo);

static void balancearInsercao(arvore_rn *arvore, NO* novoNO);

static void balancearRemocao(arvore_rn *arvore);

// Inserção padrão de arvore binária de busca;
// FUNÇÃO ANTIGA
static NO* old_inserir_bin(NO *raiz, item_t chave);

// Inserção padrão de arvore binária de busca 
static bool inserir_abb(arvore_rn *arvore, item_t chave, NO** novoNO_ptr);

// Insere o nó na arvore_rn e realiza o balanceamento.
// Retorna false caso ocorra algum erro na inserção ou se o nó já estiver presente.
bool inserir(arvore_rn *arvore, item_t chave);

// Muda o valor da chave de um nó para o valor de outro 
static void trocarChave(NO* atual, NO* novo);

// Remoção padrao de arvore binaria de busca
static bool remover_bin(arvore_rn *arvore, item_t chave);

// Remove o nó da arvore_rn e realiza o balanceamento.
// Retorna true caso remova e false se o nó não for encontrado
bool remover(arvore_rn *arvore, item_t chave);

// Realiza navegação inOrder, exibindo todos os elementos da arvore.
// Função base para a inorder 
static void inorder_base(NO* raiz);

// Realiza navegação inOrder, exibindo todos os elementos da arvore 
void inorder(arvore_rn *arvore);

void imprimePorNivel(arvore_rn *arvore);

#endif // ARVORE_RN 
