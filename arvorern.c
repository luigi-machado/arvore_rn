#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "arvorern.h"

cor_t corNO(NO *node) {
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

