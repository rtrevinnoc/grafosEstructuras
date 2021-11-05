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

void *chkMalloc(size_t sz) {
	void *mem = malloc(sz);
	if (mem == NULL) {
		printf("Sin memoria\n");
		exit(1);
	}
	return mem;
}

void addVertex(tVertex **first, char *name, int *numVertices) {
	tVertex *newest = chkMalloc(sizeof(*newest));
	strcpy(newest->name, name);
	newest->next = *first;
	newest->id = (*numVertices)++;
	*first = newest;
}

void addVertexEdge(tVertex *first, char *name_a, char *name_b, int length_b) {
	tVertex *selected, *pointed;
	tEdge *newest = chkMalloc(sizeof(*newest));
	while (first != NULL) {
		if (strcmp(first->name, name_a) == 0) {
			selected = first;
		} else if (strcmp(first->name, name_b) == 0) {
			pointed = first;
		}
		first = first->next;
	}

	tEdge *currEdge = selected->firstEdge;
	int i = 0;
	while (currEdge != NULL) {
		if (strcmp(currEdge->name, name_b) == 0)
			i++;
		currEdge = currEdge->next;
	}

	if (selected->firstEdge == NULL || i == 0) {
		strcpy(newest->name, name_b);
		newest->next = selected->firstEdge;
		newest->length = length_b;
		newest->id = pointed->id;
		selected->firstEdge = newest;
	}
}

void addEdge(tVertex *first, char *name_a, char *name_b, int length_b,
		int dir) {
	if (dir == 0) {
		addVertexEdge(first, name_a, name_b, length_b);
		addVertexEdge(first, name_b, name_a, length_b);
	} else if (dir > 0) {
		addVertexEdge(first, name_a, name_b, length_b);
	} else if (dir < 0) {
		addVertexEdge(first, name_b, name_a, length_b);
	}
}

void delVertexEdge(tEdge **first, char *name) {
	tEdge *cur = *first, *prev = NULL;
	while (cur != NULL && strcmp(cur->name, name) != 0) {
		prev = cur;
		cur = cur->next;
	}

	if (cur) {
		if (prev) {
			prev->next = cur->next;
		} else {
			*first = cur->next;
		}
	}
}

void delVertex(tVertex **first, char *name) {
	tVertex *cur = *first, *prev = NULL;
	while (cur != NULL && strcmp(cur->name, name) != 0) {
		prev = cur;
		cur = cur->next;
	}
	if (prev) {
		prev->next = cur->next;
	} else {
		*first = cur->next;
	}

	cur = *first;
	while (cur != NULL) {
		delVertexEdge(&(cur->firstEdge), name);
		cur = cur->next;
	}
}

void delEdge(tVertex **first, char *name_a, char *name_b) {
	tVertex *cur = *first;
	while (cur != NULL) {
		if (strcmp(cur->name, name_a) == 0) {
			delVertexEdge(&(cur->firstEdge), name_b);
		}
		if (strcmp(cur->name, name_b) == 0) {
			delVertexEdge(&(cur->firstEdge), name_a);
		}
		cur = cur->next;
	}
}

void dumpDetails(tVertex *currVertex) {
	while (currVertex != NULL) {
		printf("%s(%lu) has:\n", currVertex->name, currVertex->id);

		tEdge *currEdge = currVertex->firstEdge;
		if (currEdge != NULL) {
			while (currEdge != NULL) {
				printf("   [%s(%lu), %d]\n", currEdge->name, currEdge->id,
						currEdge->length);
				currEdge = currEdge->next;
			}
		}
		currVertex = currVertex->next;
	}
}

void primAlg(tVertex *currVertex, tVertex **minGraph, int *numVertices) {
	tVertex *firstVertex = currVertex;
	int numEdge = 0, selected[*numVertices], min, minPointedId,
	    minGraphVertices = 0;
	char minVertexName[50], minPointedName[50];

	memset(selected, 0, sizeof(selected));

	selected[*numVertices - 1] = 1;

	while (currVertex != NULL) {
		addVertex(*&minGraph, currVertex->name, &minGraphVertices);
		currVertex = currVertex->next;
	}

	while (numEdge < *numVertices - 1) {
		min = INT_MAX;
		currVertex = firstVertex;

		while (currVertex != NULL) {
			if (selected[currVertex->id]) {
				tEdge *currEdge = currVertex->firstEdge;
				while (currEdge != NULL) {
					if (!selected[currEdge->id]) {
						if (min > currEdge->length) {
							min = currEdge->length;
							selected[currEdge->id] = 1;
							addEdge(*minGraph, currVertex->name, currEdge->name,
									currEdge->length, 0);
						}
					}
					currEdge = currEdge->next;
				}
			}
			currVertex = currVertex->next;
		}
		numEdge++;
	}
	*&minGraph = minGraph;
}

