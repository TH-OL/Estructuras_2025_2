#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../captura/captura.h"
#include "../Estructuras/heap/heap.h"

typedef struct {
    char *nombre;
    int paginas;
} Archivo;

Archivo* crearArchivo(const char *nombre, int paginas);
void datosPrueba(Heap *heap);
int compararArchivo(void*, void*);
void imprimirArchivo(void*);
void liberarArchivo(void*);
void mostrar5primeros(Heap heap);
void mostrarEstadoHeap(Heap heap, const char *momento);
void cambiarPrioridad(Heap *heap);
void agregarArchivo(Heap *heap);
void procesarArchivo(Heap *heap);
void eliminarArchivo(Heap *heap);
void eliminarTodosArchivos(Heap *heap);

int main(void) {
    Heap heap = inicializarHeap(&imprimirArchivo, &compararArchivo);
    heap.tipo = MIN; 
    int opcion;
    
	datosPrueba(&heap);
	
    do {
        printf("\n");
		mostrar5primeros(heap);
        printf("\nMENU\n");
        printf("[0] CAMBIAR PRIORIDAD\n");
		printf("[1] MOSTRAR COLA DE IMPRESION\n");
		printf("[2] AGREGAR ARCHIVO\n");
		printf("[3] PROCESAR/IMPRIMIR ARCHIVO\n");
		printf("[4] ELIMINAR ARCHIVO\n");
		printf("[5] ELIMINAR TODOS LOS ARCHIVOS\n");
		printf("[6] TERMINAR PROGRAMA\n");
        if(inputEntero("\nOpcion: ", &opcion) != 1) {
            printf("\nEntrada invalida. Intente de nuevo.\n");
            continue;
        }
        
        switch(opcion) {
            case 0:
                cambiarPrioridad(&heap);
            break;
            case 1:
                mostrarEstadoHeap(heap, "");
            break;
            case 2:
                agregarArchivo(&heap);
            break;
            case 3:
                procesarArchivo(&heap);
            break;
            case 4:
                eliminarArchivo(&heap);
            break;
            case 5:
                eliminarTodosArchivos(&heap);
            break;
            case 6:
                eliminarTodosArchivos(&heap);
				printf("\nTerminando programa...\n");
            break;
            default:
                printf("\nOpcion invalida. Intente de nuevo.\n");
			break;
        }
        
    } while(opcion != 6);
    
    return 0;
}

Archivo* crearArchivo(const char *nombre, int paginas) {
    Archivo *nuevo = (Archivo*)malloc(sizeof(Archivo));
    if (!nuevo) {
        printf("\nError: Fallo al asignar memoria para Archivo.\n");
        return NULL;
    }
    nuevo->nombre = cadenaDinamica(nombre); 
    nuevo->paginas = paginas;
    return nuevo;
}

void datosPrueba(Heap *heap) {
    Archivo *arch1 = crearArchivo("Reporte_Anual", 85);
    Archivo *arch2 = crearArchivo("Tesis_Final", 320);
    Archivo *arch3 = crearArchivo("Factura_Venta", 12);
    Archivo *arch4 = crearArchivo("Curriculum_Juan", 3);
    Archivo *arch5 = crearArchivo("Contrato_2025", 55);

    // Insertar solo si la creación fue exitosa
    if (arch1) insertarHeap(heap, arch1);
    if (arch2) insertarHeap(heap, arch2);
    if (arch3) insertarHeap(heap, arch3);
    if (arch4) insertarHeap(heap, arch4);
    if (arch5) insertarHeap(heap, arch5);
    
    printf("Datos cargados. Cantidad de archivos: %d\n", heap->cantidad);
}

//Funcion para mostrar los 5 primeros archivos en el heap
void mostrar5primeros(Heap heap) {
    printf("Prioridad: %s\n", heap.tipo == MIN ? "MINIMO" : "MAXIMO");
	printf("\nCola de impresion:\n");
    if(heap.cantidad == 0) {
        printf("Sin archivos\n");
        return;
    }
    
    int limite = heap.cantidad < 5 ? heap.cantidad : 5;
    for(int i = 0; i < limite; i++) {
        Archivo *arch = (Archivo*)heap.arr[i]->dato;
        printf("  %d. %s Paginas: %d \n", i+1, arch->nombre, arch->paginas);
    }
}

void mostrarEstadoHeap(Heap heap, const char *momento) {
    if(strlen(momento) > 0) {
        printf("\nHEAP %s\n", momento);
    }
    
    printf("\nArreglo:\n");
    if(heap.cantidad == 0) {
        printf(" ");
    } else {
        imprimirHeap(heap);
    }
    printf("\n\nArbol:\n");
    if(heap.cantidad == 0) {
        printf(" \n");
    } else {
        imprimirArbol(heap.arbol);
        printf("\n");
    }
}

