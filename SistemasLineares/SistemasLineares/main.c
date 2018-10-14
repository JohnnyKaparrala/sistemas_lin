#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#define MAX_LENGTH 32000
#define SUCESSO 0
#define NAO_SPD -1
#define ERRO -1

typedef enum { false, true } bool;//todo ver caso onde tem q resolver de novo da apostila
//todo deixar a mstriz p/ float

void addChar(char c, char *s);
char *criarString();
void printFloatMatrix(float** matrix, int columns, int rows);
bool isNumber(char c);
int getPos(char **arr, int len, char *target);
int getSolucao(float *solucao, float **matrizSistema, int columns, int rows);
void multiplicarLinha(float *linhaAtual, int tam, float valorMult);
float *somarFila(int tam, float *fila1, float *fila2);
void trocarLinha(float **matriz, int linha1, int linha2, int tam);

char opcao;
FILE *f;
int qtdSistemas = 0;
char *qtdSistemasString;
char* stringSistema;
char ch = 0;
float **matrizSistema;

bool lendoNum = false;
bool lendoVar = false;
bool numNegativo = false;
char* stringNumAtual;
char* stringVarAtual;
int posAtualLinha = 0;
int posAtualColuna = -1;
float numASerInserido;
int posVar = -1;

char **vars;//precisa ser lista
float *resultados;

int main() {
	int i;
	int j;
	int res;

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
			addChar(ch, qtdSistemasString);
			ch = fgetc(f);
		}

		//criar as listas pra salvar o nome das variaveis
		qtdSistemas = atoi(qtdSistemasString);
		printf("%d\n\n", qtdSistemas);

		vars = (char**)malloc(qtdSistemas);

		matrizSistema = (float**)malloc(sizeof(float*) * qtdSistemas + 1);//mais um pra colocar o resultado
		for (i = 0; i < qtdSistemas + 1; i++) {
			*(matrizSistema + i) = (float*)malloc(sizeof(float*) * qtdSistemas);
		}

		for (i = 0;i < qtdSistemas + 1;i++) {//enche a matriz com 0
			for (j = 0;j < qtdSistemas;j++) {
				*(*(matrizSistema + i) + j) = 0;
			}
		}

		do
		{
			ch = (char)fgetc(f);
			if (ch != '*' && ch != ' ') {
				if (ch == '-')
					numNegativo = true;

				if ((ch == '+' || ch == '-' || ch == '=') && lendoVar) { // acabou a var, insere ela na litsa
					int posCol;
					lendoVar = false;

					//insere o valor neg ou pos
					posCol = getPos(vars, posVar + 1, stringVarAtual);
					if (posCol < 0) {//se var n existe dar pos pra ela
						posVar++;
						posCol = posVar;
						*(vars + posVar) = (char*)malloc(strlen(stringVarAtual));
						strcpy(*(vars + posVar), stringVarAtual);
					}//se existe colocar na pos dela


					*(*(matrizSistema + posAtualLinha) + posCol) = numASerInserido;

					free(stringVarAtual);
				}

				if (isNumber(ch) || ch == '.') {
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
							numASerInserido = (float)atof(stringNumAtual);
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

					*(*(matrizSistema + posAtualLinha) + qtdSistemas) = (float)atof(stringNumAtual);

					posAtualLinha++;
					lendoNum = false;
					lendoVar = false;
				}
			}
		} while (ch != EOF);
		printFloatMatrix(matrizSistema, qtdSistemas + 1, qtdSistemas);

		resultados = (float*)malloc(sizeof(int) * qtdSistemas);

		//para ser possivel fazer a solução.
		res = getSolucao(resultados, matrizSistema, qtdSistemas + 1, qtdSistemas);
		if (res != SUCESSO) {
			if (res == NAO_SPD) {
				fprintf(stderr, "%s", "O sistema nao eh S.P.D.!\n");
				//return ERRO;
			}
		}
		printf("\n==========\n");
		for (int i = 0; i < qtdSistemas; i++) {
			printf("%s = %.6f\n", *(vars + i), *(resultados + i));
		}

		printf("\nSair do programa? (s/n)\n");
		scanf(" %c", &opcao);
	} while (opcao == 'n');

	return 0;
}

