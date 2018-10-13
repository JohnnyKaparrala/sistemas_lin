#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#define MAX_LENGTH 32000

typedef enum { false, true } bool;

char opcao;
FILE *f;
int qtdSistemas = 0;
char *qtdSistemasString;
char* stringSistema;
char ch = 0;
int **matrizSistema;
void addChar(char c, char *s);
char *criarString();

bool lendoNum = false;
bool lendoVar = false;
bool numNegativo = false;
char* stringNumAtual;
char* stringVarAtual;
int posAtualLinha;
int posAtualColuna;

int main() {
	do {
		f = fopen("C:/temp/teste.txt", "r");

		if (f == NULL)
		{
			printf("erro: %s\n", strerror(errno));
			return -1;
		}
		qtdSistemasString = criarString();

		ch = fgetc(f);
		while (ch != '\n') {
			addChar (ch, qtdSistemasString);
			ch = fgetc(f);
		}

		//criar as listas pra salvar o nome das variaveis
		qtdSistemas = atoi(qtdSistemasString);
		printf("%d", qtdSistemas);

		matrizSistema = (int)malloc(sizeof(int) * qtdSistemas + 1);//mais um pra colocar o resultado
		do
		{
			ch = (char)fgetc(f);
			if (ch != '*' && ch != ' ') {
				if (ch == "-")
					numNegativo = true;

				if ((ch == "+" || ch == "-" || ch == "=") && lendoVar) { // acabou a var, insere ela na litsa
					lendoVar = false;
				}

				if (isnumber(ch)) {
					if (!lendoNum) {
						lendoNum = true;
						stringNumAtual = criarString();
					}
					addChar(ch, stringNumAtual);
				}
				else if (isdigit(ch)) {
					if (!lendoVar) {
						lendoVar = true;
						stringVarAtual = criarString();
					}
					if (lendoNum) {//estava lendo um num
						//val a ser inserido recebe o coef
					}
					else {
						//val a ser inserido recebe 1
					}

					//insere o valor neg ou pos
					lendoNum = false;
				}
				else if (ch == '\n') {
					posAtualLinha++;
					posAtualColuna = 0;
					//insere o num atual
					lendoNum = false;
					lendoVar = false;
				}

			}
		} while (ch != EOF);

		printf("%s", stringSistema);

		printf("\nSair do programa? (s/n)\n");
		scanf(" %c", &opcao);
	} while (opcao == 'n');

	return 0;
}

void addChar(char c, char *s) {
	int len = strlen(s);
	*(s + len) = c;
	*(s + len + 1) = '\0';
}

char* criarString() {
	char *s = malloc(MAX_LENGTH);
	*s = '\0';
	return s;
}