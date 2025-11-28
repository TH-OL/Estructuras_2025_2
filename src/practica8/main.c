#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <math.h>

#include "../captura/captura.h"
#include "../Estructuras/pila/pila.h"
#include "../Estructuras/cola/cola.h"
#include "../Estructuras/arbol/arbol.h"
#include "../Estructuras/nodos/nodoarbol.h"

typedef struct
{
	char caracter;
	float *valor;
}Dato;

void imprimirCharDato(void *dato);

typedef struct {
    char var;
    float valor;
    int asignado;
} RegistroVar;

RegistroVar tablaVars[52];
void inicializarTablaVars();
int indiceVar(char c);

Dato* crearDato(char caracter);
void imprimirDato(void *dato);
Cola* expresionACola(char *expresion);
void capturarValores(Cola *cola);

void ingresoExpresion(char** infix, char** postfix);
int convertirPostfix(char *infix, char** postfix);
int esOperador(char c);
int precedencia(char op);

void liberarSubArbol(NodoA *n);
int contarNodos(NodoA *n);
Arbol postfixToArbol(char *postfix, void(*imprimir)(void*));
float evaluarArbol(NodoA *n, int *error);
void capturarValoresArbol(NodoA *n);


int main(void)
{
	char* infix = NULL;
	char* postfix = NULL;
	Arbol arbol;
	arbol.raiz = NULL;
	arbol.cantidad = 0;
	arbol.imprimir = NULL;
	arbol.comparar = NULL;
	arbol.liberar = NULL;
	
	
	int opc;
	do{
		printf("\nEXPRESION: %s", infix ? infix : " ");
		printf("\n\n[0] CAPTURA EXPRESION");
		printf("\n[1] IMPRIME ARBOL EXPRESION");
		printf("\n[2] EVALUAR EXPRESION");
		printf("\n[3] TERMINAR PROGRAMA");
		inputEntero("\nOPCION: ",&opc);
		
		switch(opc){
			case 0: 
				if (arbol.raiz != NULL) {
					liberarSubArbol(arbol.raiz);
					arbol.raiz = NULL;
				}
				inicializarTablaVars();
				ingresoExpresion(&infix, &postfix); 
				arbol = postfixToArbol(postfix, imprimirCharDato);
			break;
			case 1: 
				if(arbol.raiz == NULL)
					printf("\nNo hay expresion postfix valida");
				else
					imprimirArbol(arbol);
			break;
			case 2:  
				if (arbol.raiz == NULL) {
					printf("\nNo hay arbol generado. Captura expresion.\n");
					break;
				}
				printf("\n--- CAPTURA DE VALORES ---\n");
				capturarValoresArbol(arbol.raiz);
				printf("\n--- EVALUACION ---\n");
				int error = 0;
				float resultado = evaluarArbol(arbol.raiz, &error);
				if (!error)
					printf("\nResultado: %f\n", resultado);
			break;
			case 3:
				printf("\nSaliendo...");
				if (arbol.raiz != NULL)
					liberarSubArbol(arbol.raiz);
			break;
			
		}
	}while(opc != 3);
	
	
	/*
	inputCadenaDinamica("\n Captura postfix: ",&postfix,100);	
	Cola *colaExpresion = expresionACola(postfix);
	capturarValores(colaExpresion);
	printf("\n Cola: ");	
	imprimirCola(*colaExpresion);
	printf("\n\n FIN DE PROGRAMA\n");
	*/
	
	free(infix);
	free(postfix);
	
	return 0;
}

void imprimirCharDato(void *dato) {
    if (dato == NULL) {
        printf(" (null)");
        return;
    }
    char *c = (char*) dato;
    printf(" %c", c[0]);
}

void inicializarTablaVars() {
    for (int i = 0; i < 52; i++) {
        tablaVars[i].asignado = 0;
        tablaVars[i].valor = 0;
        if (i < 26)
            tablaVars[i].var = 'A' + i;
        else
            tablaVars[i].var = 'a' + (i - 26);
    }
}

int indiceVar(char c) {
    if (c >= 'A' && c <= 'Z') return c - 'A';
    if (c >= 'a' && c <= 'z') return 26 + (c - 'a');
    return -1;
}

void liberarSubArbol(NodoA *n) {
    if (!n) return;
    if (n->izq) liberarSubArbol(n->izq);
    if (n->dch) liberarSubArbol(n->dch);
    if (n->dato) free(n->dato); // asumimos que dato fue malloc'ed (char*)
    free(n);
}

int contarNodos(NodoA *n) {
    if (!n) return 0;
    return 1 + contarNodos(n->izq) + contarNodos(n->dch);
}

