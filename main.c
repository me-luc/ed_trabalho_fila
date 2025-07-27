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
#define YELLOW "\033[33m"


// declarações das estruturas
typedef struct filano FilaNo;
typedef struct fila Fila;
typedef struct grafono GrafoNo;
typedef struct grafo Grafo;

// definições dos structs
struct fila {
    FilaNo* primeiro;
    FilaNo* ultimo;
};

struct filano {
    int senha;
    char nome[100];
    FilaNo* proximo;
};

struct grafono {
    char nome[100];
    Fila *filaGeral, *filaPref, *filaAtendida;
    FilaNo* ultimoChamado;
    GrafoNo* proximo;
};

struct grafo {
    GrafoNo** listaAdj;
    int vertices;
    int arestas;
};

// init da fila
Fila* cria_fila() {
    Fila* fila = (Fila*) malloc(sizeof(Fila));
    fila->primeiro = NULL;
    fila->ultimo = NULL;
    return fila;
}

// inserção
void insere_fila(Fila* fila, int senha, char* nome) {
    FilaNo* novo_no = (FilaNo*) malloc(sizeof(FilaNo));
    novo_no->senha = senha;
    strncpy(novo_no->nome, nome, sizeof(novo_no->nome) - 1);
    novo_no->nome[sizeof(novo_no->nome) - 1] = '\0';
    novo_no->proximo = NULL;

    if (fila->ultimo) {
        fila->ultimo->proximo = novo_no;
    } else {
        fila->primeiro = novo_no;
    }

    fila->ultimo = novo_no;
}

// remoção
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
        free(aux);
    }

    free(fila);
}

// >> IMPLEMENTACAO DE GRAFO <<
Grafo* inicializa_grafo(int n, char **nomes) {
    Grafo* G = (Grafo*) malloc(sizeof(Grafo));
    G->vertices = n;
    G->arestas = 0;
    G->listaAdj = (GrafoNo**) malloc(n * sizeof(GrafoNo*));
    
    for (int i = 0; i < n; i++) {
        G->listaAdj[i] = (GrafoNo*) malloc(sizeof(GrafoNo));
        strncpy(G->listaAdj[i]->nome, nomes[i], 49);
        G->listaAdj[i]->nome[49] = '\0';
        G->listaAdj[i]->filaGeral = cria_fila();
        G->listaAdj[i]->filaPref = cria_fila();
        G->listaAdj[i]->filaAtendida = cria_fila();
        G->listaAdj[i]->ultimoChamado = NULL;
        G->listaAdj[i]->proximo = NULL;
    }
    return G;
}

void insere_arco_grafo(Grafo* G, int origem, int destino) {
    if(origem < 0 || origem >= G->vertices || destino < 0 || destino >= G->vertices) {
        printf("Vertice invalido!\n");
        return;
    }

    // Cria novo nó para a lista de adjacência
    GrafoNo* novo_no = (GrafoNo*) malloc(sizeof(GrafoNo));
    strncpy(novo_no->nome, G->listaAdj[destino]->nome, sizeof(novo_no->nome) - 1);
    novo_no->filaGeral = NULL;
    novo_no->filaPref = NULL;
    
    // Insere no início da lista
    novo_no->proximo = G->listaAdj[origem]->proximo;
    G->listaAdj[origem]->proximo = novo_no;
    G->arestas++;
}

Grafo* libera_grafo(Grafo* G) {
    for(int i = 0; i < G->vertices; i++) {
        GrafoNo* no = G->listaAdj[i];
        while(no != NULL) {
            GrafoNo* aux = no;
            no = no->proximo;
            if(aux->filaGeral) limpa_fila(aux->filaGeral);
            if(aux->filaPref) limpa_fila(aux->filaPref);
            free(aux);
        }
    }
    free(G->listaAdj);
    free(G);
    return NULL;
}

void imprime_grafo(Grafo* G) {
    printf("\n=== GRAFO ===\n");

    for(int i = 0; i < G->vertices; i++) {
        printf("\nServico [%d] %s", i, G->listaAdj[i]->nome);
        printf("\n  Fila Geral: ");
        FilaNo* no_geral = G->listaAdj[i]->filaGeral->primeiro;
        while(no_geral != NULL) {
            printf("[%d] -> ", no_geral->senha);
            no_geral = no_geral->proximo;
        }
        printf("NULL");
        
        printf("\n  Fila Preferencial: ");
        FilaNo* no_pref = G->listaAdj[i]->filaPref->primeiro;
        while(no_pref != NULL) {
            printf("[%d] -> ", no_pref->senha);
            no_pref = no_pref->proximo;
        }
        printf("NULL");
        
        printf("\n  Conexoes: ");
        GrafoNo* adj = G->listaAdj[i]->proximo;
        while(adj != NULL) {
            printf("%s -> ", adj->nome);
            adj = adj->proximo;
        }
        printf("NULL\n");
    }
}

