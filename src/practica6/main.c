
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdarg.h>
#include <time.h>
#include "../captura/captura.h"
#include "../Estructuras/arbol/arbol.h"

int* crearEntero(int);
int compararEntero(void*,void*);
void imprimirEntero(void*);

int profundidadArbol(Arbol arbol);
void calcularAltura(NodoA *raiz,int *altura);
void eliminarDato(Arbol *arbol, void *dato);
void eliminarNodoRecursivo(NodoA **nodo, void *dato, int (*comparar)(void*, void*));
void liberarEntero(void *a);
NodoA* buscarMinimoDerecha(NodoA *nodo);
void liberarArbol(Arbol *arbol);
void liberarArbolPostorden(NodoA *nodo, void (*liberar)(void*));
void comparacionArboles(Arbol arbolA, Arbol arbolB);
int buscarDatos(NodoA* raizA, NodoA* raizB, int (*comparar)(void *, void *) );
int compararSoloDatos(NodoA *raizA, NodoA *raizB, int (*comparar)(void*, void*));
void recolectarDatos(NodoA *raiz, void **lista, int *indice);
void equilibrarArbol(Arbol *arbol);
void obtenerDatosEnOrden(NodoA *raiz, int *array, int *index);
void liberarArbolPostorden(NodoA *nodo, void (*liberar)(void*));
int contarNodos(NodoA *nodo);
NodoA* construirBalanceado(int *array, int inicio, int fin);

void myprintf(char *formato,...);

int main(void)
{
	Arbol arbolA;
	arbolA.raiz = NULL;
	arbolA.cantidad = 0;
	arbolA.imprimir = &imprimirEntero;
	arbolA.comparar = &compararEntero;
	arbolA.liberar = &liberarEntero;
	
	Arbol arbolB;
	arbolB.raiz = NULL;
	arbolB.cantidad = 0;
	arbolB.imprimir = &imprimirEntero;
	arbolB.comparar = &compararEntero;
	arbolB.liberar = &liberarEntero;
	
	insertarArbol(&arbolA, crearEntero(7));
	insertarArbol(&arbolA, crearEntero(5));
	insertarArbol(&arbolA, crearEntero(10));
	insertarArbol(&arbolA, crearEntero(4));
	insertarArbol(&arbolA, crearEntero(6));
	insertarArbol(&arbolA, crearEntero(1));
	insertarArbol(&arbolA, crearEntero(9));
	insertarArbol(&arbolA, crearEntero(11));
	insertarArbol(&arbolA, crearEntero(8));
	
	
	insertarArbol(&arbolB, crearEntero(7));
	insertarArbol(&arbolB, crearEntero(5));
	insertarArbol(&arbolB, crearEntero(10));
	insertarArbol(&arbolB, crearEntero(4));
	insertarArbol(&arbolB, crearEntero(6));
	insertarArbol(&arbolB, crearEntero(1));
	insertarArbol(&arbolB, crearEntero(9));
	insertarArbol(&arbolB, crearEntero(11));
	insertarArbol(&arbolB, crearEntero(8));
	//imprimirArbol(arbol);
	/*
	printf("\n PREORDEN: ");
	imprimirOrden(arbol,PREORDEN);
	printf("\n ORDEN: ");
	imprimirOrden(arbol,ORDEN);
	printf("\n INVERSO: ");
	imprimirOrden(arbol,INVERSO);
	printf("\n POSTORDEN: ");
	imprimirOrden(arbol,POSTORDEN);
	*/
	
	int opc, valor;
	do{
		opc = valor = 0;
		printf("\n---------- Menu de opciones ----------");
		printf("\n(1) - Insertar en Arbol A");
		printf("\n(2) - Insertar en Arbol B");
		printf("\n(3) - Comparar Arboles A y B");
		printf("\n(4) - Eliminar dato en Arbol A");
		printf("\n(5) - Eliminar dato en Arbol B");
		printf("\n(6) - Mostrar profundidades");
		printf("\n(7) - Equlibrar arboles");
		printf("\n(8) - Mostrar Arboles");
		printf("\n(9) - Terminar programa");
		inputEntero("\nOPCION: ",&opc);
		switch(opc){
			case 1:
				inputEntero("\nNumero a ingresar: ",&valor);
				insertarArbol(&arbolA, crearEntero(valor));
			break;
			case 2:
				inputEntero("\nNumero a ingresar: ",&valor);
				insertarArbol(&arbolB, crearEntero(valor));
			break;
			case 3:
				comparacionArboles(arbolA, arbolB);
			break;
			case 4:
				inputEntero("\nNumero a eliminar en arbol A: ",&valor);
				eliminarDato(&arbolA, crearEntero(valor));
			break;
			case 5:
				inputEntero("\nNumero a eliminar en arbol B: ",&valor);
				eliminarDato(&arbolB, crearEntero(valor));
			break;
			case 6:
				printf("\nProfundidad arbol A: %d", profundidadArbol(arbolA));
				printf("\nProfundidad arbol B: %d", profundidadArbol(arbolB));
			break;
			case 7:
					equilibrarArbol(&arbolA);
					equilibrarArbol(&arbolB);
					printf("\nArboles equilibrados");
			break;
			case 8:
				printf("\nArbol A:\n");
				imprimirArbol(arbolA);
				printf("\n");
				printf("\nArbol B:\n");
				imprimirArbol(arbolB);
				printf("\n");
			break;
			case 9:
				printf("\nHasta luego");
				liberarArbol(&arbolA);
				liberarArbol(&arbolB);
			break;
			default:
				printf("\n OPCION INVALIDA");
			break;
			
		}
	}while(opc != 9);
	
	printf("\n\n FIN DE PROGRAMA\n");
	return 0;
}


