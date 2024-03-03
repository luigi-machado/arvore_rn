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


bool ehFolha(NO *node) {
    return (node->direita == NULL && node->esquerda == NULL);
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
        //printf("pai: %d\n", novoNo->pai->dados);
        return true;
    } else if (chave < pai->dados) {
        pai->esquerda = novoNo;
        novoNo->pai = pai;
        //printf("pai: %d\n", novoNo->pai->dados);
        return true;
    }
    
    return false;
}


void balancearArvore(arvore_rn *arvore) {
    return;
}


bool inserir(arvore_rn *arvore, item_t chave) {
    if (inserir_bin(arvore, chave)) {
        balancearArvore(arvore);
        return true;
    }
    return false;
}


static void trocarChave(NO *atual, NO *novo) {
    //printf("chave: %d\n ", atual->dados);
    atual->dados = novo->dados;
    //printf("nova chave: %d\n ", atual->dados);
}


bool remover_bin(arvore_rn *arvore, item_t chave) {
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
    if (remover_bin(arvore, chave)) {
        balancearArvore(arvore);
        return true;
    }
    return false;
}


void inorder(NO *raiz) {
    if (raiz != NULL) {
        inorder(raiz->esquerda);
        printf("%d ", raiz->dados);
        inorder(raiz->direita);
    }
}
