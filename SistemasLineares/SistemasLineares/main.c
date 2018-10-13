#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#define MAX_LENGTH 32000

typedef enum { false, true } bool;

void addChar(char c, char *s);
char *criarString();
void printIntMatrix(int** matrix, int columns, int rows);
bool isNumber(char c);
int getPos(char* s, char **strArray);

char opcao;
FILE *f;
int qtdSistemas = 0;
char *qtdSistemasString;
char* stringSistema;
char ch = 0;
int **matrizSistema;

bool lendoNum = false;
bool lendoVar = false;
bool numNegativo = false;
char* stringNumAtual;
char* stringVarAtual;
int posAtualLinha = 0;
int posAtualColuna = -1;
int numASerInserido;
int posVar = -1;

char **vars;

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
		printf("%d\n\n", qtdSistemas);

		vars = (char**)malloc(qtdSistemas);

		matrizSistema = (int**)malloc(sizeof(int*) * qtdSistemas + 1);//mais um pra colocar o resultado
		for (int i = 0; i < qtdSistemas + 1; i++) {
			matrizSistema[i] = (int*)malloc(sizeof(int*) * qtdSistemas);
		}

		for (int i = 0;i < qtdSistemas + 1;i++) {//enche a matriz com 0
			for (int j = 0;j < qtdSistemas;j++) {
				matrizSistema[i][j] = 0;
			}
		}

		do
		{
			ch = (char)fgetc(f);
			if (ch != '*' && ch != ' ') {
				if (ch == '-')
					numNegativo = true;

				if ((ch == '+' || ch == '-' || ch == '=') && lendoVar) { // acabou a var, insere ela na litsa
					lendoVar = false;

					//insere o valor neg ou pos
					int posCol = getPos(vars, posVar + 1, stringVarAtual);
					if (posCol < 0) {//se var n existe dar pos pra ela
						posVar++;
						posCol = posVar;
						vars[posVar] = (char*)malloc(strlen(stringVarAtual));
						strcpy(vars[posVar], stringVarAtual);
					}//se existe colocar na pos dela


					matrizSistema[posAtualLinha][posCol] = numASerInserido;

					free(stringVarAtual);
				}

				if (isNumber(ch)) {
					if (!lendoNum) {
						lendoNum = true;
						stringNumAtual = criarString();
					}
					addChar(ch, stringNumAtual);
				}
				else if (isalpha(ch)) {
					if (!lendoVar) {
						if (lendoNum) {//estava lendo um num
							//val a ser inserido recebe o coef
							numASerInserido = atoi(stringNumAtual);
							free(stringNumAtual);
						}
						else {
							//val a ser inserido recebe 1
							numASerInserido = 1;
						}

						if (numNegativo) {
							numASerInserido *= -1;
							numNegativo = false;
						}

						lendoNum = false;
						lendoVar = true;
						stringVarAtual = criarString();
					}
					addChar(ch, stringVarAtual);
				}
				else if (ch == '\n' || (ch == EOF)) {
					//insere o num atual
					if (numNegativo) {
						numASerInserido *= -1;
						numNegativo = false;
					}

					matrizSistema[posAtualLinha][qtdSistemas] = atoi(stringNumAtual);

					posAtualLinha++;
					lendoNum = false;
					lendoVar = false;
				}

			}
		} while (ch != EOF);
		
		printIntMatrix(matrizSistema,qtdSistemas + 1, qtdSistemas);

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

void printIntMatrix(int** matrix, int qtdColumns, int qtdRows) {
	int rows = 0;
	int columns = 0;
	for (rows = 0;rows < qtdRows;rows++) {
		for (columns = 0;columns < qtdColumns;columns++) {
			printf(" %d ", matrix[rows][columns]);
		}
		printf("\n");
	}
}

bool isNumber(char c) {
	return (c >= '0' && c <= '9');
}

void inserirNaMatriz() {

}

int getPos(char **arr, int len, char *target) {
	int i;
	for (i = 0; i < len; i++) {
		if (strncmp(arr[i], target, strlen(target)) == 0) {
			return i;
		}
	}
	return -1;
}
