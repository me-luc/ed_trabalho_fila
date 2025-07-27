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
typedef struct grafono GrafoNo;
typedef struct grafo *Grafo;

struct fila {
    FilaNo* primeiro;
    FilaNo* ultimo;
};

struct filano {
    int senha;
    char nome[50];
    FilaNo* proximo;
};

struct grafono {
    char nome[50];
    Fila* filaGeral;
    Fila* filaPref;
    GrafoNo* proximo;
};

struct grafo {
    GrafoNo** listaAdj;
    int vertices;
    int arestas;
};


// >> IMPLEMENTACAO DE FILA <<
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


void remove_senha_fila(Fila* fila, int senha) {
    if(fila->primeiro == NULL) {
        return;
    } else {
        FilaNo* no = fila->primeiro;
        FilaNo* aux = fila->primeiro;
        while(no != NULL) {
            if(no->senha == senha) {

            }
            free(aux);
            aux = no;
            no = no->proximo;
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

// >> IMPLEMENTACAO DE GRAFO <<
Grafo inicializa_grafo(int vertices) {
    Grafo* G = malloc(sizeof(Grafo));
    G->numVertices = n;
    G->numArestas = 0;
    G->vet = malloc(n * sizeof(Vertice*));
    for (int i = 0; i < n; i++) {
        G->vet[i] = malloc(sizeof(Vertice));
        strncpy(G->vet[i]->nome, nomes[i], 49);
        G->vet[i]->nome[49] = '\0';
        G->vet[i]->filaGeral = cria_fila();
        G->vet[i]->filaPref = cria_fila();
        G->vet[i]->proxAdj = NULL;
    }
    return G;
}

void insere_arco_grafo(Grafo G, char* nome, int vertice) {
    if(vertice < 0 || vertice >= G->vertices) {
        printf("Vertice invalido!\n");
        return;
    }

    GrafoNo* novo_no = (GrafoNo*) malloc(sizeof(GrafoNo));
    strncpy(novo_no->nome, nome, sizeof(novo_no->nome) - 1);
    novo_no->proximo = G->listaAdj[vertice];
    G->listaAdj[vertice] = novo_no;
    G->arestas++;
}

Grafo libera_grafo(Grafo G) {
    for(int i = 0; i < G->vertices; i++) {
        GrafoNo* no = G->listaAdj[i];
        while(no != NULL) {
            GrafoNo* aux = no;
            no = no->proximo;
            free(aux);
        }
    }
    free(G->listaAdj);
    G->listaAdj = NULL;
    free(G);
    return NULL;
}

void imprime_grafo(Grafo G) {
    for(int i = 0; i < G->vertices; i++) {
        printf("Vertice %d: ", i);
        GrafoNo* no = G->listaAdj[i];
        while(no != NULL) {
            printf("%s -> ", no->nome);
            no = no->proximo;
        }
        printf("NULL\n");
    }
}

// >> VARIAVEIS GLOBAIS <<
Fila* fila;
Fila* fila_preferencial;
Fila* atendimentos;
Fila* atendimentos_preferenciais;
Gr
int senha = 1;
int senhaChamada;
char nomeChamado[50];

// >> MODALIDADES DE ATENDIMENTO <<
char *modalidades[50] = {
    "Psicologia", 
    "Fisioterapia", 
    "Nutricao", 
    "Odontologia", 
    "Ginecologia",
    "Pediatria",
    "Cardiologia",
};

// >> METODOS P/ TELA DO ATENDIMENTO <<
void limpa_tela(){
    system("cls");
}

void nova_senha(char nome[], int preferencial) {
    printf("\nInserindo senha: %d - paciente: %s..", senha, nome);
    sleep(1);
    if(preferencial) {
        insere_fila(fila_preferencial, senha, nome);
    } else {
        insere_fila(fila, senha, nome);
    }
    printf(GREEN "\nInserido com sucesso!" RESET);
    senha++;
    sleep(1);
    limpa_tela();
}

void nova_senha(Grafo* G, int idx, int preferencial) {
    char nome[50];
    printf("Digite o nome do paciente para %s: ", G->vet[idx]->nome);
    scanf(" %[^]", nome);
    printf("Inserindo senha %d - %s na %s...\n", proximaSenha, nome, G->vet[idx]->nome);
    sleep(1);
    if (preferencial)
        insere_fila(G->vet[idx]->filaPref, proximaSenha, nome);
    else
        insere_fila(G->vet[idx]->filaGeral, proximaSenha, nome);
    printf(GREEN "Inserido com sucesso!" RESET "\n");
    proximaSenha++;
    sleep(1);
}


void chamar_senha() {
    if(fila_preferencial->primeiro == NULL && fila->primeiro == NULL) {
        print_erro("Nenhuma senha em nenhuma das filas!");
        return;
    }

    FilaNo* proximo_paciente;
    int chamar_preferencial = 0;

    if(fila_preferencial->primeiro != NULL) {
        proximo_paciente = fila_preferencial->primeiro;
        insere_fila(atendimentos_preferenciais, proximo_paciente->senha, proximo_paciente->nome);
        chamar_preferencial = 1;
    }

    if(chamar_preferencial == 0) {
        proximo_paciente = fila->primeiro;
        insere_fila(atendimentos, proximo_paciente->senha, proximo_paciente->nome);
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

void imprime_dados_fila(Fila* fila) {
    FilaNo* no = fila->primeiro;
    while(no!=NULL) {
        printf("\n[%d] %s", no->senha, no->nome);
        no = no->proximo;
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

void historico() {
    if(atendimentos_preferenciais->primeiro == NULL && atendimentos->primeiro == NULL) {
        print_erro("Nenhum paciente atendido!");
        return;
    } 

    printf("\nRegistro de senhas/paciente atendidos\n");
    printf("-----------\n");

    if(atendimentos->primeiro != NULL) {
        printf(BLUE "\n> Fila geral" RESET);
        imprime_dados_fila(atendimentos);
    }

    if(atendimentos_preferenciais->primeiro != NULL) {
        printf(BLUE "\n\n> Fila preferencial" RESET);
        imprime_dados_fila(atendimentos_preferenciais);
    }

    while(1) {
        printf("\n\nDigite qualquer letra + ENTER para voltar pro menu! > ");
        char linha[100];
        scanf("%s", linha);
        limpa_tela();
        break;
    }
}

// >> UTILITARIOS <<
void print_erro(char* mensagem) {
    printf(RED "\nErro: %s" RESET, mensagem);
    sleep(2);
    limpa_tela();
}

int main() {
    int n = sizeof(modalidades)/sizeof(modalidades[0]);
    Grafo* G = inicia_grafo(n, modalidades);

    fila = cria_fila();
    fila_preferencial = cria_fila();
    atendimentos = cria_fila();
    atendimentos_preferenciais = cria_fila();

    limpa_tela();

    while(1) {
        printf("*** CLINICA ED ***\n");
        status_fila();
        printf("\n1 - Nova senha");
        printf("\n2 - Nova senha preferencial");
        printf("\n3 - Chamar senha");
        printf("\n4 - Historico");
        printf("\n5 - Sair");

        int opcao;
        do {
            printf("\n\n-------\nEscolha uma opcao > ");
            scanf("%d", &opcao);
        } while (opcao < 1 || opcao > 4);
        
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
            case 4:
                limpa_tela();
                historico();
                break;
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