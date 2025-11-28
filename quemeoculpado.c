#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_HASH 20

// ----------- HASH ENCADEADA -----------
typedef struct NoHash {
    char pista[100];
    char suspeito[50];
    struct NoHash* prox;
} NoHash;

NoHash* tabelaHash[TAM_HASH];

unsigned long hash(const char* str) {
    unsigned long h = 5381;
    int c;
    while ((c = *str++)) h = ((h << 5) + h) + c;
    return h % TAM_HASH;
}

void initHash() {
    for(int i=0; i<TAM_HASH; i++)
        tabelaHash[i] = NULL;
}

void inserirHash(const char* pista, const char* suspeito) {
    unsigned long h = hash(pista);
    NoHash* novo = malloc(sizeof(NoHash));
    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);
    novo->prox = tabelaHash[h];
    tabelaHash[h] = novo;
}

char* buscarSuspeito(const char* pista) {
    unsigned long h = hash(pista);
    NoHash* atual = tabelaHash[h];
    while (atual) {
        if (strcmp(atual->pista, pista) == 0)
            return atual->suspeito;
        atual = atual->prox;
    }
    return NULL;
}

// ----------- ÁRVORE BINÁRIA DAS SALAS -----------
typedef struct Sala {
    char nome[50];
    struct Sala* esq;
    struct Sala* dir;
} Sala;

Sala* criarSala(const char* nome) {
    Sala* s = malloc(sizeof(Sala));
    strcpy(s->nome, nome);
    s->esq = s->dir = NULL;
    return s;
}

// Pistas fixas por nome da sala (definidas pela lógica do código)
const char* obterPista(const char* nomeSala) {
    if (strcmp(nomeSala, "Hall") == 0) return "Pegadas no tapete";
    if (strcmp(nomeSala, "SalaEstar") == 0) return "Chave antiga";
    if (strcmp(nomeSala, "Cozinha") == 0) return "Xicara quebrada";
    if (strcmp(nomeSala, "Biblioteca") == 0) return "Pagina arrancada";
    if (strcmp(nomeSala, "Escritorio") == 0) return "Mapa misterioso";
    return NULL; // Sem pista
}

// ----------- BST DAS PISTAS COLETADAS -----------
typedef struct NodoBST {
    char pista[100];
    struct NodoBST* esq;
    struct NodoBST* dir;
} NodoBST;

NodoBST* inserirBST(NodoBST* raiz, const char* pista) {
    if (!raiz) {
        NodoBST* novo = malloc(sizeof(NodoBST));
        strcpy(novo->pista, pista);
        novo->esq = novo->dir = NULL;
        return novo;
    }
    int cmp = strcmp(pista, raiz->pista);
    if (cmp < 0)
        raiz->esq = inserirBST(raiz->esq, pista);
    else if (cmp > 0)
        raiz->dir = inserirBST(raiz->dir, pista);
    return raiz;
}

void listarBST(NodoBST* raiz) {
    if (!raiz) return;
    listarBST(raiz->esq);
    printf("- %s\n", raiz->pista);
    listarBST(raiz->dir);
}

// ----------- EXPLORAÇÃO INTERATIVA -----------
NodoBST* explorar(Sala* atual, NodoBST* bst) {
    while(atual) {
        printf("\nVocê entrou na sala: %s\n", atual->nome);
        const char* pista = obterPista(atual->nome);
        if (pista) {
            printf("Pista encontrada: \"%s\"\n", pista);
            bst = inserirBST(bst, pista);
        }
        if (!atual->esq && !atual->dir) {
            printf("Esta é uma sala sem mais caminhos.\n");
            return bst;
        }
        printf("Escolha o caminho: (e) esquerda, (d) direita, (s) sair: ");
        char opc;
        scanf(" %c", &opc);
        if (opc == 'e' && atual->esq) atual = atual->esq;
        else if (opc == 'd' && atual->dir) atual = atual->dir;
        else if (opc == 's') return bst;
        else printf("Caminho inválido! Tente novamente.\n");
    }
    return bst;
}

// ----------- CONTAR PISTAS CONTRA SUSPEITO -----------
int contarPistasContraSuspeito(NodoBST* bst, const char* suspeito) {
    if (!bst) return 0;
    int total = 0;
    char* dono = buscarSuspeito(bst->pista);
    if (dono && strcmp(dono, suspeito) == 0)
        total++;
    total += contarPistasContraSuspeito(bst->esq, suspeito);
    total += contarPistasContraSuspeito(bst->dir, suspeito);
    return total;
}

// ----------- FUNÇÃO MAIN -----------
int main() {
    printf("=== Detective Quest - Versão Final ===\n");

    // Criar hash de pistas e suspeitos (estático, definido no código)
    initHash();
    inserirHash("Pegadas no tapete", "Carlos");
    inserirHash("Chave antiga", "Marina");
    inserirHash("Xicara quebrada", "Carlos");
    inserirHash("Pagina arrancada", "Roberto");
    inserirHash("Mapa misterioso", "Marina");

    // Montar mansão: árvore binária das salas
    Sala* hall = criarSala("Hall");
    Sala* salaEstar = criarSala("SalaEstar");
    Sala* cozinha = criarSala("Cozinha");
    Sala* biblioteca = criarSala("Biblioteca");
    Sala* escritorio = criarSala("Escritorio");

    hall->esq = salaEstar;
    hall->dir = cozinha;
    salaEstar->esq = biblioteca;
    salaEstar->dir = escritorio;

    // Exploração
    NodoBST* pistasColetadas = NULL;
    pistasColetadas = explorar(hall, pistasColetadas);

    // Listar pistas coletadas
    printf("\nPistas coletadas:\n");
    listarBST(pistasColetadas);

    // Acusação
    char suspeito[50];
    printf("\nInforme o nome do suspeito para acusar: ");
    scanf("%49s", suspeito);

    int totalPistas = contarPistasContraSuspeito(pistasColetadas, suspeito);
    printf("Número de pistas contra %s: %d\n", suspeito, totalPistas);

    if (totalPistas >= 2)
        printf("Acusação válida! %s é o culpado!\n", suspeito);
    else
        printf("Acusação fraca! %s provavelmente é inocente.\n", suspeito);

    return 0;
}
