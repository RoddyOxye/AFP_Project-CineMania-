#include "filmes.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

static void toLowerCopy(const char *src, char *dest, int max) {
    int i;
    for (i = 0; i < max - 1 && src[i] != '\0'; i++) {
        dest[i] = (char)tolower((unsigned char)src[i]);
    }
    dest[i] = '\0';
}

static int cmpCaseInsensitive(const char *a, const char *b) {
    char A[256], B[256];
    toLowerCopy(a, A, sizeof(A));
    toLowerCopy(b, B, sizeof(B));
    return strcmp(A, B);
}

void inicializaColecao(Filme *colecao, int *count) {
    if (count) *count = 0;
}

static int indicePorCode(Filme *colecao, int count, int code) {
    int i;
    for (i = 0; i < count; i++) {
        if (colecao[i].code == code) return i;
    }
    return -1;
}

int adicionaFilme(Filme *colecao, int *count, const Filme *novo) {
    if (!count || *count >= MAXFILMES) return 0;
    int max = 0;
    for (int i = 0; i < *count; i++) if (colecao[i].code > max) max = colecao[i].code;
    Filme f = *novo;
    f.code = max + 1;
    colecao[*count] = f;
    (*count)++;
    return 1;
}

int eliminaFilme(Filme *colecao, int *count, int code) {
    if (!count || *count == 0) return 0;
    int idx = indicePorCode(colecao, *count, code);
    if (idx < 0) return 0;
    for (int i = idx; i < (*count) - 1; i++) colecao[i] = colecao[i + 1];
    (*count)--;
    return 1;
}

void limpaColecao(Filme *colecao, int *count) {
    if (count) *count = 0;
}


static void swapFilmes(Filme *a, Filme *b) {
    Filme tmp = *a;
    *a = *b;
    *b = tmp;
}

void listaTodos(Filme *colecao, int count, int orden) {
    if (count == 0) {
        printf("Nenhum filme em memoria.\n");
        return;
    }

    Filme *copia = (Filme *)malloc(sizeof(Filme) * count);
    if (!copia) {
        printf("Erro de memoria.\n");
        return;
    }
    for (int i = 0; i < count; i++) copia[i] = colecao[i];

    int i, j;
    for (i = 0; i < count - 1; i++) {
        for (j = 0; j < count - 1 - i; j++) {
            int troca = 0;
            if (orden == 0) {
                if (copia[j].code > copia[j + 1].code) troca = 1;
            } else if (orden == 1) {
                if (copia[j].rating < copia[j + 1].rating) troca = 1;
            } else if (orden == 2) {
                if (cmpCaseInsensitive(copia[j].title, copia[j + 1].title) > 0) troca = 1;
            }
            if (troca) swapFilmes(&copia[j], &copia[j + 1]);
        }
    }

    printf("Code | Title (trunc)                     | Genres (trunc)   | Director (trunc) | Year | Dur | Rating | Fav | Revenue\n");
    printf("-------------------------------------------------------------------------------------------------------------\n");
    for (i = 0; i < count; i++) {
        printf("%4d | %-33.33s | %-15.15s | %-16.16s | %4d | %3d | %6.1f | %3d | %7.2f\n",
               copia[i].code,
               copia[i].title,
               copia[i].genres,
               copia[i].director,
               copia[i].year,
               copia[i].duration,
               copia[i].rating,
               copia[i].favorite,
               copia[i].revenue);
    }

    free(copia);
}

int consultaPorCode(Filme *colecao, int count, int code) {
    int idx = indicePorCode(colecao, count, code);
    if (idx < 0) {
        printf("Filme com code %d nao encontrado.\n", code);
        return 0;
    }
    Filme *f = &colecao[idx];
    printf("Code: %d\nTitle: %s\nGenres: %s\nDescription: %s\nDirector: %s\nActors: %s\nYear: %d\nDuration: %d\nRating: %.1f\nFavorite: %d\nRevenue: %.2fM\n",
           f->code, f->title, f->genres, f->description, f->director, f->actors,
           f->year, f->duration, f->rating, f->favorite, f->revenue);
    return 1;
}

static int containsCaseInsensitive(const char *field, const char *pattern) {
    if (!pattern || pattern[0] == '\0') return 1;
    char F[512], P[256];
    toLowerCopy(field, F, sizeof(F));
    toLowerCopy(pattern, P, sizeof(P));
    return strstr(F, P) != NULL;
}

int pesquisaPorTitulo(Filme *colecao, int count, const char *substr) {
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (containsCaseInsensitive(colecao[i].title, substr)) {
            if (found == 0) {
                printf("Resultados:\nCode | Title\n");
            }
            printf("%4d | %s\n", colecao[i].code, colecao[i].title);
            found++;
        }
    }
    if (found == 0) printf("Nenhum resultado para o titulo.\n");
    return found;
}

int pesquisaPorGenero(Filme *colecao, int count, const char *genre) {
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (containsCaseInsensitive(colecao[i].genres, genre)) {
            if (found == 0) printf("Resultados (genre):\n");
            printf("%4d | %s | %s\n", colecao[i].code, colecao[i].title, colecao[i].genres);
            found++;
        }
    }
    if (found == 0) printf("Nenhum resultado para o genero.\n");
    return found;
}

int pesquisaPorRealizador(Filme *colecao, int count, const char *director) {
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (containsCaseInsensitive(colecao[i].director, director)) {
            if (found == 0) printf("Resultados (director):\n");
            printf("%4d | %s | %s\n", colecao[i].code, colecao[i].title, colecao[i].director);
            found++;
        }
    }
    if (found == 0) printf("Nenhum resultado para o realizador.\n");
    return found;
}

int pesquisaPorAtor(Filme *colecao, int count, const char *actor) {
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (containsCaseInsensitive(colecao[i].actors, actor)) {
            if (found == 0) printf("Resultados (actor):\n");
            printf("%4d | %s | %s\n", colecao[i].code, colecao[i].title, colecao[i].actors);
            found++;
        }
    }
    if (found == 0) printf("Nenhum resultado para o ator.\n");
    return found;
}

int alteraFilme(Filme *colecao, int count, int code, const Filme *novos) {
    int idx = indicePorCode(colecao, count, code);
    if (idx < 0) return 0;
    Filme *f = &colecao[idx];
    if (novos->title[0] != '\0') strncpy(f->title, novos->title, MAXTITLE);
    if (novos->genres[0] != '\0') strncpy(f->genres, novos->genres, MAXGENRES);
    if (novos->year != 0) f->year = novos->year;
    if (novos->duration != 0) f->duration = novos->duration;
    if (novos->rating >= 0.0f && novos->rating <= 10.0f) f->rating = novos->rating;
    if (novos->favorite >= 0) f->favorite = novos->favorite;
    if (novos->revenue >= 0.0f) f->revenue = novos->revenue;
    return 1;
}

int validaRating(float r) { return r >= 0.0f && r <= 10.0f; }
int validaYear(int y) { return y >= 1888 && y <= 3000; }
int validaDuration(int d) { return d > 0 && d <= 1000; }
