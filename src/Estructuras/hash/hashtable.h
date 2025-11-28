#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdio.h>
#include <stdlib.h>
#include "../nodos/nodo.h"
#include "../cola/cola.h"

typedef struct
{
	void **tabla;
	int tam;
	int cant;       // Elementos reales (activos)
	int cant_ocup;  // Elementos + Borrados (para rendimiento del sondeo)
	int (*folding)(void*);
	void (*imprimir)(void*);
	int (*comparar)(void*,void*);
}HashTable;

int hash(int clave,int tam);
int rehash(int indice,int k,int tam);

HashTable inicializarHashTable(int tam, int (*folding)(void*), void (*imprimir)(void*), int (*comparar)(void*,void*));
int insertarClave(HashTable *hashtable, void *dato);
int eliminarClave(HashTable *hashtable, void *dato);
Cola buscarClave(HashTable *hashtable, void *dato);
void imprimirHashTabla(HashTable *hashtable);
void liberarHashTable(HashTable *hashtable);

#endif
