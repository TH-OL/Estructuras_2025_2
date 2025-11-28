//alumno.c
#include "alumno.h"

void* crearAlumno( Alumno a)
{
	Alumno *nuevo = malloc(sizeof(Alumno));
	*nuevo = a;
	return nuevo;	
}
void imprimirAlumno(void *a)
{
	Alumno *aa;
	aa = a;
	printf("Alumno: %s Matricula: %d Semestres: %d Promedio: %.2f\n",aa->nombre,aa->matricula,aa->semestres,aa->promedio);
	
}
void liberarAlumno(void *dato)
{
    free((Alumno *)dato);
}