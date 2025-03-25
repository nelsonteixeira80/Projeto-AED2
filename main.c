#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ----------------- Estrutura da Encomenda -----------------
typedef struct {
    int id;
    char nome[100];
    float preco;
} Encomenda;

// ----------------- Lista Ligada -----------------
typedef struct Nodo {
    Encomenda encomenda;
    struct Nodo* prox;
} Nodo;

typedef struct {
    Nodo* inicio;
} ListaLigada;

void inserirEncomenda(ListaLigada* lista, Encomenda nova) {
    Nodo* novo = (Nodo*)malloc(sizeof(Nodo));
    novo->encomenda = nova;
    novo->prox = lista->inicio;
    lista->inicio = novo;
}

void removerEncomenda(ListaLigada* lista, int id) {
    Nodo* atual = lista->inicio;
    Nodo* anterior = NULL;

    while (atual != NULL && atual->encomenda.id != id) {
        anterior = atual;
        atual = atual->prox;
    }

    if (atual == NULL) return;

    if (anterior == NULL) {
        lista->inicio = atual->prox;
    } else {
        anterior->prox = atual->prox;
    }

    free(atual);
}

void listarEncomendasPendentes(ListaLigada* lista) {
    Nodo* atual = lista->inicio;
    while (atual != NULL) {
        printf("ID: %d, Nome: %s, Preço: %.2f\n", atual->encomenda.id, atual->encomenda.nome, atual->encomenda.preco);
        atual = atual->prox;
    }
}

// ----------------- Fila de Expedição -----------------
typedef struct {
    Encomenda* itens;
    int inicio, fim, tamanho, capacidade;
} FilaExpedicao;

void inicializarFila(FilaExpedicao* fila, int capacidade) {
    fila->capacidade = capacidade;
    fila->inicio = fila->fim = fila->tamanho = 0;
    fila->itens = (Encomenda*)malloc(sizeof(Encomenda) * capacidade);
}

void adicionarParaExpedicao(FilaExpedicao* fila, ListaLigada* lista, int id) {
    Nodo* atual = lista->inicio;
    Nodo* anterior = NULL;

    while (atual != NULL && atual->encomenda.id != id) {
        anterior = atual;
        atual = atual->prox;
    }

    if (atual == NULL || fila->tamanho == fila->capacidade) return;

    fila->itens[fila->fim] = atual->encomenda;
    fila->fim = (fila->fim + 1) % fila->capacidade;
    fila->tamanho++;

    // Remover da lista
    if (anterior == NULL) {
        lista->inicio = atual->prox;
    } else {
        anterior->prox = atual->prox;
    }

    free(atual);
}

void removerDaFila(FilaExpedicao* fila, Encomenda* destino) {
    if (fila->tamanho == 0) return;

    *destino = fila->itens[fila->inicio];
    fila->inicio = (fila->inicio + 1) % fila->capacidade;
    fila->tamanho--;
}

void listarFilaExpedicao(FilaExpedicao* fila) {
    int i, index;
    for (i = 0, index = fila->inicio; i < fila->tamanho; i++) {
        Encomenda e = fila->itens[index];
        printf("ID: %d, Nome: %s, Preço: %.2f\n", e.id, e.nome, e.preco);
        index = (index + 1) % fila->capacidade;
    }
}

// ----------------- Pilha de Histórico -----------------
typedef struct Pilha {
    Encomenda* itens;
    int topo, capacidade;
} PilhaHistorico;

void inicializarPilha(PilhaHistorico* pilha, int capacidade) {
    pilha->capacidade = capacidade;
    pilha->topo = -1;
    pilha->itens = (Encomenda*)malloc(sizeof(Encomenda) * capacidade);
}

void adicionarAoHistorico(PilhaHistorico* pilha, Encomenda e) {
    if (pilha->topo == pilha->capacidade - 1) return;
    pilha->itens[++(pilha->topo)] = e;
}

void listarHistorico(PilhaHistorico* pilha, int idMin, int idMax) {
    for (int i = pilha->topo; i >= 0; i--) {
        Encomenda e = pilha->itens[i];
        if (e.id >= idMin && e.id <= idMax) {
            printf("ID: %d, Nome: %s, Preço: %.2f\n", e.id, e.nome, e.preco);
        }
    }
}