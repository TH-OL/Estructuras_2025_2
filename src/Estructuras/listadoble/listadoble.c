#include "listadoble.h"
#include <stdio.h>


ListaD inicializarListaD(void)
{
	return (ListaD) {NULL,NULL,0,NULL,NULL,NULL};
}
void mostrarListaD(ListaD lista)
{
	NodoD *p;	
	printf("\n Lista[%d]:\n",lista.cant);
	for( p = lista.inicio ;  p!=NULL ; p = p->sig  )
	{
		printf(" ");
		lista.imprimir(p->dato);
		printf("->");
	}
	printf("NULL");
}
void mostrarListaDInv(ListaD lista)
{
	NodoD *p;	
	printf("\n Lista[%d]:\n",lista.cant);
	for( p = lista.fin ;  p!=NULL ; p = p->ant  )
	{
		printf(" ");
		lista.imprimir(p->dato);
		printf("->");
	}
	printf("NULL");
}
void* buscarDato(ListaD lista,void *dato)
{
	NodoD *actual;
	for( actual = lista.inicio ; actual!=NULL ;  actual = actual->sig)
	{
		if( lista.comparar(dato,actual->dato) == 0)
		{
			return actual->dato;
		}
	}	
	return NULL;
}
void borrarDato(ListaD *lista,void* dato)
{
	NodoD *actual;
	for( actual = lista->inicio ; actual!=NULL ;  actual = actual->sig)
	{
		if(lista->comparar(dato,actual->dato) == 0)
		{
			if(actual->ant != NULL)
				actual->ant->sig = actual->sig;
			else
				lista->inicio = actual->sig;
			
			if(actual->sig != NULL)
				actual->sig->ant = actual->ant;
			else
				lista->fin = actual->ant;
			
			if(lista->liberar != NULL)
				lista->liberar(actual->dato);
			
			free(actual);
			lista->cant--;
			break;
		}		
	}	
}
void insertarOrdenado(ListaD *lista, void *dato) {
    NodoD *nuevo = crearNodoD(dato);

    if (lista->inicio == NULL) {
        lista->inicio = lista->fin = nuevo;
    } else {
        NodoD *actual = lista->inicio;
        while (actual && lista->comparar(dato, actual->dato) > 0)
            actual = actual->sig;

        if (actual == lista->inicio) { // Insertar al inicio
            nuevo->sig = lista->inicio;
            lista->inicio->ant = nuevo;
            lista->inicio = nuevo;
        } else if (actual == NULL) { // Insertar al final
            nuevo->ant = lista->fin;
            lista->fin->sig = nuevo;
            lista->fin = nuevo;
        } else { // Insertar en medio
            nuevo->sig = actual;
            nuevo->ant = actual->ant;
            actual->ant->sig = nuevo;
            actual->ant = nuevo;
        }
    }
    lista->cant++;
}
void insertaListaDFinal(ListaD *lista, void *dato){
    NodoD *nuevo = crearNodoD(dato);

    if (lista->inicio == NULL) {
        // Lista vacía
        lista->inicio = nuevo;
        lista->fin = nuevo;
    } else {
        // Insertar después del último nodo
        lista->fin->sig = nuevo;
        nuevo->ant = lista->fin;
        lista->fin = nuevo;  // Actualizamos el fin
    }

    lista->cant++;
}
