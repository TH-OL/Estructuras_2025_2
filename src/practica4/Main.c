#include <stdio.h>
#include <stdlib.h>
#include "../Estructuras/pila/pila.h"

#define ARRIBA 1
#define ABAJO 2
#define DERECHA 4
#define IZQUIERDA 8

#define REN 27
#define COL 26

typedef struct
{
	int x,y;
}Coordenada;


unsigned char ** crear_laberinto(void);
void imprimirLab(unsigned char **lab);


Coordenada* crearCoordenada(int x,int y);
int compararCoordenada(void *a,void *b);
void imprimirCoordenada(void *dato);

unsigned char alternativas(unsigned char **lab,Coordenada *coordenada);
void imprimirPosibles(unsigned char);

Coordenada *buscar_caracter(unsigned char **lab, char caracter);
void resuelveLab(unsigned char **lab);

int main(void)
{

    //int i,j;
    
    unsigned char **lab; 
    lab = crear_laberinto();
	lab[3][16] = 'B';
	imprimirLab(lab);
	printf("\n");
	resuelveLab(lab);
	printf("\n");
	imprimirLab(lab);
    printf("\n\n");
    return 0;
}

void resuelveLab(unsigned char **lab){
    Pila camino = inicializarPila();
    camino.imprimir = imprimirCoordenada;
    camino.liberar = free; 
    
	Coordenada *inicio = buscar_caracter(lab, 'A');
    if (!inicio) {
        printf("\nError: No se encontro el inicio A");
        return;
    }

    push(&camino, inicio);
    
    Coordenada *actual;
    unsigned char alternativas_posibles;

    while (!vacia(camino))
    {
        actual = (Coordenada *)peek(camino);
		if (lab[actual->x][actual->y] == 'B') // Verficar si se encontro el final o punto B 
        {
            printf("\nDestino B encontrado, Camino Correcto (de Cima a Base):");
            imprimirPilaInvertida(camino); 
            eliminarPila(&camino);
            return;
        }

        alternativas_posibles = alternativas(lab, actual); // se busca una alternativa 
		if (alternativas_posibles > 0)
        {
            Coordenada *siguiente = NULL;
            int next_x = actual->x;
            int next_y = actual->y;

            // Prioridad ORIGINAL: ABAJO > DERECHA > ARRIBA > IZQUIERDA 
            // (El orden de #define es 1:ARRIBA, 2:ABAJO, 4:DERECHA, 8:IZQUIERDA)

            if (alternativas_posibles & ABAJO) {
                next_x += 1;
            } else if (alternativas_posibles & DERECHA) {
                next_y += 1;
            } else if (alternativas_posibles & ARRIBA) {
                next_x -= 1;
            } else if (alternativas_posibles & IZQUIERDA) {
                next_y -= 1;
            }
            siguiente = crearCoordenada(next_x, next_y);

            // Marcar la celda actual como visitada (migaja)
            if (lab[actual->x][actual->y] != 'A') {
                lab[actual->x][actual->y] = '.'; // Marcado de avance
            }
            push(&camino, siguiente);
            //imprimirLab(lab); 

        } else {
            // camino cerrado
            if (lab[actual->x][actual->y] != 'A') {
                lab[actual->x][actual->y] = 'o';
            }
            free(pop(&camino)); 

            //imprimirLab(lab); 
        }
    }

    printf("\n No se encontro camino al destino B");
    eliminarPila(&camino);
}
Coordenada *buscar_caracter(unsigned char **lab, char caracter){
	for (int i = 0; i < REN; i++) {
        for (int j = 0; j < COL; j++) {
            if (lab[i][j] == caracter) {
                return crearCoordenada(i, j); // x=i y=j 
            }
        }
    }
    return NULL;
}


