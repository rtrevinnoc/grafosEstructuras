#include "queue.h"
#include "stack.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct sEdge {
	char name[50];
	long id;
	int length;
	struct sEdge *next;
} tEdge;

typedef struct sVertex {
	char name[50];
	long id;
	tEdge *firstEdge;
	struct sVertex *next;
} tVertex;

// Reserva un espacio en memoria
void *chkMalloc(size_t t) {
	void *mem = malloc(t); // Intenta reservar un espacio de memoria del tamaño del parametro t
	if (mem == NULL) { // Si el bloque no pudo reservarse, terminar el programa y notificar
		printf("Sin memoria\n");
		exit(1);
	}
	return mem; // regresar el puntero al espacio de memoria reservado
}

// Agrega un vertice al grafo
void addVertex(tVertex **first, char *name, int *numVertices) {
	tVertex *newest = chkMalloc(sizeof(*newest)); // Reserva un espacio en memoria para un struct sVertex
	strcpy(newest->name, name); // Le asigna el identificador del parametro al campo name
	newest->next = *first; // El campo next apunta al primer vertice actual del grafo en la lista de adyacencia
	newest->id = (*numVertices)++; // Segun el numero actual de vertices en el grafo, el campo id es uno mas ese numero
	*first = newest; // la lista de adyacencia comienza con el nuevo vertice
}

// Agrega una arista al grafo
void addVertexEdge(tVertex *first, char *name_a, char *name_b, int length_b) {
	tVertex *selected, *pointed; // Vertices de ayuda
	tEdge *newest = chkMalloc(sizeof(*newest)); // Se reserva un espacio en la memoria para un struct de tipo sEdge
	while (first != NULL) { // Si el grafo tiene vertices
		if (strcmp(first->name, name_a) == 0) { // Buscar el vertice que tiene por identificador el parametro name_a
			selected = first; // Asignar ese vertice a la variable selected
		} else if (strcmp(first->name, name_b) == 0) { // Buscar el vertice que tiene por identificador el parametro name_a
			pointed = first; // Asignar ese vertice a la variable pointed
		}
		first = first->next; // Avanzar en la lista
	}

	tEdge *currEdge = selected->firstEdge; // Se toma la primer arista del vertice con el identificador del parametro name_a
	int i = 0;
	while (currEdge != NULL) {
		if (strcmp(currEdge->name, name_b) == 0) // Se recorre la lista de adyacencia del vertice selected para ver si ya tiene el vertice pointed agregado
			i++; // Si esta, aumentar i
		currEdge = currEdge->next; // Avanzar en la lista
	}

	if (selected->firstEdge == NULL || i == 0) { // Si no tiene vertices adyacentes, o el vertice pointed no esta en su lista
		strcpy(newest->name, name_b); // A la nueva arista se le asigna el identificador del vertice al que apunta
		newest->next = selected->firstEdge; // Se coloca por el frente, apunta al actual comienzo de la lista de adyacencia
		newest->length = length_b; // Se asigna la ponderacion de la arista
		newest->id = pointed->id; // Se le asigna el identificador entero del vertice al que apunta
		selected->firstEdge = newest; // El comienzo de la lista es ahora el nuevo vertice
	}
}

// Funcion de ayuda para crear aristas en grafos dirigidos o no dirigidos, segun el parametro dir
void addEdge(tVertex *first, char *name_a, char *name_b, int length_b,
		int dir) {
	if (dir == 0) { // Si dir es 0, la aristaen cuestion es no dirigida, y se crea doble
		addVertexEdge(first, name_a, name_b, length_b);
		addVertexEdge(first, name_b, name_a, length_b);
	} else if (dir > 0) { // Si dir es mayor a 0, la arista apunta del vertice name_a a name_b
		addVertexEdge(first, name_a, name_b, length_b);
	} else if (dir < 0) { // Si dir es menor a 0, la arista apunta del vertice name_b a name_a
		addVertexEdge(first, name_b, name_a, length_b);
	}
}

