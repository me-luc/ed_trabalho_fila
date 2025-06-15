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

#define RED   "\033[31m"
#define GREEN "\033[32m"
#define RESET "\033[0m"
#define BLUE   "\033[34m"

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
int senha = 1;
int senhaChamada;
char nomeChamado[50];

Fila* cria_fila() {
    Fila* fila = (Fila*) malloc(sizeof(Fila));
    fila->primeiro = NULL;
    fila->ultimo = NULL;
    return fila;
}

void insere_fila(Fila* fila, char* nome) {
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

void nova_senha(char nome[], int preferencial) {
    printf("\nInserindo senha: %d - paciente: %s..", senha, nome);
    sleep(1);
    if(preferencial) {
        insere_fila(fila_preferencial, nome);
    } else {
        insere_fila(fila, nome);
    }
    printf(GREEN "\nInserido com sucesso!" RESET);
    senha++;
    sleep(1);
    limpa_tela();
}

void chamar_senha() {
    if(fila_preferencial->primeiro == NULL && fila->primeiro == NULL) {
        printf(RED "Ops.. não há pacientes em nenhuma das filas.. Tente adicionar antes!" RESET);
        sleep(2);
        limpa_tela();
        return;
    }

    FilaNo* proximo_paciente;
    int chamar_preferencial;

    if(fila_preferencial->primeiro != NULL) {
        proximo_paciente = fila_preferencial->primeiro;
        chamar_preferencial = 1;
    }

    if(chamar_preferencial == 0) {
        proximo_paciente = fila->primeiro;
    }

    senhaChamada = proximo_paciente->senha;
    strcpy(nomeChamado, proximo_paciente->nome);

    printf("\nChamando senha %d - %s..", proximo_paciente->senha, proximo_paciente->nome);
    sleep(1);

    if(chamar_preferencial) {
        remove_fila(fila_preferencial);
    } else {
        remove_fila(fila);
    }

    printf(GREEN "\nChamado com sucesso!" RESET);
    sleep(1);
    limpa_tela();
}

void imprime_fila(Fila* fila) {
    FilaNo* no = fila->primeiro;
    while(no!=NULL) {
        printf("[ %d ]", no->senha);
        no = no->proximo;
        if(no!=NULL) {
            printf(" -> ");
        }
    }
}

void status_fila() {
    if(senhaChamada != 0) {
        printf(BLUE "\nSenha chamada: %d - %s", senhaChamada, nomeChamado);
    }

    if(fila->primeiro != NULL) {
        printf("\nSenhas gerais: ");
        imprime_fila(fila);
    }

    if(fila_preferencial->primeiro != NULL) {
        printf("\nSenhas preferencias: ");
        imprime_fila(fila_preferencial);
    }

    printf("\n\n" RESET);
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
                nova_senha(nome, 0);
                break;
            }
            case 2: {
                char nome[50];
                printf("Digite o nome do paciente preferencial: ");
                scanf(" %[^\n]", nome);
                nova_senha(nome, 1);
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
                limpa_fila(fila);
                limpa_fila(fila_preferencial);
                return 0;
            default:
                printf("Opcao invalida!\n");
        }

    }

    return 0;
}