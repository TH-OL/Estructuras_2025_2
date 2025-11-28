#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../captura/captura.h"
#include "../Estructuras/cola/cola.h"

#define NUMERO_CAJAS 5
#define HABILITADA 1
#define DESHABILITADA 0
#define MIN_ARTICULOS 1
#define MAX_ARTICULOS 50

typedef struct
{
	Cola cola;
	int estado;
}Caja;


int* generar_articulos(void);
void imprimirEntero(void*);
void escanearCaja(Caja *caja);

int main(void)
{
	Caja cajas[NUMERO_CAJAS];
	
	for(int i=0; i<NUMERO_CAJAS ; i++)
	{
		cajas[i].cola = inicializarCola();
		cajas[i].cola.imprimir = &imprimirEntero;
	}
	int opcion;	
	int opcion2;
	do
	{
		
		for(int i=0; i<NUMERO_CAJAS ; i++)
		{
			printf("\n Caja[%d]: ",i+1);
			imprimirCola(cajas[i].cola);
		}
		opcion = 0;
		opcion2 = 0;
		//DESPLEGAR LAS CAJAS
		printf("\n [1] AVANZAR TICK");
		printf("\n [2] ESCANEAR ARTICULOS");
		printf("\n [3] HABILITAR");
		printf("\n [4] DESHABILITAR");
		printf("\n [5] SALIR");
		inputEntero("\n Selecciona opcion: ",&opcion);
		switch(opcion)
		{
			case 1:
			for(int i=0; i<NUMERO_CAJAS ; i++)
			{
				//SI ESTA HABILITADA Y SI NO ESTA LLENA
				//GENERAR Y FORMAR CLIENTE
				if(cajas[i].estado && !llenaC(cajas[i].cola))
					enqueue( &cajas[i].cola , generar_articulos() );
			}
			break;
			case 2:
				printf("\n [2]");
				for(int i=0; i<NUMERO_CAJAS; i++){
					if(!vaciaC(cajas[i].cola))
						escanearCaja(&cajas[i]);
				}
			break;
			case 3:
				printf("\n [3]");
				inputEntero("\n Selecciona una caja(1-5): ",&opcion2);
				cajas[opcion2-1].estado = 1;
				
			break;
			case 4:
				printf("\n [4]");
				inputEntero("\n Selecciona una caja(1-5): ",&opcion2);
				cajas[opcion2-1].estado = 0;
			break;
			case 5:
				printf("\n [5]");
				printf("HASTA LUEGO!");
				for(int i=0; i<NUMERO_CAJAS; i++){
					eliminarCola(&cajas[i].cola);
				}
			break;
			default:
			printf("\n [DEFAULT]");
			break;
		}	
	}while(opcion!=5);
	
	printf("\n\n FIN DE PROGRAMA");
	for(int i=0; i<NUMERO_CAJAS; i++){
		eliminarCola(&cajas[i].cola);
		}
	return 0;
}


int* generar_articulos(void)
{		
	int *articulos = malloc(sizeof(int));
	*articulos = rand() % (MAX_ARTICULOS - MIN_ARTICULOS + 1) + MIN_ARTICULOS;
	return articulos; 
	
}


void imprimirEntero(void *a)
{
	int *aa=a;
	printf("%d",*aa);
}
void escanearCaja(Caja *caja){
	Nodo *nodo = caja->cola.inicio;
	if(nodo == NULL) return;
	*(int *)(caja->cola.inicio->dato) -= 1;	
	if(*(int *)(nodo->dato)==0) dequeue(&caja->cola);
}