// Elimina una arista del grafo, dada una lista de adyacencia anidada
void delVertexEdge(tEdge **first, char *name) {
	tEdge *cur = *first, *prev = NULL; // Vertices de ayuda
	while (cur != NULL && strcmp(cur->name, name) != 0) { // Se recorre la lista buscando un vertice que tenga el nombre del parametro name
		prev = cur; // prev se asigna al vertice actual
		cur = cur->next; // cur se asigna al vertice siguiente
	}

	if (cur) { // Si cur existe
		if (prev) { // Si prev existe
			prev->next = cur->next; // prev apunta al siguiente de cur
		} else {
			*first = cur->next; // Si no, es el comienzo de la lista y esta apunta directamente al siguiente de cur
		}
	}
}

// Eliminar Vertice
void delVertex(tVertex **first, char *name) {
	tVertex *cur = *first, *prev = NULL; // Vertices de ayuda
	while (cur != NULL && strcmp(cur->name, name) != 0) { // Se recorre la lista buscando un vertice que tenga el nombre del parametro name
		prev = cur; // prev se asigna al vertice actual
		cur = cur->next; // cur se asigna al vertice siguiente
	}
	if (prev) { // Si prev existe
		prev->next = cur->next; // prev apunta al siguiente de cur
	} else {
		*first = cur->next; // Si no, es el comienzo de la lista y esta apunta directamente al siguiente de cur
	}

	cur = *first; // cur apunta de nuevo al comienzo de la lista
	while (cur != NULL) { // Si la la lista de adyacencia no esta vacia
		delVertexEdge(&(cur->firstEdge), name); // Eliminar aristas, si existen, que conecten con el vertice con el identificador name en todos los vertices de la lista de adyacencia
		cur = cur->next; // Recorrer la lista
	}
}

// Funcion de ayuda para eliminar arista, dado todo el grafo
void delEdge(tVertex **first, char *name_a, char *name_b) {
	tVertex *cur = *first; // Cur es el comienzo de la lista
	while (cur != NULL) { // Recorrer la lista
		if (strcmp(cur->name, name_a) == 0) { // Si cur tiene el identificador de name_a
			delVertexEdge(&(cur->firstEdge), name_b); // Eliminar la conexion con el vertice con name_b en esa lista de aristas
		}
		if (strcmp(cur->name, name_b) == 0) { // Si cur tiene el identificadro de name_b
			delVertexEdge(&(cur->firstEdge), name_a); // Eliminar la conexion con el vertice con name_a en esa lista de aristas
		}
		cur = cur->next; // Avanzar en la lista
	}
}

// Imprimir vertices y sus aristas
void dumpDetails(tVertex *currVertex) {
	while (currVertex != NULL) { // Mientras la lista tenga elementos
		printf("%s(%lu) tiene:\n", currVertex->name, currVertex->id); // Imprimir el vertice

		tEdge *currEdge = currVertex->firstEdge; // Comenzar al inicio de la lista de aristas
		if (currEdge != NULL) { // Si hay aristas
			while (currEdge != NULL) { // Mientras haya aristas
				printf("\t[%s(%lu), %d]\n", currEdge->name, currEdge->id,
						currEdge->length); // Imprimir las aristas
				currEdge = currEdge->next; // Avanzar en la lista de aristas
			}
		}
		currVertex = currVertex->next; // Avanzar en la lista de vertices
	}
}

