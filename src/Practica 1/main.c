//INCLUDES
#include <stdio.h>
#include <string.h>
#include "Lista.h"
//FUNCION MAIN

// Estructura de alumno
typedef struct
{
	unsigned int matricula;
	char nombre[64];
	int semestres;
	float promedio;
}Alumno;

// Prototipos de funciones
void* crearEntero(int dato);
int compararEnteros(void *a, void *b);
void imprimirEntero(void *a);
void* crearFlotante(float dato);
int compararFlotante(void *a, void *b);
void imprimirFlotante(void *a);

void registraAlumno(Lista *lista);
void* crearAlumno( Alumno a);
int compararAlumnos(void *a,void *b);
void imprimirAlumno(void *a);
void borrarAlumno(Lista *lista);
void reordenarAlumnos(Lista *lista);
void buscarAlumno(Lista lista);
int compararMatriculas(void *a, void *b);
int compararNombres(void *a, void *b);
int compararSemestres(void *a, void *b);
int compararPromedios(void *a, void *b);
void liberarLista(Lista *lista);

int main(void)
{
	// Se crea la lista 
	Lista lista;
	lista.inicio = NULL;
	lista.cant = 0;
	lista.imprimir = &imprimirAlumno;
	lista.comparar = &compararMatriculas;
	
	int opc;
	// Datos de prueba
	insertarEnPosicion(&lista,  crearAlumno( (Alumno){120938,"David",2,78.5} ) , 0   );
	insertarEnPosicion(&lista,  crearAlumno( (Alumno){918422,"Manuel",5,65.6} ) , 1   );
	insertarEnPosicion(&lista,  crearAlumno( (Alumno){123091,"Jose",1,13.4} ) , 2   );
	insertarEnPosicion(&lista,  crearAlumno( (Alumno){309485,"Avid",3,98.1} ) , 3   );
	insertarEnPosicion(&lista,  crearAlumno( (Alumno){103928,"Jareth",10,86.5} ) , 4   );
	insertarEnPosicion(&lista,  crearAlumno( (Alumno){304958,"Carlo",7,47.4} ) , 5  );
	
	do{
		printf("\n- Menu -");
		printf("\n(1) - Registrar alumno");
		printf("\n(2) - Mostrar alumnos");
		printf("\n(3) - Reordenar lista de alumnos");
		printf("\n(4) - Buscar alumno");
		printf("\n(5) - Borrar alumno");
		printf("\n(6) - Salir");
		printf("\nSelecciona una opcion: ");
		scanf("%d", &opc);
		switch(opc){ // Menu de opciones
			case 1: registraAlumno(&lista); break;
			case 2: mostrarLista(lista); break;
			case 3: reordenarAlumnos(&lista);  break;
			case 4: buscarAlumno(lista); break;
			case 5: borrarAlumno(&lista); break;
			case 6: printf("\nHasta luego!"); liberarLista(&lista); return 0; break;
			default: printf("Opcion invalida ingrese otra"); break;
		}
		printf("\nSeguir trabajando?\n(1) - Si\n(2) - No\n");
		scanf("%d", &opc);
		
	}while(opc != 2);	
	liberarLista(&lista); // Fin de programa
	return 0;
}

