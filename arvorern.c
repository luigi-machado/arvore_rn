#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "arvorern.h"

cor_t corDoNO(NO *node) {
    if (node == NULL)
        return PRETO;
    return node->cor;
}


// Dado um nó, realiza a mudança de cor dele e de seus filhos.
// Caso seja Preto altera para vermelho,
// caso seja vermelho altera para preto
static bool trocaCor(NO *node) {
    if (node == NULL)
        return false;
    node->cor = !node->cor;
    if (node->esquerda != NULL)
        node->esquerda->cor = !node->esquerda->cor;
    if (node->direita != NULL)
        node->direita->cor = !node->direita->cor;
    return true;
}


// Aloca um novo nó a ser inserido
static NO* criarNo(item_t dados, NO *pai) {
    NO *novoNo = malloc(sizeof(NO));
    novoNo->cor = VERMELHO;
    novoNo->dados = dados;
    novoNo->pai = pai;
    novoNo->direita = NULL;
    novoNo->esquerda = NULL;

    return novoNo;
}


bool ehFolha(NO *node) {
    return (node->direita == NULL && node->esquerda == NULL);
}


static bool ehFilhoDireito(NO *node) {
    if (node == node->pai->direita)
        return true;
    return false;
}


static bool ehFilhoEsquerdo(NO *node) {
    if (node == node->pai->esquerda)
        return true;
    return false;
}


NO* sucessorImediato(NO* node) {
    NO *aux = node->direita;
    while (aux->esquerda != NULL) {
         aux = aux->esquerda;
    }
    return aux;
}


void inicializar(arvore_rn *arvore) {
    arvore->raiz = NULL;
}


NO* encontrarNO(arvore_rn *arvore, item_t chave) {
    NO *atual = arvore->raiz;
    while (atual != NULL) {
        if (atual->dados == chave)
            return atual;
        else if (chave > atual->dados)
            atual = atual->direita;
        else if (chave < atual->dados)
            atual = atual->esquerda;
    }
    return atual;
}

// Retorna um apontador para o tio do nó 
NO* encontraTio(NO* node) {
    NO* pai = node->pai;
    if (pai == NULL)
        return NULL;
    NO* avo = pai->pai;
    if (avo == NULL)
        return NULL;
    NO* tio = NULL;
    if (pai->dados > avo->dados)
        tio = avo->esquerda;
    else
        tio = avo->direita;
    return tio;
}


// Deus por favor me explica o que ta acontecendo aqui
static void rotacaoDireita(arvore_rn *arvore, NO *pivo) {
    NO *aux = pivo->esquerda;
    pivo->esquerda = aux->direita;

    // Atualiza o pai do filho direito do filho esquerdo do pivo
    if (aux->direita != NULL)
        aux->direita->pai = pivo;  // Filho direito do filho esquerdo do pivo passa a ser filho do pivo

    aux->pai = pivo->pai;
    // Se o pivo for raiz atualiza a raiz para seu filho esquerdo
    if (pivo->pai == NULL)
        arvore->raiz = aux;
    else if (ehFilhoDireito(pivo))
        pivo->pai->esquerda = aux;
    else
        pivo->pai->direita = aux;

    aux->direita = pivo;
    pivo->pai = aux;
}


// Apenas Deus sabe como isso funciona direito
static void rotacaoEsquerda(arvore_rn *arvore, NO *pivo) {
    NO *aux = pivo->direita;
    pivo->direita = aux->esquerda;

    // Atualiza o pai do filho direito do filho esquerdo do pivo
    if (aux->esquerda != NULL)
        aux->esquerda->pai = pivo;  // Filho direito do filho esquerdo do pivo passa a ser filho do pivo

    aux->pai = pivo->pai;
    // Se o pivo for raiz atualiza a raiz para seu filho esquerdo
    if (pivo->pai == NULL)
        arvore->raiz = aux;
    else if (ehFilhoEsquerdo(pivo))
        pivo->pai->direita = aux;
    else
        pivo->pai->esquerda = aux;

    aux->direita = pivo;
    pivo->pai = aux;

}


static void balancearInsercao(arvore_rn *arvore, NO* novoNO) {
    if (novoNO != arvore->raiz)
        printf("nNo: %d | pai: %d\n", novoNO->dados, novoNO->pai->dados);
    if (corDoNO(novoNO->pai) == PRETO) // Se o pai do novo nó for preto a arvore já está balanceada
        return;

    NO* pai = novoNO->pai;
    NO* avo = pai->pai;
    NO* tio = encontraTio(novoNO);

    // Caso 1: o pai e o tio são vermelhos 
    if (corDoNO(tio) == VERMELHO) {
        trocaCor(avo); // RECURSAO DEU MERDA
        balancearInsercao(arvore, avo);
    } 

    arvore->raiz->cor = PRETO;
    return;
}


static void balancearRemocao(arvore_rn *arvore) {
    return;
}


