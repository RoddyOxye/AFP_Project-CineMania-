#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "filmes.h"

int lerIntIntervalo(const char *msg, int min, int max);
float lerFloatIntervalo(const char *msg, float min, float max);

/* Read a line from stdin and remove the trailing newline. */
void lerString(const char *msg, char *dest, int max) {
    printf("%s", msg);
    fgets(dest, max, stdin);
    dest[strcspn(dest, "\n")] = '\0';
}

/* Wait for Enter before returning to the menu. */
void esperarEnter(void) {
    char tmp[4];
    lerString("\nPrima Enter para voltar ao menu...", tmp, sizeof(tmp));
}

static int soEspacos(const char *s) {
    for (int i = 0; s[i]; i++) {
        if (s[i] != ' ' && s[i] != '\t') return 0;
    }
    /* Only spaces/tabs were found. */
    return 1;
}

/* Read an integer and force numeric input. */
int lerInt(const char *msg) {
    char string[64];
    char *end = NULL;

    while (1) {
        lerString(msg, string, sizeof(string));
        /* Reject empty input or just spaces. */
        if (soEspacos(string)) {
            printf("Introduza um numero valido.\n");
            continue;
        }
        /* Convert and check if the whole input was a number. */
        long v = strtol(string, &end, 10);
        while (*end == ' ' || *end == '\t') end++;
        if (*end == '\0') return (int)v;
        printf("Introduza um numero inteiro valido.\n");
    }
}

/* Read a float and force numeric input. */
float lerFloat(const char *msg) {
    char string[64];
    char *end = NULL;

    while (1) {
        lerString(msg, string, sizeof(string));
        /* Reject empty input or just spaces. */
        if (soEspacos(string)) {
            printf("Introduza um numero valido.\n");
            continue;
        }
        /* Allow decimal comma by converting to dot. */
        for (int i = 0; string[i]; i++) {
            if (string[i] == ',') string[i] = '.';
        }
        /* Convert and check if the whole input was a number. */
        float v = strtof(string, &end);
        while (*end == ' ' || *end == '\t') end++;
        if (*end == '\0') return v;
        printf("Introduza um numero decimal valido.\n");
    }
}

