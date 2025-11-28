#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "../captura/captura.h"
#include "../Estructuras/pila/pila.h"

void ingresoExpresion(char* infix, char* postfix, int tam);
int convertirPostfix(char *infix, char* postfix);
int esOperador(char c);
int precedencia(char op);

int main(){
	int opc;
	char infix[50], postfix[50];
	do{
		printf("\nOpciones: ");
		printf("\n[1] Expresion infix a postfix");
		printf("\n[2] Salir");
		inputEntero("\nOpcion: ", &opc);
		switch(opc){
			case 1:
				ingresoExpresion(infix, postfix, sizeof(infix));
			break;
			case 2:
				printf("\nTerminando programa...");
				return 0;
			break;
			default:
				printf("\nOpcion invalida");
			break;
		}
	}while(opc != 2);
	return 0;
}

void ingresoExpresion(char* infix, char* postfix, int tam){
	if(inputCadena("\nIntroduce expresion infix: ", infix, tam) == 0){
		printf("\nError de captura");
		return;
	}
	if(convertirPostfix(infix, postfix))
		printf("\nExpresion postfix: %s", postfix);
	else
		printf("\nError: Expresion invalida revisar parentesis o caracteres");
}

int convertirPostfix(char *infix, char* postfix){
	Pila pila = {NULL, 100, 0, NULL, NULL};
	int i, k; i = k = 0;
	char *op;
	
	while(infix[i] != '\0'){ // Hasta final de cadena
		char c = infix[i];
		if(isspace(c)){ // Si hay espacio salta al siguiente caracter
			i++;
			continue;
		}
		if(isalnum(c)) // Si es un caracter lo introduce
			postfix[k++] = c;
		else if(c == '('){ // Buscamos el primer parentesis
			char *dato = malloc(sizeof(char));
			*dato = c;
			push(&pila, dato); // Lo ingresamos a la pila 
		}
		else if(c == ')'){ // Buscamos segundo parentesis
			int encontrado = 0;
			
			while(!vacia(pila)){ // Sacaremos todo de la pila por el segundo parentesis
				
				op = (char *)pop(&pila);
				if(*op == '('){ // Paramos hasta encontrar su pareja
					encontrado = 1;
					free(op);
					break;
				}
				postfix[k++] = *op; // Si no para todos los operadores y operandos van a postfix
				free(op);
			}
			if(!encontrado){ // Regresa error por no encontrar el par del parentesis cerrado
				eliminarPila(&pila);
				return 0;
			}
		}
		else if(esOperador(c)){ // Verificamos operadores 
			while(!vacia(pila)){
				Nodo* top = pila.cima;
				char *topOp = (char *)top->dato;
				//if(esOperador(*topOp) && ((precedencia(c) < precedencia(*topOp)) || (precedencia(c) == precedencia(*topOp)) && (c != '^')) ){ // Comparamos operador actual con el de la cima de pila 
				if(esOperador(*topOp) && ((precedencia(c) < precedencia(*topOp)) || ((precedencia(c) == precedencia(*topOp)) && (c != '^')))){
					op = (char*)pop(&pila);
					postfix[k++] = *op; // El operador actual es menor al de la cima de pila por lo que lo sacamos
					free(op);
				}else break;
			}
			char *dato = malloc(sizeof(char)); // Si no simplemente se guarda en pila
			*dato = c;
			push(&pila, dato);
		}
		else{
			eliminarPila(&pila);
			return 0; // Hay un caracter invalido
		}
		i++;
	}
	
	while(!vacia(pila)){ // Termino la cadena por lo que terminaremos por sacar todo lo de pila a postfix
		op = (char *)pop(&pila);
		if(*op == '(' || *op == ')'){ // No deberia haber mas parentesis por lo que mandaremos error
			free(op);
			eliminarPila(&pila);
			return 0;
		}
		postfix[k++] = *op;
		free(op);
	}
	
	postfix[k] = '\0';
	return 1; // Se completo bien la expresion
}

int precedencia(char op){
	switch(op){
		case '^': return 3;
		case '*': return 2;
		case '/': return 2;
		case '+': return 1;
		case '-': return 1;
		default: return 0;
	}
}

int esOperador(char c){
	return (c == '+' || c == '-' || c == '*' || c == '/' || c == '^');
}