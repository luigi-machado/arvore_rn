#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

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
static NO *criarNo(item_t dados, NO *pai) {
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

// Usa o apontador para o pai pois em algumas circunstancias um nó nulo pode ser
// passado Dessa forma o programa não crahsa
static bool ehFilhoDireito(NO *node, NO *pai) {
  if (node == pai->direita)
    return true;
  return false;
}

// Usa o apontador para o pai pois em algumas circunstancias um nó nulo pode ser
// passado Dessa forma o programa não crahsa
static bool ehFilhoEsquerdo(NO *node, NO *pai) {
  if (node == pai->esquerda)
    return true;
  return false;
}

// Retorna o menor elemento a direita
NO *sucessorImediato(NO *node) {
  NO *aux = node->direita;
  while (aux->esquerda != NULL) {
    aux = aux->esquerda;
  }
  return aux;
}

// Retorna o maior elemento a esquerda
NO *predecessorImediato(NO *node) {
  NO *aux = node->esquerda;
  while (aux->direita != NULL) {
    aux = aux->direita;
  }
  return aux;
}

// Deixa a arvore pronta para uso
void inicializar(arvore_rn *arvore) { arvore->raiz = NULL; }

// Realiza busca binária pelo nó
// Caso nao encontre retorna NULL
NO *encontrarNO(arvore_rn *arvore, item_t chave) {
  NO *atual = arvore->raiz;
  while (atual != NULL) {
    if (atual->dados == chave)
      return atual;
    else if (chave > atual->dados)
      atual = atual->direita;
    else if (chave < atual->dados)
      atual = atual->esquerda;
  }
  return NULL;
}

// Retorna um apontador para o tio do nó
NO *encontraTio(NO *node) {
  NO *pai = node->pai;
  if (pai == NULL) // Se o nó não possui pai também nao possui tio
    return NULL;
  NO *avo = pai->pai;
  if (avo == NULL)
    return NULL;
  NO *tio = NULL;
  if (pai->dados > avo->dados)
    tio = avo->esquerda;
  else
    tio = avo->direita;
  return tio;
}

// Retorna um apontador para o irmão do nó passado
NO *encontrarIrmao(NO *node, NO *pai) {
  if (ehFilhoDireito(node, pai))
    return pai->esquerda;
  else
    return pai->direita;
}

// Executa rotacao a direita na arvore
static void rotacaoDireita(arvore_rn *arvore, NO *pivo) {
  NO *aux = pivo->esquerda;
  pivo->esquerda = aux->direita;

  // Atualiza o pai do filho direito do filho esquerdo do pivo
  if (aux->direita != NULL)
    aux->direita->pai = pivo; // Filho direito do filho esquerdo do pivo passa a
                              // ser filho do pivo

  aux->pai = pivo->pai;
  // Se o pivo for raiz atualiza a raiz para seu filho esquerdo
  if (pivo->pai == NULL)
    arvore->raiz = aux;
  else if (ehFilhoEsquerdo(pivo, pivo->pai))
    pivo->pai->esquerda = aux;
  else
    pivo->pai->direita = aux;

  aux->direita = pivo;
  pivo->pai = aux;
}

// Executa rotacao a esquerda na arvore
static void rotacaoEsquerda(arvore_rn *arvore, NO *pivo) {
  NO *aux = pivo->direita;
  pivo->direita = aux->esquerda;

  // Atualiza o pai do filho esquerdo do filho direito do pivo
  if (aux->esquerda != NULL)
    aux->esquerda->pai = pivo; // Filho esquerdo do filho direito do pivo passa
                               // a ser filho do pivo

  aux->pai = pivo->pai;
  // Se o pivo for raiz atualiza a raiz para seu filho direito
  if (pivo->pai == NULL)
    arvore->raiz = aux;
  else if (ehFilhoEsquerdo(pivo, pivo->pai))
    pivo->pai->esquerda = aux;
  else
    pivo->pai->direita = aux;

  aux->esquerda = pivo;
  pivo->pai = aux;
}

static void balancearInsercao(arvore_rn *arvore, NO *novoNO) {
  // Se o pai do novo nó for preto a arvore já está balanceada
  while (corDoNO(novoNO) == VERMELHO && corDoNO(novoNO->pai) == VERMELHO) {
    NO *pai = novoNO->pai;
    NO *avo = pai->pai;
    NO *tio = encontraTio(novoNO);

    if (ehFilhoEsquerdo(pai, avo)) { // Checa se o pai é filho esquerdo do avô
      // Caso 1: o pai e o tio são vermelhos
      if (corDoNO(tio) == VERMELHO) {
        avo->cor = VERMELHO;
        pai->cor = PRETO; // Muda a cor do avô e dos tios
        tio->cor = PRETO;
        novoNO = avo; // Segue a verificação subindo pelo avô

        // Caso 2: o pai é Vermelho e o tio é Negro. Com o pai sendo filho
        // esquerdo
      } else {
        if (ehFilhoDireito(novoNO,
                           novoNO->pai)) { // Caso 2.1 novoNo é filho direito
          rotacaoEsquerda(arvore, pai);    // rotação dupla EsquerdaDireita
          novoNO = pai;
          pai = novoNO->pai; // Atualiza as referencias para o novo 'novoNo'
        }
        rotacaoDireita(
            arvore,
            avo); // Se não entrou no if executa Rotação simples a direita
        avo->cor = VERMELHO;
        pai->cor = PRETO;
        novoNO = pai; // Segue o balanceamento pelo pai
      }

    } else { // O pai é filho direito do avô
      // Caso 1: O pai e o tio são vermelho
      if (corDoNO(tio) == VERMELHO) {
        avo->cor = VERMELHO;
        pai->cor = PRETO; // Muda a cor do avô e dos tios
        tio->cor = PRETO;
        novoNO = avo;

        // Caso 2: o pai é Vermelho e o tio é Negro. Com o pai sendo filho
        // esquerdo
      } else {
        if (ehFilhoEsquerdo(novoNO,
                            novoNO->pai)) { // Caso 2.1 novoNo é filho esquerdo
          rotacaoDireita(arvore, pai);      // rotação dupla DireitaEsquerda
          novoNO = pai;
          pai = novoNO->pai; // atualiza as referencias para o novo 'novoNo'
        }
        rotacaoEsquerda(arvore, avo); // Caso o nó não seja filho esquerdo faz
                                      // apenas rotação simples
        avo->cor = VERMELHO;          // a Esquerda
        pai->cor = PRETO;
        novoNO = pai; // Segue o balanceamento pelo pai
      }
    }
  }
  arvore->raiz->cor = PRETO; // Força a raiz a ser preta
}

// node é o nó que substituiu o nó removido
static void balancearRemocao(arvore_rn *arvore, NO *node, NO *substituto_pai) {
  NO *irmao;
  while (node != arvore->raiz && corDoNO(node) == PRETO) {
    // O nó é movido enquanto sua cor for preto ou até que vire a raiz
    irmao = encontrarIrmao(node, substituto_pai);

    if (ehFilhoEsquerdo(node,
                        substituto_pai)) { // Checa se o nó é filho esquerdo
      // Se o nó é filho esquerdo então o irmao é filho direito
      // Caso 1:
      if (corDoNO(irmao) ==
          VERMELHO) { // Checa se a cor do seu irmão é vermelha
        irmao->cor = PRETO;
        substituto_pai->cor = VERMELHO;
        rotacaoEsquerda(arvore,
                        substituto_pai); // Rotação simples com o pai como pivô
        irmao = encontrarIrmao(node,
                               substituto_pai); // Corrige o apontador do irmão

        // Caso 2:
      } else if (corDoNO(irmao->direita) ==
                     PRETO && // O irmão e seus dois filhos são pretos
                 corDoNO(irmao->esquerda) == PRETO) {
        irmao->cor = VERMELHO;
        node = substituto_pai; // Continua o balanceamento subindo pelo pai

        // Caso 3:
      } else {
        if (corDoNO(irmao->direita) ==
            PRETO) { // Checa se o filho direito é preto
          irmao->esquerda->cor = PRETO;
          irmao->cor = VERMELHO;
          rotacaoDireita(
              arvore,
              irmao); // Nesse caso ocorre uma rotação dupla DireitaEsquerda
          irmao = encontrarIrmao(node, substituto_pai);
        }
        irmao->cor =
            irmao->pai->cor; // Caso contrário, apenas rotação a esquerda
        substituto_pai->cor = PRETO;
        irmao->direita->cor = PRETO;
        rotacaoEsquerda(arvore, substituto_pai);
        node = arvore->raiz;
      }
    } else { // O nó é filho direito
      // Se o nó é filho direito então o irmao é filho esquerdo
      // Caso 1:
      if (corDoNO(irmao) ==
          VERMELHO) { // Checa se a cor do seu irmão é vermelha
        irmao->cor = PRETO;
        substituto_pai->cor = VERMELHO;
        rotacaoDireita(arvore,
                       substituto_pai); // Rotação simples com o pai como pivô
        irmao = encontrarIrmao(node,
                               substituto_pai); // Corrige o apontador do irmão

        // Caso 2:
      } else if (corDoNO(irmao->direita) ==
                     PRETO && // O irmão e seus dois filhos são pretos
                 corDoNO(irmao->esquerda) == PRETO) {
        irmao->cor = VERMELHO;
        node = substituto_pai; // Continua o balanceamento subindo pelo pai

        // Caso 3:
      } else {
        if (corDoNO(irmao->esquerda) ==
            PRETO) { // Checa se o filho esquerdo é preto
          irmao->direita->cor = PRETO;
          irmao->cor = VERMELHO;
          rotacaoEsquerda(
              arvore,
              irmao); // Nesse caso ocorre uma rotação dupla EsquerdaDireita
          irmao = encontrarIrmao(node, substituto_pai);
        }
        irmao->cor =
            irmao->pai->cor; // Caso contrário, apenas rotação a Direita
        substituto_pai->cor = PRETO;
        irmao->esquerda->cor = PRETO;
        rotacaoDireita(arvore, substituto_pai);
        node = arvore->raiz;
      }
    }
  }
  node->cor = PRETO;
}

// Inserção padrão de arvore binária de busca
static bool inserir_abb(arvore_rn *arvore, item_t chave, NO **novoNO_ptr) {
  NO *novoNo = criarNo(chave, NULL);
  *novoNO_ptr = novoNo;
  if (novoNo == NULL)
    return false;
  NO *atual = arvore->raiz;
  NO *pai = NULL;

  // Encontra a posição de inserção do nó
  while (atual != NULL) {
    pai = atual;
    if (atual->dados == chave) // Se o nó atual tiver chave igual a de inserção
      return false; // O nó não será inserido e a função retorna false
    else if (chave > atual->dados)
      atual = atual->direita;
    else if (chave < atual->dados)
      atual = atual->esquerda;
  }

  if (pai == NULL) { // Se o pai for NULL o nó deve ser inserido na raiz
    arvore->raiz = novoNo;
    return true;
  } else if (chave > pai->dados) { // Se for maior será inserido na direita
    pai->direita = novoNo;
    novoNo->pai = pai;
    return true;
  } else if (chave < pai->dados) { // Se for menor será inserido na esquerda
    pai->esquerda = novoNo;
    novoNo->pai = pai;
    return true;
  }

  return false;
}

// Realiza a inserção de um nó na arvore, ajustando o balanceamento.
// Caso haja alguma falha na inserção, retorna false.
// Não permite duplicidade
bool inserir(arvore_rn *arvore, item_t chave) {
  NO *novo; // Apontador para o novo nó inserido
  if (inserir_abb(
          arvore, chave,
          &novo)) { // Inserção padrao da ABB retornando a referenia do novo nó
    balancearInsercao(arvore, novo); // Realiza o balanceamento após a inserção
    return true;
  }
  return false;
}

// Muda o valor da chave de um nó para o valor de outro.
// É usado na hora de remover um nó com sub-arvore
static void trocarChave(NO *atual, NO *novo) { atual->dados = novo->dados; }

// Baseada na funcao padrao de remocao da arvore binaria.
// Dá preferencia ao Predecessor
bool remover(arvore_rn *arvore, item_t chave) {
  cor_t cor_remov;
  NO *substituto;
  NO *pai;
  NO *substituto_pai = NULL;
  NO *atual = encontrarNO(arvore, chave);
  if (atual == NULL) // Retorna false se a chave não for encontrada
    return false;

  pai = atual->pai;

  if (ehFolha(atual)) { // Se o nó removido é uma folha, apaga-se sua referencia
                        // no pai
    if (pai == NULL)
      arvore->raiz = NULL;
    else if (atual->dados > pai->dados) // Verifica se é filho direito
      pai->direita = NULL;
    else
      pai->esquerda = NULL;

    cor_remov = atual->cor;
    free(atual);
    substituto = NULL;
    substituto_pai = pai;

  } else if (atual->esquerda != NULL) {
    NO *pred = predecessorImediato(atual);
    // Se o predecessor nao é uma folha basta apenas reajustar
    // o ponteiro do pai após fazer a troca de chave
    if (!ehFolha(pred)) {
      if (ehFilhoDireito(pred, pred->pai))
        pred->pai->direita =
            pred->esquerda; // predecessor nunca terá um filho direito
      else
        pred->pai->esquerda = pred->esquerda;

    } else {
      if (ehFilhoDireito(pred, pred->pai)) {
        // Se for uma folha remove-se a referencia do filho correspondente
        pred->pai->direita = NULL;
      } else { // Pode ser que o predecessor seja o primeiro nó da esquerda
        pred->pai->esquerda = NULL;
      }
    }
    trocarChave(atual, pred);
    cor_remov = pred->cor;
    free(pred);
    substituto = atual;
    substituto_pai = substituto->pai;

  } else if (atual->direita != NULL) {
    NO *aux = atual->direita;
    trocarChave(atual, aux);
    atual->direita = aux->direita;
    atual->esquerda = aux->esquerda;
    cor_remov = aux->cor;
    free(aux);
    substituto = atual;
    substituto_pai = substituto->pai;
  }
  if (cor_remov ==
      PRETO) // Se o balanceamento foi ferido, refaz o balanceamento
    balancearRemocao(arvore, substituto, substituto_pai);
  return true;
}

bool vazio(NO *node) {
  if (node == NULL)
    return true;
  return false;
}

// Função base para desalocar toda a arvore
static void destruir_base(NO *node) {
  if (!vazio(node)) {
    destruir_base(node->esquerda);
    destruir_base(node->direita);
    free(node);
  }
}

// Desaloca todos os nós da arvore e ajusta o ponteiro da raiz.
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

// Realiza navegação inOrder, exibindo todos os elementos da arvore
void inorder(arvore_rn *arvore) { inorder_base(arvore->raiz); }

// Retorna a altura de uma sub-arvore
static int altura(NO *node) {
  if (node == NULL)
    return 0;

  int altura_direita = altura(node->direita);
  int altura_esquerda = altura(node->esquerda);

  if (altura_direita > altura_esquerda)
    return altura_direita + 1;
  else
    return altura_esquerda + 1;
  ;
}

static void imprime_nivel(NO *raiz, int nivel) {
  if (raiz == NULL)
    return;
  if (nivel == 1) {
    printf("%d ", raiz->dados);
    if (raiz->cor == VERMELHO)
      printf("V ");
    else
      printf("P ");
    if (raiz->pai != NULL)
      printf("pai: %d |", raiz->pai->dados);
  } else if (nivel > 1) {
    imprime_nivel(raiz->esquerda, nivel - 1);
    imprime_nivel(raiz->direita, nivel - 1);
  }
}

// Imprime os nós de cada nível da arvore
void imprimePorNivel(arvore_rn *arvore) {
  if (vazio(arvore->raiz)) {
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
