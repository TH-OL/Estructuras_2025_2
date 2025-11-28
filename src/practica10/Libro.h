#ifndef LIBRO_H
#define LIBRO_H

#include <stdio.h>
#define CANT_LIBROS 50
#define MAX_ISBN 21
#define MAX_TITULO 45
#define MAX_AUTOR 30


typedef struct
{
    char isbn[MAX_ISBN];   //International Standard Book Number
    char titulo[MAX_TITULO];
    char autor[MAX_AUTOR];
    int fecha;
    int disponible;
}Libro;


Libro* obtener_libros(void);
void imprimirLibro(void* vlibro);

#endif