void kruskalAlg(tVertex *currVertex, tVertex **minGraph, int *numVertices) {
	tVertex *firstVertex = currVertex;
	int numEdge = 0, selected[*numVertices], min, minGraphVertices = 0,
	    iMin_vertex, iMin_pointed;
	char cMin_vertex[50], cMin_pointed[50];

	memset(selected, 0, sizeof(selected));

	while (currVertex != NULL) {
		addVertex(*&minGraph, currVertex->name, &minGraphVertices);
		currVertex = currVertex->next;
	}

	while (numEdge < *numVertices - 1) {
		min = INT_MAX;
		currVertex = firstVertex;

		while (currVertex != NULL) {
			tEdge *currEdge = currVertex->firstEdge;
			while (currEdge != NULL) {
				if ((selected[currVertex->id] || numEdge == 0) &&
						!selected[currEdge->id]) {
					if (min > currEdge->length) {
						min = currEdge->length;
						iMin_vertex = currVertex->id;
						iMin_pointed = currEdge->id;
						strcpy(cMin_vertex, currVertex->name);
						strcpy(cMin_pointed, currEdge->name);
					}
				}
				currEdge = currEdge->next;
			}
			currVertex = currVertex->next;
		}
		selected[iMin_vertex] = 1;
		selected[iMin_pointed] = 1;
		addEdge(*minGraph, cMin_vertex, cMin_pointed, min, 0);
		numEdge++;
	}
	*&minGraph = minGraph;
}

void bfsAlg(tVertex *currVertex, char *pivot, tVertex **minGraph,
		int *numVertices) {
	tVertex *firstVertex = currVertex;
	int numEdge = 0, selected[*numVertices], minGraphVertices = 0;
	char order[*numVertices][50];
	struct Queue *queue = createQueue(*numVertices);

	memset(selected, 0, sizeof(selected));

	while (currVertex != NULL) {
		addVertex(*&minGraph, currVertex->name, &minGraphVertices);
		currVertex = currVertex->next;
	}

	while (numEdge < *numVertices) {
		currVertex = firstVertex;
		while (currVertex != NULL) {
			tEdge *currEdge = currVertex->firstEdge;
			while (currEdge != NULL) {
				if (numEdge == 0 && strcmp(currVertex->name, pivot) == 0) {
					selected[currVertex->id] = 1;
					selected[currEdge->id] = 1;
					enqueue(queue, currEdge->id);
					strcpy(order[numEdge], currVertex->name);
					addEdge(*minGraph, currVertex->name, currEdge->name, currEdge->length,
							0);
				} else if (numEdge > 0 && front(queue) == currVertex->id) {
					if (!selected[currEdge->id]) {
						enqueue(queue, currEdge->id);
						addEdge(*minGraph, currVertex->name, currEdge->name,
								currEdge->length, 0);
					}
					strcpy(order[numEdge], currVertex->name);
				}
				currEdge = currEdge->next;
			}
			currVertex = currVertex->next;
		}
		if (numEdge > 0)
			dequeue(queue);
		numEdge++;
	}

	printf("\n#(order)=> ");
	for (int i = 0; i < numEdge; i++) {
		printf("->%s", order[i]);
	}
	printf("\n");
	*&minGraph = minGraph;
}

void dfsAlg(tVertex *currVertex, char *pivot, tVertex **minGraph,
		int *numVertices) {
	tVertex *firstVertex = currVertex;
	int numEdge = 0, selected[*numVertices], minGraphVertices = 0;
	char order[*numVertices][50];
	struct Stack *stack = createStack(*numVertices);

	memset(selected, 0, sizeof(selected));

	while (currVertex != NULL) {
		addVertex(*&minGraph, currVertex->name, &minGraphVertices);
		currVertex = currVertex->next;
	}

	while (numEdge < *numVertices) {
		currVertex = firstVertex;

		while (currVertex != NULL) {
			tEdge *currEdge = currVertex->firstEdge;
			if (numEdge == 0 && strcmp(currVertex->name, pivot) == 0) {
				while (currEdge != NULL) {
					addEdge(*minGraph, currVertex->name, currEdge->name, currEdge->length, 0);
					push(stack, currEdge->id);
					strcpy(order[numEdge], currVertex->name);
					selected[currEdge->id] = 1;
					currEdge = currEdge->next;
				}
				selected[currVertex->id] = 1;
			} else if (numEdge > 0 && top(stack) == currVertex->id) {
				pop(stack);
				while (currEdge != NULL) {
					if (!selected[currEdge->id]) {
						push(stack, currEdge->id);
						addEdge(*minGraph, currVertex->name, currEdge->name, currEdge->length, 0);
					}
					currEdge = currEdge->next;
				}
				strcpy(order[numEdge], currVertex->name);
				selected[currVertex->id] = 1;
				numEdge++;
			}
			currVertex = currVertex->next;
		}
		if (numEdge == 0) numEdge++;
	}

	printf("\n#(order)=> ");
	for (int i = 0; i < numEdge; i++) {
		printf("->%s", order[i]);
	}
	printf("\n");

	*&minGraph = minGraph;
}

