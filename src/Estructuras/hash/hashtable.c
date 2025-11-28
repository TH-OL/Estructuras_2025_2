#include "hashtable.h"

static int _marcador_borrado; 
#define BORRADO ((void*)&_marcador_borrado)


int esPrimo(int n) {
    if (n <= 1) return 0;
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) return 0;
    }
    return 1;
}

int siguientePrimo(int n) {
    while (!esPrimo(n)) n++;
    return n;
}

int hash(int clave, int tam) {
    return abs(clave) % tam;
}

int rehash(int indice, int k, int tam) {
    return (indice + (k * k)) % tam;
}

int validarHashTable(HashTable *hashtable) {
    if (!hashtable->tabla || !hashtable->folding || !hashtable->comparar || !hashtable->imprimir)
        return 0;
    return 1;
}

HashTable inicializarHashTable(int tam, int (*folding)(void*), void (*imprimir)(void*), int (*comparar)(void*,void*)) {
    HashTable hashtable;
    hashtable.tabla = (void**)calloc(tam, sizeof(void*));
    hashtable.tam = tam;
    hashtable.cant = 0;
    hashtable.cant_ocup = 0;
    hashtable.folding = folding;
    hashtable.imprimir = imprimir;
    hashtable.comparar = comparar;
    return hashtable;
}

void redimensionar(HashTable *ht) {
    int nuevoTam = (int)(ht->cant / 0.6);
    if (nuevoTam < 10) nuevoTam = 10;
    nuevoTam = siguientePrimo(nuevoTam);

    void **viejaTabla = ht->tabla;
    int viejoTam = ht->tam;

    ht->tabla = (void**)calloc(nuevoTam, sizeof(void*));
    ht->tam = nuevoTam;
    ht->cant = 0;
    ht->cant_ocup = 0;

    for (int i = 0; i < viejoTam; i++) {
        if (viejaTabla[i] != NULL && viejaTabla[i] != BORRADO) {
            insertarClave(ht, viejaTabla[i]);
        }
    }
    free(viejaTabla);
}

int insertarClave(HashTable *hashtable, void *dato) {
    if (!validarHashTable(hashtable)) return 0;

    float factor = (float)(hashtable->cant + 1) / hashtable->tam;
    if (factor > 0.6) {
        redimensionar(hashtable);
    }

    int clave = hashtable->folding(dato);
    int indice = hash(clave, hashtable->tam);
    int i = indice;
    int k = 0;

    while (hashtable->tabla[i] != NULL && hashtable->tabla[i] != BORRADO) {
        k++;
        i = rehash(indice, k, hashtable->tam);
        if(k > hashtable->tam) return 0; 
    }

    hashtable->tabla[i] = dato;
    hashtable->cant++;
    hashtable->cant_ocup++;
    return 1;
}

int eliminarClave(HashTable *hashtable, void *dato) {
    if (!validarHashTable(hashtable)) return 0;

    int clave = hashtable->folding(dato);
    int indice = hash(clave, hashtable->tam);
    int i = indice;
    int k = 0;

    while (hashtable->tabla[i] != NULL) {
        if (hashtable->tabla[i] != BORRADO) {
            if (hashtable->comparar(hashtable->tabla[i], dato) == 0) {
                if (hashtable->tabla[i] == dato) {
                    hashtable->tabla[i] = BORRADO;
                    hashtable->cant--;
                    if ((float)hashtable->cant / hashtable->tam < 0.3 && hashtable->tam > 10) 
                        redimensionar(hashtable);
                    return 1;
                }
            }
        }
        k++;
        i = rehash(indice, k, hashtable->tam);
        if(k > hashtable->tam) break;
    }
    return 0;
}

Cola buscarClave(HashTable *hashtable, void *dato) {
    Cola cola = inicializarCola();
    if (!validarHashTable(hashtable)) return cola;

    int clave = hashtable->folding(dato);
    int indice = hash(clave, hashtable->tam);
    int i = indice;
    int k = 0;

    while (hashtable->tabla[i] != NULL) {
        if (hashtable->tabla[i] != BORRADO) {
            if (hashtable->comparar(hashtable->tabla[i], dato) == 0) {
                enqueue(&cola, hashtable->tabla[i]);
            }
        }
        k++;
        i = rehash(indice, k, hashtable->tam);
        if (k > hashtable->tam) break;
    }
    return cola;
}

void imprimirHashTabla(HashTable *hashtable) {
    if (!validarHashTable(hashtable)) return;
    for (int i = 0; i < hashtable->tam; i++) {
        if (hashtable->tabla[i] != NULL) {
            printf("\n [%d] = ", i);
            if (hashtable->tabla[i] == BORRADO)
                printf("[BORRADO]");
            else
                hashtable->imprimir(hashtable->tabla[i]);
        } else {
            printf("\n [%d] = NULL", i);
        }
    }
}

void liberarHashTable(HashTable *hashtable) {
    if(hashtable->tabla) {
        free(hashtable->tabla);
        hashtable->tabla = NULL;
        hashtable->cant = 0;
        hashtable->tam = 0;
    }
}