#include <stdio.h>
#include <stdlib.h>

int main() {
	char opcao;
	FILE *f;
	char c = 0;
	f = fopen("teste.txt", "r");

	do {
		while((c=fgetc(f))!=EOF){
            printf("%c",c);
        }

		printf("\nSair do programa? (s/n)\n");
		scanf(" %c", &opcao);
	} while (opcao == 'n');

	return 0;
}