int* crearEntero(int dato)
{
	int *nuevo = malloc(sizeof(int));
	*nuevo = dato;
	return nuevo;
}

int compararEntero(void*a,void*b)
{
	int *aa = a,*bb=b;
	if(*aa==*bb)return 0;
	else if(*aa<*bb)return -1;
	else return 1;
}

void imprimirEntero(void *a)
{
	int *aa=a;
	printf("%d",*aa);
}

int profundidadArbol(Arbol arbol){
	if(!arbol.raiz)
		return 0;
	int altura = 1;
	calcularAltura(arbol.raiz,&altura);
	return altura;
}
void calcularAltura(NodoA *raiz,int *altura)
{
	int izquierdo,derecho;
	izquierdo = derecho = *altura;
	if(raiz->izq && izquierdo++ )
		calcularAltura(raiz->izq,&izquierdo);
	if(raiz->dch && derecho++ )
		calcularAltura(raiz->dch,&derecho); 
	*altura = (izquierdo>derecho) ? izquierdo:derecho;
}

void eliminarDato(Arbol *arbol, void *dato){
	if(!arbol->raiz){ // Si no hay arbol existente
		printf("Arbol vacio");
		return;
	}
	eliminarNodoRecursivo(&(arbol->raiz),dato, arbol->comparar);
	
}
void eliminarNodoRecursivo(NodoA **nodo, void *dato, int (*comparar)(void*, void*)){
    if (*nodo == NULL)
        return;
    int comp = comparar(dato, (*nodo)->dato);

    if(comp < 0){
        eliminarNodoRecursivo(&((*nodo)->izq), dato, comparar);
    } 
    else if(comp > 0){
        eliminarNodoRecursivo(&((*nodo)->dch), dato, comparar);
    } 
    else{
        // Caso 1: sin hijos
        if(!(*nodo)->izq && !(*nodo)->dch){
            free(*nodo);
            *nodo = NULL;
        }
        // Caso 2: un solo hijo
        else if(!(*nodo)->izq){
            NodoA *temp = (*nodo)->dch;
            free(*nodo);
            *nodo = temp;
        } 
        else if (!(*nodo)->dch){
            NodoA *temp = (*nodo)->izq;
            free(*nodo);
            *nodo = temp;
        } 
        // Caso 3: dos hijos
        else{
            NodoA *min = buscarMinimoDerecha((*nodo)->dch);
            (*nodo)->dato = min->dato;
            eliminarNodoRecursivo(&((*nodo)->dch), min->dato, comparar);
        }
    }
}
NodoA* buscarMinimoDerecha(NodoA *nodo){
	while(nodo->izq)
		nodo = nodo->izq;
	return nodo;
}
void liberarEntero(void *a)
{
	free(a);
}
void liberarArbol(Arbol *arbol)
{
	if (arbol->raiz)
	{
		liberarArbolPostorden(arbol->raiz, arbol->liberar);
		arbol->raiz = NULL;
		arbol->cantidad = 0;
	}
}
void liberarArbolPostorden(NodoA *nodo, void (*liberar)(void*))
{
	if (!nodo)
		return;
	
	liberarArbolPostorden(nodo->izq, liberar);
	liberarArbolPostorden(nodo->dch, liberar);
	liberar(nodo->dato);
	free(nodo);
}
void comparacionArboles(Arbol arbolA, Arbol arbolB){
	if(!arbolA.raiz || !arbolB.raiz){
		if(!arbolA.raiz)
			printf("\nNo hay arbol A");
		else
			printf("\nNo hay arbol B");
		printf("\nLos arboles no son iguales ni en datos ni estructura");
		return;
	}
	int iguales = buscarDatos(arbolA.raiz, arbolB.raiz, arbolA.comparar);
	if(iguales){
		printf("\n Los arboles son iguales en datos y estructura\n");
        return;
	}
	int igualesDatos = compararSoloDatos(arbolA.raiz, arbolB.raiz, arbolA.comparar);
	if(igualesDatos)
		printf("\nLos arboles son iguales solo en datos\n");
	else
		printf("\nLos arboles no son iguales ni en datos ni estructura\n");
	
}
int buscarDatos(NodoA* raizA, NodoA* raizB, int (*comparar)(void *, void *) ){
	
	if(raizA == NULL && raizB == NULL)
        return 1;
    if(raizA == NULL || raizB == NULL)
        return 0;
    if(comparar(raizA->dato, raizB->dato) != 0)
        return 0;
    return buscarDatos(raizA->izq, raizB->izq, comparar) && buscarDatos(raizA->dch, raizB->dch, comparar);
	
}
int compararSoloDatos(NodoA *raizA, NodoA *raizB, int (*comparar)(void*, void*)){
    // Primero recolectamos todos los datos de ambos árboles
    void *listaA[100];  // puedes usar malloc si el tamaño es dinámico
    void *listaB[100];
    int countA = 0, countB = 0;

    recolectarDatos(raizA, listaA, &countA);
    recolectarDatos(raizB, listaB, &countB);

    // Si tienen distinta cantidad de datos → no son iguales
    if(countA != countB)
        return 0;

    // Ordenamos ambos conjuntos (si no estás seguro del orden del árbol)
    // puedes hacer un simple bubble sort con la función comparar()
    for(int i=0;i<countA-1;i++){
        for(int j=i+1;j<countA;j++){
            if(comparar(listaA[i], listaA[j]) > 0){
                void *tmp = listaA[i];
                listaA[i] = listaA[j];
                listaA[j] = tmp;
            }
            if(comparar(listaB[i], listaB[j]) > 0){
                void *tmp = listaB[i];
                listaB[i] = listaB[j];
                listaB[j] = tmp;
            }
        }
    }

    // Comparamos los datos ordenados
    for(int i=0;i<countA;i++){
        if(comparar(listaA[i], listaB[i]) != 0)
            return 0;
    }

    return 1; // Todos los datos son iguales
}
void recolectarDatos(NodoA *raiz, void **lista, int *indice){
    if(raiz == NULL) return;
    recolectarDatos(raiz->izq, lista, indice);
    lista[(*indice)++] = raiz->dato;
    recolectarDatos(raiz->dch, lista, indice);
}
void equilibrarArbol(Arbol *arbol)
{
	if (!arbol->raiz)
		return;
	
	int cantidad = contarNodos(arbol->raiz);
	int *array = (int*)calloc(cantidad, sizeof(int));
	int index = 0;
	
	obtenerDatosEnOrden(arbol->raiz, array, &index);
	
	liberarArbolPostorden(arbol->raiz, arbol->liberar);
	
	arbol->raiz = construirBalanceado(array, 0, cantidad - 1);
	free(array);
}
void obtenerDatosEnOrden(NodoA *raiz, int *array, int *index)
{
	if (!raiz)
		return;
	obtenerDatosEnOrden(raiz->izq, array, index);
	int *dato = (int*)raiz->dato;
	array[(*index)++] = *dato;
	obtenerDatosEnOrden(raiz->dch, array, index);
}
int contarNodos(NodoA *nodo)
{
	if (!nodo)
		return 0;
	return 1 + contarNodos(nodo->izq) + contarNodos(nodo->dch);
}
NodoA* construirBalanceado(int *array, int inicio, int fin)
{
	if (inicio > fin)
		return NULL;
	
	int medio = (inicio + fin) / 2;
	int *dato = (int*)malloc(sizeof(int));
	*dato = array[medio];
	
	NodoA *nodo = crearNodoA(dato);
	nodo->izq = construirBalanceado(array, inicio, medio - 1);
	nodo->dch = construirBalanceado(array, medio + 1, fin);
	
	return nodo;
}
/*
void myprintf(char *formato,...)
{
	va_list args;
    va_start(args, strlen(formato));
	while(*formato!='\0')
	{
		switch(*formato)
		{
			case 'd':
				printf("%d",(int)va_arg(args, int));
				break;
			case 'c':
				printf("%c",(char)va_arg(args, char));				
				break;
		}
		formato++;
	}
	va_end(args);
}
*/