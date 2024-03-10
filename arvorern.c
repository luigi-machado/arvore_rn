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


// Dado um nó, realiza a mudança de sua cor 
static bool trocaCor(NO *node) {
    if (node == NULL)
        return false;
    node->cor = !node->cor;
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


NO* predecessorImediato(NO* node) {
    NO* aux = node->esquerda;
    while (aux->direita != NULL) {
        aux = aux->direita;
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


NO* encontrarIrmao(NO* node) {
    if (ehFilhoDireito(node))
        return node->pai->esquerda; 
    else
        return node->pai->direita;
}


// SOCORRO
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
    else if (ehFilhoEsquerdo(pivo))
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

    // Atualiza o pai do filho esquerdo do filho direito do pivo
    if (aux->esquerda != NULL)
        aux->esquerda->pai = pivo;  // Filho esquerdo do filho direito do pivo passa a ser filho do pivo

    aux->pai = pivo->pai;
    // Se o pivo for raiz atualiza a raiz para seu filho direito
    if (pivo->pai == NULL)
        arvore->raiz = aux;
    else if (ehFilhoEsquerdo(pivo))
        pivo->pai->esquerda = aux;
    else
        pivo->pai->direita = aux;

    aux->esquerda = pivo;
    pivo->pai = aux;
}


static void balancearInsercao(arvore_rn *arvore, NO* novoNO) {
    // Se o pai do novo nó for preto a arvore já está balanceada
    while (corDoNO(novoNO) == VERMELHO && corDoNO(novoNO->pai) == VERMELHO) {
        NO* pai = novoNO->pai;
        NO* avo = pai->pai;
        NO* tio = encontraTio(novoNO);
        // Tio é filho direito
        if (ehFilhoEsquerdo(pai)) {
            // Caso 1: o pai e o tio são vermelhos
            if (corDoNO(tio) == VERMELHO) {
                avo->cor = VERMELHO; 
                pai->cor = PRETO; // Muda a cor do avô e dos tios
                tio->cor = PRETO;
                novoNO = avo; // Segue a verificação subindo pelo avô
            // Caso 2: o pai é Vermelho e o tio é Negro. Com o pai sendo filho esquerdo
            } else {
                if (ehFilhoDireito(novoNO)) { // Caso 2.  rotação dupla a Direita
                    rotacaoEsquerda(arvore, pai);
                    novoNO = pai;
                    pai = novoNO->pai; // Atualiza as referencias para o novo 'novoNo'
                }
                rotacaoDireita(arvore, avo); // Se não entrou no if executa Rotação simples a direita
                avo->cor = VERMELHO;
                pai->cor = PRETO;
                novoNO = pai;
            }

        } else {
            if (corDoNO(tio) == VERMELHO) {
                avo->cor = VERMELHO; 
                pai->cor = PRETO; // Muda a cor do avô e dos tios
                tio->cor = PRETO;
                novoNO = avo;
            } else {
                if (ehFilhoEsquerdo(novoNO)) {
                    rotacaoDireita(arvore, pai);
                    novoNO = pai;
                    pai = novoNO->pai; // atualiza as referencias para o novo 'novoNo'
                }
                rotacaoEsquerda(arvore, avo);
                avo->cor = VERMELHO;
                pai->cor = PRETO;
                novoNO = pai;
            }
        }
    
    }
    arvore->raiz->cor = PRETO;
}


static void balancearRemocao(arvore_rn *arvore, NO* node) {
    while (node != arvore->raiz && corDoNO(node) == PRETO) {
        NO* irmao = encontrarIrmao(node);
        //NO* pai = node->pai;
        if (ehFilhoEsquerdo(node)) { // Irmao é direito
            if (corDoNO(irmao) == VERMELHO) {
                irmao->cor = PRETO;
                node->pai->cor = VERMELHO;
                rotacaoEsquerda(arvore, node->pai);
                irmao = encontrarIrmao(node);
            } else if (corDoNO(irmao->direita) == PRETO && corDoNO(irmao->esquerda) == PRETO) {
                irmao->cor = VERMELHO;
                node = node->pai;
            } else {
                if (corDoNO(irmao->direita) == PRETO) {
                    irmao->esquerda->cor = PRETO;
                    irmao->cor = VERMELHO;
                    rotacaoDireita(arvore, irmao);
                    irmao = encontrarIrmao(node);
                }
                irmao->cor = irmao->pai->cor;
                node->pai->cor = PRETO;
                irmao->direita->cor = PRETO;
                rotacaoEsquerda(arvore, node->pai);
                node = arvore->raiz;
            }
        } else {
            if (corDoNO(irmao) == VERMELHO) {
                irmao->cor = PRETO;
                node->pai->cor = VERMELHO;
                rotacaoDireita(arvore, node->pai);
                irmao = encontrarIrmao(node);
            } else if (corDoNO(irmao->direita) == PRETO && corDoNO(irmao->esquerda) == PRETO) {
                irmao->cor = VERMELHO;
                node = node->pai;
            } else {
                if (corDoNO(irmao->esquerda) == PRETO) {
                    irmao->direita->cor = PRETO;
                    irmao->cor = VERMELHO;
                    rotacaoEsquerda(arvore, irmao);
                    irmao = encontrarIrmao(node);
                }
                irmao->cor = irmao->pai->cor;
                node->pai->cor = PRETO;
                irmao->esquerda->cor = PRETO;
                rotacaoDireita(arvore, node->pai);
                node = arvore->raiz;
            }
        }
    }
    node->cor = PRETO;
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


// Funcao padrao de remocao da arvore binaria.
// Dá preferencia ao Predecessor
static bool remover_abb(arvore_rn *arvore, item_t chave, NO** substituto, cor_t *cor_remov) {
    NO* atual = encontrarNO(arvore, chave);
    if (atual == NULL) // Retorna false se a chave não for encontrada
        return false;

    NO* pai = atual->pai;

    if (ehFolha(atual)) {
        if (pai == NULL) 
            arvore->raiz = NULL;
        else if (atual->dados > pai->dados)
            pai->direita = NULL;
        else
            pai->esquerda = NULL;
        
        *cor_remov = atual->cor;
        free(atual);
        *substituto = NULL; 
        return true;

    } else if (atual->esquerda != NULL){
        NO* pred = predecessorImediato(atual);
        // Se o predecessor nao é uma folha basta apenas reajustar 
        // o ponteiro do pai após fazer a troca de chave
        if (!ehFolha(pred)) 
            pred->pai->esquerda = pred->esquerda; // predecessor nunca terá um filho direito
        else { 
            if (ehFilhoDireito(pred)) { // Se for uma folha remove-se a referencia do filho correspondente
                pred->pai->direita = NULL;
            } else {
                pred->pai->esquerda = NULL;
            }
        }
        trocarChave(atual, pred);
        *cor_remov = pred->cor;
        free(pred);
        *substituto = atual;
        return true;

    } else if (atual->direita != NULL){
        NO* aux = atual->direita;
        trocarChave(atual, aux);
        atual->direita = aux->direita;
        atual->esquerda = aux->esquerda;
        *cor_remov = aux->cor;
        free(aux);
        *substituto = atual;
        return true; 

    } 
    return false;
}



bool remover(arvore_rn *arvore, item_t chave) {
    NO* substituto;
    cor_t cor_removido;
    if (remover_abb(arvore, chave, &substituto, &cor_removido)) {
        if (cor_removido == PRETO)
            balancearRemocao(arvore, substituto);
        return true;
    }
    return false;
}


bool vazia(NO* node) {
    if (node == NULL || node->esquerda == NULL && node->direita == NULL)
        return true;
    return false;
}


static void destruir_base(NO* node) {
    if (!vazia(node)) {
        destruir_base(node->esquerda);
        destruir_base(node->direita);
        free(node);
    }

}


void destruir(arvore_rn *arvore) {
    destruir_base(arvore->raiz);
    arvore->raiz = NULL;
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
    if (vazia(arvore->raiz)) {
        printf("VAZIA\n");
        return;
    }
    int h = altura(arvore->raiz);
    for (int i = 1; i <= h; i++) {
        imprime_nivel(arvore->raiz, i);
        printf("\n");
    }
    printf("\n");

}
