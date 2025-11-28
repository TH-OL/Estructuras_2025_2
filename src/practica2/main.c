#include <stdio.h>

#include "../Estructuras/listadoble/listadoble.h"
#include "../captura/captura.h"
#include "alumno.h"


#define LONGITUD 5

void registraAlumno(ListaD *lista);
int compararEnteros(void *a, void *b);
void mostrarAlumnos(ListaD lista);
void buscarAlumno(ListaD lista);
void borraAlumno(ListaD *lista);
void reordenaLista(ListaD *lista);
void liberarLista(ListaD *lista);

int compararMatriculas(void *a, void *b);
int compararNombres(void *a, void *b);
int compararSemestres(void *a, void *b);
int compararPromedios(void *a, void *b);

int main(void)
{
	
	ListaD lista;
	lista = inicializarListaD();
	lista.imprimir = &imprimirAlumno;
	lista.liberar = liberarAlumno;
	/*
	mostrarListaD(lista);
	int x;
	char cadena[LONGITUD];
	inputEntero("\n Captura x: ",&x);
	inputCadena("\n Captura nombre: ",cadena,LONGITUD); 
	inputEntero("\n Captura x: ",&x);
	inputCadena("\n Captura nombre: ",cadena,LONGITUD); 
	inputEntero("\n Captura x: ",&x);
	inputCadena("\n Captura nombre: ",cadena,LONGITUD); 
	
	printf("\n %d %s",x,cadena);
	printf("\n %d %s",x,cadena);
	*/
	
	insertaListaDFinal(&lista,crearAlumno((Alumno){120938,"David",2,78.5}));
	insertaListaDFinal(&lista,crearAlumno((Alumno){918422,"Manuel",5,65.7}));
	insertaListaDFinal(&lista,crearAlumno((Alumno){123091,"Jose",6,98.6}));
	insertaListaDFinal(&lista,crearAlumno((Alumno){309485,"Avid",3,100}));
	insertaListaDFinal(&lista,crearAlumno((Alumno){103928,"Jareth",4,34.4}));
	insertaListaDFinal(&lista,crearAlumno((Alumno){304958,"Carlo",7,47.7}));
	
	
	int opc;
	do{
		//mostrarListaD(lista);
		printf("\nMenu de opciones:");
		printf("\n[1] Registrar alumno");
		printf("\n[2] Desplegar alumnos");
		printf("\n[3] Reordenar lista");
		printf("\n[4] Buscar alumno");
		printf("\n[5] Borrar alumno");
		printf("\n[6] Finalizar programa");
		inputEntero("\nSelecciona una opcion: ", &opc);
		switch(opc){
			case 1:
				registraAlumno(&lista);
			break;
			case 2:
				mostrarAlumnos(lista);
			break;
			case 3:
				reordenaLista(&lista);
			break;
			case 4:
				buscarAlumno(lista);
			break;
			case 5:
				borraAlumno(&lista);
			break;
			case 6:
				printf("\nHasta luego!"); liberarLista(&lista);
			break;
			default:
				printf("\nOpcion invalida, selecciona una opcion correcta");
			break;
			
		}
		
	}while(opc != 6);
	
	liberarLista(&lista);
	return 0;
}

