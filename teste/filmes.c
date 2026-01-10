#include "filmes.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

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

void inicializaColecao(Filmes *colecaoFilmes, int *numFilmes) {
    *numFilmes = 0;
}

/* Adiciona filme com código automático */
int adicionaFilme(Filmes *colecaoFilmes, int *numFilmes, Filmes novoFilme) {
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

    Filmes filmesTemp[MAX_FILMES];

    for (int i = 0; i < numFilmes; i++) {
        filmesTemp[i] = colecaoFilmes[i];
    }

    for(int i = 0; i < numFilmes - 1; i++) {
        for( int j = 0; j < numFilmes - 1 - i; j++) {

            int troca = 0;

            if (order == 0 && filmesTemp[j].code > filmesTemp[j + 1].code) {
                troca = 1;
            }

            if (order == 1 && filmesTemp[j].rating < filmesTemp[j + 1].rating) {
                troca = 1;
            }

            if(order == 2 && strcmp(filmesTemp[j].title, filmesTemp[j + 1].title) > 0) {
                troca = 1;
            }

            if (troca) {
                Filmes temp = filmesTemp[j];
                filmesTemp[j] = filmesTemp[j + 1];
                filmesTemp[j + 1] = temp;
            }
        }
    }

    printf("\nCódigo | Titulo                          | Ano | Rating\n");
    printf("----------------------------------------------------\n");

    for (int i = 0; i < numFilmes; i++) {
        printf("%4d | %-30s | %4d | %.1f\n",
            filmesTemp[i].code, filmesTemp[i].title,
            filmesTemp[i].year, filmesTemp[i].rating);
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

void pesquisaFilmes(Filmes *colecaoFilmes, int numFilmes, int tipoPesquisa, char *pesquisa) {
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
    FILE *f = fopen(filename, "r"); 
    if (!f) {
        return 0; // Falha ao abrir o ficheiro
    }

    Filmes filmeTemp;
    while (fscanf(f, "%d;%127[^;];%63[^;];%255[^;];%63[^;];%127[^;];%d;%d;%f;%d;%f\n",
        &filmeTemp.code, filmeTemp.title, filmeTemp.gender, filmeTemp.description,
        filmeTemp.director, filmeTemp.actors, &filmeTemp.year, &filmeTemp.duration,
        &filmeTemp.rating, &filmeTemp.favorites, &filmeTemp.revenue) == 11) {

        colecaoFilmes[*numFilmes] = filmeTemp;
        (*numFilmes)++;
    }
    fclose(f);
    return 1; // Importação bem-sucedida
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