// Encontrar arbol recubridor minimo con el algoritmo de Prim
void primAlg(tVertex *currVertex, tVertex **minGraph, int *numVertices) {
	tVertex *firstVertex = currVertex; // Vertices de ayuda
	int numEdge = 0, selected[*numVertices], min, minGraphVertices = 0; // indice de aristas del grafo minimo, lista de vertices visitados, minima ponderacion actual, numero de vertices del arbol recubridor minimo

	memset(selected, 0, sizeof(selected)); // marcar todos los vertices como no visitados

	selected[*numVertices - 1] = 1; // Marcar el ultimo vertice como visitado, de ahi comenzaremos

	while (currVertex != NULL) { // Recorrer todos los vertices de la lista de adyacencia
		addVertex(*&minGraph, currVertex->name, &minGraphVertices); // Agregar todos los vertices al grafo minimo minGraph
		currVertex = currVertex->next;
	}

	while (numEdge < *numVertices - 1) { // Repetir el proceso por cada arista que debe tener el arbol recubridor minimo
		min = INT_MAX; // minimo es el mayor entero posible
		currVertex = firstVertex; // Comenzar en el primer vertice de la lista de adyacencia

		while (currVertex != NULL) { // Recorrer la lista de vertices
			if (selected[currVertex->id]) { // Si el vertice esta seleccionado
				tEdge *currEdge = currVertex->firstEdge; // Comenzar en su primer arista
				while (currEdge != NULL) { // Recorrer la lista de aristas
					if (!selected[currEdge->id]) { // Si el vertice con el que conecta la arista no ha sido seleccionado
						if (min > currEdge->length) { // Si la ponderacion de la arista es menor que el minimo actual
							min = currEdge->length; // Actualizar el minimo de esta pasada
							selected[currEdge->id] = 1; // Marcar el vertice como visitado
							addEdge(*minGraph, currVertex->name, currEdge->name,
									currEdge->length, 0); // Añadir la arista al arbol recubridor minimo
						}
					}
					currEdge = currEdge->next; // Avanzar en la lista de aristas
				}
			}
			currVertex = currVertex->next; // Avanzar en la lista de vertices
		}
		numEdge++; // Aumentar el indice de aristas del arbol recubridor minimo
	}
	*&minGraph = minGraph; // Actualizar el grafo del argumento minGraph por el arbol recubridor minimo recien creado
}

// Algoritmo de Kruskal
void kruskalAlg(tVertex *currVertex, tVertex **minGraph, int *numVertices) {
	tVertex *firstVertex = currVertex; // Vertices de ayuda
	int numEdge = 0, selected[*numVertices], min, minGraphVertices = 0,
	    iMin_vertex, iMin_pointed; // indice de aristas del grafo minimo, vertices seleccionados, minimo actual, vertices del arbol recubridor minimo, indice del vertice minimo y del apuntado
	char cMin_vertex[50], cMin_pointed[50]; // Nombre del vertice minimo, y del apuntado por la arista

	memset(selected, 0, sizeof(selected)); // Ningun vertice ha sido visitado

	while (currVertex != NULL) { // Recorrer toda la lista de vertices
		addVertex(*&minGraph, currVertex->name, &minGraphVertices); // Agregar todos los vertices del grafo al arbol recubridor minimo
		currVertex = currVertex->next;
	}

	while (numEdge < *numVertices - 1) { // Repetir por cada arista que debe tener el arbol minimo
		min = INT_MAX; // ponderacion minima actual empieza por el maximo entero posible
		currVertex = firstVertex; // empieza al comienzo de la lista de vertices

		while (currVertex != NULL) { // mientras haya vertices
			tEdge *currEdge = currVertex->firstEdge; // empezar por la primer arista
			while (currEdge != NULL) { // recorrer lista de aristas
				if ((selected[currVertex->id] || numEdge == 0) &&
						!selected[currEdge->id]) { // Si no esta seleccionado el vertice o es la primer pasada y que el vertice apuntado no esta seleccionado
					if (min > currEdge->length) { // Si la ponderacion de esta arista es menor que el minimo actual
						min = currEdge->length; // Actualizar el minimo
						iMin_vertex = currVertex->id; // Guardar identificacion del vertice minimo
						iMin_pointed = currEdge->id; // Guardar identificacion del vertice minimo apuntado
						strcpy(cMin_vertex, currVertex->name); // Guardar nombre del vertice minimo
						strcpy(cMin_pointed, currEdge->name); // Guardar nombre del vertice minimo apuntado
					}
				}
				currEdge = currEdge->next; // Recorrer lista de aristas
			}
			currVertex = currVertex->next; // Recorrer lista de vertices
		}
		selected[iMin_vertex] = 1; // Marcar como seleccionado el minimo vertice
		selected[iMin_pointed] = 1; // Marcar como seleccionado el minimo vertice apuntado
		addEdge(*minGraph, cMin_vertex, cMin_pointed, min, 0); // Añadir arista minima al arbol recubridor minimo
		numEdge++; // Avanzar en el numero de aristas que debe tener el arbol recubridor minimo
	}
	*&minGraph = minGraph; // Actualizar el grafo del argumento minGraph por el arbol recubridor minimo recien creado

}

