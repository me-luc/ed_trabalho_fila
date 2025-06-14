/*
    Trabalho 1 - Filas/Pilhas
    Disciplina: Estruturas de Dados 
    Aluno: Lucas Almeida dos Santos
    Matricula: 242039390
*/

#include <stdio.h>
#include <stdlib.h>

typedef struct filano FilaNo;
typedef struct fila Fila;

struct fila {
    FilaNo* primeiro;
    FilaNo* ultimo;
};

struct filano {
    int senha;
    char nome[50];
    FilaNo* proximo;
};

Fila* cria_fila() {
    Fila* fila = (Fila*) malloc(sizeof(Fila));
    fila->primeiro = NULL;
    fila->ultimo = NULL;
    return fila;
}

void insere_fila(Fila* fila, int senha, const char* nome) {
    FilaNo* novo_no = (FilaNo*) malloc(sizeof(FilaNo));
    novo_no->senha = senha;
    strncopy(novo_no->nome, nome, sizeof(novo_no->nome) - 1);
    novo_no->proximo = NULL;

    if (fila->ultimo) {
        fila->ultimo->proximo = novo_no;
    } else {
        fila->primeiro = novo_no;
    }
    
    fila->ultimo = novo_no;
}

int main() {
    printf("Hello, World!\n");
    return 0;
}