void capturarValoresArbol(NodoA *n) {
    if(n == NULL) return;
	capturarValoresArbol(n->izq);

    char *dato = (char *)n->dato;
    char c = dato[0];

    // Si es una variable 
    if(isalnum((unsigned char)c)) {

        int idx = indiceVar(c);
        if(idx != -1) {
            // ¿Ya existe valor?
            if(!tablaVars[idx].asignado) {
                printf("Introduce valor para %c: ", c);
                scanf("%f", &tablaVars[idx].valor);
                tablaVars[idx].asignado = 1;
            }
            // Copiar valor al nodo
            if(n->valor == NULL)
                n->valor = malloc(sizeof(float));

            *(n->valor) = tablaVars[idx].valor;
        }
    }
    capturarValoresArbol(n->dch);
}

float evaluarArbol(NodoA *n, int *error) {
    if (n == NULL) {
        *error = 1;
        return 0;
    }
    char *dato = (char *)n->dato;
    char c = dato[0];
	// operando
    if (isalnum((unsigned char)c)) {
        if (n->valor == NULL) {
            printf("\nError: falta capturar el valor de %c\n", c);
            *error = 1;
            return 0;
        }
        return *(n->valor);
    }

    // Operador
    float izq = evaluarArbol(n->izq, error);
    if (*error) return 0;

    float der = evaluarArbol(n->dch, error);
    if (*error) return 0;

    switch(c) {
        case '+': return izq + der;
        case '-': return izq - der;
        case '*': return izq * der;
        case '/': 
            if (der == 0) {
                printf("\nError: division entre cero!\n");
                *error = 1;
                return 0;
            }
            return izq / der;
        case '^': return pow(izq, der);
        default:
            printf("\nError: operador desconocido %c\n", c);
            *error = 1;
            return 0;
    }
}

Arbol postfixToArbol(char *postfix, void(*imprimir)(void*)){
	Arbol arbol = {NULL, 0, imprimir, NULL,NULL};
	
	if(postfix == NULL) return arbol;
	
	Pila pila = inicializarPila();
	pila.capacidad = -1;
	int i = 0;
	char c;
	
	while((c = postfix[i]) != '\0'){
		if(isspace((unsigned char)c)){
			i++;
			continue;
		}
		
		if(isalnum((unsigned char)c)){
			char *dato = malloc(2);
			if(!dato){
				while(!vacia(pila)){
					NodoA* tmp = (NodoA*)pop(&pila);
					liberarSubArbol(tmp);
				}
				return arbol;
			}
			dato[0] = c; dato[1] = '\0';
			NodoA* n = crearNodoA(dato);
			push(&pila, n);
		}
		else if(esOperador(c)){
			char *dato = malloc(2);
			if(!dato){
				while(!vacia(pila)){
					NodoA* tmp = (NodoA*) pop(&pila);
					liberarSubArbol(tmp);
				}
				return arbol;
			}
			dato[0] = c; dato[1] = '\0';
			NodoA* nOper = crearNodoA(dato);
			
			NodoA* right = (NodoA*) pop(&pila);
			if(!right){
				free(dato);
				free(nOper);
				while(!vacia(pila)){
					NodoA* tmp = (NodoA*) pop(&pila);
					liberarSubArbol(tmp);
				}
				return arbol;
			}
			
			NodoA* left = (NodoA*)pop(&pila);
			if(!left){
				liberarSubArbol(right);
				free(dato);
				free(nOper);
				while(!vacia(pila)){
					NodoA* tmp = (NodoA*) pop(&pila);
					liberarSubArbol(tmp);
				}
				return arbol;
			}
			
			nOper->dch = right;
			nOper->izq = left;
			push(&pila, nOper);
		}
		else{
			while(!vacia(pila)){
				NodoA* tmp = (NodoA*) pop(&pila);
				liberarSubArbol(tmp);
			}
			return arbol;
		}
		i++;
	}
	
	NodoA* root = (NodoA*)pop(&pila);
	if(!root){
		while(!vacia(pila)){
			NodoA* tmp = (NodoA*) pop(&pila);
			liberarSubArbol(tmp);
		}
		return arbol;
	}
	
	if(!vacia(pila)){
		liberarSubArbol(root);
		while(!vacia(pila)){
			NodoA* tmp = (NodoA*) pop(&pila);
			liberarSubArbol(tmp);
		}
		return arbol;
	}
	
	arbol.raiz = root;
	arbol.cantidad = contarNodos(root);
	arbol.imprimir = imprimir;
	
	return arbol;
}

void ingresoExpresion(char** infix, char** postfix){
	if(inputCadenaDinamica("\nIntroduce expresion infix: ", infix, 100) == 0){
		printf("\nError de captura");
		return;
	}
	
	if(*postfix != NULL){
		free(*postfix);
		*postfix = NULL;
	}
	
	if(convertirPostfix(*infix, postfix))
		printf("\nExpresion postfix: %s", *postfix);
	else
		printf("\nError: Expresion invalida revisar parentesis o caracteres");
}