/* Main menu loop. */
int main() {

    Filmes filmes[MAX_FILMES];
    int totalFilmes;

    inicializarColecao(filmes, &totalFilmes);
    
    char opcao[8];

    printf("  ____ ______  ___  ______________ _____  ______ _\n");
    printf(" / __// / __ \\/ _ \\/ __  __  / __ `/ __ \\/ / __ `/\n");
    printf("/ /__/ / / / /  __/ / / / / / /_/ / / / / / /_/ /\n");
    printf("\\___/_/_/ /_/\\___/_/ /_/ /_/\\__,_/_/ /_/_/\\__,_/\n");

    while(1) {
        printf("\n=== CINEMANIA ===\n");
        printf("1. Listar filmes\n");
        printf("2. Pesquisar filmes\n");
        printf("3. Consultar filme\n");
        printf("4. Adicionar filme\n");
        printf("5. Alterar filme\n");
        printf("6. Remover filme\n");
        printf("7. Limpar filmes\n");
        printf("8. Importar ficheiro\n");
        printf("9. Exportar ficheiro\n");
        printf("0. Sair\n\n");

        int opcaoMenu = lerIntIntervalo("Opcao: ", 0, 9);

            if (opcaoMenu == 0) {
                char confirmar[4]; /* string em vez de char porque assim o utilizador pode escrever "sim" ou "s", pois só verifica a primeira letra */
                lerString("Confirmar saida (s/n): ", confirmar, 4);
                if (confirmar[0] == 's' || confirmar[0] == 'S') break;
            }
            else if (opcaoMenu == 1) {
            listarFilmes(filmes, totalFilmes, lerIntIntervalo("\nOrdenar por Code (0), Rating (1), Title (2): ", 0, 2));
            esperarEnter();
            }
            else if (opcaoMenu == 2) {
                pesquisarFilmes(filmes, totalFilmes,
                    lerIntIntervalo("1-Titulo 2-Genero 3-Realizador 4-Ator: ", 1, 4),
                    ({ static char texto[128]; lerString("Texto: ", texto, 128); texto; }));
                esperarEnter();
            }
            else if (opcaoMenu == 3) {
            consultarFilme(filmes, totalFilmes, lerInt("Codigo: "));
            esperarEnter();
            }
            else if (opcaoMenu == 4) {
                Filmes novoFilme;

                lerString("Titulo: ", novoFilme.title, MAX_TITLE);
                lerString("Genero: ", novoFilme.gender, MAX_GENRE);
                lerString("Descricao: ", novoFilme.description, MAX_DESCRIPTION);
                lerString("Realizador: ", novoFilme.director, MAX_DIRECTOR);
                lerString("Atores: ", novoFilme.actors, MAX_ACTOR);
            novoFilme.year = lerInt("Ano: ");
            novoFilme.duration = lerInt("Duracao (minutos): ");
            novoFilme.rating = lerFloatIntervalo("Rating (0.0 - 10.0): ", 0.0f, 10.0f);
            novoFilme.favorites = lerInt("Numero de favoritos: ");
            novoFilme.revenue = lerFloat("Receita (em milhoes): ");
            adicionarFilme(filmes, &totalFilmes, novoFilme);
            printf("\nFilme adicionado com sucesso!\n");
            esperarEnter();
            }
            else if (opcaoMenu == 5) {
                int code = lerInt("Codigo do filme a alterar: ");
                Filmes novoFilme;

                lerString("Novo Titulo: ", novoFilme.title, MAX_TITLE);
                lerString("Novo Genero: ", novoFilme.gender, MAX_GENRE);
                lerString("Nova Descricao: ", novoFilme.description, MAX_DESCRIPTION);
                lerString("Novo Realizador: ", novoFilme.director, MAX_DIRECTOR);
                lerString("Novos Atores: ", novoFilme.actors, MAX_ACTOR);
            novoFilme.year = lerInt("Novo Ano: ");
            novoFilme.duration = lerInt("Nova Duracao (minutos): ");
            novoFilme.rating = lerFloatIntervalo("Novo Rating (0.0 - 10.0): ", 0.0f, 10.0f);
            novoFilme.favorites = lerInt("Novo Numero de favoritos: ");
            novoFilme.revenue = lerFloat("Nova Receita (em milhoes): ");    
            alterarFilme(filmes, totalFilmes, code, novoFilme);
            printf("\nFilme alterado com sucesso!\n");
            esperarEnter();
            }
            else if (opcaoMenu == 6) {
                removerFilme(filmes, &totalFilmes, lerInt("Codigo do filme a remover: "));
                printf("\nFilme removido com sucesso!\n");
                esperarEnter();
            }
            else if (opcaoMenu == 7) {
                limparFilmes(filmes, &totalFilmes);
                printf("\nTodos os filmes foram removidos!\n");
                esperarEnter();
            }
        else if (opcaoMenu == 8) {
            int importados = importarFicheiro(filmes, &totalFilmes,
                ({ static char filename[128]; lerString("Nome do ficheiro para importar: ", filename, 128); filename; }));
            if (importados > 0) {
                printf("\nImportados %d filmes.\n", importados);
            } else {
                printf("\nFalha na importacao ou nenhum filme importado.\n");
            }
            esperarEnter();
        }
            else if (opcaoMenu == 9) {
              exportarFicheiro(filmes, totalFilmes,
                    ({ static char filename[128]; lerString("Nome do ficheiro para exportar: ", filename, 128); filename; }));
                printf("\nFicheiro exportado com sucesso!\n");
                esperarEnter();
            }
            else {
                printf("\nOpcao invalida. Tente novamente.\n");
            }    
        }
    return 0;
}
/* Read an integer within a range (inclusive). */
int lerIntIntervalo(const char *msg, int min, int max) {
    while (1) {
        int v = lerInt(msg);
        /* Keep asking until the value is inside [min, max]. */
        if (v >= min && v <= max) return v;
        printf("Introduza um numero entre %d e %d.\n", min, max);
    }
}

/* Read a float within a range (inclusive). */
float lerFloatIntervalo(const char *msg, float min, float max) {
    while (1) {
        float v = lerFloat(msg);
        /* Keep asking until the value is inside [min, max]. */
        if (v >= min && v <= max) return v;
        printf("Introduza um numero entre %.1f e %.1f.\n", min, max);
    }
}
