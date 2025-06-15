#ifndef FILA_H
#define FILA_H

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

Fila* cria_fila();
void insere_fila(Fila* fila, int senha, char* nome);
void remove_fila(Fila* fila);
void limpa_fila(Fila* fila);

#endif
