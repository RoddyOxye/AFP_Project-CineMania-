#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "filmes.h"

/* Read a line from stdin and remove the trailing newline. */
void lerString(const char *msg, char *dest, int max) {
    printf("%s", msg);
    fgets(dest, max, stdin);
    dest[strcspn(dest, "\n")] = '\0';
}

/* Read an integer from stdin (simple conversion). */
int lerInt(const char *msg) {
    char string[32];
    lerString(msg, string, sizeof(string));
    return atoi(string);
}

/* Read a float from stdin (simple conversion). */
float lerFloat(const char *msg) {
    char string[32];
    lerString(msg, string, sizeof(string));
    return atof(string);
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

        lerString("Opcao: ", opcao, sizeof(opcao));

            if (strcmp(opcao, "0") == 0) {
                char confirmar[4]; /* string em vez de char porque assim o utilizador pode escrever "sim" ou "s", pois só verifica a primeira letra */
                lerString("Confirmar saida (s/n): ", confirmar, 4);
                if (confirmar[0] == 's' || confirmar[0] == 'S') break;
            }
            else if (strcmp(opcao, "1") == 0) {
            listarFilmes(filmes, totalFilmes, lerInt("\nOrdenar por Code (0), Rating (1), Title (2): "));  
            }
            else if (strcmp(opcao, "2") == 0) {
            pesquisarFilmes(filmes, totalFilmes,
                lerInt("1-Titulo 2-Genero 3-Realizador 4-Ator: "),
                ({ static char texto[128]; lerString("Texto: ", texto, 128); texto; }));
            }
            else if (strcmp(opcao, "3") == 0) {
            consultarFilme(filmes, totalFilmes, lerInt("Codigo: "));
            }
            else if (strcmp(opcao, "4") == 0) {
                Filmes novoFilme;

                lerString("Titulo: ", novoFilme.title, MAX_TITLE);
                lerString("Genero: ", novoFilme.gender, MAX_GENRE);
                lerString("Descricao: ", novoFilme.description, MAX_DESCRIPTION);
                lerString("Realizador: ", novoFilme.director, MAX_DIRECTOR);
                lerString("Atores: ", novoFilme.actors, MAX_ACTOR);
                novoFilme.year = lerInt("Ano: ");
                novoFilme.duration = lerInt("Duracao (minutos): ");
                novoFilme.rating = lerFloat("Rating (0.0 - 10.0): ");
                novoFilme.favorites = lerInt("Numero de favoritos: ");
                novoFilme.revenue = lerFloat("Receita (em milhoes): ");
                adicionarFilme(filmes, &totalFilmes, novoFilme);
                printf("\nFilme adicionado com sucesso!\n");
            }
            else if (strcmp(opcao, "5") == 0) {
                int code = lerInt("Codigo do filme a alterar: ");
                Filmes novoFilme;

                lerString("Novo Titulo: ", novoFilme.title, MAX_TITLE);
                lerString("Novo Genero: ", novoFilme.gender, MAX_GENRE);
                lerString("Nova Descricao: ", novoFilme.description, MAX_DESCRIPTION);
                lerString("Novo Realizador: ", novoFilme.director, MAX_DIRECTOR);
                lerString("Novos Atores: ", novoFilme.actors, MAX_ACTOR);
                novoFilme.year = lerInt("Novo Ano: ");
                novoFilme.duration = lerInt("Nova Duracao (minutos): ");
                novoFilme.rating = lerFloat("Novo Rating (0.0 - 10.0): ");
                novoFilme.favorites = lerInt("Novo Numero de favoritos: ");
                novoFilme.revenue = lerFloat("Nova Receita (em milhoes): ");    
                alterarFilme(filmes, totalFilmes, code, novoFilme);
                printf("\nFilme alterado com sucesso!\n");
            }
            else if (strcmp(opcao, "6") == 0) {
                removerFilme(filmes, &totalFilmes, lerInt("Codigo do filme a remover: "));
                printf("\nFilme removido com sucesso!\n");
            }
            else if (strcmp(opcao, "7") == 0) {
                limparFilmes(filmes, &totalFilmes);
                printf("\nTodos os filmes foram removidos!\n");
            }
        else if (strcmp(opcao, "8") == 0) {
            int importados = importarFicheiro(filmes, &totalFilmes,
                ({ static char filename[128]; lerString("Nome do ficheiro para importar: ", filename, 128); filename; }));
            if (importados > 0) {
                printf("\nImportados %d filmes.\n", importados);
            } else {
                printf("\nFalha na importacao ou nenhum filme importado.\n");
            }
        }
            else if (strcmp(opcao, "9") == 0) {
              exportarFicheiro(filmes, totalFilmes,
                    ({ static char filename[128]; lerString("Nome do ficheiro para exportar: ", filename, 128); filename; }));
                printf("\nFicheiro exportado com sucesso!\n");
            }
            else {
                printf("\nOpcao invalida. Tente novamente.\n");
            }    
        }
    return 0;
}
