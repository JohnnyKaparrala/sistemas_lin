#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

typedef
struct No
{
	void*      info;
	struct No* prox;
}
No;

typedef
struct Lista
{
	No*  inicio;
	int(*compareTo) (void*, void*);
	void(*print)     (void*);
}
Lista;


int getValorInt(Lista* lis, int posNo)
{
	int i = 0; //0 é no prim
	No* aux;

	if (posNo < 0)
		return -1;

	if (posNo == 0)
		return lis->inicio->info;

	aux = lis->inicio;
	for (i; i < posNo; i++)
		if (aux->prox != NULL)
			aux = aux->prox; //a ultima vez será da posição
		else
			return -1;


	return aux->info;
}

char getValorChar(Lista* lis, char posNo)
{
	int i = 0; //0 é no prim
	No* aux;

	if (posNo < 0)
		return "-1";

	if (posNo == 0)
		return lis->inicio->info;


	aux = lis->inicio;
	for (i; i < posNo; i++)
		if (aux->prox != NULL)
			aux = aux->prox; //a ultima vez será da posição
		else
			return "-1";

	return aux->info;
}

char insira(Lista* lis, void* inf)
{
	No* atual;
	if (lis->inicio == NULL)
	{
		lis->inicio = (No*)malloc(sizeof(No));
		lis->inicio->info = inf;
		return 1/*true*/;
	}
	else
	{
		atual = lis->inicio;
		for (;;) // parar ao achar onde inserir
		{
			if (atual->prox == NULL)
				break;

			atual = atual->prox;
		}

		No* novo = (No*)malloc(sizeof(No));
		novo->info = inf;
		novo->prox = atual->prox;
		atual->prox = novo;
	}
}

int intCompareTo(int* a, int* b)
{
	if (*a < *b)
		return -666;

	if (*a == *b)
		return 0;

	//if (*a>*b)
	return 666;
}