#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Sala {
    char nome[50];
    struct Sala* esquerda;
    struct Sala* direita;
} Sala;

Sala* criarSala(const char* nome) {
    Sala* nova = (Sala*)malloc(sizeof(Sala));
    if (nova == NULL) {
        printf("Erro ao alocar memória!\n");
        exit(1);
    }
    strcpy(nova->nome, nome);
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}

void explorar(Sala* atual) {
    while (atual != NULL) {
        printf("\nVocê está em: %s\n", atual->nome);

        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("Fim do caminho! Esta sala não possui mais saídas.\n");
            return;
        }

        char opcao;
        printf("Escolha um caminho - esquerda (e) ou direita (d): ");
        scanf(" %c", &opcao);

        if (opcao == 'e' || opcao == 'E') {
            if (atual->esquerda != NULL) {
                atual = atual->esquerda;
            } else {
                printf("Não há caminho à esquerda!\n");
            }
        } 
        else if (opcao == 'd' || opcao == 'D') {
            if (atual->direita != NULL) {
                atual = atual->direita;
            } else {
                printf("Não há caminho à direita!\n");
            }
        } 
        else {
            printf("Opção inválida!\n");
        }
    }
}

// Libera memória da árvore
void liberar(Sala* s) {
    if (s == NULL) return;
    liberar(s->esquerda);
    liberar(s->direita);
    free(s);
}

int main() {

    // Construção automática da mansão
    Sala* hall = criarSala("Hall de Entrada");
    Sala* salaEstar = criarSala("Sala de Estar");
    Sala* cozinha = criarSala("Cozinha");
    Sala* biblioteca = criarSala("Biblioteca");
    Sala* salaSecreta = criarSala("Sala Secreta");


    hall->esquerda = salaEstar;
    hall->direita = cozinha;

    salaEstar->esquerda = biblioteca;
    salaEstar->direita = salaSecreta;

    printf("=== Detective Quest ===\n");
    printf("Explorando a mansão...\n");
    explorar(hall);

    liberar(hall);

    return 0;
}