// >> VARIAVEIS GLOBAIS <<
Grafo* grafoClinica;
int senha = 1;
int atendimentosCount = 0;

// >> MODALIDADES DE ATENDIMENTO <<
char *modalidades[] = {
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
    system("clear || cls");
}

void print_erro(char* mensagem) {
    printf(RED "\nErro: %s" RESET, mensagem);
    sleep(2);
    limpa_tela();
}

void nova_senha(Grafo* G, int idx, int preferencial) {
    char nome[50];
    printf("Digite o nome do paciente: ");
    scanf(" %[^\n]", nome);
    
    printf("\nInserindo senha: %d - paciente: %s..", senha, nome);
    
    if(preferencial) {
        insere_fila(G->listaAdj[idx]->filaPref, senha, nome);
    } else {
        insere_fila(G->listaAdj[idx]->filaGeral, senha, nome);
    }
    
    printf(GREEN "\nInserido com sucesso no servico %s!" RESET, G->listaAdj[idx]->nome);
    senha++;
    sleep(1);
    limpa_tela();
}

void chamar_senha(Grafo* G, int idx) {
    if(G->listaAdj[idx]->filaPref->primeiro == NULL && 
       G->listaAdj[idx]->filaGeral->primeiro == NULL) {
        print_erro("Nenhuma senha para este servico!");
        return;
    }

    FilaNo* proximo_paciente = NULL;
    int chamar_preferencial = 0;
    char nomeAux[100];

    // verifica fila preferencial primeiro
    if(G->listaAdj[idx]->filaPref->primeiro != NULL) {
        proximo_paciente = G->listaAdj[idx]->filaPref->primeiro;
        snprintf(nomeAux, sizeof(nomeAux), "%s (Preferencial)", proximo_paciente->nome, G->listaAdj[idx]->nome);
        chamar_preferencial = 1;
    } 
    // se não tiver preferencial, chama da geral
    else {
        proximo_paciente = G->listaAdj[idx]->filaGeral->primeiro;
        snprintf(nomeAux, sizeof(nomeAux), "%s (Geral)", proximo_paciente->nome, G->listaAdj[idx]->nome);
    }
    
    atendimentosCount++;
    insere_fila(G->listaAdj[idx]->filaAtendida, proximo_paciente->senha, nomeAux);
    
    G->listaAdj[idx]->ultimoChamado = G->listaAdj[idx]->filaAtendida->ultimo;

    printf("\nChamando senha %d - %s..", proximo_paciente->senha, proximo_paciente->nome);
    sleep(1);

    // remove da fila correspondente
    if(chamar_preferencial) {
        remove_fila(G->listaAdj[idx]->filaPref);
    } else {
        remove_fila(G->listaAdj[idx]->filaGeral);
    }

    printf(GREEN "\nChamado com sucesso no servico %s!" RESET, G->listaAdj[idx]->nome);
    sleep(1);
    limpa_tela();
}

void imprime_fila(Fila* fila) {
    FilaNo* no = fila->primeiro;
    while(no != NULL) {
        printf("[%d]", no->senha);
        no = no->proximo;
        if(no != NULL) {
            printf(" -> ");
        }
    }
    if(fila->primeiro == NULL) {
        printf("VAZIA");
    }
}

void imprime_dados_fila(Fila* fila) {
    FilaNo* no = fila->primeiro;
    while(no != NULL) {
        printf("\n[%d] %s", no->senha, no->nome);
        no = no->proximo;
    }
}

void senhas_chamadas(Grafo* G) {
    if(atendimentosCount > 0) {
        printf(BLUE "\n---- SENHAS CHAMADAS ----");
        for(int i = 0; i < G->vertices; i++) {
            if(G->listaAdj[i]->ultimoChamado != NULL) {
                printf("\nSENHA [%d] - %s (%s)", 
                       G->listaAdj[i]->ultimoChamado->senha, 
                       G->listaAdj[i]->ultimoChamado->nome, 
                       G->listaAdj[i]->nome);
            }
        }
        printf(BLUE "\n-------------------------\n" RESET);
    }
}

