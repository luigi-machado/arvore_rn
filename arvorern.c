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


bool trocaCor(NO *node) {
    if (node == NULL)
        return false;
    node->cor = !node->cor;
    if (node->esquerda != NULL)
        node->esquerda->cor = !node->esquerda->cor;
    if (node->direita != NULL)
        node->direita->cor = !node->direita->cor;
    return true;
}


NO* criarNo(item_t dados, NO *pai) {
    NO *novoNo = malloc(sizeof(NO));
    novoNo->cor = VERMELHO;
    novoNo->dados = dados;
    novoNo->pai = pai;
    novoNo->direita = NULL;
    novoNo->esquerda = NULL;

    return novoNo;
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


static bool inserir_bin(arvore_rn *arvore, item_t chave) {
    NO* novoNo = criarNo(chave, NULL);
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


void balancearArvore(arvore_rn *arvore) {
    return;
}


void inserir(arvore_rn *arvore, item_t chave) {
    inserir_bin(arvore, chave);
    balancearArvore(arvore);
}


void trocarChave(NO *atual, NO *novo) {
    atual->dados = novo->dados;
}


bool remover_bin(arvore_rn *arvore, item_t chave) {
    NO* atual = encontrarNO(arvore, chave);
    if (atual == NULL) // Retorna false se a chave não for encontrada
        return false;
}


void inorder(NO *raiz) {
    if (raiz != NULL) {
        inorder(raiz->esquerda);
        printf("%d ", raiz->dados);
        inorder(raiz->direita);
    }
}
