#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Sala {
    char nome[50];
    char pista[100];   // pista opcional (pode ser vazia)
    struct Sala* esquerda;
    struct Sala* direita;
} Sala;

Sala* criarSala(const char* nome, const char* pista) {
    Sala* nova = (Sala*)malloc(sizeof(Sala));
    if (nova == NULL) {
        printf("Erro ao alocar memória!\n");
        exit(1);
    }
    strcpy(nova->nome, nome);
    strcpy(nova->pista, pista);
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}

typedef struct NodoBST {
    char pista[100];
    struct NodoBST* esquerda;
    struct NodoBST* direita;
} NodoBST;

NodoBST* criarNodoBST(const char* pista) {
    NodoBST* novo = (NodoBST*)malloc(sizeof(NodoBST));
    strcpy(novo->pista, pista);
    novo->esquerda = NULL;
    novo->direita = NULL;
    return novo;
}

NodoBST* inserirBST(NodoBST* raiz, const char* pista) {
    if (raiz == NULL)
        return criarNodoBST(pista);

    if (strcmp(pista, raiz->pista) < 0)
        raiz->esquerda = inserirBST(raiz->esquerda, pista);
    else if (strcmp(pista, raiz->pista) > 0)
        raiz->direita = inserirBST(raiz->direita, pista);

    return raiz;
}

void mostrarBST(NodoBST* raiz) {
    if (!raiz) return;
    mostrarBST(raiz->esquerda);
    printf("- %s\n", raiz->pista);
    mostrarBST(raiz->direita);
}

void liberarBST(NodoBST* raiz) {
    if (!raiz) return;
    liberarBST(raiz->esquerda);
    liberarBST(raiz->direita);
    free(raiz);
}


NodoBST* explorar(Sala* atual, NodoBST* bst) {
    while (atual != NULL) {

        printf("\nVocê está em: %s\n", atual->nome);

        if (strlen(atual->pista) > 0) {
            printf("Pista encontrada: \"%s\"\n", atual->pista);
            bst = inserirBST(bst, atual->pista);
        } else {
            printf("Nenhuma pista neste cômodo.\n");
        }

        printf("\nOpções:\n");
        printf("(e) Seguir para a esquerda\n");
        printf("(d) Seguir para a direita\n");
        printf("(s) Sair da mansão\n");
        printf("Escolha: ");

        char op;
        scanf(" %c", &op);

        if (op == 'e' || op == 'E') {
            if (atual->esquerda)
                atual = atual->esquerda;
            else
                printf("Não há sala à esquerda!\n");
        }
        else if (op == 'd' || op == 'D') {
            if (atual->direita)
                atual = atual->direita;
            else
                printf("Não há sala à direita!\n");
        }
        else if (op == 's' || op == 'S') {
            printf("\nExploração encerrada pelo usuário.\n");
            return bst;
        }
        else {
            printf("Opção inválida.\n");
        }

        // Caso chegue a um nó sem filhos
        if (atual != NULL && atual->esquerda == NULL && atual->direita == NULL) {
            printf("\nVocê chegou a um cômodo sem saídas.\n");
            return bst;
        }
    }

    return bst;
}

void liberarSalas(Sala* s) {
    if (!s) return;
    liberarSalas(s->esquerda);
    liberarSalas(s->direita);
    free(s);
}

int main() {

    Sala* hall        = criarSala("Hall de Entrada", "Pegadas no tapete");
    Sala* salaEstar   = criarSala("Sala de Estar", "Uma chave antiga");
    Sala* cozinha     = criarSala("Cozinha", "Xícara quebrada");
    Sala* biblioteca  = criarSala("Biblioteca", "Página arrancada de um livro");
    Sala* salaSecreta = criarSala("Sala Secreta", "Mapa misterioso");

    hall->esquerda = salaEstar;
    hall->direita = cozinha;

    salaEstar->esquerda = biblioteca;
    salaEstar->direita = salaSecreta;

    NodoBST* bstPistas = NULL;

    printf("=== Detective Quest — Coleta de Pistas ===\n");

    bstPistas = explorar(hall, bstPistas);

    printf("\n=====================================\n");
    printf("PISTAS COLETADAS (ORDEM ALFABÉTICA):\n");
    printf("=====================================\n");

    mostrarBST(bstPistas);

    liberarSalas(hall);
    liberarBST(bstPistas);

    return 0;
}
