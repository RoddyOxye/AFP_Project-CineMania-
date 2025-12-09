#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "filmes.h"

void lerString(const char *msg, char *dest, int max) {
    printf("%s", msg);
    fgets(dest, max, stdin);
    dest[strcspn(dest, "\n")] = '\0';
}

int lerInt(const char *msg) {
    char s[32];
    lerString(msg, s, sizeof(s));
    return atoi(s);
}

float lerFloat(const char *msg) {
    char s[32];
    lerString(msg, s, sizeof(s));
    return atof(s);
}

int main() {
    Filme lista[MAXFILMES];
    int n;
    inicializarColecao(lista, &n);

    char op[8];

    while (1) {
        printf("  ____ ______  ___  ______________ _____  ______ _\n");
        printf(" / __// / __ \\/ _ \\/ __  __  / __ `/ __ \\/ / __ `/\n");
        printf("/ /__/ / / / /  __/ / / / / / /_/ / / / / / /_/ /\n");
        printf("\\___/_/_/ /_/\\___/_/ /_/ /_/\\__,_/_/ /_/_/\\__,_/\n");
        printf("\n=== MENU ===\n");
        printf("1. Listar filmes\n");
        printf("2. Pesquisar filmes\n");
        printf("3. Consultar filme\n");
        printf("4. Adicionar filme\n");
        printf("0. Sair\n");

        lerString("Opcao: ", op, sizeof(op));

        if (strcmp(op, "0") == 0) break;

        else if (strcmp(op, "1") == 0) {
            int ord = lerInt("Ordenar (0=code, 1=rating, 2=title): ");
            listarTodos(lista, n, ord);
        }

        else if (strcmp(op, "2") == 0) {
            char txt[128];
            printf("a) Titulo\nb) Genero\nc) Realizador\nd) Ator\n");
            lerString("Escolha: ", op, sizeof(op));
            lerString("Texto: ", txt, sizeof(txt));

            if (op[0] == 'a') pesquisarTitulo(lista, n, txt);
            else if (op[0] == 'b') pesquisarGenero(lista, n, txt);
            else if (op[0] == 'c') pesquisarRealizador(lista, n, txt);
            else if (op[0] == 'd') pesquisarAtor(lista, n, txt);
            else printf("Opcao invalida.\n");
        }

        else if (strcmp(op, "3") == 0) {
            int code = lerInt("Code do filme: ");
            consultarCode(lista, n, code);
        }

        else if (strcmp(op, "4") == 0) {
            Filme f;

            lerString("Titulo: ", f.title, MAXTITLE);
            lerString("Generos: ", f.genres, MAXGENRES);
            lerString("Descricao: ", f.description, MAXDESCRIPTION);
            lerString("Realizador: ", f.director, MAXDIRECTOR);
            lerString("Atores: ", f.actors, MAXACTORS);

            f.year     = lerInt("Ano: ");
            f.duration = lerInt("Duracao: ");
            f.rating   = lerFloat("Rating: ");

            int fav = lerInt("Favorito? (1=sim,0=nao): ");
            f.favorite = (fav == 1);

            f.revenue = lerFloat("Receita (M): ");

            if (adicionarFilme(lista, &n, &f))
                printf("Filme adicionado.\n");
            else
                printf("Erro: cheio.\n");
        }

        else {
            printf("Opcao invalida.\n");
        }
    }

    return 0;
}
