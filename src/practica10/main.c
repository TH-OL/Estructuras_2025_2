#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "../Estructuras/hash/hashtable.h"
#include "../Estructuras/arbol/arbol.h"
#include "Libro.h"
#include "../captura/captura.h"
#include "../Estructuras/cola/cola.h" 

HashTable ht_isbn, ht_titulo, ht_autor, ht_anio, ht_ini_tit, ht_ini_aut;

void buscarISBN();
void buscarTitulo(); 
void buscarAutor(); 
void buscarFecha(); 
void eliminarLibro(); 

int folding_isbn(void *dato);
int cmp_isbn(void *a, void *b);

int folding_titulo(void *dato);
int cmp_titulo(void *a, void *b);

int folding_autor(void *dato);
int cmp_autor(void *a, void *b);

int folding_anio(void *dato);
int cmp_anio(void *a, void *b);

int folding_inicial_titulo(void *dato);
int cmp_inicial_titulo(void *a, void *b);

int folding_inicial_autor(void *dato);
int cmp_inicial_autor(void *a, void *b);

void inicializarHashTables();
void mostrarResultados(Cola *c, int (*comparador)(void*, void*));
void agregarLibro();

int main(void)
{
    inicializarHashTables();

    Libro *libros = obtener_libros();
    for(int i = 0; i < CANT_LIBROS; i++) {
		imprimirLibro(&libros[i]);
        insertarClave(&ht_isbn, &libros[i]);
        insertarClave(&ht_titulo, &libros[i]);
        insertarClave(&ht_autor, &libros[i]);
        insertarClave(&ht_anio, &libros[i]);
        insertarClave(&ht_ini_tit, &libros[i]);
        insertarClave(&ht_ini_aut, &libros[i]);
    }
    int opc;
    do {
        printf("\nMENU\n");
        printf("\n[1] Buscar por ISBN");
        printf("\n[2] Buscar por TITULO");
        printf("\n[3] Buscar por AUTOR");
        printf("\n[4] Buscar por FECHA");
        printf("\n[5] AGREGAR Libro");
        printf("\n[6] ELIMINAR Libro");
        printf("\n[7] Salir");
        inputEntero("\nSelecciona: ", &opc);

        switch(opc) {
			case 1: buscarISBN(); break;
			case 2: buscarTitulo(); break;
			case 3: buscarAutor(); break;
			case 4: buscarFecha(); break;
			case 5: agregarLibro(); break;
			case 6: eliminarLibro(); break;
		}
    } while(opc != 7);
    return 0;
}
void buscarISBN() {
    Libro libroAux;
    Cola resultados;

    inputCadena("Ingrese ISBN a buscar: ", libroAux.isbn, MAX_ISBN);
    resultados = buscarClave(&ht_isbn, &libroAux);
    mostrarResultados(&resultados, cmp_isbn);
}
void buscarTitulo() {
    int subopc;
    Libro libroAux;

    inputEntero("\n[1] Titulo Exacto\n[2] Rango de Iniciales\nSelecciona: ", &subopc);

    if(subopc == 1) {
        inputCadena("Ingrese Titulo: ", libroAux.titulo, MAX_TITULO);
        Cola res = buscarClave(&ht_titulo, &libroAux);
        mostrarResultados(&res, cmp_titulo);
    } 
    else {
        char ini, fin;
        inputChar("Inicial Inicio (A-Z): ", &ini);
        inputChar("Inicial Fin (A-Z): ", &fin);

        Cola acumulada = inicializarCola();

        for(char c = toupper(ini); c <= toupper(fin); c++) {
            libroAux.titulo[0] = c;
            libroAux.titulo[1] = '\0';

            Cola parcial = buscarClave(&ht_ini_tit, &libroAux);
            while(!vaciaC(parcial))
                enqueue(&acumulada, dequeue(&parcial));
        }

        mostrarResultados(&acumulada, cmp_titulo);
    }
}
void buscarAutor() {
    int subopc;
    Libro libroAux;

    inputEntero("\n[1] Nombre Exacto\n[2] Rango de Iniciales\nSeleccionar: ", &subopc);

    if(subopc == 1) {
        inputCadena("Ingrese Autor: ", libroAux.autor, MAX_AUTOR);
        Cola res = buscarClave(&ht_autor, &libroAux);
        mostrarResultados(&res, cmp_autor);
    } 
    else {
        char ini, fin;
        inputChar("Inicial Inicio (A-Z): ", &ini);
        inputChar("Inicial Fin (A-Z): ", &fin);

        Cola acumulada = inicializarCola();

        for(char c = toupper(ini); c <= toupper(fin); c++) {
            libroAux.autor[0] = c;
            libroAux.autor[1] = '\0';

            Cola parcial = buscarClave(&ht_ini_aut, &libroAux);
            while(!vaciaC(parcial))
                enqueue(&acumulada, dequeue(&parcial));
        }

        mostrarResultados(&acumulada, cmp_autor);
    }
}
void buscarFecha() {
    int subopc;
    Libro libroAux;

    inputEntero("\n[1] Anio Fijo\n[2] Rango de Anios\nSeleccionar: ", &subopc);

    if(subopc == 1) {
        inputEntero("Ingrese Anio: ", &libroAux.fecha);
        Cola res = buscarClave(&ht_anio, &libroAux);
        mostrarResultados(&res, cmp_anio);
    } 
    else {
        int ini, fin;
        inputEntero("Anio Inicio: ", &ini);
        inputEntero("Anio Fin: ", &fin);

        Cola acumulada = inicializarCola();

        for(int y = ini; y <= fin; y++) {
            libroAux.fecha = y;

            Cola parcial = buscarClave(&ht_anio, &libroAux);
            while(!vaciaC(parcial))
                enqueue(&acumulada, dequeue(&parcial));
        }

        mostrarResultados(&acumulada, cmp_anio);
    }
}
void eliminarLibro() {
    Libro libroAux;
    Cola resultados;

    inputCadena("Ingrese ISBN: ", libroAux.isbn, MAX_ISBN);
    resultados = buscarClave(&ht_isbn, &libroAux);

    if(vaciaC(resultados)) {
        printf("Libro no encontrado\n");
        return;
    }

    Libro *libro = (Libro*)dequeue(&resultados);

    eliminarClave(&ht_isbn, libro);
    eliminarClave(&ht_titulo, libro);
    eliminarClave(&ht_autor, libro);
    eliminarClave(&ht_anio, libro);
    eliminarClave(&ht_ini_tit, libro);
    eliminarClave(&ht_ini_aut, libro);

    printf("Libro '%s' eliminado de todas las tablas exitosamente.\n", libro->titulo);

    free(libro);
}
void agregarLibro() {
    Libro *nuevoLibro = (Libro*)malloc(sizeof(Libro));
    

    inputCadena("ISBN: ", nuevoLibro->isbn, MAX_ISBN);

    Libro aux;
    strcpy(aux.isbn, nuevoLibro->isbn);
    Cola resultado = buscarClave(&ht_isbn, &aux);
    
    if(!vaciaC(resultado)) {
        printf("\nError: Ya existe un libro con ese ISBN\n");
        free(nuevoLibro);
        return;
    }
    
    inputCadena("Titulo: ", nuevoLibro->titulo, MAX_TITULO);
    inputCadena("Autor: ", nuevoLibro->autor, MAX_AUTOR);
    inputEntero("Anio: ", &nuevoLibro->fecha);
    

    insertarClave(&ht_isbn, nuevoLibro);
    insertarClave(&ht_titulo, nuevoLibro);
    insertarClave(&ht_autor, nuevoLibro);
    insertarClave(&ht_anio, nuevoLibro);
    insertarClave(&ht_ini_tit, nuevoLibro);
    insertarClave(&ht_ini_aut, nuevoLibro);
    
    printf("\nLibro agregado\n");
}

