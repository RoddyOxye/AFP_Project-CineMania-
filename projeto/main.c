#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "filmes.h"

// Função para ler strings
void lerString(const char *msg, char *dest, int max) {
    printf("%s", msg);
    fgets(dest, max, stdin);
    dest[strcspn(dest, "\n")] = '\0';
}

// Função para ler inteiros como strings
int lerInt(const char *msg) {
    char s[32];
    lerString(msg, s, sizeof(s));
    return atoi(s);
}

// Função para ler floats como strings
float lerFloat(const char *msg) {
    char s[32];
    lerString(msg, s, sizeof(s));
    return atof(s);
}

int main() {

    Filme lista[MAXFILMES];
    int n;
    inicializarColecao(lista, &n);

    char op[16];

    printf("  ____ ______  ___  ______________ _____  ______ _\n");
    printf(" / __// / __ \\/ _ \\/ __  __  / __ `/ __ \\/ / __ `/\n");
    printf("/ /__/ / / / /  __/ / / / / / /_/ / / / / / /_/ /\n");
    printf("\\___/_/_/ /_/\\___/_/ /_/ /_/\\__,_/_/ /_/_/\\__,_/\n");

    while (1) {
        printf("\n===== MENU =====\n");
        printf("1. Listar filmes\n");
        printf("2. Pesquisar filmes\n");
        printf("3. Consultar filme\n");
        printf("4. Adicionar filme\n");
        printf("0. Sair\n\n");

        lerString("Opcao: ", op, sizeof(op));

        if (!strcmp(op, "0")) {
            printf("\nTem a certeza que pretende sair? (S/N): ");
            char c = getchar();

            if (c == 'S' || c == 's')
            {
                printf("  ____ ______  ___  ______________ _____  ______ _\n");
                printf(" / __// / __ \\/ _ \\/ __  __  / __ `/ __ \\/ / __ `/\n");
                printf("/ /__/ / / / /  __/ / / / / / /_/ / / / / / /_/ /\n");
                printf("\\___/_/_/ /_/\\___/_/ /_/ /_/\\__,_/_/ /_/_/\\__,_/\n\n");
                break;
            } else {
                printf("\nOperacao cancelada.\n");
                while (getchar() != '\n'); // Clear input buffer
            }
            
            
        }

        else if (!strcmp(op, "1")) {
            int ord = lerInt("\nOrdenar por Code (0), Rating (1), Title (2): ");
            listarTodos(lista, n, ord);
        }

        else if (!strcmp(op, "2")) {
            char txt[128];
            printf("\nPesquisar por:\n");
            printf("a) Titulo\n");
            printf("b) Genero\n");
            printf("c) Realizador\n");
            printf("d) Ator\n\n");
            lerString("Escolha: ", op, sizeof(op));
            lerString("\nTexto: ", txt, sizeof(txt));

            if (op[0] == 'a') pesquisarTitulo(lista, n, txt);
            else if (op[0] == 'b') pesquisarGenero(lista, n, txt);
            else if (op[0] == 'c') pesquisarRealizador(lista, n, txt);
            else if (op[0] == 'd') pesquisarAtor(lista, n, txt);
            else printf("\nOpcao invalida.\n");
        }

        else if (!strcmp(op, "3")) {
            int code = lerInt("\nCode do filme: ");
            consultarCode(lista, n, code);
        }

        else if (!strcmp(op, "4")) {
            Filme f;

            lerString("\nTitulo: ", f.title, MAXTITLE);
            lerString("Generos: ", f.genres, MAXGENRES);
            lerString("Descricao: ", f.description, MAXDESCRIPTION);
            lerString("Realizador: ", f.director, MAXDIRECTOR);
            lerString("Atores: ", f.actors, MAXACTORS);

            f.year = lerInt("Ano: ");
            f.duration = lerInt("Duracao: ");
            f.rating = lerFloat("Rating: ");

            int fav = lerInt("Favorito? (1=Sim, 0=Nao): ");
            f.favorite = (fav == 1);

            f.revenue = lerFloat("Receita (M): ");

            if (adicionarFilme(lista, &n, &f))
                printf("\nFilme adicionado.\n");
            else
                printf("\nErro: lista cheia.\n");
        }

        else {
            printf("\nOpcao invalida.\n");
        }
    }

    return 0;
}