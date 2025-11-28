#include "nododoble.h"


NodoD* crearNodoD(void *dato)
{
	NodoD *nuevo = (NodoD*) calloc(1,sizeof(NodoD));
	nuevo->dato = dato;
	nuevo->sig = NULL;
	nuevo->ant = NULL;
	return nuevo;
}