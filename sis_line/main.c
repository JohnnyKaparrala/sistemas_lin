#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main() {
	char opcao;
	FILE *f;
	char c = 0;
	int8_t qtdSistemas= 0;
	int linha = 0;
	char* stringSistema;
    int *coeficientes;
    char **nomesVariaveis;
    int qtdLetras = 0;
    int termo = 0;

	do {
        f = fopen("teste.txt", "r");
	    qtdSistemas = (int8_t)fgetc(f) - '0';// converte o primeiro char para int

        coeficientes = (int *) malloc(sizeof(int) * qtdSistemas);
        nomesVariaveis = malloc(qtdSistemas * sizeof(char*));

        printf("%d", qtdSistemas);

        stringSistema = malloc(sizeof(char) * 256);
        int len = 0;

		while ( (c = fgetc(f))!= EOF ) {
            if (c != ' ' && c != '*') {
                stringSistema += c;
                len++;
            }
        }
        stringSistema = malloc(sizeof(char) * 256);

        printf("\n%s", stringSistema);

		printf("\nSair do programa? (s/n)\n");
		scanf(" %c", &opcao);
	} while (opcao == 'n');

	return 0;
}
