/*
    Trabalho 1 - Filas/Pilhas
    Disciplina: Estruturas de Dados 
    Aluno: Lucas Almeida dos Santos
    Matricula: 242039390
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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

//criando variaveis globais p/ facilitar acesso das funcoes 
Fila* fila;
Fila* fila_preferencial;
int senhaChamada;
char nomeChamado[50];

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
        printf("Limpando senha %d - %s", aux->senha, aux->nome);
        free(aux);
    }

    free(fila);
}

void limpa_tela(){
    system("cls");
}

void nova_senha(char nome[]) {
    int senha = 0;
    if(fila->primeiro == NULL) {
        senha = 1;
    } else {
        senha = fila->ultimo->senha + 1;
    }

    printf("\nInserindo senha: %d - paciente: %s..", senha, nome);
    sleep(1);
    insere_fila(fila, senha, nome);
    printf("\nInserido com sucesso!");
    sleep(1);
    limpa_tela();
}

void chamar_senha() {
    if(fila->primeiro == NULL) {
        printf("Ops.. não há pacientes na fila. Tente adicionar antes!");
        sleep(2);
        limpa_tela();
        return;
    }

    FilaNo* proximo_paciente = fila->primeiro;

    senhaChamada = proximo_paciente->senha;
    strcpy(nomeChamado, proximo_paciente->nome);

    printf("\nChamando senha %d - %s..", proximo_paciente->senha, proximo_paciente->nome);
    sleep(1);

    remove_fila(fila);
    printf("\nChamado com sucesso!");
    sleep(1);
    limpa_tela();
}

void status_fila() {
    if(fila->primeiro == NULL) {
        return;
    }

    if(senhaChamada != 0) {
        printf("\nSenha chamada: %d - %s", senhaChamada, nomeChamado);
    }
    printf("\nPróximas senhas: ");

    FilaNo* no = fila->primeiro;
    while(no!=NULL) {
        printf("[ %d ]", no->senha);
        no = no->proximo;
        if(no!=NULL) {
            printf(" -> ");
        }
    }
    printf("\n\n");
}

int main() {
    fila = cria_fila();
    fila_preferencial = cria_fila();

    limpa_tela();

    while(1) {
        printf("*** CLINICA ED ***\n");
        status_fila();
        printf("\n1 - Nova senha");
        printf("\n2 - Nova senha preferencial");
        printf("\n3 - Chamar senha");
        printf("\n4 - Chamar senha preferencial");
        printf("\n5 - Sair");

        int opcao;
        printf("\n\n-------\nEscolha uma opcao > ");
        scanf("%d", &opcao);

        limpa_tela();

        switch(opcao) {
            case 1: {
                char nome[50];
                printf("Digite o nome do paciente: ");
                scanf(" %[^\n]", nome);
                nova_senha(nome);
                break;
            }
            case 2: {
                char nome[50];
                printf("Digite o nome do paciente preferencial: ");
                scanf("%s", nome);
                break;
            }
            case 3: {
                limpa_tela();
                chamar_senha();
                break;
            }
            case 4: {
                limpa_tela();
                break;
            }
            case 5:
                return 0;
            default:
                printf("Opcao invalida!\n");
        }

    }

    return 0;
}