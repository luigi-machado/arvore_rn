#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
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


NO* inserir(NO* raiz, item_t chave) {
    if (raiz == NULL)
        return criarNo(chave, NULL);
    if (chave > raiz->dados) {
        raiz->direita = inserir(raiz->direita, chave);
    }
    else if (chave < raiz->dados) {
        raiz->esquerda = inserir(raiz->esquerda, chave);
    }
    return raiz;
}


void balancearArvore(arvore_rn *arvore) {
    return;
}


void adicionarNo(arvore_rn *arvore, item_t chave) {
    inserir(arvore->raiz, chave);
    balancearArvore(arvore);
}