void printGraph(tVertex *currVertex, int *numVertices) {
	int selected[*numVertices];
	char vertices[*numVertices][50], espacios[*numVertices];
	int x,y;

	memset(selected, 0, sizeof(selected));
	for (int i = 0; i < *numVertices; i++) {
		espacios[i] = ' ';
	}

	while (currVertex != NULL) {
		tEdge *currEdge = currVertex->firstEdge;
		if (currEdge != NULL) {
			while (currEdge != NULL) {
				if (!selected[currEdge->id]) {
					/*printf("[%s, %s]\n", currVertex->name, currEdge->name);*/

					if (currVertex->id > currEdge->id) {
						x = currEdge->id;
						y = currVertex->id;
					} else {
						x = currVertex->id;
						y = currEdge->id;
					}
					/*printf("#=> x: %d\ty: %d\n", x, y);*/

					for (int i = 0; i < x; i++) {
						printf("%c ", espacios[i]);
					}
					printf("+");
					for (int i = 0; i < y-x-1; i++) {
						printf("--");
					}
					printf("-+ ");
					for (int i = y+1; i < *numVertices; i++) {
						printf("%c ", espacios[i]);
					}
					printf("\n");

					espacios[x] = '|';
					espacios[y] = '|';

					for (int i = 0; i < *numVertices; i++) {
						printf("%c ", espacios[i]);
					}

					printf("\n");

				}
				currEdge = currEdge->next;
			}
		}
		strcpy(vertices[currVertex->id], currVertex->name);
		selected[currVertex->id] = 1;


		currVertex = currVertex->next;
	}
	printf("\n");

	for (int i = 0; i < *numVertices; i++) {
		if (selected[i]) {
			printf("%d ", i);
		} else {
			printf("  ");
		}
	}
	printf("\n\n");

	for (int i = 0; i < *numVertices; i++) {
		if (selected[i]) {
			printf("(%d) %s\n", i, vertices[i]);
		}
	}
	printf("\n");
}

void printPath(int parent[], int j, char nombres[][50]) {
	// Base Case : If j is source
	if (parent[j] == -1) {
		printf("->(%s)", nombres[j]);
		return;
	}

	printPath(parent, parent[j], nombres);

	printf("->(%s)", nombres[j]);
}

void dijkstraAlg(tVertex *currVertex, char *pivot, int *numVertices) {
	tVertex *firstVertex = currVertex;
	int numEdge = 0, distances[*numVertices], selected[*numVertices], caminos[*numVertices];
	char nombres[*numVertices][50];

	memset(selected, 0, sizeof(selected));
	for (int i = 0; i < *numVertices; i++) {
		distances[i] = INT_MAX;
		caminos[i] = -1;
	}

	while (currVertex != NULL) {
		if (strcmp(currVertex->name, pivot) == 0) {
			distances[currVertex->id] = 0;
			selected[currVertex->id] = 1;
		}
		strcpy(nombres[currVertex->id], currVertex->name);
		currVertex = currVertex->next;
	}

	while (numEdge < *numVertices) {
		currVertex = firstVertex;
		while (currVertex != NULL) {
			if (selected[currVertex->id]) {
				tEdge *currEdge = currVertex->firstEdge;
				while (currEdge != NULL) {
					if (distances[currEdge->id] > currEdge->length && !selected[currEdge->id]) {
						distances[currEdge->id] = distances[currVertex->id] + currEdge->length;
						caminos[currEdge->id] = currVertex->id;
					}
					/*printf("%s - %s (%d)\n", currVertex->name, currEdge->name, currEdge->length);*/
					selected[currEdge->id] = 1;
					currEdge = currEdge->next;
				}
			}
			currVertex = currVertex->next;
		}
		numEdge++;
	}

	printf("\nCaminos más cortos\n");
	for (int i = 1; i < *numVertices; i++) {
		printf("[");
		printPath(caminos, i, nombres);
		printf("]: -> %d\n", distances[i]);
	}
	printf("\n");
}