unsigned char ** crear_laberinto(void)
{
    int i,j;
    char *tmp = 
"xxxxxxxxxxxxxxxxxxxxxxxxxx\
x*x*****x****************x\
x*xxx*xAx*xxxxxxxxx*xxxx*x\
x*****x*x******x**x****x*x\
xxxxxxx*xxxxxx*xx*xxxx*xxx\
x*****x*x****x*x****xx***x\
x*xxx*x*xxxx*x*x*xxxxx*x*x\
x***x*x****x*****xxxxxxx*x\
xxx*x*xxxx*xxxxxxx****x**x\
x*x*x***xx****xx***xx*x*xx\
x*x*x*x*xxxxx**x*xxxx*x**x\
x*x*x*x***x*xx*x****x*xx*x\
x*x*x*xxx****x*x*xx*x****x\
x*x*x*xxxxxxxx*x**x*xxxx*x\
x***x********x*xx*x*x****x\
x*xxxxxxxxxx*x**xxx*x*xxxx\
x***x******x**x*****x**x*x\
xxx*x*xxxxxxx*xxxxxxxx*x*x\
x*x*x*******x****xx****x*x\
x*x*x*xxxxx*xxxx*xx*xxxx*x\
x*x*x****xx***x**xx*x****x\
x*x*xxxxxxx*x**x*xx*x*x*xx\
x*x*********xx*x*xx*xxx*xx\
x*xxxxxxxxxxx**x*********x\
x***x***x***x*xxxxxxxxxx*x\
x*x***x***x**************x\
xxxxxxxxxxxxxxxxxxxxxxxxxx\0";

 
    
    unsigned char **lab;
    lab = (unsigned char**) malloc(sizeof(unsigned char *)*REN);
    
    for(i=0;i<REN;i++)
    {
        lab[i] = (unsigned char*) malloc(sizeof(unsigned char)*COL);
    }
    
    for(i=0;i<REN;i++)
    {
        //printf("\n\r");
        for(j=0;j<COL;j++)
        {
            //printf("%c",tmp[(i*COL)+j]);
            lab[i][j] = tmp[(i*COL)+j];
        }   
    }
    return lab;
}


void imprimirLab(unsigned char **lab)
{
	int i,j;
	for(i=0;i<REN;i++)
    {
        printf("\n\r");
        for(j=0;j<COL;j++)
        {
            if(lab[i][j]=='*')printf("  ");
			else printf("%c ",lab[i][j]);
        }   
    }
}

Coordenada* crearCoordenada(int x,int y)
{
	Coordenada *nueva = malloc(sizeof(Coordenada));
	nueva->x=x;
	nueva->y=y;
	return nueva;
}


void imprimirCoordenada(void *dato)
{
	Coordenada *a = dato;
	printf("[%-2d,%-2d]", a->x,a->y);
}

int compararCoordenada(void *a,void *b)
{
	Coordenada *aa=a,*bb=b;
	if(aa->x==bb->x && aa->y==bb->y)
		return 1;
	return 0;
}


unsigned char alternativas(unsigned char **lab,Coordenada *coordenada)
{		
	unsigned char caracter;
	unsigned char banderas;
	banderas = 0;	
	//EVALUAR ALTERNATIVA Y REGISTRAR POSIBILIDAD DE MOVIMIENTO
	//EN UN REGISTRO DE BITS/BANDERAS	
	caracter = lab[coordenada->x-1][coordenada->y];
	if(caracter!='x' && caracter!='.' && caracter!='o' && caracter!='A')
		banderas |= ARRIBA;
	
	caracter = lab[coordenada->x+1][coordenada->y];
	if(caracter!='x' && caracter!='.' && caracter!='o' && caracter!='A')
		banderas |= ABAJO;
	
	caracter = lab[coordenada->x][coordenada->y+1];
	if(caracter!='x' && caracter!='.' && caracter!='o' && caracter!='A')
		banderas |= DERECHA;
	
	caracter = lab[coordenada->x][coordenada->y-1];
	if(caracter!='x' && caracter!='.' && caracter!='o' && caracter!='A')
		banderas |= IZQUIERDA;
	return banderas;	
}


void imprimirPosibles(unsigned char caminos)
{
	if(caminos&ARRIBA)
		printf("\n ARRIBA");
	if(caminos&ABAJO)
		printf("\n ABAJO");
	if(caminos&IZQUIERDA)
		printf("\n IZQUIERDA");
	if(caminos&DERECHA)
		printf("\n DERECHA");
}