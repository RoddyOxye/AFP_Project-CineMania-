#ifndef FILMESH
#define FILMESH

#define MAX_FILMES 2000
#define MAX_TITLE 128
#define MAX_GENRE 64
#define MAX_DESCRIPTION 256
#define MAX_DIRECTOR 64
#define MAX_ACTOR 128

typedef struct 
{
    int code;
    char title[MAX_TITLE];
    char gender[MAX_GENRE];
    char description[MAX_DESCRIPTION];
    char director[MAX_DIRECTOR];
    char actors[MAX_ACTOR];
    int year;
    int duration;
    float rating;
    int favorites;
    float revenue;
} Filmes;

/*Fase 1*/

void inicializarColecao(Filmes *colecaoFilmes, int *numFilmes);

int adicionarFilme(Filmes *colecaoFilmes, int *numFilmes, Filmes novoFilme);

void listarFilmes(Filmes *colecaoFilmes, int numFilmes, int order);

int consultarFilme(Filmes *colecaoFilmes, int numFilmes, int code);

void pesquisarFilmes(Filmes *colecaoFilmes, int numFilmes, int tipoPesquisa, char *pesquisa);

/*Fase 2*/

int alterarFilme(Filmes *colecaoFilmes, int numFilmes, int code, Filmes novo);

int removerFilme(Filmes *colecaoFilmes, int *numFilmes, int code);

void limparFilmes(Filmes *colecaoFilmes, int *numFilmes);

int importarFicheiro(Filmes *colecaoFilmes, int *numFilmes, const char *filename);

int exportarFicheiro(Filmes *colecaoFilmes, int numFilmes, const char *filename);

#endif