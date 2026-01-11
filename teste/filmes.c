#include "filmes.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_IMPORT_BYTES (1024 * 1024)
#define MAX_LINE_LEN 4096

static void trim_whitespace(char *s) {
    size_t len = strlen(s);
    size_t start = 0;
    while (s[start] && isspace((unsigned char)s[start])) {
        start++;
    }
    while (len > start && isspace((unsigned char)s[len - 1])) {
        len--;
    }
    if (start > 0) {
        memmove(s, s + start, len - start);
    }
    s[len - start] = '\0';
}

static void normalize_decimal(char *s) {
    for (size_t i = 0; s[i]; i++) {
        if (s[i] == ',') s[i] = '.';
    }
}

static int parse_csv_line(const char *line, char fields[][512], int max_fields) {
    int f = 0;
    int i = 0;
    while (line[i] && f < max_fields) {
        int pos = 0;
        if (line[i] == '"') {
            i++;
            while (line[i] && line[i] != '\n' && line[i] != '\r') {
                if (line[i] == '"' && line[i + 1] == '"') {
                    if (pos < 511) fields[f][pos++] = '"';
                    i += 2;
                    continue;
                }
                if (line[i] == '"') {
                    i++;
                    break;
                }
                if (pos < 511) fields[f][pos++] = line[i];
                i++;
            }
        } else {
            while (line[i] && line[i] != ';' && line[i] != '\n' && line[i] != '\r') {
                if (pos < 511) fields[f][pos++] = line[i];
                i++;
            }
        }
        fields[f][pos] = '\0';
        if (line[i] == ';') i++;
        f++;
        while (line[i] == '\r' || line[i] == '\n') i++;
    }
    return f;
}

static int is_header_line(const char *field0) {
    char tmp[32];
    size_t n = strlen(field0);
    if (n >= sizeof(tmp)) n = sizeof(tmp) - 1;
    for (size_t i = 0; i < n; i++) {
        tmp[i] = (char)tolower((unsigned char)field0[i]);
    }
    tmp[n] = '\0';
    return strcmp(tmp, "code") == 0 || strcmp(tmp, "codigo") == 0;
}

static int has_xlsx_extension(const char *filename) {
    const char *dot = strrchr(filename, '.');
    if (!dot) return 0;
    return strcmp(dot, ".xlsx") == 0 || strcmp(dot, ".XLSX") == 0;
}

/*Converter string para minúsculas*/
static void toLower(const char *textoOriginal, char *textoConvertido) {
    int i = 0;
    while (textoOriginal[i]) {
        textoConvertido[i] = tolower((unsigned char)textoOriginal[i]);
        i++;
    }
    textoConvertido[i] = '\0';
}

/* Pesquisa ignorando maiúsculas/minúsculas */
static int contem(const char *campo, const char *pesquisa) {
    char campoAux[512], pesquisaAux[256];
    toLower(campo, campoAux);
    toLower(pesquisa, pesquisaAux);
    return strstr(campoAux, pesquisaAux) != NULL;
}

void inicializarColecao(Filmes *colecaoFilmes, int *numFilmes) {
    *numFilmes = 0;
}

/* Adiciona filme com código automático */
int adicionarFilme(Filmes *colecaoFilmes, int *numFilmes, Filmes novoFilme) {
    if (*numFilmes >= MAX_FILMES) {
        return 0; // Coleção cheia
    }
    colecaoFilmes[*numFilmes] = novoFilme;
    (*numFilmes)++;
    return 1; // Filme adicionado com sucesso
}

/* Listagem com ordenação simples (bubble sort) */
void listarFilmes(Filmes *colecaoFilmes, int numFilmes, int order) {
    if (numFilmes == 0) {
        printf("Nenhum filme registrado.\n");
        return;
    }

    for (int i = 0; i < numFilmes - 1; i++) {
        for (int j = 0; j < numFilmes - 1 - i; j++) {
            int troca = 0;

            if (order == 0 && colecaoFilmes[j].code > colecaoFilmes[j + 1].code) troca = 1;
            if (order == 1 && colecaoFilmes[j].rating < colecaoFilmes[j + 1].rating) troca = 1;
            if (order == 2 && strcmp(colecaoFilmes[j].title, colecaoFilmes[j + 1].title) > 0) troca = 1;

            if (troca) {
                Filmes temp = colecaoFilmes[j];
                colecaoFilmes[j] = colecaoFilmes[j + 1];
                colecaoFilmes[j + 1] = temp;
            }
        }
    }

    printf("\n%-5s | %-40s | %-4s | %-6s\n", "Codigo", "Titulo", "Ano", "Rating");
    printf("----- | ---------------------------------------- | ---- | ------\n");
    for (int i = 0; i < numFilmes; i++) {
        printf("%5d | %-40s | %4d | %6.1f\n",
               colecaoFilmes[i].code, colecaoFilmes[i].title,
               colecaoFilmes[i].year, colecaoFilmes[i].rating);
    }
}

int consultarFilme(Filmes *colecaoFilmes, int numFilmes, int code) {
    for (int i = 0; i < numFilmes; i++) {
        if (colecaoFilmes[i].code == code) {
            Filmes filme = colecaoFilmes[i];

             printf("\nTitulo: %s\nGeneros: %s\nDescricao: %s\n",
                   filme.title, filme.gender, filme.description);
            printf("Realizador: %s\nAtores: %s\n", filme.director, filme.actors);
            printf("Ano: %d | Duracao: %d\nRating: %.1f\n",
                   filme.year, filme.duration, filme.rating);
            printf("Favoritos: %d | Receita: %.2f M\n",
                   filme.favorites, filme.revenue);
            return 1;
        }
    }
    printf("Filme com codigo %d nao encontrado.\n", code);
    return 0; // Filme não encontrado
}

