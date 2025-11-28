#ifndef LISTA_DOBLE_H
#define LISTA_DOBLE_H

#include "nododoble.h"

typedef struct 
{
	NodoD *inicio;
	NodoD *fin;
	int cant;
	int (*comparar)(void *datoA,void *dataB);
	void (*imprimir)(void *datoA);
	void (*liberar)(void *datoA);
}ListaD;

ListaD inicializarListaD(void);
void mostrarListaD(ListaD lista);
void insertaListaDFinal(ListaD *lista, void *dato);
void mostrarListaDInv(ListaD lista);
void* buscarDato(ListaD lista,void *dato);
void borrarDato(ListaD *lista,void* dato);
void insertarOrdenado(ListaD *lista, void *dato);


#endif