// Recorrido por amplitud
void bfsAlg(tVertex *currVertex, char *pivot, tVertex **minGraph,
		int *numVertices) {
	tVertex *firstVertex = currVertex; // Vertices de ayuda
	int numEdge = 0, selected[*numVertices], minGraphVertices = 0; // indice de aristas, lista de vertices visitados, numero de vertices 
	char order[*numVertices][50]; // lista con el orden en que se visitan los vertices
	struct Queue *queue = createQueue(*numVertices); // crear una cola para los vertices por visitar

	memset(selected, 0, sizeof(selected)); // marcar todos los vertices como no visitados

	while (currVertex != NULL) { // Recorrer toda la lista de vertices
		addVertex(*&minGraph, currVertex->name, &minGraphVertices); // Agregar los vertices al grafo minimo
		currVertex = currVertex->next;
	}

	while (numEdge < *numVertices) { // Recorrer por todas las aristas que tiene el grafo minimo
		currVertex = firstVertex; // Comenzar por el inicio de la lista de vertices
		while (currVertex != NULL) { // Recorrer lista de vertices
			tEdge *currEdge = currVertex->firstEdge; // Comenzar por el comienzo de la lista de aristas
			while (currEdge != NULL) { // Recorrer la lista de aristas
				if (numEdge == 0 && strcmp(currVertex->name, pivot) == 0) { // Si es la primer pasad y el vertice es igual al vertice por el que queremos comenzar
					selected[currVertex->id] = 1; // Marcar el vertice como visitado
					selected[currEdge->id] = 1; // Marcar el vertice al que conecta la arista como visitado
					enqueue(queue, currEdge->id); // Agregar el vertice al que conecta a la cola
					strcpy(order[numEdge], currVertex->name); // Agregar el vertice al orden en que fue visitado
					addEdge(*minGraph, currVertex->name, currEdge->name, currEdge->length,
							0); // agregar la arista al vertice minimo
				} else if (numEdge > 0 && front(queue) == currVertex->id) { // Si es cualquier pasada consecuente y el vertice es igual al primero de la cola
					if (!selected[currEdge->id]) { // Si el vertice al que conecta no esta visitado
						enqueue(queue, currEdge->id); // Agrgar el vertice al que conecta a la cola
						addEdge(*minGraph, currVertex->name, currEdge->name,
								currEdge->length, 0); // Agregar la arista al grafo minimo
					}
					strcpy(order[numEdge], currVertex->name); // Agregar el vertice a la lista con el orden
				}
				currEdge = currEdge->next; // Continuar en la lista de aristas
			}
			currVertex = currVertex->next; // Conitnuar en la lista de vertices
		}
		if (numEdge > 0) // Si es cualquier pasada consecuente
			dequeue(queue); // Eliminar el primero de la cola
		numEdge++; // Avanzar a la siguiente pasada
	}

	printf("\n#(order)=> ");
	for (int i = 0; i < numEdge; i++) { // Por cada elemento de la lista con el orden de vertices
		printf("->%s", order[i]); // Imprimir el vertice
	}
	printf("\n");
	*&minGraph = minGraph; // Actualizar el grafo del argumento minGraph por el arbol recubridor minimo recien creado
}