void exibe_filas(Grafo* G) {
    printf("\n\n=== FILAS POR SERVICO ===");
    for(int i = 0; i < G->vertices; i++) {
        printf("\n\n[%d] - %s", i, G->listaAdj[i]->nome);
        printf("\n  Geral: ");
        imprime_fila(G->listaAdj[i]->filaGeral);
        printf("\n  Preferencial: ");
        imprime_fila(G->listaAdj[i]->filaPref);
    }
    printf("\n");
}

void historico() {
    printf("\n\n=== HISTORICO DE ATENDIMENTOS ===\n");

    for(int i = 0; i < grafoClinica->vertices; i++) {
        printf("\nServico [%d] %s", i, grafoClinica->listaAdj[i]->nome);
        FilaNo* no = grafoClinica->listaAdj[i]->filaAtendida->primeiro;
        imprime_dados_fila(grafoClinica->listaAdj[i]->filaAtendida);
        printf("\n");
    }

    pressione_enter();
}

void pressione_enter() {
    printf("\n\nPressione ENTER para continuar...");
    getchar();
    getchar();
    limpa_tela();
}

int main() {
    int n = sizeof(modalidades)/sizeof(modalidades[0]);
    grafoClinica = inicializa_grafo(n, modalidades);
    
    // Adiciona algumas conexões entre serviços
    insere_arco_grafo(grafoClinica, 0, 1);  // Psicologia -> Fisioterapia
    insere_arco_grafo(grafoClinica, 1, 2);  // Fisioterapia -> Nutrição
    insere_arco_grafo(grafoClinica, 3, 6);  // Odontologia -> Cardiologia

    limpa_tela();

    while(1) {
        printf(YELLOW "*** CLINICA ED ***\n" RESET);
        senhas_chamadas(grafoClinica);
        
        printf("\n\n1 - Nova senha (Geral)");
        printf("\n2 - Nova senha (Preferencial)");
        printf("\n3 - Chamar senha");
        printf("\n4 - Historico de atendimentos");
        printf("\n5 - Visualizar filas");
        printf("\n6 - Sair\n");

        int opcao;
        do {
            printf("\nEscolha uma opcao > ");
            scanf("%d", &opcao);
        } while (opcao < 1 || opcao > 6);
        
        limpa_tela();

        switch(opcao) {
            case 1: {
                printf("Servicos disponiveis:\n");
                for (int i = 0; i < grafoClinica->vertices; i++) {
                    printf("%d - %s\n", i, grafoClinica->listaAdj[i]->nome);
                }
                int idx;
                do {
                    printf("\nEscolha um servico > ");
                    scanf("%d", &idx);
                } while (idx < 0 || idx >= grafoClinica->vertices);

                nova_senha(grafoClinica, idx, 0);
                break;
            }
            case 2: {
                printf("Servicos disponiveis:\n");
                for (int i = 0; i < grafoClinica->vertices; i++) {
                    printf("%d - %s\n", i, grafoClinica->listaAdj[i]->nome);
                }
                int idx;
                printf("\nEscolha o servico: ");
                scanf("%d", &idx);
                if (idx < 0 || idx >= grafoClinica->vertices) {
                    print_erro("Servico invalido!");
                } else {
                    nova_senha(grafoClinica, idx, 1);
                }
                break;
            }
            case 3: {
                printf("Servicos disponiveis:\n");
                for (int i = 0; i < grafoClinica->vertices; i++) {
                    printf("%d - %s\n", i, grafoClinica->listaAdj[i]->nome);
                }
                int idx;
                printf("\nEscolha o servico para chamar: ");
                scanf("%d", &idx);
                if (idx < 0 || idx >= grafoClinica->vertices) {
                    print_erro("Servico invalido!");
                } else {
                    chamar_senha(grafoClinica, idx);
                }
                break;
            }
            case 4:
                historico();
                break;
            case 5:
                exibe_filas(grafoClinica);
                printf("\nPressione ENTER para continuar...");
                getchar();
                getchar();
                limpa_tela();
                break;
            case 6:
                grafoClinica = libera_grafo(grafoClinica);
                return 0;
            default:
                printf("Opcao invalida!\n");
        }
    }

    return 0;
}