int folding_isbn(void *dato) {
    Libro *l = (Libro*)dato;
    int suma = 0;
    char *str = l->isbn;
    while(*str) {
        if(isdigit(*str)) suma += (*str - '0'); 
        str++;
    }
    return suma;
}

int cmp_isbn(void *a, void *b) {
    Libro *l1 = (Libro*)a;
    Libro *l2 = (Libro*)b;
    return strcmp(l1->isbn, l2->isbn);
}

int folding_titulo(void *dato) {
    Libro *l = (Libro*)dato;
    int suma = 0;
    char *str = l->titulo;
    for(int i=0; str[i]; i++) suma += (int)str[i];
    return suma;
}

int cmp_titulo(void *a, void *b) {
    Libro *l1 = (Libro*)a;
    Libro *l2 = (Libro*)b;
    return strcmp(l1->titulo, l2->titulo);
}

int folding_autor(void *dato) {
    Libro *l = (Libro*)dato;
    int suma = 0;
    char *str = l->autor;
    for(int i=0; str[i]; i++) suma += (int)str[i];
    return suma;
}

int cmp_autor(void *a, void *b) {
    Libro *l1 = (Libro*)a;
    Libro *l2 = (Libro*)b;
    return strcmp(l1->autor, l2->autor);
}

int folding_anio(void *dato) {
    Libro *l = (Libro*)dato;
    int anio = l->fecha;
    
    int parte1 = anio / 100;
    int parte2 = anio % 100;
    
    return parte1 + parte2;
}