// Recorrido por profundidad
void dfsAlg(tVertex *currVertex, char *pivot, tVertex **minGraph,
		int *numVertices) {
	tVertex *firstVertex = currVertex; // Vertices de ayuda
	int numEdge = 0, selected[*numVertices], minGraphVertices = 0; // indice de vertices, lista de vertices visitados, numero de vertices del grafo minimo
	char order[*numVertices][50]; // lista con el orden de visita
	struct Stack *stack = createStack(*numVertices); // stack para visitar vertices

	memset(selected, 0, sizeof(selected)); // marcar todos los vertices como por visitar

	while (currVertex != NULL) { // Recorrer la lista de vertices
		addVertex(*&minGraph, currVertex->name, &minGraphVertices); // agregar los vertices al grafo minimo
		currVertex = currVertex->next;
	}

	while (numEdge < *numVertices) { // Por cada vertice en el grafo
		currVertex = firstVertex; // Empezar en el primer vertice

		while (currVertex != NULL) { // Recorrer la lista de vertices
			tEdge *currEdge = currVertex->firstEdge; // Comenzar por la primer arista
			if (numEdge == 0 && strcmp(currVertex->name, pivot) == 0) { // Si es la primer pasada y el vertice actual es igual a nuestro parametro de punto de partida, pivot
				while (currEdge != NULL) { // Recorrer aristas
					addEdge(*minGraph, currVertex->name, currEdge->name, currEdge->length, 0); // Agregar arista al grafo minimo
					push(stack, currEdge->id); // Agregar vertice al que conecta a la pila
					strcpy(order[numEdge], currVertex->name); // copiar el nombre del vertice actual al orden de visita
					selected[currEdge->id] = 1; // marcar el vertice al que conecta como visitado
					currEdge = currEdge->next; // avanzar en la lista de vertices
				}
				selected[currVertex->id] = 1; // marcar el vertice actual como vistado
			} else if (numEdge > 0 && top(stack) == currVertex->id) { // En las pasadas consecuentes, si el ultimo de la pila es igual al vertice actual
				pop(stack); // Eliminar ultimo de la pila
				while (currEdge != NULL) { // Recorrer la lista de aristas
					if (!selected[currEdge->id]) { // Si el vertice al que conecta no esta visitado
						push(stack, currEdge->id); // Agregar a la pila
						addEdge(*minGraph, currVertex->name, currEdge->name, currEdge->length, 0); // Agregar arista al grafo minimo
					}
					currEdge = currEdge->next; // Avanzar en lista de aristas
				}
				strcpy(order[numEdge], currVertex->name); // Agregar vertice al orden devisita
				selected[currVertex->id] = 1; // Marcar vertice como visitado
				numEdge++; // Siguiente pasada
			}
			currVertex = currVertex->next; // Avanzar en vertices
		}
		if (numEdge == 0) numEdge++; // Si es la primer pasada, avanzar pasada
	}

	printf("\n#(order)=> ");
	for (int i = 0; i < numEdge; i++) { // Por cada elemento en la lista del orden
		printf("->%s", order[i]); // Imprimir nombre del vertice en la lista
	}
	printf("\n");

	*&minGraph = minGraph; // Actualizar el grafo del argumento minGraph por el arbol recubridor minimo recien creado
}

