cflags = -Wall -Werror -pedantic -std=c99 -O2
dflags = -Wall -Werror -pedantic -std=c99 -g

# NOTA: He corregido la ruta de include a -I../captura basándome en tu estructura
includes = -I. -I../captura -I../src/Estructuras

# Librerías existentes (Hash y Cola)
libs_path = -L../../lib
libs = -lHashTable -lCola

# ARCHIVOS FUENTE
# He agregado ../captura/captura.c al final de esta lista
source = main.c Libro.c ../captura/captura.c ../Estructuras/arbol/arbol.c ../Estructuras/nodos/nodoarbol.c

output = ../../bin/practica10

build:
	gcc $(cflags) $(source) -o $(output) $(includes) $(libs_path) $(libs)
	@echo "Compilo Exitosamente!!!"

rebuild:
	make -C ../captura
	make -C ../Estructuras/hash
	make -C ../Estructuras/cola 
	gcc $(cflags) $(source) -o $(output) $(includes) $(libs_path) $(libs)
	@echo "Compilo Exitosamente!!!"

run:
	../../bin/practica10.exe