static NO* old_inserir_bin(NO* raiz, item_t chave) {
    if (raiz == NULL)
        return criarNo(chave, NULL);
    if (chave > raiz->dados) {
        raiz->direita = old_inserir_bin(raiz->direita, chave);
    }
    else if (chave < raiz->dados) {
        raiz->esquerda = old_inserir_bin(raiz->esquerda, chave);
    }
    return raiz;
}


// Inserção padrão de arvore binária de busca 
static bool inserir_abb(arvore_rn *arvore, item_t chave, NO** novoNO_ptr) {
    NO* novoNo = criarNo(chave, NULL);
    *novoNO_ptr = novoNo;
    if (novoNo == NULL)
        return false;
    NO* atual = arvore->raiz;
    NO* pai = NULL;
    
    // Encontra a posição de inserção do nó 
    while (atual != NULL) {
        pai = atual;
        if (atual->dados == chave) // Se o nó atual tiver chave igual a de inserção
            return false;          // O nó não será inserido e a função retorna false
        else if (chave > atual->dados)
            atual = atual->direita;
        else if (chave < atual->dados)
            atual = atual->esquerda;
    }
    
    if (pai == NULL) { // Se o pai for NULL o nó deve ser inserido na raiz
        arvore->raiz = novoNo;
        return true;
    } else if (chave > pai->dados) {
        pai->direita = novoNo;
        novoNo->pai = pai;
        return true;
    } else if (chave < pai->dados) {
        pai->esquerda = novoNo;
        novoNo->pai = pai;
        return true;
    }
    
    return false;
}


bool inserir(arvore_rn *arvore, item_t chave) {
    NO* novo;
    if (inserir_abb(arvore, chave, &novo)) {
        balancearInsercao(arvore, novo);
        return true;
    }
    return false;
}


// Muda o valor da chave de um nó para o valor de outro 
static void trocarChave(NO *atual, NO *novo) {
    atual->dados = novo->dados;
}


// Remoção padrao de arvore binaria de busca
static bool remover_abb(arvore_rn *arvore, item_t chave) {
    NO* atual = encontrarNO(arvore, chave);
    NO* pai = atual->pai;
    if (atual == NULL) // Retorna false se a chave não for encontrada
        return false;

    if (ehFolha(atual)) {
        if (pai == NULL) 
            arvore->raiz = NULL;
        else if (atual->dados > pai->dados)
            pai->direita = NULL;
        else
            pai->esquerda = NULL;
        
        free(atual);
        return true;

    } else if (atual->direita != NULL){
        NO* sucessor = sucessorImediato(atual);
        // Se o sucessor nao é uma folha basta apenas reajustar 
        // o ponteiro do pai após fazer a troca de chave
        if (!ehFolha(sucessor)) 
            sucessor->pai->direita = sucessor->direita; // Nunca terá um filho esquerdo
        else { 
            if (sucessor->dados > sucessor->pai->dados) {
                sucessor->pai->direita = NULL;
            } else {
                sucessor->pai->esquerda = NULL;
            }
        }
        trocarChave(atual, sucessor);
        free(sucessor);
        return true;

    } else if (atual->esquerda != NULL){
        trocarChave(atual, atual->esquerda);
        atual->direita = atual->esquerda->direita;
        atual->esquerda = atual->esquerda->esquerda;
        return true; 

    } 
    return false;
}


bool remover(arvore_rn *arvore, item_t chave) {
    if (remover_abb(arvore, chave)) {
        balancearRemocao(arvore);
        return true;
    }
    return false;
}


// Realiza navegação inOrder, exibindo todos os elementos da arvore.
// Função base para a inorder 
static void inorder_base(NO *raiz) {
    if (raiz != NULL) {
        inorder_base(raiz->esquerda);
        printf("%d ", raiz->dados);
        inorder_base(raiz->direita);
    }
}


void inorder(arvore_rn *arvore) {
    inorder_base(arvore->raiz);
}


static int altura(NO* node) {
    if (node == NULL)
        return 0;
    
    int altura_direita = altura(node->direita);
    int altura_esquerda = altura(node->esquerda);

    if (altura_direita > altura_esquerda)
        return altura_direita + 1;
    else
        return altura_esquerda + 1;;
}

static void imprime_nivel(NO* raiz, int nivel) {
    if (raiz == NULL)
        return;
    if (nivel == 1) {
        printf("%d ", raiz->dados);
        if (raiz->cor == VERMELHO)
            printf("V | ");
        else
            printf("P | ");
    }
    else if (nivel > 1) {
        imprime_nivel(raiz->esquerda, nivel - 1);
        imprime_nivel(raiz->direita, nivel - 1);
    }
}

void imprimePorNivel(arvore_rn *arvore) {
    int h = altura(arvore->raiz);
    for (int i = 1; i <= h; i++) {
        /*int n = (h - i);
        for (int j = 0; j < n; j++) {
            printf(" ");
        }*/
        imprime_nivel(arvore->raiz, i);
        printf("\n");
    }
    printf("\n");

}
