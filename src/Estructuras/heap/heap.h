#ifndef HEAP_H
#define HEAP_H

//#include "../nodos/nodoarbol.h"
#include "../arbol/arbol.h"
#include <stdio.h>

#define MAX 1
#define MIN 0
#define HIJO_IZQUIERDO(p) 2*(p)+1
#define HIJO_DERECHO(p) 2*(p)+2

typedef struct
{
	NodoA **arr;
	int cantidad;
	Arbol arbol;
	void (*imprimir)(void*);
	int (*comparar)(void*,void*);
	int tipo;
}Heap;

Heap inicializarHeap(void (*imprimir)(void*),int (*comparar)(void*,void*) );
void insertarHeap(Heap *heap, void *dato);
void* quitarHeap(Heap *heap);
void heapify(Heap *heap);
void imprimirHeap(Heap heap);





#endif