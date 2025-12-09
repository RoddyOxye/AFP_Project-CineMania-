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
    return (float)atof(s);
}

int main() {
    Filme lista[MAXFILMES];
    int n;

    inicializarColecao(lista, &n);

    char op[8];

    while (1) {
        printf("\n=== MENU ===\n");
        printf("1. Listar filmes\n");
        printf("2. Pesquisar filmes\n");
        printf("3. Consultar filme\n");
        printf("4. Adicionar filme\n");
        printf("0. Sair\n");

        lerString("Opcao: ", op, sizeof(op));

        if (strcmp(op, "0") == 0) {
            printf("A sair...\n");
            break;
        }

        if (strcmp(op, "1") == 0) {
            int ord = lerInt("Ordenar por (0=code, 1=rating, 2=title): ");
            listarTodos(lista, n, ord);
        }

        else if (strcmp(op, "2") == 0) {
            char texto[128];

            printf("a) Titulo\nb) Genero\nc) Realizador\nd) Ator\n");
            lerString("Escolha: ", op, sizeof(op));
            lerString("Texto: ", texto, sizeof(texto));

            if (op[0] == 'a') pesquisarTitulo(lista, n, texto);
            else if (op[0] == 'b') pesquisarGenero(lista, n, texto);
            else if (op[0] == 'c') pesquisarRealizador(lista, n, texto);
            else if (op[0] == 'd') pesquisarAtor(lista, n, texto);
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

            f.year = lerInt("Ano: ");
            f.duration = lerInt("Duracao: ");
            f.rating = lerFloat("Rating: ");

            int fav = lerInt("E favorito? (1=Sim, 0=Nao): ");
            f.favorite = (fav == 1);

            f.revenue = lerFloat("Receita (M): ");

            if (adicionarFilme(lista, &n, &f))
                printf("Filme adicionado com sucesso!\n");
            else
                printf("Erro: colecao cheia.\n");
        }

        else {
            printf("Opcao invalida.\n");
        }
    }

    return 0;
}
int main (){

    printf("  ____ ______  ___  ______________ _____  ______ _");
    printf(" / __// / __ \/ _ \/ __  __  / __ `/ __ \/ / __ `/");
    printf("/ /__/ / / / /  __/ / / / / / /_/ / / / / / /_/ /");
    printf("\___/_/_/ /_/\___/_/ /_/ /_/\__,_/_/ /_/_/\__,_/");
    
}