void pesquisarFilmes(Filmes *colecaoFilmes, int numFilmes, int tipoPesquisa, char *pesquisa) {
    for (int i = 0; i < numFilmes; i++) {
       if ((tipoPesquisa == 1 && contem(colecaoFilmes[i].title, pesquisa)) ||
           (tipoPesquisa == 2 && contem(colecaoFilmes[i].gender, pesquisa)) ||
           (tipoPesquisa == 3 && contem(colecaoFilmes[i].director, pesquisa)) ||
           (tipoPesquisa == 4 && contem(colecaoFilmes[i].actors, pesquisa))) {
                printf("%4d | %s\n", colecaoFilmes[i].code, colecaoFilmes[i].title);
        }
    }
}

int alterarFilme(Filmes *colecaoFilmes, int numFilmes, int code, Filmes novo) {
   for(int i = 0; i < numFilmes; i++) {
       if (colecaoFilmes[i].code == code) {
           colecaoFilmes[i] = novo;
           colecaoFilmes[i].code = code; // Mantém o código original
           return 1; // Filme alterado com sucesso
       }
   }
   return 0; // Filme não encontrado
}

int removerFilme(Filmes *colecaoFilmes, int *numFilmes, int code) {
    for (int i = 0; i < *numFilmes; i++) {
        if (colecaoFilmes[i].code == code) {
            for (int j = i; j < *numFilmes - 1; j++) {
                colecaoFilmes[j] = colecaoFilmes[j + 1];
            }
            (*numFilmes)--;
            return 1; // Filme removido com sucesso
        }
    }
    return 0; // Filme não encontrado
}

void limparFilmes(Filmes *colecaoFilmes, int *numFilmes) {
    *numFilmes = 0;
}

int importarFicheiro(Filmes *colecaoFilmes, int *numFilmes, const char *filename) {
    if (has_xlsx_extension(filename)) {
        printf("Formato .xlsx nao suportado. Exporte para .csv.\n");
        return 0;
    }

    FILE *f = fopen(filename, "r");
    if (!f) {
        return 0; // Falha ao abrir o ficheiro
    }

    if (fseek(f, 0, SEEK_END) == 0) {
        long size = ftell(f);
        rewind(f);
        if (size > MAX_IMPORT_BYTES) {
            printf("Ficheiro muito grande (%.2f MB). Confirmar importacao (s/n): ",
                   size / (1024.0 * 1024.0));
            char resp[8];
            if (!fgets(resp, sizeof(resp), stdin) ||
                (resp[0] != 's' && resp[0] != 'S')) {
                fclose(f);
                return 0;
            }
        }
    }

    int importados = 0;
    char line[MAX_LINE_LEN];
    while (fgets(line, sizeof(line), f)) {
        char campos[11][512];
        int total = parse_csv_line(line, campos, 11);
        if (total < 11) continue;
        if (is_header_line(campos[0])) continue;

        for (int i = 0; i < 11; i++) {
            trim_whitespace(campos[i]);
        }
        normalize_decimal(campos[8]);
        normalize_decimal(campos[10]);

        if (*numFilmes >= MAX_FILMES) break;

        Filmes filmeTemp;
        filmeTemp.code = (int)strtol(campos[0], NULL, 10);
        strncpy(filmeTemp.title, campos[1], MAX_TITLE - 1);
        filmeTemp.title[MAX_TITLE - 1] = '\0';
        strncpy(filmeTemp.gender, campos[2], MAX_GENRE - 1);
        filmeTemp.gender[MAX_GENRE - 1] = '\0';
        strncpy(filmeTemp.description, campos[3], MAX_DESCRIPTION - 1);
        filmeTemp.description[MAX_DESCRIPTION - 1] = '\0';
        strncpy(filmeTemp.director, campos[4], MAX_DIRECTOR - 1);
        filmeTemp.director[MAX_DIRECTOR - 1] = '\0';
        strncpy(filmeTemp.actors, campos[5], MAX_ACTOR - 1);
        filmeTemp.actors[MAX_ACTOR - 1] = '\0';
        filmeTemp.year = (int)strtol(campos[6], NULL, 10);
        filmeTemp.duration = (int)strtol(campos[7], NULL, 10);
        filmeTemp.rating = strtof(campos[8], NULL);
        filmeTemp.favorites = (int)strtol(campos[9], NULL, 10);
        filmeTemp.revenue = strtof(campos[10], NULL);

        colecaoFilmes[*numFilmes] = filmeTemp;
        (*numFilmes)++;
        importados++;
    }
    fclose(f);
    return importados; // Numero de filmes importados
}

int exportarFicheiro(Filmes *colecaoFilmes, int numFilmes, const char *filename) {
    FILE *f = fopen(filename, "w");
    if (!f) {
        return 0; // Falha ao abrir o ficheiro
    }

     for (int i = 0; i < numFilmes; i++) {
        fprintf(f, "%d;%s;%s;%s;%s;%s;%d;%d;%.1f;%d;%.2f\n",
            colecaoFilmes[i].code, colecaoFilmes[i].title, colecaoFilmes[i].gender, colecaoFilmes[i].description,
            colecaoFilmes[i].director, colecaoFilmes[i].actors, colecaoFilmes[i].year, colecaoFilmes[i].duration,
            colecaoFilmes[i].rating, colecaoFilmes[i].favorites, colecaoFilmes[i].revenue);
    }
    fclose(f);
    return 1;
}


