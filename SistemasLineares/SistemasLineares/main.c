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

		matrizSistema = (float**)malloc(sizeof(float*) * qtdSistemas + 1);//mais um pra colocar o resultado
		for (int i = 0; i < qtdSistemas + 1; i++) {
			matrizSistema[i] = (float*)malloc(sizeof(float*) * qtdSistemas);
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

					matrizSistema[posAtualLinha][qtdSistemas] = (float)atof(stringNumAtual);

					posAtualLinha++;
					lendoNum = false;
					lendoVar = false;
				}

			}
		} while (ch != EOF);
		printFloatMatrix(matrizSistema,qtdSistemas + 1, qtdSistemas);

		resultados = (float*)malloc(sizeof(int) * qtdSistemas);

		conferirMatriz(matrizSistema, qtdSistemas + 1, qtdSistemas); //conferindo se a matriz tem algum 0 na diagonal e se tem mudando as linhas
		//para ser possivel fazer a solução.
		

		int res = getSolucao(resultados, matrizSistema, qtdSistemas + 1, qtdSistemas);
		if (res != SUCESSO) {
			if (res == NAO_SPD) {
				fprintf(stderr, "%s", "O sistema nao eh S.P.D.!\n");
				//return ERRO;
			}
		}
		printf("\n==========\n");
		for (int i = 0; i < qtdSistemas; i++) {
			printf("%s = %.6f\n", vars[i], resultados[i]);
		}
		
		printf("\nSair do programa? (s/n)\n");
		scanf(" %c", &opcao);
	} while (opcao == 'n');

	return 0;
}

int conferirMatriz(float **matrizSistema, int columns, int rows)
{
	int i = 0; //aonde conferirá as outras posições do vetor;
	int j = 0;
	int r = 0;
	int contagem = 0;
	int trocou = 0;
	float **matrizSistemaNova = (float**)malloc(sizeof(float*) * qtdSistemas + 1);//mais um pra colocar o resultado
	matrizSistemaNova = matrizSistema;

	if (columns - 1 == rows) //collumns-1 porque na matriz tem uma coluna dos resultados da equação na matriz
	{
		for (i; i <= rows; i++)
		{
			if (matrizSistema[i][i] == 0)
			{
				if (i != rows) // se não é a ultima linha
				{
					for (r; r <= rows; r++)
					{
						contagem++; //ver quantas linhas ele andou.

						if (matrizSistema[r][i] != 0) //conferindo a proxima linha na mesma posição. 
						{
							for (j; j <= columns; j++)
							{
								matrizSistemaNova[i][j] = matrizSistema[r][j];  //linha de baixo da matrizSistema da linha i virará a nova linha i.
								matrizSistemaNova[r][j] = matrizSistema[i][j]; //linha i  da matrizSistema virará a linha de baixo de i.
								trocou = 1;
							}
							j = 0; //zerando var j.
						}

					}
					if (trocou != 1 && contagem != rows) //se ele nao fez nenhuma troca, e se ele nao percorreu todas as linhas.
						goto conferirLinhasDescrescente;

					r = 0; //zerando var r
				}
				else //se for a ultima linha
				{
				conferirLinhasDescrescente: //goto. vai conferir as linhas para cima.

					r = rows;
					for (r; r >= 0; r--) //camos fazer mesmo processo mas voltando.
					{
						if (matrizSistema[r][i] != 0) //conferindo a proxima linha na mesma posição.
						{
							for (j; j <= columns; j++)
							{
								matrizSistemaNova[i][j] = matrizSistema[r][j];  //linha de baixo da matrizSistema da linha i virará a nova linha i.
								matrizSistemaNova[r][j] = matrizSistema[i][j]; //linha i  da matrizSistema virará a linha de baixo de i.
								trocou = 1;
							}
							j = 0;
						}

					}

					//nao preciso zerar r pois quando ele sair do for estará já zero.

					if (trocou == 0) // se ele nao encontrou outra posição para trocar pois todas sao zero nessa posicao ele vai retornar -1. 
						// a matriz vai continuar da mesma forma que estava antes, onde do getSolucao será tratado corretamente do erro.
						return -1;

				}
			}
		}
		matrizSistema = matrizSistemaNova;
		free(matrizSistemaNova);
		return 0; //tudo deu certo. a troca foi bem sucedida. ou nao tinha oque mudar.

	}// se nao for uma matriz quadrada, não é nescessario fazer nada. quando for para o getSolucao ele tratará disso.
		
	
	return -1;
}

int getSolucao(float * solucao, float **matrizSistema, int columns, int rows) {
	float valorDiagonalPrincipal;
	float *linhaAtual;//precisa virar lista
	float valorMult;
	int cont = 0;

	for (int i = 0; i < rows; i++) {
		valorDiagonalPrincipal = matrizSistema[i][i];

		for (int j = 0; j < rows; j++) {
			if (j != i) {
				linhaAtual = (float*)malloc(sizeof(float) * columns);
				for (int k = 0; k < columns; k++) {
					linhaAtual[k] = matrizSistema[i][k];
				}//tem a linha atual

				if (matrizSistema[j][i] != 0) {
					valorMult = -(matrizSistema[j][i] / valorDiagonalPrincipal);

					multiplicarLinha(linhaAtual, columns, valorMult);
					matrizSistema[j] = somarFila(columns, matrizSistema[j], linhaAtual);

					for (int z = 0; z < columns; z++) {//ve se a linha inteira eh zero, se for, nao eh spd
						if (matrizSistema[j][z] == 0)
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
		multiplicarLinha(matrizSistema[i], qtdSistemas + 1, valorMult);
	}
	
	for (int i = 0; i < rows; i++) {
		solucao[i] = matrizSistema[i][rows];
	}

	return SUCESSO;
}

float *somarFila(int tam, float *fila1, float *fila2) {
	float *ret = malloc(tam * sizeof(int));

	for (int i = 0; i < tam; i++) {
		ret[i] = fila1[i] + fila2[i];
	}

	return ret;
}

void multiplicarLinha(float *linhaAtual,int tam, float valorMult) {
	for (int i = 0; i < tam; i++) {
		linhaAtual[i] *= valorMult;
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
			printf(" %.6f ", matrix[rows][columns]);
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