// Imprimir representacion ASCII del grafo
void printGraph(tVertex *currVertex, int *numVertices) {
	int selected[*numVertices]; // Lista de vertices visitados
	char vertices[*numVertices][50], espacios[*numVertices]; // lista de nombres de vertices y de espaciado entre ellos
	int x,y;

	memset(selected, 0, sizeof(selected)); // marcar todos los vertices como no visitados
	for (int i = 0; i < *numVertices; i++) {
		espacios[i] = ' '; // agregar un espacio por cada vertice en el grafo
	}

	while (currVertex != NULL) { // Recorrer lista de vertices
		tEdge *currEdge = currVertex->firstEdge; // Comenzar en la primer arista
		if (currEdge != NULL) { // Si hay aristas
			while (currEdge != NULL) { // Recorrer lista de aristas
				if (!selected[currEdge->id]) { // Si no se ha visitado el vertice al que apunta
					if (currVertex->id > currEdge->id) { // Si el indice del vertice es menor al del vertice al que conecta
						x = currEdge->id; // El primero en imprimirse sera la el vertice al que conecta
						y = currVertex->id; // El segundo en imprimirse es el vertice actual
					} else {
						x = currVertex->id; // De lo contrario, el primero es el vertice actual
						y = currEdge->id; // Evidentemente, el otro sera el segundo
					}

					for (int i = 0; i < x; i++) { // Mientras sea i menor al primer indice del primer vertice
						printf("%c ", espacios[i]); // Imprimir un espacio de la lista
					}
					printf("+"); // Imprimir una conexion de arista
					for (int i = 0; i < y-x-1; i++) { // Por cada numero en la diferencia de posicion del primer y segundo elemento en imprimir
						printf("--"); // Imprimir una linea que conecte las aristas
					}
					printf("-+ "); // Terminar la conexion
					for (int i = y+1; i < *numVertices; i++) { // Desde el ultimo elemento en imprimir hasta el numero de vertices
						printf("%c ", espacios[i]); // Imprimir un espacio
					}
					printf("\n");

					// Poner una linea que conecte con el elemento en su posicion
					espacios[x] = '|';
					espacios[y] = '|';

					for (int i = 0; i < *numVertices; i++) { // Por cada vertice en el grafo
						printf("%c ", espacios[i]); // Imprimir espacio
					}

					printf("\n");

				}
				currEdge = currEdge->next; // Pasar a la siguiente arista
			}
		}
		strcpy(vertices[currVertex->id], currVertex->name); // Copiar nombre del vertice actual a la lista
		selected[currVertex->id] = 1; // Marcar vertice como visitado


		currVertex = currVertex->next; // Pasar al siguiente vertice
	}
	printf("\n");

	for (int i = 0; i < *numVertices; i++) { // Por cada vertice
		if (selected[i]) { // Si fue visitado
			printf("%d ", i); // Imprimir su indice debajo de su arista
		} else {
			printf("  "); // De lo contrario, omitirlo, pues ha sido eliminado
		}
	}
	printf("\n\n");

	for (int i = 0; i < *numVertices; i++) { // por cada vertice
		if (selected[i]) { // Si fue visitado
			printf("(%d) %s\n", i, vertices[i]); // Imprimir la leyenda que relaciona su indice con su nombre legible a humanos (el campo name)
		}
	}
	printf("\n");
}

void imprimirCamino(int parent[], int j, char nombres[][50]) {
	if (parent[j] == -1) { // Caso base, si el vertice con indice j es el comienzo
		printf("->(%s)", nombres[j]); // Imprimir el nombre del vertice
		return; // terminar la funcion
	}

	imprimirCamino(parent, parent[j], nombres); // Llamada recursiva con el vertice que antecede al acutal

	printf("->(%s)", nombres[j]); // Imprimir el nombre del vertice
}

