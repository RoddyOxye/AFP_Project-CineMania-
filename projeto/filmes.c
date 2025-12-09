#include "filmes.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

static void toLowerCopy(const char *src, char *dest, int max) {
    int i;
    for (i = 0; i < max - 1 && src[i] != '\0'; i++)
        dest[i] = (char)tolower((unsigned char)src[i]);
    dest[i] = '\0';
}

static int containsCaseInsensitive(const char *field, const char *pattern) {
    char A[512], B[256];
    toLowerCopy(field, A, sizeof(A));
    toLowerCopy(pattern, B, sizeof(B));
    return strstr(A, B) != NULL;
}

static void swapFilmes(Filme *a, Filme *b) {
    Filme tmp = *a;
    *a = *b;
    *b = tmp;
}

void inicializarColecao(Filme *colecao, int *count) {
    *count = 0;
}

int adicionarFilme(Filme *colecao, int *count, const Filme *novo) {
    if (*count >= MAXFILMES) return 0;

    int max = 0;
    for (int i = 0; i < *count; i++)
        if (colecao[i].code > max) max = colecao[i].code;

    Filme f = *novo;
    f.code = max + 1;

    colecao[*count] = f;
    (*count)++;
    return 1;
}

static int indiceCode(Filme *colecao, int count, int code) {
    for (int i = 0; i < count; i++)
        if (colecao[i].code == code)
            return i;
    return -1;
}

int consultarCode(Filme *colecao, int count, int code) {
    int idx = indiceCode(colecao, count, code);
    if (idx < 0) {
        printf("Filme nao encontrado.\n");
        return 0;
    }

    Filme *f = &colecao[idx];

    printf("\n=== DETALHES DO FILME ===\n");
    printf("Code: %d\n", f->code);
    printf("Title: %s\n", f->title);
    printf("Genres: %s\n", f->genres);
    printf("Description: %s\n", f->description);
    printf("Director: %s\n", f->director);
    printf("Actors: %s\n", f->actors);
    printf("Year: %d\n", f->year);
    printf("Duration: %d min\n", f->duration);
    printf("Rating: %.1f\n", f->rating);
    printf("Favorite: %s\n", f->favorite ? "Sim" : "Nao");
    printf("Revenue: %.2f M\n", f->revenue);

    return 1;
}

void listarTodos(Filme *colecao, int count, int orden) {
    if (count == 0) {
        printf("Nenhum filme em memoria.\n");
        return;
    }

    Filme copia[MAXFILMES];
    for (int i = 0; i < count; i++)
        copia[i] = colecao[i];

    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - 1 - i; j++) {

            int troca = 0;

            if (orden == 0 && copia[j].code > copia[j + 1].code)
                troca = 1;

            if (orden == 1 && copia[j].rating < copia[j + 1].rating)
                troca = 1;

            if (orden == 2 && strcmp(copia[j].title, copia[j + 1].title) > 0)
                troca = 1;

            if (troca)
                swapFilmes(&copia[j], &copia[j + 1]);
        }
    }

    printf("\nCode | Title\n");
    for (int i = 0; i < count; i++)
        printf("%4d | %s\n", copia[i].code, copia[i].title);
}

int pesquisarTitulo(Filme *colecao, int count, const char *substr) {
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (containsCaseInsensitive(colecao[i].title, substr)) {
            printf("%4d | %s\n", colecao[i].code, colecao[i].title);
            found = 1;
        }
    }
    if (!found) printf("Nenhum resultado.\n");
    return found;
}

int pesquisarGenero(Filme *colecao, int count, const char *genre) {
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (containsCaseInsensitive(colecao[i].genres, genre)) {
            printf("%4d | %s | %s\n", colecao[i].code, colecao[i].title, colecao[i].genres);
            found = 1;
        }
    }
    if (!found) printf("Nenhum resultado.\n");
    return found;
}

int pesquisarRealizador(Filme *colecao, int count, const char *director) {
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (containsCaseInsensitive(colecao[i].director, director)) {
            printf("%4d | %s\n", colecao[i].code, colecao[i].title);
            found = 1;
        }
    }
    if (!found) printf("Nenhum resultado.\n");
    return found;
}

int pesquisarAtor(Filme *colecao, int count, const char *actor) {
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (containsCaseInsensitive(colecao[i].actors, actor)) {
            printf("%4d | %s\n", colecao[i].code, colecao[i].title);
            found = 1;
        }
    }
    if (!found) printf("Nenhum resultado.\n");
    return found;
}