int getSolucao(float * solucao, float **matrizSistema, int columns, int rows) {
	float valorDiagonalPrincipal;
	float *linhaAtual;//precisa virar lista
	float valorMult;
	int cont = 0;
	float valorColuna;
comeco:
	for (int i = 0; i < rows; i++) {
		valorDiagonalPrincipal = *(*(matrizSistema + i) + i);

		if (valorDiagonalPrincipal == 0) {
			cont = i;
			do {
				valorColuna = *(*(matrizSistema + cont) + i);
				cont++;
			} while (valorColuna == 0 && cont < qtdSistemas);

			if (valorColuna == 0) {
				return NAO_SPD;//significa q n tem como resolver
			}
			else {
				cont--;
				trocarLinha(matrizSistema, i, cont, qtdSistemas + 1);
				goto comeco;
			}
			cont = 0;
		}

		for (int j = 0; j < rows; j++) {
			if (j != i) {
				linhaAtual = (float*)malloc(sizeof(float) * columns);
				for (int k = 0; k < columns; k++) {
					*(linhaAtual + k) = *(*(matrizSistema + i) + k);
				}//tem a linha atual

				if (*(*(matrizSistema + j) + i) != 0) {
					valorMult = -(*(*(matrizSistema + j) + i) / valorDiagonalPrincipal);

					multiplicarLinha(linhaAtual, columns, valorMult);
					*(matrizSistema + j) = somarFila(columns, *(matrizSistema + j), linhaAtual);

					for (int z = 0; z < columns; z++) {//ve se a linha inteira eh zero, se for, nao eh spd
						if (*(*(matrizSistema + j) + z) == 0)
							cont++;
					}
					if (cont >= columns) {
						return NAO_SPD;
					}
					else {
						cont = 0;
					}
				}
				printf("\n =\n");
				printFloatMatrix(matrizSistema, qtdSistemas + 1, qtdSistemas);
				free(linhaAtual);
			}
		}//deixa tudo menos a diagonal principal igual a zero e o valor da diagonal principal igual a 1

		valorMult = 1 / valorDiagonalPrincipal;
		multiplicarLinha(*(matrizSistema + i), qtdSistemas + 1, valorMult);
	}

	for (int i = 0; i < rows; i++) {
		*(solucao + i) = *(*(matrizSistema + i) + rows);
	}

	return SUCESSO;
}

float *somarFila(int tam, float *fila1, float *fila2) {
	float *ret = malloc(tam * sizeof(int));

	for (int i = 0; i < tam; i++) {
		*(ret + i) = *(fila1 + i) + *(fila2 + i);
	}

	return ret;
}

void multiplicarLinha(float *linhaAtual, int tam, float valorMult) {
	for (int i = 0; i < tam; i++) {
		*(linhaAtual + i) *= valorMult;
	}
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

void printFloatMatrix(float** matrix, int qtdColumns, int qtdRows) {
	int rows = 0;
	int columns = 0;
	for (rows = 0;rows < qtdRows;rows++) {
		for (columns = 0;columns < qtdColumns;columns++) {
			printf(" %.6f ", *(*(matrix + rows) + columns));
		}
		printf("\n");
	}
}

bool isNumber(char c) {
	return (c >= '0' && c <= '9');
}

void inserirNaMatriz() {

}

void trocarLinha(float **matriz, int linha1, int linha2, int tam) {
	int i;
	float *linhaAux = (float*)malloc(sizeof(float) * tam);
	for (i = 0; i < tam; i++) {
		*(linhaAux + i) = *(*(matriz + linha1) + i);
	}

	for (i = 0; i < tam; i++) {
		*(*(matriz + linha1) + i) = *(*(matriz + linha2) + i);
	}

	for (i = 0; i < tam; i++) {
		*(*(matriz + linha2) + i) = *(linhaAux + i);
	}

	free(linhaAux);
}

int getPos(char **arr, int len, char *target) {
	int i;
	for (i = 0; i < len; i++) {
		if (strncmp(*(arr + i), target, strlen(target)) == 0) {
			return i;
		}
	}
	return -1;
}