void cambiarPrioridad(Heap *heap) {
    printf("\nPrioridad de heap cambiada");
	mostrarEstadoHeap(*heap, "ANTES");
	
    if(heap->tipo == MIN) {
        heap->tipo = MAX;
    } else {
        heap->tipo = MIN;
    }
    
    heapify(heap);
	mostrarEstadoHeap(*heap, "DESPUES");
}

void agregarArchivo(Heap *heap) {
	Archivo *nuevo = (Archivo*)malloc(sizeof(Archivo));
    if(!nuevo) {
        printf("\nError asignando memoria\n");
        return;
    }
    
    char nombre[100];
    if(inputCadena("Nombre del archivo: ", nombre, 100) != 1) {
        free(nuevo);
        printf("\nError de captura\n");
        return;
    }
    int paginas;
    if(inputEntero("Numero de paginas: ", &paginas) != 1 || paginas <= 0) {
        free(nuevo);
        printf("\nError de captura\n");
        return;
    }
    
    nuevo->nombre = cadenaDinamica(nombre);
    nuevo->paginas = paginas;
	
	printf("\nArchivo agregado");
	mostrarEstadoHeap(*heap, "ANTES");
    insertarHeap(heap, nuevo);
	mostrarEstadoHeap(*heap, "DESPUES");
}

void procesarArchivo(Heap *heap) {
    printf("\nArchivo procesado!");
	mostrarEstadoHeap(*heap, "ANTES");
    
    if(heap->cantidad == 0) {
        printf("\nNo hay archivos.\n");
        return;
    }
    Archivo *arch = (Archivo*)quitarHeap(heap);
    if(arch)
        liberarArchivo(arch);
		
    mostrarEstadoHeap(*heap, "DESPUES");
}

void eliminarArchivo(Heap *heap) {
    if(heap->cantidad == 0) {
        printf("\nNo hay archivos\n");
        return;
    }
    
    printf("\nArchivos:\n");
    for(int i = 0; i < heap->cantidad; i++) {
        Archivo *arch = (Archivo*)heap->arr[i]->dato;
        printf("[%d] %s (%d pags)\n", i+1, arch->nombre, arch->paginas);
    }
    
    int opc;
    if(inputEntero("\nArchivo a eliminar: ", &opc) != 1) {
        printf("\nEntrada invalida.\n");
        return;
    }
    opc--;
    if(opc < 0 || opc >= heap->cantidad) {
        printf("\nIndice fuera de rango.\n");
        return;
    }
    
    mostrarEstadoHeap(*heap, "ANTES");
    
    Archivo *arch = (Archivo*)heap->arr[opc]->dato;

    liberarArchivo(arch);
    free(heap->arr[opc]);
    
    // Ponemos ultimo donde se elimina
    heap->arr[opc] = heap->arr[heap->cantidad - 1];
    heap->cantidad--;
    
    if(heap->cantidad > 0) {
        heap->arr = realloc(heap->arr, sizeof(NodoA*) * heap->cantidad);
        heapify(heap);
    } else {
        free(heap->arr);
        heap->arr = NULL;
    }
    
    mostrarEstadoHeap(*heap, "DESPUES");
}

void eliminarTodosArchivos(Heap *heap) {
    mostrarEstadoHeap(*heap, "ANTES");
	if(heap->cantidad == 0)
        return;
	
    // Liberar todos los archivos
    for(int i = 0; i < heap->cantidad; i++) {
        Archivo *arch = (Archivo*)heap->arr[i]->dato;
        liberarArchivo(arch);
        free(heap->arr[i]);
    }
    
    free(heap->arr);
    heap->arr = NULL;
    heap->cantidad = 0;
    heap->arbol.raiz = NULL;
    heap->arbol.cantidad = 0;
	mostrarEstadoHeap(*heap, "DESPUES");
}

int compararArchivo(void *a, void *b) {
    Archivo *aa = (Archivo*)a;
    Archivo *bb = (Archivo*)b;
    
    if(aa->paginas == bb->paginas) return 0;
    else if(aa->paginas < bb->paginas) return -1;
    else return 1;
}

void imprimirArchivo(void *a) {
    Archivo *arch = (Archivo*)a;
    printf("%s(%d) ", arch->nombre, arch->paginas);
}

void liberarArchivo(void *a) {
    Archivo *arch = (Archivo*)a;
    if(arch) {
        if(arch->nombre) free(arch->nombre);
        free(arch);
    }
}