void registraAlumno(Lista *lista){
	int mat,smtrs;
	float prom;
	char nom[64];
	Alumno registrado; 
	
	printf("Ingresa matricula de alumno: ");
	scanf("%d", &mat);
	Nodo *aux = lista->inicio;
	while(aux != NULL){ // Buscar si ya existe la matricula en la lista
		Alumno *a = (Alumno *)aux->dato; // Se toma el dato Alumno del nodo actual
		if(compararEnteros(&mat, &a->matricula) == 0){ // Se usa funcion para comparar
			do{
				printf("Matricula existente ingrese otra: ");
				scanf("%d", &mat);
			}while(compararEnteros(&mat, &a->matricula) == 0); // Se repite hasta ingresar matricula correcta
			break;
		}
		aux = aux->sig;
	} // Registra los datos restantes
	printf("Ingresa nombre del alumno: ");
	scanf("%s", nom);
	printf("Ingresa cuantos semestres lleva: ");
	scanf("%d", &smtrs);
	printf("Ingresa promedio del alumno: ");
	scanf("%f", &prom);
	registrado.matricula = mat;
	strcpy(registrado.nombre, nom);
	registrado.semestres = smtrs;
	registrado.promedio = prom;
	
	insertarFinal(lista, crearAlumno(registrado)); // Ingresa a lista
	printf("\nAlumno registrado!");
}
void reordenarAlumnos(Lista *lista){
	Lista lista2; // Lista auxiliar
	lista2.inicio = NULL;
	lista2.comparar = &compararAlumnos;
	lista2.imprimir = &imprimirAlumno;
	lista2.cant = 0;
	
	int opc;
	printf("\nComo quieres acomodar la lista?");
	printf("\n(1) - Por matriculas");
	printf("\n(2) - Por nombres");
	printf("\n(3) - Por semestres");
	printf("\n(4) - Por promedio");
	printf("\nSeleccion: ");
	scanf("%d", &opc);
	
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
	Nodo *aux = lista->inicio; // Nodo para recorrer la lista original
	while(aux!=NULL){
		insertarOrdenado(&lista2, aux->dato); // Ingresa segun la funcion de comparacion dada, se usa dentro de la funcion insertar
		aux = aux->sig;
	}
	lista->inicio = lista2.inicio; // Se modifica la lista original
	lista->cant = lista2.cant;	
}
void buscarAlumno(Lista lista){
	int opc, mat,smtrs;
	float prom;
	char nom[64];
	Alumno buscado; // Alumno auxiliar 
	printf("\nComo quieres buscar al alumno?");
	printf("\n(1) - Matricula");
	printf("\n(2) - Nombre");
	printf("\n(3) - Semestres");
	printf("\n(4) - Promedio");
	printf("\nSelecciona una opcion: ");
	scanf("%d", &opc);
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
void borrarAlumno(Lista *lista){
	int matr;
	printf("\nIngresa la matricula del alumno a eliminar: ");
	scanf("%d", &matr);
	Alumno temp;
	temp.matricula = matr;
	borrarDato(lista, &temp); // Si encuentra la matricula borra el alumno 
	printf("\nAlumno eliminado");
}
void* crearEntero(int dato)
{
	int *nuevo = malloc(sizeof(int));
	*nuevo = dato;
	return nuevo;
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

void imprimirEntero(void *a)
{
	int *ea;
	ea = a;
	printf("%d",*ea);
}
void* crearFlotante(float dato)
{
	float *nuevo = malloc(sizeof(float));
	*nuevo = dato;
	return nuevo;
}
int compararFlotante(void *a, void *b)
{
	float *ea,*eb;
	ea = a;
	eb = b;
	if( *ea < *eb )
		return -1;
	else if ( *ea > *eb)
		return 1;
	else
		return 0;	
}
void imprimirFlotante(void *a)
{
	float *ea;
	ea = a;
	printf("%.2f",*ea);
}
void* crearAlumno( Alumno a)
{
	Alumno *nuevo = malloc(sizeof(Alumno));
	*nuevo = a;
	return nuevo;	
}

int compararAlumnos(void *a,void *b)
{
/*	Alumno *aa,*ab;
	aa = a;
	ab = b;
	return strcmp( aa->nombre,ab->nombre);
*/}
void imprimirAlumno(void *a)
{
	Alumno *aa;
	aa = a;
	printf("Alumno: %s Matricula: %d Semestres: %d Promedio: %.2f\n",aa->nombre,aa->matricula,aa->semestres,aa->promedio);
	
}
// Funcion para liberar la lista
void liberarLista(Lista *lista)
{
    Nodo *actual = lista->inicio;
    Nodo *siguiente;
    
    while (actual != NULL)
    {
        siguiente = actual->sig;
        free(actual->dato);  // liberar el alumno
        free(actual);        // liberar el nodo
        actual = siguiente;
    }
    
    lista->inicio = NULL;
    lista->cant = 0;
}