// Algoritmo de Dijkstra
void dijkstraAlg(tVertex *currVertex, char *pivot, int *numVertices) {
	tVertex *firstVertex = currVertex; // Vertice de ayuda
	int numEdge = 0, distances[*numVertices], selected[*numVertices], caminos[*numVertices]; // indice de vertices, lista de distancias, listade vertices visitados, lista de vertices antecesores a cada vertice
	char nombres[*numVertices][50]; // nombre de cada vertice

	memset(selected, 0, sizeof(selected)); // Marcar todos los vertice como no visitados
	for (int i = 0; i < *numVertices; i++) { // Por cada vertice
		distances[i] = INT_MAX; // Marcar su distancia como la distancia maxima
		caminos[i] = -1; // Marcar todos los vertices como que son sus propios antecesores (cambia adelante)
	}

	while (currVertex != NULL) { // Recorrer lista de vertices
		if (strcmp(currVertex->name, pivot) == 0) { // Si el vertice actual es igual a nuestra fuente del parametro pivot
			distances[currVertex->id] = 0; // Marcar la distancia a si mismo como 0
			selected[currVertex->id] = 1; // Marcarlo como visitado
		}
		strcpy(nombres[currVertex->id], currVertex->name); // Copiar su nombre a la lista de nombres
		currVertex = currVertex->next; // Avanzar
	}

	while (numEdge < *numVertices) { // Por cada vertice en el grafo
		currVertex = firstVertex; // Comenzar en el primer vertice
		while (currVertex != NULL) { // Recorrer lista de vertices
			if (selected[currVertex->id]) { // Si el vertice ha sido seleccionado
				tEdge *currEdge = currVertex->firstEdge; // Comenzar por la primer arista
				while (currEdge != NULL) { // Recorrer lista de aristas
					if (distances[currEdge->id] > currEdge->length && !selected[currEdge->id]) { // Si la distancia guardada al vertice que apunta es menor a la de la arista acutal y el vertice apuntado no ha sido visitado
						distances[currEdge->id] = distances[currVertex->id] + currEdge->length; // Actualizar la distancia guardada por la suma de la distancia del vertice actual mas la ponderacion de la arista actual
						caminos[currEdge->id] = currVertex->id; // Agregar el vertice actual como el antecesor del vertice al que apunta
					}
					selected[currEdge->id] = 1; // Marcar el vertice apuntado como visitado
					currEdge = currEdge->next; // Avanzar en la lista de aristas
				}
			}
			currVertex = currVertex->next; // Avanzar en la lista de vertices
		}
		numEdge++; // Siguiente pasada
	}

	printf("\nCaminos más cortos\n");
	for (int i = 1; i < *numVertices; i++) { // Por cada vertice
		printf("[");
		imprimirCamino(caminos, i, nombres); // Imprimir camino más corto
		printf("]: -> %d\n", distances[i]); // Imprimir distancia del camino
	}
	printf("\n");
}

// Leer un numero de entrada estandar y guardarlo en la variable del puntero
// input, con un tipo segun el primer argumento
void getNumero(int entero, void *input) {
	int length, i = 0;
	char aux[' '];

	do {
		// Obtener valor del usuario con fgets para prevenir desbordamiento
		fgets(aux, ' ', stdin);
		// Eliminar el salto de linea
		aux[strcspn(aux, "\n")] = 0;
		length = strlen(aux);
		if (aux[0] == 'N')
			exit(0);
		if (length != 0) {
			// Recorrer cada caracter de la cadena
			for (i = 0; i < length; ++i) {
				// Si algun caracter no es numero, un punto o el primero un signo de
				// negativo
				if ((!isdigit(aux[i]) && aux[i] != '.') || (i == 0 && aux[i] == '-')) {
					// Romper ciclo para reiniciar y recibir otro valor
					printf("Introduzca correctamente el dato -> ");
					break;
				}
			}
		} else {
			// Forzar la condicion de terminacion del ciclo
			i = length;
		}
	} while (i != length); // Cuando se haya recorrido toda la cadena, salir

	// Si el primer parametro es 0, retornar como flotante
	if (entero == 0) {
		*(float *)input = atof(aux);
		// Si el primer parametro es 2, retornar como long
	} else if (entero == 2) {
		*(long *)input = atol(aux);
		// Si el primer parametro es diferente a lo anterior, retornar como entero
	} else {
		*(int *)input = atoi(aux);
	}
}

void instrucciones() {
	printf("\n\t\t ### GRAFOS ### \n");
	printf("\n\t\t\t ¿Qué opcion desea? :\n"
			"   \t\t 1 insertar vertice\n"
			"   \t\t 2 conectar vertices\n"
			"   \t\t 3 eliminar vertice\n"
			"   \t\t 4 eliminar conexion\n"
			"   \t\t 5 imprimir grafo\n"
			"   \t\t 6 ejecutar algoritmo de Prim\n"
			"   \t\t 7 ejecutar algoritmo de Kruskal\n"
			"   \t\t 8 Busqueda por profundidad\n"
			"   \t\t 9 Busqueda por amplitud\n"
			"   \t\t 10 Encontrar caminos mas cortos\n"
			"   \t\t 0 salir\n");
}