int cmp_anio(void *a, void *b) {
    Libro *l1 = (Libro*)a;
    Libro *l2 = (Libro*)b;
    return l1->fecha - l2->fecha;
}

int folding_inicial_titulo(void *dato) {
    Libro *l = (Libro*)dato;
    return (int)toupper(l->titulo[0]);
}

int cmp_inicial_titulo(void *a, void *b) {
    Libro *l1 = (Libro*)a;
    Libro *l2 = (Libro*)b;
    return toupper(l1->titulo[0]) - toupper(l2->titulo[0]);
}

int folding_inicial_autor(void *dato) {
    Libro *l = (Libro*)dato;
    return (int)toupper(l->autor[0]);
}

int cmp_inicial_autor(void *a, void *b) {
    Libro *l1 = (Libro*)a;
    Libro *l2 = (Libro*)b;
    return toupper(l1->autor[0]) - toupper(l2->autor[0]);
}

void inicializarHashTables() {
    int tam = 13; 
    ht_isbn = inicializarHashTable(tam, folding_isbn, imprimirLibro, cmp_isbn);
    ht_titulo = inicializarHashTable(tam, folding_titulo, imprimirLibro, cmp_titulo);
    ht_autor = inicializarHashTable(tam, folding_autor, imprimirLibro, cmp_autor);
    ht_anio = inicializarHashTable(tam, folding_anio, imprimirLibro, cmp_anio);
    ht_ini_tit = inicializarHashTable(tam, folding_inicial_titulo, imprimirLibro, cmp_inicial_titulo);
    ht_ini_aut = inicializarHashTable(tam, folding_inicial_autor, imprimirLibro, cmp_inicial_autor);
}

void mostrarResultados(Cola *c, int (*comparador)(void*, void*)) {
    if(vaciaC(*c)) { 
        printf(" No se encontraron resultados\n");
        return;
    }

    Arbol arbol;
    arbol.raiz = NULL;
    arbol.cantidad = 0;
    arbol.comparar = comparador; 
    arbol.liberar = NULL; 
    arbol.imprimir = (void (*)(void*))imprimirLibro; 

    while(!vaciaC(*c)) {
        insertarArbol(&arbol, dequeue(c));
    }

    printf("%-18s  %-41s  %-25s  %s  %s\n", "ISBN", "TITULO", "AUTOR", "ANIO", "ESTADO");

    imprimirOrden(arbol, ORDEN);
    eliminarArbol(&arbol);
}