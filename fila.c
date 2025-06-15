#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fila.h"

Fila* cria_fila() {
    Fila* fila = (Fila*) malloc(sizeof(Fila));
    fila->primeiro = NULL;
    fila->ultimo = NULL;
    return fila;
}

void insere_fila(Fila* fila, int senha, char* nome) {
    // Criando novo nó
    FilaNo* novo_no = (FilaNo*) malloc(sizeof(FilaNo));
    novo_no->senha = senha;
    strncpy(novo_no->nome, nome, sizeof(novo_no->nome) - 1);
    novo_no->proximo = NULL;

    if (fila->ultimo) {
        fila->ultimo->proximo = novo_no;
    } else {
        fila->primeiro = novo_no;
    }
    
    //atualizando ultimo nó
    fila->ultimo = novo_no;
}

void remove_fila(Fila* fila) {
    if(fila->primeiro == NULL) {
        return;
    } else {
        FilaNo* aux = fila->primeiro;
        fila->primeiro = fila->primeiro->proximo;
        free(aux);
        if (fila->primeiro == NULL) {
            fila->ultimo = NULL;
        }
    }
}

void limpa_fila(Fila* fila) {
    FilaNo* no = fila->primeiro;

    while(no!=NULL) {
        FilaNo* aux = no;
        no = no->proximo;
        printf("\nLimpando senha %d - %s", aux->senha, aux->nome);
        free(aux);
    }

    free(fila);
}