int main(void) {
	tVertex *firstVertex = NULL;
	int firstGraphVertices = 0;

	int op, peso;
	char nombre[50], nombre_b[50];

	do {

		tVertex *minGraph = NULL;

		instrucciones();
		printf("\nOpción -> ");
		getNumero(1, &op);
		switch (op) {
			case 1:
				printf("\n*** Inserta un vertice ***\n");

				printf("Nombre -> ");
				fgets(nombre, 50, stdin);
				nombre[strcspn(nombre, "\n")] = 0;

				addVertex(&firstVertex, nombre, &firstGraphVertices);
				break;
			case 2:
				printf("\n*** Conecta dos vertices ***\n");

				printf("Nombre del primer nodo -> ");
				fgets(nombre, 50, stdin);
				nombre[strcspn(nombre, "\n")] = 0;

				printf("Nombre del segundo nodo -> ");
				fgets(nombre_b, 50, stdin);
				nombre_b[strcspn(nombre_b, "\n")] = 0;

				printf("Inserte peso del enlace -> ");
				getNumero(1, &peso);

				addEdge(firstVertex, nombre, nombre_b, peso, 0);
				break;
			case 3:
				printf("\n*** Elimina un vertice ***\n");

				printf("Nombre del primer nodo -> ");
				fgets(nombre, 50, stdin);
				nombre[strcspn(nombre, "\n")] = 0;

				delVertex(&firstVertex, nombre);

				break;
			case 4:
				printf("\n*** Elimina conexion ***\n");

				printf("Nombre del primer nodo -> ");
				fgets(nombre, 50, stdin);
				nombre[strcspn(nombre, "\n")] = 0;

				printf("Nombre del segundo nodo -> ");
				fgets(nombre_b, 50, stdin);
				nombre_b[strcspn(nombre_b, "\n")] = 0;

				delEdge(&firstVertex, nombre, nombre_b);

				break;
			case 5:
				printf("\n*** Imprime grafo ***\n");

				dumpDetails(firstVertex);
				printGraph(firstVertex, &firstGraphVertices);

				break;
			case 6:
				printf("\n*** Ejecutar algoritmo de Prim ***\n");
				primAlg (firstVertex, &minGraph, &firstGraphVertices);
				dumpDetails(minGraph);
				printGraph(minGraph, &firstGraphVertices);

				break;
			case 7:
				printf("\n*** Ejecutar algoritmo de Kruskal ***\n");
				kruskalAlg (firstVertex, &minGraph, &firstGraphVertices);
				dumpDetails(minGraph);
				printGraph(minGraph, &firstGraphVertices);

				break;
			case 8:
				printf("\n*** Busqueda por profundidad ***\n");

				printf("Nombre del pivote -> ");
				fgets(nombre, 50, stdin);
				nombre[strcspn(nombre, "\n")] = 0;

				bfsAlg(firstVertex, nombre, &minGraph, &firstGraphVertices);
				dumpDetails(minGraph);

				break;
			case 9:
				printf("\n*** Busqueda por amplitud ***\n");

				printf("Nombre del pivote -> ");
				fgets(nombre, 50, stdin);
				nombre[strcspn(nombre, "\n")] = 0;

				bfsAlg(firstVertex, nombre, &minGraph, &firstGraphVertices);
				dumpDetails(minGraph);

				break;
			case 10:
				printf("\n*** Encontrar caminos mas cortos ***\n");

				printf("Nombre del pivote -> ");
				fgets(nombre, 50, stdin);
				nombre[strcspn(nombre, "\n")] = 0;

				dijkstraAlg(firstVertex, nombre, &firstGraphVertices);

				break;
		}
	} while (op != 0);

	return 0;
}