void getNumero(int entero, void *input) {
	int length, i = 0;
	char aux[' '];

	do {
		fgets(aux, ' ', stdin);
		aux[strcspn(aux, "\n")] = 0;
		length = strlen(aux);
		if (length != 0) {
			for (i = 0; i < length; ++i) {
				if ((!isdigit(aux[i]) && aux[i] != '.') || (i == 0 && aux[i] == '-')) {
					printf("Introduzca correctamente el dato -> ");
					break;
				}
			}
		} else {
			i = length;
		}
	} while (i != length);

	if (entero == 0) {
		*(float*)input = atof(aux);
	} else if (entero == 2) {
		*(long*)input = atol(aux);
	} else {
		*(int*)input = atoi(aux);
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

	// ############# PIA #############
	addVertex(&firstVertex, "Tacos Chava", &firstGraphVertices);
	addVertex(&firstVertex, "Felipe Carrillo Puerto", &firstGraphVertices);
	addVertex(&firstVertex, "Nexxus", &firstGraphVertices);
	addVertex(&firstVertex, "San Genaro", &firstGraphVertices);
	addVertex(&firstVertex, "Celestino Gasca", &firstGraphVertices);
	addVertex(&firstVertex, "Ricardo Flores Magon", &firstGraphVertices);
	addVertex(&firstVertex, "Sector Jardines", &firstGraphVertices);
	addVertex(&firstVertex, "Infonavit Monterreal", &firstGraphVertices);
	addVertex(&firstVertex, "Privadas El Sauce", &firstGraphVertices);
	addVertex(&firstVertex, "La Loma", &firstGraphVertices);

	addEdge(firstVertex, "Tacos Chava", "Infonavit Monterreal", 1800, 0);
	addEdge(firstVertex, "Tacos Chava", "Nexxus", 3100, 0);
	addEdge(firstVertex, "Tacos Chava", "Ricardo Flores Magon", 3400, 0);
	addEdge(firstVertex, "Tacos Chava", "Privadas del Sauce", 4100, 0);
	addEdge(firstVertex, "Tacos Chava", "Felipe Carrillo", 3100, 0);
	addEdge(firstVertex, "Tacos Chava", "La Loma", 2000, 0);

	addEdge(firstVertex, "Ricardo Flores Magon", "Infonavit Monterreal", 1600, 0);
	addEdge(firstVertex, "Ricardo Flores Magon", "Sector Jardines", 1600, 0);

	addEdge(firstVertex, "Felipe Carrillo Puerto", "Celestino Gasca", 1400, 0);
	addEdge(firstVertex, "Felipe Carrillo Puerto", "Nexxus", 2800, 0);
	addEdge(firstVertex, "Felipe Carrillo Puerto", "San Genaro", 4700, 0);

	addEdge(firstVertex, "Privadas El Sauce", "Ricardo Flores Magon", 4400, 0);

	addEdge(firstVertex, "Infonavit Monterreal", "Felipe Carrillo Puerto", 2000, 0);

	addEdge(firstVertex, "Nexxus", "Celestino Gasca", 1500, 0);

	addEdge(firstVertex, "Celestino Gasca", "San Genaro", 4600, 0);


	//################# EJEMPLO ################
	/*addVertex(&firstVertex, "A", &firstGraphVertices);*/
	/*addVertex(&firstVertex, "B", &firstGraphVertices);*/
	/*addVertex(&firstVertex, "C", &firstGraphVertices);*/
	/*addVertex(&firstVertex, "D", &firstGraphVertices);*/
	/*addVertex(&firstVertex, "E", &firstGraphVertices);*/
	/*addVertex(&firstVertex, "F", &firstGraphVertices);*/

	/*addEdge (firstVertex, "A", "B", 4, 0);*/
	/*addEdge (firstVertex, "A", "C", 4, 0);*/
	/*addEdge (firstVertex, "B", "C", 2, 0);*/
	/*addEdge (firstVertex, "C", "D", 3, 0);*/
	/*addEdge (firstVertex, "C", "E", 1, 0);*/
	/*addEdge (firstVertex, "C", "F", 6, 0);*/
	/*addEdge (firstVertex, "D", "F", 2, 0);*/
	/*addEdge (firstVertex, "E", "F", 3, 0);*/

	/*printGraph(firstVertex, &firstGraphVertices);*/
	/*dijkstraAlg(firstVertex, "Tacos Chava", &firstGraphVertices);*/
	/*dijkstraAlg(firstVertex, "A", &firstGraphVertices);*/
	/*exit(1);*/

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
