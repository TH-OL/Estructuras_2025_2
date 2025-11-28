//INCLUDES
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "../captura/captura.h"
#include "../Estructuras/pila/pila.h"
//FUNCION MAIN

#define LONGITUD 64

void cadenaPalindromo();
void verificaParentesis();

void imprimirCadena(void *a);
void liberarCadena(void *a);

int main(void)
{
	// {INICIO NULO, SIN LIMITE, CANTIDAD 0, COMO IMPRIMIR, COMO LIBERAR}
	/*Pila pila = {NULL,-1,0,&imprimirCadena,&liberarCadena};
	push(&pila, cadenaDinamica("David") );
	push(&pila, cadenaDinamica("Carlo") );
	push(&pila, cadenaDinamica("J") );
	push(&pila, cadenaDinamica("Nicol") );
	push(&pila, cadenaDinamica("Abraham") );
	imprimirPila(pila);
	liberarCadena( pop(&pila) );
	liberarCadena( pop(&pila) );
	imprimirPila(pila);
	eliminarPila(&pila);
	imprimirPila(pila);	
	
	printf("\n\n FIN DE PROGRAMA");
	*/
	int opc;
	do{
		printf("\nMenu de opciones:");
		printf("\n[1] - Verificar si palabra es palindromo");
		printf("\n[2] - Verificar cadena con simbolos: () {} [] ");
		printf("\n[3] - Salir");
		inputEntero("\nOpcion: ", &opc);
		switch(opc){
			case 1:
				cadenaPalindromo();
			break;
			case 2:
				verificaParentesis();
			break;
			case 3:
				printf("\nSaliendo...");
			break;
			default: 
				printf("Error: Opcion invalida");
			break;
		}
	}while(opc != 3);
	
	
	return 0;
}

void cadenaPalindromo(){
	char cadena[100];
	Pila pila = {NULL,-1,0,NULL,NULL};
	inputCadena("Ingresa cadena: ", cadena, 100);
	char* caracter;
	
	// Metemos toda la cadena a la pila ignorando espacios
	for(int i = 0; cadena[i] != '\0'; i++){
		if(cadena[i] != ' '){
			caracter = malloc(sizeof(char));
			*caracter = tolower(cadena[i]);
			push(&pila, caracter);
		}
	}
	
	// Recorremos la cadena de vuelta pero ahora comparando con pila
	for(int i = 0; cadena[i] != '\0'; i++){
		if(cadena[i] != ' '){
			char c = tolower(cadena[i]);
			char *tope = (char*)pop(&pila);
			
			if(!tope || c != *tope){
				if(tope)
					free(tope);
				eliminarPila(&pila);
				printf("\nLa palabra NO es palindromo");
				return;
			}
			free(tope);
		}
	}
	
	eliminarPila(&pila);
	printf("\nLa palabra SI es palindromo");
}
void verificaParentesis(){
	char cadena[100];
	Pila pila = {NULL,-1,0,NULL,NULL};
	inputCadena("Ingresa cadena: ", cadena, 100);
	char* caracter;
	
	// recorremos la cadena
	for(int i = 0; cadena[i] != '\0'; i++){
		char c = cadena[i];
		
		// Si es un simbolo abierto lo mete a la pila
		if(c == '(' || c == '{' || c == '['){
			caracter = malloc(sizeof(char));
			*caracter = c;
			push(&pila, caracter);
		}
		// Si es uno que cierra verificara si tiene el que lo abre
		else if(c == ')' || c == '}' || c == ']'){
			if(vacia(pila)){
				eliminarPila(&pila); // Si no hay nada en pila es incorrecto
				printf("\nError: cadena invalida, verificar simbolos( \"()\", \"{}\", \"[]\" )\n");
				return;
			}
			
			char* tope = (char*)pop(&pila);
			char abierto = *tope;
			free(tope);
			// que simbolo es y si tiene su simbolo abierto 
			if((c == ')' && abierto != '(') || (c == '}' && abierto != '{') || (c == ']' && abierto != '[')){
				eliminarPila(&pila);
				printf("\nError: cadena invalida, verificar simbolos( \"()\", \"{}\", \"[]\" )\n");
				return;
			}
		}
	}
	// Si al final la pila queda vacia es cadena valida
	if(vacia(pila))
		printf("\nCadena valida\n");
	else
		printf("\nError: cadena invalida, verificar simbolos( \"()\", \"{}\", \"[]\" )\n");
	eliminarPila(&pila);
}
void imprimirCadena(void *a)
{
	char *aa = a;
	printf("%s",aa);
	
}

void liberarCadena(void *a)
{
	char *aa = a;
	free(aa);
}