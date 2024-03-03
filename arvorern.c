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


static NO* inserir_bin(NO* raiz, item_t chave) {
    if (raiz == NULL)
        return criarNo(chave, NULL);
    if (chave > raiz->dados) {
        raiz->direita = inserir_bin(raiz->direita, chave);
    }
    else if (chave < raiz->dados) {
        raiz->esquerda = inserir_bin(raiz->esquerda, chave);
    }
    return raiz;
}


void balancearArvore(arvore_rn *arvore) {
    return;
}


void inserir(arvore_rn *arvore, item_t chave) {
    if (arvore->raiz == NULL)
        arvore->raiz = inserir_bin(arvore->raiz, chave);
    else
        inserir_bin(arvore->raiz, chave);

    balancearArvore(arvore);
}


void inorder(NO *raiz) {
    if (raiz != NULL) {
        inorder(raiz->esquerda);
        printf("%d ", raiz->dados);
        inorder(raiz->direita);
    }
}
