#ifndef FILMESH
#define FILMESH
#include <stdio.h>
#include <stdbool.h>

#define MAXFILMES 2000
#define MAXTITLE 128
#define MAXGENRES 64
#define MAXDESCRIPTION 256
#define MAXDIRECTOR 64
#define MAXACTORS 128

typedef struct Filme {
    int code;
    char title[MAXTITLE];
    char genres[MAXGENRES];
    char description[MAXDESCRIPTION];
    char director[MAXDIRECTOR];
    char actors[MAXACTORS];
    int year;
    int duration;
    float rating;
    bool favorite;
    float revenue;
} Filme;

void inicializaColecao(Filme *colecao, int *count);

int adicionaFilme(Filme *colecao, int *count, const Filme *novo);

int eliminaFilme(Filme *colecao, int *count, int code);

void limpaColecao(Filme *colecao, int *count);

void listaTodos(Filme *colecao, int count, int orden);

int consultaPorCode(Filme *colecao, int count, int code);

int pesquisaPorTitulo(Filme *colecao, int count, const char *substr);
int pesquisaPorGenero(Filme *colecao, int count, const char *genre);
int pesquisaPorRealizador(Filme *colecao, int count, const char *director);
int pesquisaPorAtor(Filme *colecao, int count, const char *actor);

int alteraFilme(Filme *colecao, int count, int code, const Filme *novos);

int validaRating(float r);
int validaYear(int y);
int validaDuration(int d);

#endif