void registraAlumno(ListaD *lista){
	int mat,smtrs;
	float prom;
	char nom[64];
	Alumno registrado; 
	
	inputEntero("Ingresa matricula de estudiante a registrar: ", &mat);
	NodoD *aux = lista->inicio;
	while(aux != NULL){ // Buscar si ya existe la matricula en la lista
		Alumno *a = (Alumno *)aux->dato; // Se toma el dato Alumno del nodo actual
		if(compararEnteros(&mat, &a->matricula) == 0){ // Se usa funcion para comparar
			do{
				inputEntero("Matricula existente ingrese otra: ", &mat);
			}while(compararEnteros(&mat, &a->matricula) == 0); // Se repite hasta ingresar matricula correcta
			break;
		}
		aux = aux->sig;
	}
	
	inputCadena("Ingresa nombre del alumno: ", nom, sizeof(nom));
	inputEntero("Ingresa cuantos semestres lleva: ", &smtrs);
	inputFloat("Ingresa promedio del alumno: ", &prom);
	
	registrado.matricula = mat;
	strcpy(registrado.nombre, nom);
	registrado.semestres = smtrs;
	registrado.promedio = prom;
	
	insertaListaDFinal(lista,crearAlumno(registrado));
}
void mostrarAlumnos(ListaD lista){
	int opc;
	printf("\n[1] Desplegar lista de orden ASCENDENTE");
	printf("\n[2] Desplegar lista de orden DESCENDENTE");
	inputEntero("\nOpcion: ", &opc);
	
	switch(opc){
		case 1:
			mostrarListaD(lista);
		break;
		case 2:
			mostrarListaDInv(lista);
		break;
		default:
			printf("\nOpcion invalida");
		break;
	}
}
void buscarAlumno(ListaD lista){
	int opc, mat,smtrs;
	float prom;
	char nom[64];
	Alumno buscado; // Alumno auxiliar 
	printf("\nComo quieres buscar al alumno?");
	printf("\n(1) - Matricula");
	printf("\n(2) - Nombre");
	printf("\n(3) - Semestres");
	printf("\n(4) - Promedio");
	inputEntero("\nOpcion: ", &opc);
	
	switch(opc){
		case 1: // Se cambia la funcion de comparar de acuerdo a la opcion elegida, y el alumno se modifica con solo un atributo para la comparacion a buscar
			lista.comparar = &compararMatriculas;
			printf("\nIngresa matricula: ");
			scanf("%d", &mat);
			buscado.matricula = mat;
		break;
		case 2: 
			lista.comparar = &compararNombres;
			printf("\nIngresa nombre: ");
			scanf("%s", nom);
			strcpy(buscado.nombre, nom);
		break;
		case 3:   
			lista.comparar = &compararSemestres;
			printf("\nIngresa semestres: ");
			scanf("%d", &smtrs);
			buscado.semestres = smtrs;
		break;
		case 4:   
			lista.comparar = &compararPromedios;
			printf("\nIngresa promedio: ");
			scanf("%f", &prom);
			buscado.promedio = prom;
		break;
		default: printf("\nOpcion invalida"); break;
	}
	void *encontrado = buscarDato(lista, &buscado); // buscar dato regresara un alumno y se guarda aqui, si no encuentra el dato igual regresa NULL
	if(encontrado != NULL){
		printf("\nAlumno encontrado\n");
		imprimirAlumno(encontrado);
	}
	else
		printf("\nAlumno no encontrado\n");
}
void borraAlumno(ListaD *lista){
	int matr;
	inputEntero("\nIngresa matricula del alumno a eliminar: ", &matr);
	Alumno temp;
	temp.matricula = matr;
	lista->comparar = &compararMatriculas;
	lista->liberar = &liberarAlumno;
	borrarDato(lista, &temp); // Si encuentra la matricula borra el alumno 
	printf("\nAlumno eliminado");
	
}
void reordenaLista(ListaD *lista){
	ListaD lista2; // Lista auxiliar
	lista2.inicio = NULL;
	lista2.fin = NULL;
	lista2.comparar = &compararMatriculas;
	lista2.imprimir = &imprimirAlumno;
	lista2.cant = 0;
	
	
	int opc;
	printf("\nComo quieres acomodar la lista?");
	printf("\n(1) - Por matriculas");
	printf("\n(2) - Por nombres");
	printf("\n(3) - Por semestres");
	printf("\n(4) - Por promedio");
	inputEntero("\nOpcion: ", &opc);
	switch(opc){ // Opciones
		case 1: 
			lista2.comparar = &compararMatriculas; // Se cambia la funcion de comparar de la lista
			printf("\nLista ordenada por matriculas!");
		break;
		case 2: 
			lista2.comparar = &compararNombres; // Se cambia la funcion de comparar de la lista
			printf("\nLista ordenada por nombres!");
		break;
		case 3:   
			lista2.comparar = &compararSemestres; // Se cambia la funcion de comparar de la lista
			printf("\nLista ordenada por semestres!");
		break;
		case 4:   
			lista2.comparar = &compararPromedios; // Se cambia la funcion de comparar de la lista
			printf("\nLista ordenada por promedios!");
		break;
		default: printf("Opcion invalida"); break;
	}
	NodoD *aux = lista->inicio; // Nodo para recorrer la lista original
	while(aux!=NULL){
		insertarOrdenado(&lista2, aux->dato); // Ingresa segun la funcion de comparacion dada, se usa dentro de la funcion insertar
		aux = aux->sig;
	}
	lista->inicio = lista2.inicio; // Se modifica la lista original
	lista->cant = lista2.cant;
	lista->fin = lista2.fin;
}
void liberarLista(ListaD *lista)
{
    NodoD *actual = lista->inicio;
    NodoD *siguiente;
    
    while (actual != NULL)
    {
        siguiente = actual->sig;
        if(lista->liberar != NULL)
			lista->liberar(actual->dato);
		else
			free(actual->dato);
		
		free(actual);
		actual = siguiente;
    }
    
    lista->inicio = NULL;
    lista->fin = NULL;
	lista->cant = 0;
}

// Funciones de comparacion( cambia solo el atributo usado del alumno) 
int compararMatriculas(void *a, void *b)
{
	Alumno *ea = (Alumno *)a;
	Alumno *eb = (Alumno *)b;
	if( ea->matricula < eb->matricula )
		return -1;
	else if ( ea->matricula > eb->matricula )
		return 1;
	else
		return 0;	
}
int compararNombres(void *a, void *b)
{
	Alumno *ea = (Alumno *)a;
	Alumno *eb = (Alumno *)b;
	if( strcmp(ea->nombre,eb->nombre) < 0 )
		return -1;
	else if ( strcmp(ea->nombre,eb->nombre) > 0 )
		return 1;
	else
		return 0;	
}
int compararSemestres(void *a, void *b)
{
	Alumno *ea = (Alumno *)a;
	Alumno *eb = (Alumno *)b;
	if( ea->semestres < eb->semestres )
		return -1;
	else if ( ea->semestres > eb->semestres )
		return 1;
	else
		return 0;	
}
int compararPromedios(void *a, void *b)
{
	Alumno *ea = (Alumno *)a;
	Alumno *eb = (Alumno *)b;
	if( ea->promedio < eb->promedio )
		return -1;
	else if ( ea->promedio > eb->promedio )
		return 1;
	else
		return 0;	
}

int compararEnteros(void *a, void *b)
{
	int *ea,*eb;
	ea = a;
	eb = b;
	if( *ea < *eb )
		return -1;
	else if ( *ea > *eb)
		return 1;
	else
		return 0;	
}
