#ifndef FILMESH
#define FILMESH

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

void inicializarColecao(Filme *colecao, int *count);

int adicionarFilme(Filme *colecao, int *count, const Filme *novo);

int consultarCode(Filme *colecao, int count, int code);

void listarTodos(Filme *colecao, int count, int orden);

int pesquisarTitulo(Filme *colecao, int count, const char *substr);
int pesquisarGenero(Filme *colecao, int count, const char *genre);
int pesquisarRealizador(Filme *colecao, int count, const char *director);
int pesquisarAtor(Filme *colecao, int count, const char *actor);

#endif