int convertirPostfix(char *infix, char** postfix){
	Pila pila = {NULL, -1, 0, NULL, NULL};
	int i, k; i = k = 0;
	char *op;
	
	*postfix = malloc(strlen(infix)+1);
	if(*postfix == NULL) return 0;
	
	while(infix[i] != '\0'){ // Hasta final de cadena
		char c = infix[i];
		if(isspace(c)){ // Si hay espacio salta al siguiente caracter
			i++;
			continue;
		}
		if(isalnum(c)) // Si es un caracter lo introduce
			(*postfix)[k++] = c;
		else if(c == '('){ // Buscamos el primer parentesis
			char *dato = malloc(sizeof(char));
			*dato = c;
			push(&pila, dato); // Lo ingresamos a la pila 
		}
		else if(c == ')'){ // Buscamos segundo parentesis
			int encontrado = 0;
			
			while(!vacia(pila)){ // Sacaremos todo de la pila por el segundo parentesis
				op = pop(&pila);
				if(*op == '('){ // Paramos hasta encontrar su pareja
					encontrado = 1;
					free(op);
					break;
				}
				(*postfix)[k++] = *op; // Si no para todos los operadores y operandos van a postfix
				free(op);
			}
			if(!encontrado){ // Regresa error por no encontrar el par del parentesis cerrado
				eliminarPila(&pila);
				free(*postfix);
				*postfix = NULL;
				return 0;
			}
		}
		else if(esOperador(c)){ // Verificamos operadores 
			while(!vacia(pila)){
				Nodo* top = pila.cima;
				char *topOp = (char *)top->dato;
				//if(esOperador(*topOp) && ((precedencia(c) < precedencia(*topOp)) || (precedencia(c) == precedencia(*topOp)) && (c != '^')) ){ // Comparamos operador actual con el de la cima de pila 
				if(esOperador(*topOp) && ((precedencia(c) < precedencia(*topOp)) || ((precedencia(c) == precedencia(*topOp)) && (c != '^')))){
					op = pop(&pila);
					(*postfix)[k++] = *op; // El operador actual es menor al de la cima de pila por lo que lo sacamos
					free(op);
				}else break;
			}
			char *dato = malloc(sizeof(char)); // Si no simplemente se guarda en pila
			*dato = c;
			push(&pila, dato);
		}
		else{
			eliminarPila(&pila);
			free(*postfix);
			*postfix = NULL;
			return 0; // Hay un caracter invalido
		}
		i++;
	}
	
	while(!vacia(pila)){ // Termino la cadena por lo que terminaremos por sacar todo lo de pila a postfix
		op = pop(&pila);
		if(*op == '(' || *op == ')'){ // No deberia haber mas parentesis por lo que mandaremos error
			free(op);
			eliminarPila(&pila);
			free(*postfix);
			*postfix = NULL;
			return 0;
		}
		(*postfix)[k++] = *op;
		free(op);
	}
	
	(*postfix)[k] = '\0';
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


void capturarValores(Cola *cola)
{
	int i;
	for( i = 0 ; i< cola->cantidad ; i++)
	{
		NodoA *nodo = peekC(*cola);
		Dato *dato = nodo->dato;
		if(dato->valor == NULL && !esOperador(dato->caracter))			
		{
			dato->valor = calloc(1,sizeof(float));
			printf("\n Captura valor de %c: ",dato->caracter);
			inputFloat("",dato->valor);
		}
		enqueue(cola,dequeue(cola));
	}
}

Dato* crearDato(char caracter)
{
	Dato *nuevo = calloc(1,sizeof(Dato));
	nuevo->caracter = caracter;
	return nuevo;
}

void imprimirDato(void *dato)
{
	NodoA *nodo = dato;
	Dato *info = nodo->dato;
	if(info->valor)
		printf(" %c = %f",info->caracter,*info->valor);
	else 
		printf(" %c ",info->caracter);
}

int compararCaracteres(void *param_a,void *param_b)
{	
	char *datoa  = param_a;
	NodoA *nodob = param_b;
	Dato *datob  = nodob->dato;
	if( *datoa == datob->caracter)
		return 0;
	return 1;
}


Cola* expresionACola(char *expresion)
{
	Cola *cola = calloc(1,sizeof(Cola));
	*cola = inicializarCola();
	cola->imprimir = &imprimirDato;
	int i;
	for( i = 0; expresion[i]!='\0' ; i++)
	{
		NodoA *nodo = buscarC(*cola,&expresion[i],&compararCaracteres);
		Dato* dato;
		if(nodo)
			dato = nodo->dato;
		else 
			dato = crearDato(expresion[i]);
		NodoA *nodoa = crearNodoA(dato);
		enqueue(cola,nodoa);
	}
	return cola;
}