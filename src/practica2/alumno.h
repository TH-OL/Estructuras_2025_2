//alumno.h
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct
{
	unsigned int matricula;
	char nombre[64];
	int semestres;
	float promedio;
}Alumno;

void* crearAlumno( Alumno a);
void imprimirAlumno(void *a);
void liberarAlumno(void *dato);
